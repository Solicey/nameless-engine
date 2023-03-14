#pragma once

#include "ECS/Component/Component.h"
#include "ECS/Entity/ID.h"

namespace NL
{
	class IdentityComponent : public ComponentBase
	{
	public:
		IdentityComponent() = default;
		IdentityComponent(const IdentityComponent&) = default;
		IdentityComponent(const ID& id, const std::string& name)
			: ID(id), Name(name) {}

		ID				ID;
		std::string		Name;
	};
}