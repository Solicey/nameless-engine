#pragma once

#include "Core/Math/Math.h"
#include "Core/Misc/PtrWrapper.h"

#include <set>

namespace NL
{
	struct BoneInfo
	{
		nlm::mat4 Offset = nlm::mat4(1.0f);
		nlm::mat4 Transformation = nlm::mat4(1.0f);
		std::string Name;

		int parentID = -1;
		int ID;
		std::set<int> Childrens;

		nlm::vec3 LocalOffset;
		nlm::quat LocalRotation;
		nlm::vec3 WorldPosition;
		nlm::quat WorldRotation;
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