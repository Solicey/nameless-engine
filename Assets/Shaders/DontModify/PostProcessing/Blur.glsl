// Gray scale shader
// For post-processing only, don't use it on entities
// You don't need to expose any properties

#use post
#tag ssao;ssao;

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
	
void main()
{
	vec2 texelSize = 1.0 / vec2(textureSize(u_ColorTex, 0));
	vec3 result = vec3(0);
	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(u_ColorTex, v_TexCoords + offset).rgb;
		}
	}
	color = vec4(result / (5.0 * 5.0), 1.0);
}			