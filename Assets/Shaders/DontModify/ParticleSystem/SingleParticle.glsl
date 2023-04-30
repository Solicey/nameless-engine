#use particle

#prop

#end

// nlsl template shader file

#type vertex
#version 450 core

layout (location = 0) in float a_Type;
layout (location = 1) in vec3 a_Position;
layout (location = 2) in vec3 a_Velocity;
layout (location = 3) in float a_Lifetime;

out float v_Type;
out vec3 v_Position;
out vec3 v_Velocity;
out float v_Lifetime;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
};

uniform mat4 u_Transform;
uniform float u_DeltaTime;
			
void main()
{
	v_Type = a_Type;
    v_Position = a_Position + vec3(0, u_DeltaTime * 1, 0);
    v_Velocity = a_Velocity;
    v_Lifetime = a_Lifetime;
	gl_Position = u_Projection * u_View * u_Transform * vec4(v_Position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in float v_Type;
layout (location = 1) in vec3 v_Position;
layout (location = 2) in vec3 v_Velocity;
layout (location = 3) in float v_Lifetime;

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;
layout (location = 2) out vec4 color3;

// uniform vec3 u_Color;
// uniform bool u_IsSelected;
// uniform int u_EntityId;
			
void main()
{
	color = vec4(1, 1, 1, 1);
	
	//color2 = u_EntityId;

	color2 = -1;
	color3 = vec4(0.1, 0.1, 0.1, 1);
	//if (u_IsSelected)
		//color3 = vec4(1, 1, 1, 1);
}			