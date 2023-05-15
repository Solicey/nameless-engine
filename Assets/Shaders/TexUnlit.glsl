// nlsl template shader file

#use model

#prop

color3 u_Color;
sampler2D u_Custom;
sampler2D u_Diffuse;

#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
};

uniform mat4 u_Transform;
			
layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
			
void main()
{
	v_Position = a_Position;
	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;

uniform vec3 u_Color;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Custom;
uniform bool u_IsSelected;
uniform int u_EntityId;
			
void main()
{
	f_Color = vec4(texture2D(u_Diffuse, v_TexCoord).rgb, 1.0) * vec4(u_Color, 1.0);
	f_EntityId = u_EntityId;
}			