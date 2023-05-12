// Gray scale shader
// For post-processing only, don't use it on entities
// You don't need to expose any properties

#use post

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

float luminance(vec3 color)
{
	return 0.2125 * color.r + 0.7154 * color.g + 0.0721 * color.b;
}
			
void main()
{
	vec4 sampled = texture(u_ColorTex, v_TexCoords).rgba;
	vec3 diffuse = sampled.rgb;
    float grayVal = luminance(diffuse);
	color = vec4(grayVal, grayVal, grayVal, sampled.a);
}			