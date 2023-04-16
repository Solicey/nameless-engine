#prop

#end

// skybox

#type vertex
#version 450 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
			
layout (location = 0) out vec3 v_TexCoords;
			
void main()
{
	v_TexCoords = a_Position;
	vec4 pos = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_TexCoords;

layout (location = 0) out vec4 color;

uniform samplerCube u_Skybox;
			
void main()
{
	color = texture(u_Skybox, v_TexCoords);
}			