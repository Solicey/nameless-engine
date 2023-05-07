// sprite

#prop
#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout (location = 1) in vec3 a_Position;
layout (location = 4) in vec4 a_Color;
layout (location = 5) in float a_Size;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
};
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;
layout (location = 2) out vec4 color3;

// uniform vec4 u_Color;
// uniform bool u_IsSelected;
// uniform int u_EntityId;
			
void main()
{
	color = vec4(1, 1, 1, 1);
	
	color2 = -1;

	color3 = vec4(0.1, 0.1, 0.1, 1);
	// if (u_IsSelected)
		// color3 = vec4(1, 1, 1, 1);
}			