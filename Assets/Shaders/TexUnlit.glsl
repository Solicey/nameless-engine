// nlsl template shader file

#prop

color3 u_Color;
sampler2D u_Diffuse;

#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
			
layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out flat int v_EntityID;
			
void main()
{
	v_Position = a_Position;
	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

out vec4 color;
out int color2;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in flat int v_EntityID;

uniform vec3 u_Color;
uniform sampler2D u_Diffuse;
			
void main()
{
	color = vec4(texture2D(u_Diffuse, v_TexCoord).rgb, 1.0) * vec4(u_Color, 1.0);
	color2 = v_EntityID;
}			