// Gray scale shader
// For post-processing only, don't use it on entities
// You don't need to expose any properties

#use post
#lit 32

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

uniform sampler2D u_ColorTex;
uniform isampler2D u_EntityTex;
uniform sampler2D u_PositionDepthTex;
uniform sampler2D u_NormalTex;
			
void main()
{
	vec3 albedo = texture(u_ColorTex, v_TexCoords).rgb;
	float spec = texture(u_ColorTex, v_TexCoords).a;
	vec3 fragPos = texture(u_PositionDepthTex, v_TexCoords).xyz;
	float depth = texture(u_PositionDepthTex, v_TexCoords).w;
	vec3 normal = texture(u_NormalTex, v_TexCoords).rgb;
	int id = texture(u_EntityTex, v_TexCoords).r;
	
	color = vec4(albedo, 1.0);
}			