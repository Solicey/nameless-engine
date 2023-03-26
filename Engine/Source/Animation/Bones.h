#pragma once

#include "Core/Math/Math.h"

namespace NL
{
	struct BoneInfo
	{
		nlm::mat4 Offset;
		nlm::mat4 Transformation;
		std::string Name;

		int parentID = -1;
		int ID;
	};
}