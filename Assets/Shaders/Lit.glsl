// nlsl template shader file

#use model

#prop

color3 u_Color;
float u_AmbientStrength;
color3 u_Test;

#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_View;
uniform mat4 u_Projection;
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

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;
layout (location = 2) out vec4 color3;

// Object Color
uniform vec3 u_Color;
uniform bool u_IsSelected;
uniform int u_EntityId;
uniform float u_AmbientStrength;
uniform vec3 u_Test;

#define MAX_LIGHT_COUNT 4

struct PointLight
{
	vec3 Position;
	vec3 Color;
};
uniform PointLight u_PointLights[MAX_LIGHT_COUNT];

struct DirLight
{
	vec3 Direction;
	vec3 Color;
};
uniform DirLight u_DirLights[MAX_LIGHT_COUNT];
			
void main()
{
	if (u_DirLights[0].Color.r >= 0)
	{
		vec3 ambient = u_AmbientStrength * u_DirLights[0].Color;
		vec3 result = ambient * u_Color;
		color = vec4(result, 1.0);
	}
	else
	{
		color = vec4(u_Color, 1.0);
	}

	// color = vec4(1, 1, 1, 1.0);

	// Dont Modify
	color2 = u_EntityId;
	color3 = vec4(0.1, 0.1, 0.1, 1);
	if (u_IsSelected)
		color3 = vec4(1, 1, 1, 1);
}			