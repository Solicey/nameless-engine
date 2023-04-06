// Editor Outline shader
// For post-processing only, don't use it on entities
// You don't need to expose any properties

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

uniform int u_ScreenWidth;
uniform int u_ScreenHeight;
uniform sampler2D u_EntityTex;
uniform sampler2D u_ColorTex;

float sobel_y[] =
{
    -1,-2,-1,
     0, 0, 0,
     1, 2, 1,
};
 
float sobel_x[] =
{
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1,
};
			
void main()
{
	float conX = 0, conY = 0;
	int k = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			float tmp = texture(u_EntityTex, v_TexCoords + vec2(1.0 * i / u_ScreenWidth, 1.0 * j / u_ScreenHeight)).r;
			conX += sobel_x[k] * tmp;
			conY += sobel_y[k++] * tmp; 
		}
	}

	float conv = sqrt(conX * conX + conY * conY);

	color = texture(u_ColorTex, v_TexCoords);
	
	if (conv > 1.0 && texture(u_EntityTex, v_TexCoords).r < 1.0)
	{
		color = vec4(conv * vec3(1, 0.647, 0) / 2.0, 1.0);
	}

	// color = texture(u_EntityTex, v_TexCoords);
}			