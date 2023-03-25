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
		
		void NewScene();
		void OpenScene();
		void OpenScene(const std::string& path);

		void SaveScene();
		void SaveSceneAs();
		void SerializeScene(Ref<Scene> scene, const std::string& path);

		void OnScenePlay();
		void OnSceneStop();

		inline bool IsEditorMode() { return m_ViewportMode == ViewportMode::Editor; }

	private:

		EditorCamera m_EditorCamera;
		Ref<Scene> m_EditorScene;

		Entity m_RuntimeCameraEntity = {};
		Ref<Scene> m_RuntimeScene;
		
		std::string m_EditorScenePath = "";

		// Viewport framebuffer
		Ref<Framebuffer> m_Framebuffer;

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
		HierarchyPanel m_HierarchyPanel;

		// Icons
		Ref<Texture2D> m_PlayButton;
		Ref<Texture2D> m_StopButton;

		// Runtime Settings
		bool m_IsMaximizeOnPlay = true;

	};
}