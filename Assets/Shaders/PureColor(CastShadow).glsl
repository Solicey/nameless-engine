#use model

#tag shadow;

#prop
color3 u_Color;
float u_Specular;
float u_EnableSRGBCorrection;
float u_EnableGammaCorrection;
float u_ShadowBiasModifier;
#end

// nlsl template shader file

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec3 a_Normal;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

uniform mat4 u_Transform;
uniform mat3 u_NormalMatrixVS;
uniform mat3 u_NormalMatrixWS;
			
layout (location = 0) out vec3 v_Position;	// View Space
layout (location = 1) out vec3 v_PositionWS;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out vec3 v_NormalWS;
			
void main()
{
	// G buffer under view space
	v_Position = vec3(u_View * u_Transform * vec4(a_Position, 1.0));
	v_PositionWS = vec3(u_Transform * vec4(a_Position, 1.0));
	//mat3 normalMatrix = transpose(inverse(mat3(u_View * u_Transform)));
	v_Normal = normalize(u_NormalMatrixVS * a_Normal);
	v_NormalWS = normalize(u_NormalMatrixWS * a_Normal);
	//v_Normal = normalize(a_Normal);
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;	// View Space
layout (location = 1) in vec3 v_PositionWS;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in vec3 v_NormalWS;

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;
layout (location = 2) out vec4 f_PositionDepth;
layout (location = 3) out vec4 f_Normal;

uniform vec3 u_Color;
uniform float u_Specular;
uniform int u_EntityId;
uniform sampler2DArray u_ShadowMaps;
uniform float u_EnableSRGBCorrection;
uniform float u_EnableGammaCorrection;
uniform float u_ShadowBiasModifier;

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

float ShadowCaster(vec3 fragPosVS, vec3 fragPosWS)
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

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (fragDepth > 1.0)
		return 0.0;

	//if (u_ShadowCascadeLevels[0] == 20.0)
	//if (u_LightIntensity == 1.0)
		//return 1.0;
	//return 0.0;
	//return texture(u_ShadowMaps, vec3(fragPosNDC.xy, layer)).r;
	
	float bias = max(0.05 * (1.0 - dot(v_NormalWS, -u_LightDir)), 0.005);
	//const float biasModifier = 0.5f;//(layer == 0 ? 1.0f : 2.0f) / (layer + 1.2f);
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
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float sampleDepth = texture(u_ShadowMaps, vec3(fragPosNDC.xy + vec2(x, y) * texelSize, layer)).r;
			shadow += (fragDepth - bias) > sampleDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	return shadow;
}


float LinearizeDepth(float depth, float near, float far)
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}
			
void main()
{
	float shadow = 0.5 * ShadowCaster(v_Position, v_PositionWS);

	vec3 color = u_Color;
	if (u_EnableSRGBCorrection != 0.0)
	{
		color = pow(color, vec3(2.2));
	}

	color = (1 - shadow) * color;
	//f_Color = vec4(u_Color, u_Specular);
	f_EntityId = u_EntityId;

	f_PositionDepth = vec4(v_Position, LinearizeDepth(gl_FragCoord.z, u_Near, u_Far));//LinearizeDepth(depth, u_Near, u_Far));
	f_Normal = vec4(v_Normal, 1.0);

	if (u_EnableGammaCorrection != 0.0)
	{
		color = pow(color, vec3(1.0 / 2.2));
	}

	f_Color = vec4(color, 1.0);
	//f_Color = vec4(vec3(shadow), 1.0);
}			