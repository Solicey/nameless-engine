#pragma once

#include "Core/Math/Math.h"

namespace NL
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const nlm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const nlm::vec3& position)
		{
			m_Position = position; RecalculateViewMatrix();
		}

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation)
		{
			m_Rotation = rotation; 
			RecalculateViewMatrix();
		}

		const nlm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const nlm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const nlm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		nlm::mat4 m_ProjectionMatrix;
		nlm::mat4 m_ViewMatrix;
		nlm::mat4 m_ViewProjectionMatrix;

		nlm::vec3 m_Position = { 0, 0, 0 };
		float m_Rotation = 0.0f;

	};
}