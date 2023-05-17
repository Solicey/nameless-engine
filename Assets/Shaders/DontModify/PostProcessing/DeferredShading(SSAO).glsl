// Gray scale shader
// For post-processing only, don't use it on entities

#use post
#lit 32
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

layout (location = 0) out vec4 color;

// ssao tex
uniform sampler2D u_ColorTex;
uniform sampler2D u_PositionDepthTex;
uniform sampler2D u_NormalTex;
uniform sampler2D u_SrcColorTex;

#define MAX_LIGHT_COUNT 32

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
	float spec = texture(u_SrcColorTex, v_TexCoords).a;
	vec3 fragPos = texture(u_PositionDepthTex, v_TexCoords).xyz;
	float depth = texture(u_PositionDepthTex, v_TexCoords).w;
	vec3 normal = texture(u_NormalTex, v_TexCoords).rgb;
	float ambientOcclusion = texture(u_ColorTex, v_TexCoords).r;
	
	if (depth <= 0)
	{
		color = vec4(albedo, spec);
		return;
	}	

	if (u_PointLights[0].Color.r >= 0)
	{
		PointLight light = u_PointLights[0];
		vec3 ambient = vec3(0.2 * ambientOcclusion);
		vec3 lighting = ambient;
		vec3 viewDir = normalize(-fragPos);
		// diffuse
		vec3 lightPos = vec3(u_View * vec4(light.Position, 1.0));
		vec3 lightDir = normalize(lightPos - fragPos);
		vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * light.Color;
		// specular
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
		vec3 specular = light.Color * spec;
		// atten
		float dist = length(lightPos - fragPos);
		float atten = 1.0 / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * dist * dist);
		diffuse *= atten;
		specular *= atten;
		lighting += diffuse + specular;
		color = vec4(lighting, 1.0);
	}
	else
	{
		color = vec4(vec3(ambientOcclusion) * albedo, 1.0);
	}
	//color = vec4(normal, 1.0);
}			