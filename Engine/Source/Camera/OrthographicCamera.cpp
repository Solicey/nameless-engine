#include "nlpch.h"

#include "OrthographicCamera.h"

namespace NL
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(nlm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		nlm::mat4 transform = nlm::translate(nlm::mat4(1.0f), m_Position) * nlm::rotate(nlm::mat4(1.0f), nlm::radians(m_Rotation), nlm::vec3(0, 0, 1));

		m_ViewMatrix = nlm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}