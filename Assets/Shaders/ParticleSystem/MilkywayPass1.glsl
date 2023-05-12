#use particle1

#prop
float u_AlphaBias;
float u_MaxHeight;
float u_MaxRadius;
float u_MinRadius;
float u_GravitationalConst;
float u_MaxCelestialRadius;
float u_LuminanceThreshold;
sampler2D u_Milkyway;
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
uniform float u_AlphaBias;
uniform float u_MaxHeight;
uniform float u_MaxRadius;
uniform float u_MinRadius;
uniform float u_GravitationalConst;
uniform float u_MaxCelestialRadius;
uniform float u_LuminanceThreshold;
uniform sampler2D u_Milkyway;

#define PARTICLE_TYPE_LAUNCHER 0.0
#define PARTICLE_TYPE_SHELL 1.0
#define PI 3.1415926535897932384626433832795

// 0-1
float Random(float value)
{
	float random = fract(sin(value + 0.546) * 143758.5964);
    return random;
}

float luminance(vec3 color)
{
	return 0.2125 * color.r + 0.7154 * color.g + 0.0721 * color.b;
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
		if (r >= u_MaxRadius)
			r = Random(z - x + y) * u_MaxRadius;

		y = y + (Random(x - z + r) - 0.5) * 2 * u_MaxHeight * (u_MaxRadius - r) / u_MaxRadius;
		float theta = asin(z / r) * 2;

		g_Position = vec3(r, theta, y);
		g_Lifetime = v_Lifetime[0];

		g_Size = v_Size[0] * Random(x - y - z) * u_MaxCelestialRadius;

		if (r > u_MinRadius)
			g_Velocity = vec3(0, sqrt(u_GravitationalConst / r) / r, 0);
		else
			g_Velocity = vec3(0, sqrt(u_GravitationalConst / u_MinRadius) / u_MinRadius, 0);

		g_TotalLifetime = v_TotalLifetime[0];

		float u = (r / u_MaxRadius * cos(theta)) * 0.5 + 0.5;
		float v = (r / u_MaxRadius * sin(theta)) * 0.5 + 0.5;
		g_Color = texture2D(u_Milkyway, vec2(u, v));
		float lumin = luminance(g_Color.rgb) + (Random(x + y - z) * 2 - 1) * u_AlphaBias ;
		if (lumin < u_LuminanceThreshold)
			return;
		//if (r < u_MinRadius)
			//lumin =  lumin * 0.4;
		g_Color.a = lumin * ((r + u_MinRadius) / (u_MaxRadius + u_MinRadius));
		// float lerp1 = Random(x + y - z), lerp2 = Random(x - y + z) * (1 - lerp1);
		// g_Color = vec4(lerp1 * u_Color1 + lerp2 * u_Color2 + (1 - lerp1 - lerp2) * u_Color3, min(Random(- x + y - z) * r / u_MaxRadius, 1));

		gl_Position = u_Projection * u_View * u_Transform * vec4(r * cos(theta), y, r * sin(theta), 1.0);
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
	