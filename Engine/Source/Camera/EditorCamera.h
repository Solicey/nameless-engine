#pragma once

#include "Core/Misc/TimeStep.h"
#include "Camera/Camera.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"

namespace NL
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera();
		EditorCamera(ProjectionType type, float fovOrSize, unsigned int width, unsigned int height, float nearClip, float farClip);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline const nlm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline nlm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		inline nlm::vec3 GetUpDirection() const;
		inline nlm::vec3 GetRightDirection() const;
		inline nlm::vec3 GetForwardDirection() const;
		inline nlm::vec3 GetPosition() const { return m_Position; }
		inline nlm::quat GetOrientation() const;

		virtual void SetAspectRatio(uint32_t width, uint32_t height) override;

	private:
		void Pan(const nlm::vec2& delta);
		void Rotate(const nlm::vec2& delta);
		void Zoom(float delta);

		bool OnMouseScrollEvent(MouseScrolledEvent& e);

		void RecalculateViewMatrix();

		std::pair<float, float> PanSpeed() const;
		float RotateSpeed() const { return 0.8f; }
		float ZoomSpeed() const;

	private:
		float m_ViewportWidth = 1280.0f;
		float m_ViewportHeight = 720.0f;

		float m_CameraSpeed = 1.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		// ½¹µã
		nlm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		// ½¹¾à
		float m_Distance = 10.0f;

		nlm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		nlm::vec2 m_MousePositionLastFrame = { 0.0f, 0.0f };

		nlm::mat4 m_ViewMatrix;
	};
}