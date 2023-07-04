#use model

#prop
color3 u_Color;
#end

// nlsl template shader file

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec3 a_Normal;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

uniform mat4 u_Transform;
uniform mat3 u_NormalMatrixVS;
uniform mat3 u_NormalMatrixWS;
			
layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;	// Normal VS
layout (location = 2) out vec3 v_NormalWS;
			
void main()
{
	// G buffer under view space
	v_Position = vec3(u_View * u_Transform * vec4(a_Position, 1.0));
	//v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	//mat3 normalMatrix = transpose(inverse(mat3(u_View * u_Transform)));
	v_Normal = normalize(u_NormalMatrixVS * a_Normal);
	v_NormalWS = normalize(u_NormalMatrixWS * a_Normal);
	//v_Normal = normalize(a_Normal);
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_NormalWS;

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;
layout (location = 2) out vec4 f_PositionDepth;
layout (location = 3) out vec4 f_Normal;

uniform vec3 u_Color;
uniform int u_EntityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

float LinearizeDepth(float depth, float near, float far)
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}
			
void main()
{
	f_Color = vec4(u_Color, 1.0); // * vec4(u_Color, 1.0);
	f_EntityId = u_EntityId;

	f_PositionDepth = vec4(v_Position, LinearizeDepth(gl_FragCoord.z, u_Near, u_Far));//LinearizeDepth(depth, u_Near, u_Far));
	f_Normal = vec4(v_Normal, 1.0);
}			