#use particle1

#prop

#end

// nlsl template shader file

#type vertex
#version 450 core

layout (location = 0) in float a_Type;
layout (location = 1) in vec3 a_Position;
layout (location = 2) in vec3 a_Velocity;
layout (location = 3) in float a_Lifetime;
layout (location = 4) in vec4 a_Color;
layout (location = 5) in float a_Size;
layout (location = 6) in float a_TotalLifetime;

out float v_Type;
out vec3 v_Position;
out vec3 v_Velocity;
out float v_Lifetime;
out vec4 v_Color;
out float v_Size;
out float v_TotalLifetime;
			
void main()
{
	v_Type = a_Type;
    v_Position = a_Position;
    v_Velocity = a_Velocity;
    v_Lifetime = a_Lifetime;
	v_Color = a_Color;
	v_Size = a_Size;
	v_TotalLifetime = a_TotalLifetime;
}

#type geometry
#version 450 core

layout (points) in;
layout (points) out;

in float v_Type[];
in vec3 v_Position[];
in vec3 v_Velocity[];
in float v_Lifetime[];
in vec4 v_Color[];
in float v_Size[];
in float v_TotalLifetime[];

out float g_Type;
out vec3 g_Position;
out vec3 g_Velocity;
out float g_Lifetime;
out vec4 g_Color;
out float g_Size;
out float g_TotalLifetime;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

uniform mat4 u_Transform;
uniform float u_DeltaTime;

void main()
{
	g_Type = v_Type[0];
	g_Position = v_Position[0] + vec3(0, u_DeltaTime * v_Velocity[0].y, 0);
	if (g_Position.y > 30)
		g_Position.y = 0; 
	g_Velocity = v_Velocity[0];
	g_Lifetime = v_Lifetime[0];
	g_Color = v_Color[0];
	g_Size = v_Size[0];
	g_TotalLifetime = v_TotalLifetime[0];

	gl_Position = u_Projection * u_View * u_Transform * vec4(g_Position, 1.0);

	EmitVertex();
    EndPrimitive();
}
	