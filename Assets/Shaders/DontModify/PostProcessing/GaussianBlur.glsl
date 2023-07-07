// Gray scale shader
// For post-processing only, don't use it on entities
// You don't need to expose any properties

#use post
#tag ssao;ssao;

#prop
float u_IsHorizontal;
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
uniform float u_IsHorizontal;
uniform float u_Weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 texelSize = 2.0 / vec2(textureSize(u_ColorTex, 0));
	vec3 result = vec3(0);
	if (u_IsHorizontal != 0.0)
	{
		for(int i = 0; i < 5; ++i)
        {
            result += texture(u_ColorTex, v_TexCoords + vec2(texelSize.x * i, 0.0)).rgb * u_Weights[i];
            result += texture(u_ColorTex, v_TexCoords - vec2(texelSize.x * i, 0.0)).rgb * u_Weights[i];
        }
	}
	else
	{
		for(int i = 0; i < 5; ++i)
        {
            result += texture(u_ColorTex, v_TexCoords + vec2(0.0, texelSize.y * i)).rgb * u_Weights[i];
            result += texture(u_ColorTex, v_TexCoords - vec2(0.0, texelSize.y * i)).rgb * u_Weights[i];
        }
	}
	color = vec4(result, 1.0);
}			