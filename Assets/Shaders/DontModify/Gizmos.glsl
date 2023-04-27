// gizmos

#prop
#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;
			
layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
			
void main()
{
	v_Position = a_Position;
	v_TexCoord = a_TexCoord;
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;
layout (location = 2) out vec4 color3;

uniform sampler2D u_Sprite;
uniform vec4 u_Color;
uniform bool u_IsSelected;
uniform int u_EntityId;
			
void main()
{
	color = texture2D(u_Sprite, v_TexCoord);
	if (color.a < 0.1)
		discard;
	else color = u_Color;
	
	color2 = u_EntityId;

	color3 = vec4(0.1, 0.1, 0.1, 1);
	if (u_IsSelected)
		color3 = vec4(1, 1, 1, 1);
}			