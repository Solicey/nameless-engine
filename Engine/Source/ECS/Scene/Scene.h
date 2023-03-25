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

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = "Entity");
		Entity CreateEntityWithID(ID id, const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		void OnStartRuntime();
		void OnStopRuntime();
		void OnUpdateRuntime(TimeStep ts, Camera* camera);
		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);

		void OnViewportResize(uint32_t width, uint32_t height);
		
		/*std::unordered_map<Entity, Ref<Camera>>& GetRuntimeCameraMapNotConst() { return m_RuntimeCameraMap; }
		bool ContainRuntimeCamera(const Entity& entity) const
		{
			return m_RuntimeCameraMap.contains(entity);
		}
		// Should call ContainRuntimeCamera(entity) first
		Ref<Camera> GetRuntimeCamera(const Entity& entity)
		{
			return m_RuntimeCameraMap.at(entity);
		}
		Ref<Camera> GetAnyRuntimeCamera()
		{
			if (!m_RuntimeCameraMap.empty())
			{
				auto iter = m_RuntimeCameraMap.begin();
				return (*iter).second;
			}
			return nullptr;
		}*/

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