// sprite

#prop
#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 1) in vec3 a_Position;
layout (location = 4) in vec4 a_Color;
layout (location = 5) in float a_Size;

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
};

layout(location = 0) out vec2 g_TexCoord;

uniform mat4 u_Transform;

void main()
{
	vec3 Pos = gl_in[0].gl_Position.xyz;

	vec3 forward = normalize(u_CameraPosition - Pos);
	vec3 right = normalize(cross(forward, vec3(0.0, 1.0, 0.0)));
	vec3 up = normalize(cross(right, forward));

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

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;
layout (location = 2) out vec4 color3;

uniform sampler2D u_Sprite;
// uniform vec4 u_Color;
// uniform bool u_IsSelected;
// uniform int u_EntityId;
			
void main()
{
	color = texture2D(u_Sprite, g_TexCoord);
	if (color.a <= 0)
		discard;
	
	color2 = -1;

	color3 = vec4(0.1, 0.1, 0.1, 1);
	// if (u_IsSelected)
		// color3 = vec4(1, 1, 1, 1);
}			