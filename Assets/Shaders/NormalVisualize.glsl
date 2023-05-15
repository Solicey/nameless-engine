// Normal Visualize, use Geometry Shader

#use model

#prop
color3 u_Color;
float u_Magnitude;
#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec3 a_Normal;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
    vec3 u_CameraPosition;
};

uniform mat4 u_Transform;
			
out VS_OUT
{
	vec3 normal;
} vs_out;
			
void main()
{
	gl_Position = u_View * u_Transform * vec4(a_Position, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(u_View * u_Transform)));
	vs_out.normal = normalize(vec3(vec4(normalMatrix * a_Normal, 0.0)));
}

#type geometry
#version 450 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
    vec3 u_CameraPosition;
};

uniform float u_Magnitude;

void GenerateLine(int index)
{
    gl_Position = u_Projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = u_Projection * (gl_in[index].gl_Position + 
                                vec4(gs_in[index].normal, 0.0) * u_Magnitude);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); 
    GenerateLine(1); 
    GenerateLine(2); 
}

#type fragment
#version 450 core

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;

uniform vec3 u_Color;
uniform bool u_IsSelected;
uniform int u_EntityId;
			
void main()
{
	f_Color = vec4(u_Color, 1.0);
    f_EntityId = u_EntityId;
}			