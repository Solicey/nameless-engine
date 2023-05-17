#pragma once

#include "ECS/Component/AllComponents.h"
#include "Core/Misc/PtrWrapper.h"
#include "Core/Misc/TimeStep.h"
#include "Core/Log/Log.h"
#include "Camera/EditorCamera.h"
#include "Renderer/Renderer.h"

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
		friend class Entity;
		friend class System;

		Scene();
		~Scene();

		static Ref<Scene> DuplicateScene(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = "Entity");
		Entity CreateEntityWithID(ID id, const std::string& name = "Entity");
		Entity DuplicateEntity(Entity entity);
		// Called when entity is deleted.
		void DestroyEntity(Entity entity);
		// Must be called every time a scene is deleted!
		void DestroyScene();

		void OnStartRuntime();
		void OnStopRuntime(Scene* editorScene);
		void OnUpdateRuntime(TimeStep ts, bool isRuntimeViewportFocused);

		void OnStartEditor();
		void OnUpdateEditor(TimeStep ts);

		Entity GetEntityWithID(ID id);
		Entity FindEntityByName(std::string_view name);

		bool IsEditor() const { return m_SceneState == SceneState::Editor; }
		bool IsPlaying() const { return m_SceneState == SceneState::Play; }
		bool IsPaused() const { return m_SceneState == SceneState::Pause; }

		static void SetRuntimeCamera(const std::string& str) { s_RuntimeCameraName = str; }
		static const std::string& GetRuntimeCamera() { return s_RuntimeCameraName; }

		std::vector<PointLightShadingData>& GetPointLightShadingDataNotConst() { return m_PointLightShadingDatas; }
		std::vector<DirLightShadingData>& GetDirLightShadingDataNotConst() { return m_DirLightShadingDatas; }
		// Should only have one
		Entity GetSettingsEntity();
		// Called by RenderSystem
		void PackUpLightDatas();

	private:
		template<Component C>
		void OnComponentAdded(Entity entity, C& component);
		template<Component C>
		void OnComponentRemoved(Entity entity, C& component);
		
	public:
		entt::registry Registry;		

	private:
		std::unordered_map<ID, entt::entity> m_EntityMap;
		std::vector<Scope<System>> m_Systems;
		SceneState m_SceneState = SceneState::Editor;

		// Modified by C# Scripts
		static std::string s_RuntimeCameraName;

		// Environment Settings
		// Default
		std::vector<std::string> m_SkyboxTextures;
		std::vector<PointLightShadingData> m_PointLightShadingDatas;
		std::vector<DirLightShadingData> m_DirLightShadingDatas;
	};
}