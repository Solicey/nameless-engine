// sprite

#prop
#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Transform;

void main()
{
	gl_Position = u_Transform * vec4(a_Position, 1.0);
}

#type geometry
#version 450 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

layout(location = 0) out vec2 g_TexCoord;

uniform int u_CameraReaction;
uniform mat4 u_Transform;

void main()
{
	vec3 Pos = gl_in[0].gl_Position.xyz;
	vec3 forward;
	vec3 up;
	vec3 right;

	if (u_CameraReaction == 0)	// Normal
	{
		forward = normalize((u_Transform * vec4(0.0, 0.0, 1.0, 0.0)).xyz);
		up = normalize((u_Transform * vec4(0.0, 1.0, 0.0, 0.0)).xyz);
		right = normalize(cross(forward, up));
	}
	else if (u_CameraReaction == 1) // LookAt
	{
		forward = normalize(u_CameraPosition - Pos);
		right = normalize(cross(forward, vec3(0.0, 1.0, 0.0)));
		up = normalize(cross(right, forward));
	}
	else // Billboarding
	{
		forward = normalize(u_CameraPosition - Pos);
		up = vec3(0.0, 1.0, 0.0);
		right = normalize(cross(forward, up));
	}

	mat4 VP = u_Projection * u_View;

	Pos -= (right + up) * 0.5;
    gl_Position = VP * vec4(Pos, 1.0);
    g_TexCoord = vec2(1.0, 1.0);
    EmitVertex();

    Pos += up;
    gl_Position = VP * vec4(Pos, 1.0);
    g_TexCoord = vec2(1.0, 0.0);
    EmitVertex();

    Pos -= up;
    Pos += right;
    gl_Position = VP * vec4(Pos, 1.0);
    g_TexCoord = vec2(0.0, 1.0);
    EmitVertex();

    Pos += up;
    gl_Position = VP * vec4(Pos, 1.0);
    g_TexCoord = vec2(0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}

#type fragment
#version 450 core

layout (location = 0) in vec2 g_TexCoord;

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;

uniform sampler2D u_Sprite;
uniform vec4 u_Color;
uniform int u_EntityId;
			
void main()
{
	f_Color = texture2D(u_Sprite, g_TexCoord);
	if (f_Color.a <= 0)
		discard;
	f_Color = u_Color;
	
	f_EntityId = u_EntityId;

}			