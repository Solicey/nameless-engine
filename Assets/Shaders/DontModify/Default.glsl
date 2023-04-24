#prop

#end

// nlsl template shader file

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
			
layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
			
void main()
{
	v_Position = a_Position;
	v_Normal = a_Normal;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;
layout (location = 2) out vec4 color3;

// uniform vec3 u_Color;
uniform bool u_IsSelected;
uniform int u_EntityId;
			
void main()
{
	color = vec4(v_Normal.x * 0.5 + 0.5, v_Normal.y * 0.5 + 0.5, v_Normal.z * 0.5 + 0.5, 1.0); // * vec4(u_Color, 1.0);
	color2 = u_EntityId;

	color3 = vec4(0.1, 0.1, 0.1, 1);
	if (u_IsSelected)
		color3 = vec4(1, 1, 1, 1);
}			