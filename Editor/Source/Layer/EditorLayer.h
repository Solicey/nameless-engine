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
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;

		Ref<Shader> m_NormalShader;
		Ref<VertexArray> m_SquareVA;

		Ref<Model> m_Box;
		Ref<Model> m_Sphere;
		Ref<Model> m_CameraModel;

		EditorCamera m_EditorCamera;
		Ref<Scene> m_EditorScene;

	};
}