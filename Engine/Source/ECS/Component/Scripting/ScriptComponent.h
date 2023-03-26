#pragma once

#include "ECS/Component/Component.h"
#include <string>

namespace NL
{
	class ScriptComponent : public ComponentBase
	{
	public:
		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;

		std::string ClassName;
	};
}