// Gray scale shader
// For post-processing only, don't use it on entities

#use post
#lit 4
#tag src;

#prop
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

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};
			
void main()
{
	vec3 albedo = texture(u_SrcColorTex, v_TexCoords).rgb;
	float specStrength = texture(u_SrcColorTex, v_TexCoords).a;
	vec3 fragPos = texture(u_PositionDepthTex, v_TexCoords).xyz;
	float depth = texture(u_PositionDepthTex, v_TexCoords).w;
	vec3 normal = texture(u_NormalTex, v_TexCoords).xyz;
	vec3 ssao = texture(u_ColorTex, v_TexCoords).rgb;
	
	if (depth <= 0)
	{
		f_Color = vec4(albedo, specStrength);
		return;
	}	

	vec3 result = vec3(0, 0, 0);

	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		if (u_DirLights[i].Color.r >= 0)
		{
			DirLight light = u_DirLights[i];
			vec3 viewDir = normalize(-fragPos);
			// diffuse
			vec3 lightDir = -normalize(vec3(u_View * vec4(light.Direction, 0)));
			vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * light.Color;
			// specular
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
			vec3 specular = light.Color * spec * specStrength;

			result += diffuse + specular;
		}

		if (u_PointLights[i].Color.r >= 0)
		{
			PointLight light = u_PointLights[i];
			vec3 viewDir = normalize(-fragPos);
			// diffuse
			vec3 lightPos = vec3(u_View * vec4(light.Position, 1.0));
			vec3 lightDir = normalize(lightPos - fragPos);
			vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * light.Color;
			// specular
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
			vec3 specular = light.Color * spec * specStrength;
			// atten
			float dist = length(lightPos - fragPos);
			float atten = 1.0 / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * dist * dist);
			diffuse *= atten;
			specular *= atten;
			result += diffuse + specular;
		}
	}

	f_Color = vec4(result * ssao, 1.0);
}			