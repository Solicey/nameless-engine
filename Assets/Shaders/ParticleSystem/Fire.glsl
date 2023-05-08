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
layout (points, max_vertices = 10) out;

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

uniform float u_DeltaTime;
uniform float u_TotalTime;
uniform float u_Radius;
uniform vec3 u_MinVelocity;
uniform vec3 u_MaxVelocity;
uniform float u_MinTotalLifetime;
uniform float u_MaxTotalLifetime;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
};

uniform mat4 u_Transform;

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f

// fract(sin(x + 0.546) * 143758.5964)
float Random1DTo1D(float value, float a, float b){
	//make value more random by making it bigger
	float random = fract(sin(value + b) * a);
    return random;
}

float Random(float value)
{
	float random = fract(sin(value + 0.546) * 143758.5964);
    return random;
}

void main()
{
	// v_Lifetime is remaining age
	float lifetime = v_Lifetime[0] - u_DeltaTime;
	if (v_Type[0] == PARTICLE_TYPE_LAUNCHER)
	{
		if (lifetime <= 0)
		{
			// launch shell
			g_Type = PARTICLE_TYPE_SHELL;
			g_Position = v_Position[0];
			g_Velocity = (u_MaxVelocity - u_MinVelocity) * Random(v_Position[0].x) + u_MinVelocity;
			
			g_Lifetime = (u_MaxTotalLifetime - u_MinTotalLifetime) * Random(v_Position[0].y) + u_MinTotalLifetime;
			float dist = sqrt(g_Position.x * g_Position.x + g_Position.z * g_Position.z);
			if (dist <= u_Radius)
				g_Lifetime *= 1.3;
			g_TotalLifetime = g_Lifetime;

			g_Color = v_Color[0];
			g_Size = v_Size[0];
			
			gl_Position = u_Projection * u_View * u_Transform * vec4(g_Position, 1.0);
			EmitVertex();
            EndPrimitive();

			lifetime = (u_MaxTotalLifetime - u_MinTotalLifetime) * Random(v_Position[0].z) + u_MinTotalLifetime;
		}

		g_Type = PARTICLE_TYPE_LAUNCHER;
		g_Position = v_Position[0];
		g_Velocity = v_Velocity[0];
		g_Lifetime = lifetime;
		g_Color = v_Color[0];
		g_Size = v_Size[0];
		g_TotalLifetime = v_TotalLifetime[0];

		gl_Position = u_Projection * u_View * u_Transform * vec4(g_Position, 1.0);
		EmitVertex();
        EndPrimitive();
	}
	else
	{
		if (lifetime >= 0)
		{
			vec3 deltaPosition = v_Velocity[0] * u_DeltaTime;
			vec3 deltaVelocity = u_DeltaTime * vec3(0.0, 1.0, 0.0);
			g_Type = PARTICLE_TYPE_SHELL;
			g_Position = v_Position[0] + deltaPosition;
			g_Velocity = v_Velocity[0] + deltaVelocity;
			g_Lifetime = lifetime;
			g_TotalLifetime = v_TotalLifetime[0];
			//float factor =  1.0f / ((lifetime - g_TotalLifetime / 2.0f) * (lifetime - g_TotalLifetime / 2.0f) + 1);
			g_Color = v_Color[0];
			g_Size = v_Size[0];

			gl_Position = u_Projection * u_View * u_Transform * vec4(g_Position, 1.0);
			EmitVertex();
        	EndPrimitive();
		}
	}
}

#type fragment
#version 450 core

layout (location = 0) in float g_Type;
layout (location = 1) in vec3 g_Position;
layout (location = 2) in vec3 g_Velocity;
layout (location = 3) in float g_Lifetime;
layout (location = 4) in vec4 g_Color;
layout (location = 5) in float g_Size;
layout (location = 6) in float g_TotalLifetime;

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;
layout (location = 2) out vec4 color3;

// uniform vec3 u_Color;
// uniform bool u_IsSelected;
// uniform int u_EntityId;
			
void main()
{
	color = g_Color;
	
	//color2 = u_EntityId;

	color2 = -1;
	color3 = vec4(0.1, 0.1, 0.1, 1);
	//if (u_IsSelected)
		//color3 = vec4(1, 1, 1, 1);
}			