#use model

#prop

#end

// nlsl template shader file

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

layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = 1) uniform Shadow
{
    mat4 u_LightSpaceMatrices[16];
};

void main()
{          
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = u_LightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
} 


#type fragment
#version 450 core
			
void main()
{
}			