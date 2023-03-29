#pragma once

#include "Core/Math/Math.h"
#include "Core/Misc/PtrWrapper.h"

#include <set>
#include <assimp/matrix4x4.h>

namespace NL
{
	struct BoneInfo
	{
		aiMatrix4x4 Offset;
		aiMatrix4x4 Transformation;
		aiMatrix4x4 RootPreRotation;

		nlm::vec3 Rotation = nlm::vec3(0.0f);
		std::string Name;

		int parentID = -1;
		int ID;
		std::set<int> Childrens;

	};

	struct ChainInfo
	{
		std::vector<BoneInfo*>		Bones;
		int							ID;
		bool						IsRootFixed = true;
		int							Root;

		nlm::vec3					TipLocalOffset;
		nlm::quat					TipLocalRotation;
		nlm::vec3					TipWorldPosition;
		nlm::quat					TipWorldRotation;
	};
}