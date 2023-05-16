#use model

#prop

#end

// nlsl template shader file

#type vertex
#version 450 core

const int MAX_BONES = 100;

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec3 a_Normal;
layout(location = 5) in ivec4 a_BoneIndex;
layout(location = 6) in vec4 a_BoneWeight;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

uniform mat4 u_Transform;
uniform mat4 u_FinalBoneMatrices[MAX_BONES];
			
layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec4 v_BoneColor;
			
void main()
{
	mat4 bone_transform = u_FinalBoneMatrices[a_BoneIndex[0]] * a_BoneWeight[0];
	bone_transform += u_FinalBoneMatrices[a_BoneIndex[1]] * a_BoneWeight[1]; 
	bone_transform += u_FinalBoneMatrices[a_BoneIndex[2]] * a_BoneWeight[2]; 
	bone_transform += u_FinalBoneMatrices[a_BoneIndex[3]] * a_BoneWeight[3]; 

	v_Position = a_Position;
	v_Normal = a_Normal;
	gl_Position = u_Projection * u_View * u_Transform * bone_transform * vec4(a_Position, 1.0);

	float weight = a_BoneIndex[0] * a_BoneWeight[0] + a_BoneIndex[1] * a_BoneWeight[1] + a_BoneIndex[2] * a_BoneWeight[2] + a_BoneIndex[3] * a_BoneWeight[3];
	v_BoneColor = vec4(weight / 16.0f, weight / 16.0f, weight / 16.0f, 1.0f);
}

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec4 v_BoneColor;

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;

// uniform vec3 u_Color;
uniform int u_EntityId;

void main()
{
	// color = v_BoneColor;
	f_Color =  vec4(v_Normal.x * 0.5 + 0.5, v_Normal.y * 0.5 + 0.5, v_Normal.z * 0.5 + 0.5, 1.0); // * vec4(u_Color, 1.0);
	f_EntityId = u_EntityId;
}			