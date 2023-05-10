#use particle1

#prop
float u_MaxHeight;
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
};

uniform mat4 u_Transform;
uniform float u_DeltaTime;
uniform float u_MaxHeight;

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PI 3.1415926535897932384626433832795

// 0-1
float Random(float value)
{
	float random = fract(sin(value + 0.546) * 143758.5964);
    return random;
}

void main()
{
	if (v_Type[0] == PARTICLE_TYPE_LAUNCHER)
	{
		g_Type = PARTICLE_TYPE_SHELL;
		float x = v_Position[0].x;
		float y = v_Position[0].y;
		float z = v_Position[0].z;
		float r = sqrt(x * x + z * z);
		if (r <= 0.0)
			return;

		y = y + (Random(x * z) * 2.0 - 1.0) * u_MaxHeight;
		float theta = acos(x / r);

		g_Position = vec3(r, theta, y);

		g_Velocity = v_Velocity[0];
		g_Lifetime = v_Lifetime[0];
		g_Color = v_Color[0];
		g_Size = Random(y * z) * 1.3;
		g_TotalLifetime = v_TotalLifetime[0];

		gl_Position = u_Projection * u_View * u_Transform * vec4(x, y, z, 1.0);
		EmitVertex();
        EndPrimitive();
	}
	else
	{
		g_Type = v_Type[0];
		g_Velocity = v_Velocity[0];
		g_Lifetime = v_Lifetime[0];
		g_Color = v_Color[0];
		g_Size = v_Size[0];
		g_TotalLifetime = v_TotalLifetime[0];

		float r = v_Position[0].x;
		float theta = v_Position[0].y;
		float y = v_Position[0].z;
		theta = theta + g_Velocity.y * u_DeltaTime;

		if (theta > 2 * PI)
			theta = theta - 2 * PI;
		
		g_Position = vec3(r, theta, y);
		gl_Position = u_Projection * u_View * u_Transform * vec4(r * cos(theta), y, r * sin(theta), 1.0);
		EmitVertex();
    	EndPrimitive();
	}	
}
	