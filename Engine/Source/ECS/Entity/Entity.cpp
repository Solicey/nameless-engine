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
		if (HasComponent<IdentityComponent>())
			return GetComponent<IdentityComponent>().ID;
		return ID();
	}

	const std::string& Entity::GetName()
	{ 
		if (HasComponent<IdentityComponent>())
			return GetComponent<IdentityComponent>().Name;
		return "";
	}
}