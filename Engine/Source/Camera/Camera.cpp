#include "nlpch.h"

#include "Camera.h"

namespace NL
{
	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjectionMatrix();
	}

	void Camera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjectionMatrix();
	}

	void Camera::SetAspectRatio(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		RecalculateProjectionMatrix();
	}

	void Camera::RecalculateProjectionMatrix()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_ProjectionMatrix = nlm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
		else
		{
			m_ProjectionMatrix = nlm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
	}
}