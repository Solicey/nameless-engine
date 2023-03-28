#pragma once

#include "Core/Math/Math.h"

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

		nlm::vec3 WorldPosition;
		nlm::vec3 WorldDirection;
	};
}