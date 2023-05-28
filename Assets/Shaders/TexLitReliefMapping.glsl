// nlsl template shader file

#use model
#lit 4

#prop

float u_AmbientStrength;
float u_SpecularStrength;
sampler2D u_Diffuse;
sampler2D u_Specular;
sampler2D u_Normals;
float u_IsNormalsCompressed;
sampler2D u_ReliefMap;
float u_ReliefParam;

#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

uniform mat4 u_Transform;
// Normal Matrix
uniform mat3 u_NormalMatrixWS;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out mat3 v_TBN;
			
void main()
{
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	v_TexCoord = a_TexCoord;
	v_Normal = normalize(u_NormalMatrixWS * a_Normal);
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);

	vec3 T = normalize(u_NormalMatrixWS * a_Tangent);
	vec3 N = normalize(u_NormalMatrixWS * a_Normal);
	T = normalize(T - dot(T, N) * N); // Orthogonalization
	vec3 B = cross(N, T);
	v_TBN = mat3(T, B, N); // Tangent Space to World Space
}

// FRAGMENT

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in mat3 v_TBN;

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

// Object Color
uniform int u_EntityId;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Specular;
uniform sampler2D u_Normals;
uniform sampler2D u_ReliefMap;
uniform float u_IsNormalsCompressed;
uniform float u_ReliefParam;

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

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewPos, vec3 diffuseColor, vec3 specularColor, float shininess)
{
	vec3 viewDir = normalize(viewPos - v_Position);
	vec3 lightDir = normalize(-light.Direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 ambient = u_AmbientStrength * diffuseColor;
	vec3 diffuse = diff * diffuseColor;
	vec3 specular = u_SpecularStrength * spec * specularColor;
	if (u_SpecularStrength == 0)
		return (ambient + diffuse) * light.Color;
	else 
		return (ambient + diffuse + specular) * light.Color;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos, vec3 diffuseColor, vec3 specularColor, float shininess)
{
	vec3 viewDir = normalize(viewPos - v_Position);
	vec3 lightDir = normalize(light.Position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	float dist = length(light.Position - fragPos);
	float atten = 1.0 / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * dist * dist);
	vec3 ambient = u_AmbientStrength * diffuseColor;
	vec3 diffuse = diff * diffuseColor;
	vec3 specular = u_SpecularStrength * spec * specularColor;
	if (u_SpecularStrength == 0)
		return atten * (ambient + diffuse) * light.Color;
	else 
		return atten * (ambient + diffuse + specular) * light.Color;
}

vec2 ReliefMapping(vec3 viewDirTS)
{
	const float numLayers = 10;
	float deltaLayerDepth = 1.0 / numLayers;
	float currLayerDepth = 0.0;
	vec2 deltaTexCoord = viewDirTS.xy * u_ReliefParam / numLayers;

	vec2 currTexCoord = v_TexCoord;
	float currDepthMapValue = texture(u_ReliefMap, currTexCoord).r;

	while (currLayerDepth < currDepthMapValue)
	{
		currTexCoord -= deltaTexCoord;
		currDepthMapValue = texture(u_ReliefMap, currTexCoord).r;
		currLayerDepth += deltaLayerDepth;
	}

	deltaTexCoord = deltaTexCoord / 2;
	deltaLayerDepth = deltaLayerDepth / 2;

	currTexCoord += deltaTexCoord;
	currLayerDepth -= deltaLayerDepth;

	const int numSearches = 8;
	for (int i = 0; i < numSearches; i++)
	{
		deltaTexCoord / 2;
		deltaLayerDepth / 2;

		currDepthMapValue = texture(u_ReliefMap, currTexCoord).r;
		if (currDepthMapValue > currLayerDepth)
		{
			currTexCoord -= deltaTexCoord;
			currLayerDepth += deltaLayerDepth;
		}
		else
		{
			currTexCoord += deltaTexCoord;
			currLayerDepth -= deltaLayerDepth;
		}
	}

	return currTexCoord;
}
			
void main()
{
	mat3 invTBN = transpose(v_TBN);		// World Space to Tangent Space
	vec3 viewPosTS = invTBN * u_CameraPosition;
	vec3 fragPosTS = invTBN * v_Position;
	vec3 viewDirTS = normalize(viewPosTS - fragPosTS);
	vec2 texCoord = ReliefMapping(viewDirTS);
	//texCoord = v_TexCoord;

	vec4 diffuseFactor = texture(u_Diffuse, texCoord).rgba;
	if (diffuseFactor.a <= 0.05) discard;
	vec3 diffuseColor = diffuseFactor.rgb;

	vec4 specularFactor = texture(u_Specular, texCoord).rgba;
	vec3 specularColor  = specularFactor.rgb;
	float shininess     = specularFactor.a * 64;

	vec3 normal = texture(u_Normals, texCoord).rgb;
	// For compressed normal map
	if (u_IsNormalsCompressed != 0)
		normal.z = sqrt(1 - normal.x * normal.x - normal.y * normal.y);
	normal = normalize(normal * 2.0 - vec3(1.0));
	normal = normalize(v_TBN * normal);

	if (specularFactor.a == 0)
	{
		specularColor = vec3(0);
		shininess = 0;
	}
	if (texture(u_Normals, v_TexCoord).a == 0)
		normal = v_Normal;

	vec3 result = vec3(0, 0, 0);

	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		if (u_DirLights[i].Color.r >= 0)
		{
			result += CalculateDirLight(u_DirLights[i], normal, u_CameraPosition, diffuseColor, specularColor, shininess);
		}

		if (u_PointLights[i].Color.r >= 0)
		{
			result += CalculatePointLight(u_PointLights[i], normal, v_Position, u_CameraPosition, diffuseColor, specularColor, shininess);
		}
	}

	f_Color = vec4(result, 1.0);

	// f_Color = vec4(normal, 1.0);

	// Dont Modify
	f_EntityId = u_EntityId;
}			