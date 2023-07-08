// Gray scale shader
// For post-processing only, don't use it on entities

#use post
#lit 4
#tag shadow;src;

#prop
bool u_EnableShadow;
float u_ShadowBiasModifier;
bool u_EnableSRGBCorrection;
bool u_EnableGammaCorrection;
#end

#type vertex
#version 450 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoords;

layout (location = 0) out vec2 v_TexCoords;
			
void main()
{
	v_TexCoords = a_TexCoords;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
}

#type fragment
#version 450 core

layout (location = 0) in vec2 v_TexCoords;

layout (location = 0) out vec4 f_Color;

// ssao tex
uniform sampler2D u_ColorTex;
uniform sampler2D u_PositionDepthTex;
uniform sampler2D u_NormalTex;
uniform sampler2D u_SrcColorTex;
uniform sampler2DArray u_ShadowMaps;
uniform bool u_EnableShadow;
uniform float u_ShadowBiasModifier;
uniform bool u_EnableSRGBCorrection;
uniform bool u_EnableGammaCorrection;

#define MAX_LIGHT_COUNT 4

struct PointLight
{
	vec3 Position;
	vec3 Color;
	vec3 Attenuation;
};
uniform PointLight u_PointLights[MAX_LIGHT_COUNT];

struct DirLight
{
	vec3 Direction;
	vec3 Color;
};
uniform DirLight u_DirLights[MAX_LIGHT_COUNT];

layout (std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

layout (std140, binding = 1) uniform Shadow
{
    mat4 u_LightSpaceMatrices[16];
	float u_ShadowCascadeLevels[16];
	vec3 u_LightDir;
	float u_LightIntensity;
	float u_FarClip;
	int u_CascadeLevelCount;
};

float ShadowCaster(vec3 fragPosVS, vec3 fragPosWS, vec3 normalVS)
{
	float depth = abs(fragPosVS.z);

	int layer = -1;
	for (int i = 0; i < u_CascadeLevelCount; i++)
	{
		if (depth < u_ShadowCascadeLevels[i])
		{
			layer = i;
			break;
		}
	}
	if (layer == -1)
	{
		layer = u_CascadeLevelCount;
	}

	vec4 fragPosLS = u_LightSpaceMatrices[layer] * vec4(fragPosWS, 1.0);
	vec3 fragPosNDC = fragPosLS.xyz / fragPosLS.w;
	fragPosNDC = fragPosNDC * 0.5 + 0.5;

	float fragDepth = fragPosNDC.z;

	//return fragDepth;
	//return texture(u_ShadowMaps, vec3(fragPosNDC.xy, layer)).r;
	//return layer / 5.0f;

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (fragDepth > 1.0)
		return 0.0;

	//if (u_FarClip == 1000.0)
	//if (u_LightIntensity == 1.0)
		//return 1.0;
	//return 0.0;
	
	float bias = max(0.05 * (1.0 - dot(normalVS, -u_LightDir)), 0.005);
	if (layer == u_CascadeLevelCount)
	{
		bias *= 1 / (u_FarClip * u_ShadowBiasModifier);
	}
	else
	{
		bias *= 1 / (u_ShadowCascadeLevels[layer] * u_ShadowBiasModifier);
	}

	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / vec2(textureSize(u_ShadowMaps, 0));
	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			float sampleDepth = texture(u_ShadowMaps, vec3(fragPosNDC.xy + vec2(x, y) * texelSize, layer)).r;
			shadow += (fragDepth - bias) > sampleDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 25.0;
	return shadow;
}
			
void main()
{
	vec3 albedo = texture(u_SrcColorTex, v_TexCoords).rgb;
	float specStrength = texture(u_SrcColorTex, v_TexCoords).a;
	vec3 fragPos = texture(u_PositionDepthTex, v_TexCoords).xyz;
	float depth = texture(u_PositionDepthTex, v_TexCoords).w;
	vec3 normal = texture(u_NormalTex, v_TexCoords).xyz;
	vec3 ssdo = texture(u_ColorTex, v_TexCoords).rgb;
	
	if (depth <= 0)
	{
		f_Color = vec4(albedo, specStrength);
		return;
	}	

	vec3 result = vec3(0, 0, 0);
	float shadow = ShadowCaster(fragPos, vec3(inverse(u_View) * vec4(fragPos, 1.0)), normal);

	bool hasCastShadow = false;
	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		if (u_DirLights[i].Color.r >= 0)
		{
			DirLight light = u_DirLights[i];
			vec3 lightColor = light.Color;
			if (u_EnableSRGBCorrection)
			{
				lightColor = pow(lightColor, vec3(2.2));
			}

			vec3 viewDir = normalize(-fragPos);
			// diffuse
			vec3 lightDir = -normalize(vec3(u_View * vec4(light.Direction, 0)));
			vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * lightColor;
			// specular
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
			vec3 specular = lightColor * spec * specStrength;

			vec3 ds = diffuse + specular;

			if (!hasCastShadow && u_EnableShadow)
			{
				hasCastShadow = true;
				ds = ds * (1.0 - shadow);
			}

			result += ds;
		}

		if (u_PointLights[i].Color.r >= 0)
		{
			PointLight light = u_PointLights[i];
			vec3 lightColor = light.Color;
			if (u_EnableSRGBCorrection)
			{
				lightColor = pow(lightColor, vec3(2.2));
			}

			vec3 viewDir = normalize(-fragPos);
			// diffuse
			vec3 lightPos = vec3(u_View * vec4(light.Position, 1.0));
			vec3 lightDir = normalize(lightPos - fragPos);
			vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * lightColor;
			// specular
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
			vec3 specular = lightColor * spec * specStrength;
			// atten
			float dist = length(lightPos - fragPos);
			float atten = 1.0 / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * dist * dist);
			diffuse *= atten;
			specular *= atten;
			result += diffuse + specular;
		}
	}

	vec3 color = result + albedo * ssdo;

	// Gamma Correction
	if (u_EnableGammaCorrection)
	{
		color = pow(color, vec3(1.0 / 2.2));
	}
	f_Color = vec4(color, 1.0);

	//albedo = pow(albedo, vec3(1.0 / 2.2));
	//f_Color = vec4(albedo, 1.0);
}			