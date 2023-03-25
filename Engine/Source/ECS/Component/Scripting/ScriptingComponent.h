#pragma once

#include "ECS/Component/Component.h"
#include <string>

namespace NL
{
	class ScriptingComponent : public ComponentBase
	{
	public:
		ScriptingComponent() = default;
		ScriptingComponent(const ScriptingComponent&) = default;

		std::string ClassName;
	};
}