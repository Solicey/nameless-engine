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
			
void main()
{
	float adjust_factor = 0.0;
	for (int i = 0; i < 4; i++) 
		adjust_factor += a_BoneWeight[i] * 0.25;
	mat4 bone_transform = mat4(1.0);
	if (adjust_factor > 1e-3)
	{
		bone_transform -= bone_transform;
		for (int i = 0; i < 4; i++)
		{
			if (a_BoneIndex[i] != -1)
				bone_transform += u_FinalBoneMatrices[a_BoneIndex[i]] * a_BoneWeight[i] / adjust_factor;
		}
	}

	v_Position = a_Position;
	v_Normal = a_Normal;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * u_Transform * bone_transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

out vec4 color;
out int color2;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in flat int v_EntityID;

// uniform vec3 u_Color;
			
void main()
{
	color = vec4(v_Normal.x * 0.5 + 0.5, v_Normal.y * 0.5 + 0.5, v_Normal.z * 0.5 + 0.5, 1.0); // * vec4(u_Color, 1.0);
	color2 = v_EntityID;
}			