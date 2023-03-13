#include "nlpch.h"

#include "Core/Misc/Directives.h"
#include "EditorCamera.h"
#include "Input/Input.h"

namespace NL
{
	EditorCamera::EditorCamera()
	{
		SetAspectRatio(m_ViewportWidth, m_ViewportHeight);
		SetPerspective(45.0f, 0.1f, 1000.0f);
		RecalculateViewMatrix();
	}

	EditorCamera::EditorCamera(ProjectionType type, float fovOrSize, unsigned int width, unsigned int height, float nearClip, float farClip) : m_ViewportWidth(width), m_ViewportHeight(height)
	{
		SetAspectRatio(m_ViewportWidth, m_ViewportHeight);
		if (type == ProjectionType::Orthographic)
		{
			SetOrthographic(fovOrSize, nearClip, farClip);
		}
		else
		{
			SetPerspective(fovOrSize, nearClip, farClip);
		}
		RecalculateViewMatrix();
	}

	void EditorCamera::OnUpdate(TimeStep ts)
	{
		nlm::vec2 currMousePosition = Input::GetMousePosition();
		nlm::vec2 delta = (currMousePosition - m_MousePositionLastFrame) * 0.003f;
		m_MousePositionLastFrame = currMousePosition;

		if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
			Pan(delta);
		else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
			Rotate(delta);

		RecalculateViewMatrix();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(NL_BIND_EVENT_FN(EditorCamera::OnMouseScrollEvent));
	}

	nlm::vec3 EditorCamera::GetUpDirection() const
	{
		return nlm::rotate(GetOrientation(), nlm::vec3(0.0f, 1.0f, 0.0f));
	}

	nlm::vec3 EditorCamera::GetRightDirection() const
	{
		return nlm::rotate(GetOrientation(), nlm::vec3(1.0f, 0.0f, 0.0f));
	}

	nlm::vec3 EditorCamera::GetForwardDirection() const
	{
		return nlm::rotate(GetOrientation(), nlm::vec3(0.0f, 0.0f, -1.0f));
	}

	inline nlm::quat EditorCamera::GetOrientation() const
	{
		return nlm::quat(nlm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::SetAspectRatio(uint32_t width, uint32_t height)
	{
		Camera::SetAspectRatio(width, height);
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}

	void EditorCamera::Pan(const nlm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		// NL_ENGINE_TRACE("Pan: ({0}, {1})", delta.x, delta.y);
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::Rotate(const nlm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		// NL_ENGINE_TRACE("Rotate: ({0}, {1})", delta.x, delta.y);
		m_Yaw += yawSign * delta.x * RotateSpeed();
		m_Pitch += delta.y * RotateSpeed();
		// NL_ENGINE_TRACE("Pitch: {0}", m_Pitch);
	}

	void EditorCamera::Zoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	bool EditorCamera::OnMouseScrollEvent(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		Zoom(delta);
		RecalculateViewMatrix();
		return false;
	}

	void EditorCamera::RecalculateViewMatrix()
	{
		m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;

		nlm::quat orientation = GetOrientation();
		m_ViewMatrix = nlm::translate(nlm::mat4(1.0f), m_Position) * nlm::toMat4(orientation);
		m_ViewMatrix = nlm::inverse(m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		// magic number...
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}
}