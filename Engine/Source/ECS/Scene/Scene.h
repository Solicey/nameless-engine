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

	enum class SceneState
	{
		Editor, Play, Pause
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = "Entity");
		Entity CreateEntityWithID(ID id, const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		void OnStartRuntime();
		void OnStopRuntime(Scene* editorScene);
		void OnUpdateRuntime(TimeStep ts, Entity cameraEntity);

		void OnStartEditor();
		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);

		Entity GetEntityWithID(ID id);
		Entity FindEntityByName(std::string_view name);

		bool IsEditor() const { return m_SceneState == SceneState::Editor; }
		bool IsPlaying() const { return m_SceneState == SceneState::Play; }
		bool IsPaused() const { return m_SceneState == SceneState::Pause; }

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
		std::unordered_map<ID, entt::entity> m_EntityMap;
		std::vector<Scope<System>> m_Systems;
		SceneState m_SceneState = SceneState::Editor;
	};
}