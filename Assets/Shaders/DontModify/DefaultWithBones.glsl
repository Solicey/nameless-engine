#prop

#end

// nlsl template shader file

#type vertex
#version 450 core

const int MAX_BONES = 100;

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec3 a_Normal;
layout(location = 5) in int a_EntityID;
layout(location = 6) in ivec4 a_BoneIndex;
layout(location = 7) in vec4 a_BoneWeight;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_FinalBoneMatrices[MAX_BONES];
			
layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out flat int v_EntityID;
layout (location = 3) out vec4 v_BoneColor;
			
void main()
{
	mat4 bone_transform = u_FinalBoneMatrices[a_BoneIndex[0]] * a_BoneWeight[0];
	bone_transform += u_FinalBoneMatrices[a_BoneIndex[1]] * a_BoneWeight[1]; 
	bone_transform += u_FinalBoneMatrices[a_BoneIndex[2]] * a_BoneWeight[2]; 
	bone_transform += u_FinalBoneMatrices[a_BoneIndex[3]] * a_BoneWeight[3]; 

	v_Position = a_Position;
	v_Normal = a_Normal;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * u_Transform * bone_transform * vec4(a_Position, 1.0);

	float weight = a_BoneIndex[0] * a_BoneWeight[0] + a_BoneIndex[1] * a_BoneWeight[1] + a_BoneIndex[2] * a_BoneWeight[2] + a_BoneIndex[3] * a_BoneWeight[3];
	v_BoneColor = vec4(weight / 16.0f, weight / 16.0f, weight / 16.0f, 1.0f);
}

#type fragment
#version 450 core

out vec4 color;
out int color2;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in flat int v_EntityID;
layout (location = 3) in vec4 v_BoneColor;

// uniform vec3 u_Color;
			
void main()
{
	// color = v_BoneColor;
	color =  vec4(v_Normal.x * 0.5 + 0.5, v_Normal.y * 0.5 + 0.5, v_Normal.z * 0.5 + 0.5, 1.0); // * vec4(u_Color, 1.0);
	color2 = v_EntityID;
}			