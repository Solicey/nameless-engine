// nlsl template shader file

#use model

#prop

color3 u_Color;
float u_AmbientStrength;
float u_SpecularStrength;
sampler2D u_Diffuse;
sampler2D u_Normals;

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
};

uniform mat4 u_Transform;
// Normal Matrix
uniform mat4 u_NormalMatrix;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out mat3 v_TBN;
			
void main()
{
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	v_TexCoord = a_TexCoord;
	v_Normal = normalize(vec3(u_NormalMatrix * vec4(a_Normal, 0.0)));
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);

	vec3 T = vec3(normalize(u_NormalMatrix * vec4(a_Tangent, 0)));
	vec3 B = vec3(normalize(u_NormalMatrix * vec4(a_Bitangent, 0)));
	vec3 N = vec3(normalize(u_NormalMatrix * vec4(a_Normal, 0)));
	v_TBN = mat3(T, B, N);
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
};

// Object Color
uniform vec3 u_Color;
uniform bool u_IsSelected;
uniform int u_EntityId;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Normals;

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

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewPos, vec3 diffuseColor)
{
	vec3 viewDir = normalize(viewPos - v_Position);
	vec3 lightDir = normalize(-light.Direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 ambient = light.Color * u_AmbientStrength * diffuseColor;
	vec3 diffuse = light.Color * diff * diffuseColor;
	vec3 specular = light.Color * u_SpecularStrength * spec * u_Color;
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos, vec3 diffuseColor)
{
	vec3 viewDir = normalize(viewPos - v_Position);
	vec3 lightDir = normalize(light.Position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float dist = length(light.Position - fragPos);
	float atten = 1.0 / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * dist * dist);
	vec3 ambient = light.Color * u_AmbientStrength * diffuseColor;
	vec3 diffuse = light.Color * diff * diffuseColor;
	vec3 specular = light.Color * u_SpecularStrength * spec * u_Color;
	return atten * (ambient + diffuse + specular);
}
			
void main()
{
	vec4 diffuseFactor = texture(u_Diffuse, v_TexCoord).rgba;
	if (diffuseFactor.a <= 0.05) discard;
	vec3 diffuseColor = diffuseFactor.rgb;

	vec3 normal = texture(u_Normals, v_TexCoord).rgb;
	// For compressed normal map
	normal.z = sqrt(1 - normal.x * normal.x - normal.y * normal.y);
	normal = normalize(normal * 2.0 - vec3(1.0));
	normal = normalize(v_TBN * normal);

	vec3 result = vec3(0, 0, 0);

	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		if (u_DirLights[i].Color.r >= 0)
		{
			result += CalculateDirLight(u_DirLights[i], normal, u_CameraPosition, diffuseColor);
		}

		if (u_PointLights[i].Color.r >= 0)
		{
			result += CalculatePointLight(u_PointLights[i], normal, v_Position, u_CameraPosition, diffuseColor);
		}
	}

	f_Color = vec4(result, 1.0);

	// Dont Modify
	f_EntityId = u_EntityId;
}			