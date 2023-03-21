#pragma once

#include "ECS/Component/AllComponents.h"
#include "Core/Misc/PtrWrapper.h"
#include "Core/Misc/TimeStep.h"
#include "Core/Log/Log.h"
#include "Camera/EditorCamera.h"

#include <entt/entt.hpp>

namespace NL
{
	class Entity;
	class System;

	class Scene
	{
	public:
		Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		Entity CreateEntityWithID(ID id, const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		void OnStartRuntime();
		void OnStopRuntime();
		void OnUpdateRuntime(TimeStep ts);
		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);

	private:
		template<Component C>
		void OnComponentAdded(Entity entity, C& component);
		template<Component C>
		void OnComponentRemoved(Entity entity, C& component);

	public:
		entt::registry m_Registry;

	private:
		friend class Entity;
		friend class System;

	private:
		std::vector<Scope<System>> m_Systems;

	};
}