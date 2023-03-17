#pragma once

#include "Core/Math/Math.h"

namespace NL
{
	struct Vertex
	{
		nlm::vec3	Position;
		nlm::vec2	TexCoord;
		nlm::vec3	Normal;
		nlm::vec3	Tangent;
		nlm::vec3	Bitangent;
		int			EntityID;
	};
}
