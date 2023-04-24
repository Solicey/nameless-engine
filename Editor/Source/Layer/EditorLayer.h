#pragma once

#include "Panels/HierarchyPanel.h"

#include <Engine.h>
#include <ImGuizmo.h>

namespace NL
{
	enum class ViewportMode
	{
		Editor, Runtime
	};

	enum class AntiAliasingType : int
	{
		None, MSAA
	};

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(TimeStep ts);
		virtual void OnImGuiRender();
		virtual void OnEvent(Event& event);

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnRuntimeCameraSwitched(RuntimeCameraSwitchedEvent& event);
		
		void NewScene();
		void OpenScene();
		void OpenScene(const std::string& path);

		void SaveScene();
		void SaveSceneAs();
		void SerializeScene(Ref<Scene> scene, const std::string& path);

		void OnScenePlay();
		void OnSceneStop();

		inline bool IsEditorMode() { return m_ViewportMode == ViewportMode::Editor; }

		// TODO: Optimize
		void UpdateRuntimeAspect();

		void UpdateFramebuffer();

	private:

		friend class HierarchyPanel;

		EditorCamera m_EditorCamera;
		Ref<Scene> m_EditorScene;
		// ClearFlagType m_EditorCameraClearFlagType = ClearFlagType::Skybox;

		Entity m_RuntimeCameraEntity = {};
		Ref<Scene> m_RuntimeScene;
		nlm::vec2 m_RuntimeAspect = {};
		
		std::string m_EditorScenePath = "";

		// Viewport framebuffer
		Ref<Framebuffer> m_MultisampledFramebuffer;
		Ref<Framebuffer> m_Framebuffer;

		// Post-processing
		std::vector<PostProcessingType> m_EditorPostProcessingQueue;
		std::vector<PostProcessingType> m_RuntimePostProcessingQueue;
		Ref<PostProcessing> m_PostProcessing;

		// Viewport variables
		bool m_ShowViewport;
		nlm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		nlm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		Entity m_EntityHovered = {};
		ImGuizmo::OPERATION m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
		// bool m_ShowTRS;
		Entity m_TRSEntity = {};
		ViewportMode m_ViewportMode = ViewportMode::Editor;

		// Hierarchy variables
		bool m_ShowHierarchy;
		Ref<HierarchyPanel> m_HierarchyPanel;

		// Icons
		Ref<Texture2D> m_PlayButton;
		Ref<Texture2D> m_StopButton;

		// Runtime Settings
		bool m_IsMaximizeOnPlay = true;

		// Scene Settings
		/*
		* Skybox
		* Anti-Aliasing
		* Runtime PostProcessing
		*/
		bool m_ShowSceneSettings;
		int m_MSAASamples = 4;
		AntiAliasingType m_AntiAliasingType = AntiAliasingType::MSAA;
		// bool m_ShowGizmos;

		// Runtime Viewport Cursor
		bool m_IsRuntimeViewportFocused = false;
	};
}