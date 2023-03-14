#include "nlpch.h"

#include "Entity.h"

#include "ECS/Component/Basic/IdentityComponent.h"

namespace NL
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

	ID Entity::GetID()
	{
		return GetComponent<IdentityComponent>().ID;
	}

	const std::string& Entity::GetName()
	{
		return GetComponent<IdentityComponent>().Name;
	}
}