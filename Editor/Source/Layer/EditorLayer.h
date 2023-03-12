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

	private:
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;

		Ref<Shader> m_NormalShader;
		Ref<VertexArray> m_SquareVA;

		Ref<Model> m_Box;
		Ref<Model> m_Sphere;
		Ref<Model> m_CameraModel;

		OrthographicCamera m_Camera;
		nlm::vec3 m_CameraPosition;
		float m_CameraTranslateSpeed = 5.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotateSpeed = 180.0f;

		nlm::vec3 m_SquarePosition;
		float m_SquareTranslateSpeed = 2.0f;
	};
}