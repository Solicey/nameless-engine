#prop

#end

// Postprocessing outline

#type vertex
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;
			
void main()
{
	v_TexCoords = a_TexCoords;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
}

#type fragment
#version 450 core

out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_ScreenTex;
uniform sampler2D u_EntityTex;

// from https://computergraphics.stackexchange.com/questions/3646/opengl-glsl-sobel-edge-detection-filter
// Sobel
mat3 sx = mat3( 
    -1.0, 0.0, 1.0, 
    -2.0, 0.0, 2.0, 
    -1.0, 0.0, 1.0 
);
mat3 sy = mat3( 
    -1.0, -2.0, -1.0, 
    0.0, 0.0, 0.0, 
    1.0, 2.0, 1.0 
);
			
void main()
{
	color = texture(u_ScreenTex, v_TexCoords.st);
}			