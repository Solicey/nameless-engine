#prop

#end

// skybox

#type vertex
#version 450 core

layout (location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
};

uniform mat4 u_Transform;
			
layout (location = 0) out vec3 v_TexCoords;
			
void main()
{
	v_TexCoords = a_Position;
	vec4 pos = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_TexCoords;

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;

uniform samplerCube u_Skybox;
			
void main()
{
	f_Color = texture(u_Skybox, v_TexCoords).rgba;
	f_EntityId = -1;
}			