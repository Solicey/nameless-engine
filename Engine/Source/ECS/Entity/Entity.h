#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Core/Log/Log.h"
#include "ECS/Scene/Scene.h"
#include "ECS/Component/Component.h"

#include <entt/entt.hpp>
#include <tuple>

namespace NL
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);

		template<Component C, typename... Args>
		C& AddComponent(Args&&... args)
		{
			NL_ENGINE_ASSERT(!HasComponent<C>(), "Entity already has this component!");
			C& component = m_Scene->m_Registry.emplace<C>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<C>(*this, component);
			return component;
		}

		template<Component C, typename... Args>
		C& AddOrReplaceComponent(Args&&... args)
		{
			C& component = m_Scene->m_Registry.emplace_or_replace<C>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<C>(*this, component);
			return component;
		}

		template<Component C>
		C& GetComponent()
		{
			NL_ENGINE_ASSERT(HasComponent<C>(), "Entity does NOT have this component!");
			return m_Scene->m_Registry.get<C>(m_EntityHandle);
		}

		template<Component... C>
		std::tuple<C&...> GetComponents()
		{
			NL_ENGINE_ASSERT((HasComponent<C>() && ...), "Entity does NOT have some of these components!");
			return std::make_tuple<C&...>((m_Scene->m_Registry.get<C>(m_EntityHandle))...);
		}

		template<Component... C>
		std::tuple<const C& const...> GetConstComponents() const
		{
			NL_ENGINE_ASSERT((HasComponent<C>() && ...), "Entity does NOT have some of these components!");
			return std::make_tuple<const C& const...>((m_Scene->m_Registry.get<C>(m_EntityHandle))...);
		}

		template<Component C>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<C>(m_EntityHandle);
		}

		template<Component C>
		void RemoveComponent()
		{
			NL_ENGINE_ASSERT(HasComponent<C>(), "Entity does NOT have this component!");
			m_Scene->OnComponentRemoved<C>(*this, this->GetComponent<C>());
			m_Scene->m_Registry.remove<C>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		bool operator == (const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}
		bool operator != (const Entity& other) const
		{
			return !(*this == other);
		}

		ID GetID();
		const std::string& GetName();

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
	
}