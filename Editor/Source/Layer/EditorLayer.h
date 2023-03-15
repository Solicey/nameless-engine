#pragma once

#include <Engine.h>

namespace NL
{
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
		bool OnWindowResizeEvent(WindowResizeEvent& event);

	private:

		EditorCamera m_EditorCamera;
		Ref<Scene> m_EditorScene;

		// Viewport framebuffer
		Ref<Framebuffer> m_Framebuffer;

		// Viewport variables
		bool m_ShowViewport;
		nlm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		nlm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

	};
}