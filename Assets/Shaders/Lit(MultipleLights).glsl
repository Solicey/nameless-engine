// nlsl template shader file

#use model
#lit 4

#prop

color3 u_Color;
float u_AmbientStrength;
float u_SpecularStrength;

#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
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
uniform mat3 u_NormalMatrixWS;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
			
void main()
{
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	v_TexCoord = a_TexCoord;
	v_Normal = normalize(u_NormalMatrixWS * a_Normal);
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

// FRAGMENT

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;

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
uniform vec3 u_Color;
uniform int u_EntityId;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;

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

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewPos)
{
	vec3 viewDir = normalize(viewPos - v_Position);
	vec3 lightDir = normalize(-light.Direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 ambient = light.Color * u_AmbientStrength * u_Color;
	vec3 diffuse = light.Color * diff * u_Color;
	vec3 specular = light.Color * u_SpecularStrength * spec * u_Color;
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
	vec3 viewDir = normalize(viewPos - v_Position);
	vec3 lightDir = normalize(light.Position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float dist = length(light.Position - fragPos);
	float atten = 1.0 / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * dist * dist);
	vec3 ambient = light.Color * u_AmbientStrength * u_Color;
	vec3 diffuse = light.Color * diff * u_Color;
	vec3 specular = light.Color * u_SpecularStrength * spec * u_Color;
	return atten * (ambient + diffuse + specular);
}
			
void main()
{
	vec3 result = vec3(0, 0, 0);

	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		if (u_DirLights[i].Color.r >= 0)
		{
			result += CalculateDirLight(u_DirLights[i], v_Normal, u_CameraPosition);
		}

		if (u_PointLights[i].Color.r >= 0)
		{
			result += CalculatePointLight(u_PointLights[i], v_Normal, v_Position, u_CameraPosition);
		}
	}

	f_Color = vec4(result, 1.0);

	// Dont Modify
	f_EntityId = u_EntityId;
}			