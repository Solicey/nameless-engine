#pragma once

#include "Core/Math/Math.h"

namespace NL
{

#define MAX_BONE_INFLUENCE 4

	struct Vertex
	{
		nlm::vec3	Position;
		nlm::vec2	TexCoord;
		nlm::vec3	Normal;
		nlm::vec3	Tangent;
		nlm::vec3	Bitangent;
		int			EntityID;
	};

	struct SkinnedVertex
	{
		nlm::vec3	Position;
		nlm::vec2	TexCoord;
		nlm::vec3	Normal;
		nlm::vec3	Tangent;
		nlm::vec3	Bitangent;
		int			EntityID;
		nlm::ivec4	BoneIDs;
		nlm::vec4	Weights;
		// int			BoneIDs[MAX_BONE_INFLUENCE];
		// float		Weights[MAX_BONE_INFLUENCE];
	};
}
