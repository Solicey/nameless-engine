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

void main()
{
	if (u_PointLights[0].Color.r >= 0)
	{
		vec3 ambient = u_AmbientStrength * u_PointLights[0].Color;

		vec3 norm = normalize(v_Normal);
		vec3 lightDir = normalize(u_PointLights[0].Position - v_Position);

		float dist = length(u_PointLights[0].Position - v_Position);
		float atten = 1.0 / (u_PointLights[0].Attenuation.x + u_PointLights[0].Attenuation.y * dist + u_PointLights[0].Attenuation.z * dist * dist);

		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * u_PointLights[0].Color;

		vec3 viewDir = normalize(u_CameraPosition - v_Position);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = u_SpecularStrength * spec * u_PointLights[0].Color;

		vec3 result = atten * (ambient + diffuse + specular) * u_Color;
		f_Color = vec4(result, 1.0);
	}
	else
	{
		f_Color = vec4(u_Color, 1.0);
	}

	// color = vec4(1, 1, 1, 1.0);

	// Dont Modify
	f_EntityId = u_EntityId;
}			