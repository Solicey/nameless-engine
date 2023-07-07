// Gray scale shader
// For post-processing only, don't use it on entities

#use post
#lit 4
#tag shadow;src;

#prop
float u_EnableShadow;
float u_ShadowBiasModifier;
float u_EnableSRGBCorrection;
color3 u_AmbientColor;
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
uniform float u_EnableShadow;
uniform float u_ShadowBiasModifier;
uniform float u_EnableSRGBCorrection;
uniform vec3 u_AmbientColor;

#define MAX_LIGHT_COUNT 4
const float PI = 3.14159265359;

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

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
			
void main()
{
	vec3 albedo = texture(u_ColorTex, v_TexCoords).rgb;
	float roughness = texture(u_ColorTex, v_TexCoords).a;
	vec3 fragPos = texture(u_PositionDepthTex, v_TexCoords).xyz;
	float depth = texture(u_PositionDepthTex, v_TexCoords).w;
	vec3 normal = texture(u_NormalTex, v_TexCoords).xyz;
	float metallic = texture(u_NormalTex, v_TexCoords).w;
	
	if (depth <= 0)
	{
		f_Color = vec4(albedo, roughness);
		return;
	}	

	mat4 invView = inverse(u_View);
	mat4 transView = transpose(u_View);
	vec3 worldPos = vec3(invView * vec4(fragPos, 1.0));	// Pos WS
	vec3 N = vec3(transView * vec4(normal, 0.0));	// Normal WS
	vec3 V = normalize(u_CameraPosition - worldPos);
	vec3 Lo = vec3(0.0);
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	float shadow = ShadowCaster(fragPos, vec3(inverse(u_View) * vec4(fragPos, 1.0)), normal);

	float hasCastShadow = 0;
	float test;

	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		if (u_DirLights[i].Color.r >= 0)
		{
			DirLight light = u_DirLights[i];
			vec3 lightColor = light.Color;
			vec3 lightDir = light.Direction;
			if (u_EnableSRGBCorrection != 0)
			{
				lightColor = pow(lightColor, vec3(2.2));
			}

			vec3 L = -normalize(lightDir);
			vec3 H = normalize(V + L);

			vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
			float NDF = DistributionGGX(N, H, roughness);
			float G = GeometrySmith(N, V, L, roughness);

			vec3 nominator = NDF * G * F;
			float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
			vec3 specular = nominator / denominator;

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;

			float NdotL = max(dot(N, L), 0.0);
			vec3 ds = (kD * albedo / PI + specular) * lightColor * NdotL;
			test = NdotL;

			/*vec3 viewDir = normalize(-fragPos);
			// diffuse
			vec3 lightDir = -normalize(vec3(u_View * vec4(light.Direction, 0)));
			vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * lightColor;
			// specular
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
			vec3 specular = lightColor * spec * specStrength;*/

			if (hasCastShadow == 0.0 && u_EnableShadow != 0.0)
			{
				hasCastShadow = 1.0;
				ds = ds * (1.0 - shadow);
			}

			Lo += ds;
		}

		if (u_PointLights[i].Color.r >= 0)
		{
			PointLight light = u_PointLights[i];
			vec3 lightColor = light.Color;
			vec3 lightPos = light.Position;
			vec3 lightAtten = light.Attenuation;

			if (u_EnableSRGBCorrection != 0)
			{
				lightColor = pow(lightColor, vec3(2.2));
			}

			vec3 L = normalize(lightPos - worldPos);
			vec3 H = normalize(V + L);

			float dist = length(lightPos - worldPos);
			// float atten = 1.0 / (lightAtten.x + lightAtten.y * dist + lightAtten.z * dist * dist);
			float atten = 1.0 / (dist * dist);
			vec3 radiance = lightColor * atten;

			vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
			float NDF = DistributionGGX(N, H, roughness);
			float G = GeometrySmith(N, V, L, roughness);

			vec3 nominator = NDF * G * F;
			float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
			vec3 specular = nominator / denominator;

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;

			float NdotL = max(dot(N, L), 0.0);
			Lo += (kD * albedo / PI + specular) * radiance * NdotL;

			/*vec3 viewDir = normalize(-fragPos);
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
			specular *= atten;*/
		}
	}

	f_Color = vec4(Lo + u_AmbientColor * albedo, 1.0);

	//albedo = pow(albedo, vec3(1.0 / 2.2));
	//f_Color = vec4(N, 1.0);
}			