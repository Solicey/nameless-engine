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

		// viewportAltered: whether viewport should be altered, usually true when mouse hovering on viewport
		void OnUpdate(TimeStep ts, bool m_ViewportHovered);
		void OnEvent(Event& e);

		inline const nlm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline nlm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		inline nlm::vec3 GetUpDirection() const;
		inline nlm::vec3 GetRightDirection() const;
		inline nlm::vec3 GetForwardDirection() const;
		inline nlm::vec3 GetPosition() const { return m_Position; }
		inline nlm::quat GetOrientation() const;
		nlm::vec3 GetPitchAndYaw() const;
		inline float GetFocalDistance() const { return m_Distance; }
		inline const nlm::vec3& GetFocalPoint() const { return m_FocalPoint; }

		// For Viewport Interactions
		bool IsMouseButtonHolding() const { return m_IsMouseButtonHolding; }

		void SetState(const nlm::vec3& pos, const nlm::vec3& py, const nlm::vec3& focalPoint);
		void SetCenter(const nlm::vec3& center);

	private:
		void Pan(const nlm::vec2& delta);
		void Rotate(const nlm::vec2& delta);
		void Zoom(float delta);

		bool OnMouseScrollEvent(MouseScrolledEvent& e);

		void RecalculateViewMatrix();

		std::pair<float, float> PanSpeed() const;
		float RotateSpeed() const { return 1.0f; }
		float ZoomSpeed() const;

	private:
		float m_CameraSpeed = 1.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		// ½¹µã
		nlm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		// ½¹¾à
		float m_Distance = 10.0f;

		nlm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		nlm::vec2 m_MousePositionLastFrame = { 0.0f, 0.0f };

		nlm::mat4 m_ViewMatrix;

		bool m_IsMouseButtonHolding = false;
	};
}