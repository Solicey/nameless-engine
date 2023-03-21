#pragma once

#include "Core/Math/Math.h"

namespace NL
{
	class Camera
	{
	public:
		enum class ProjectionType
		{
			Orthographic,
			Perspective
		};

	public:
		Camera() { RecalculateProjectionMatrix(); }
		virtual ~Camera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		virtual void SetAspectRatio(uint32_t width, uint32_t height);

		const nlm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		void RecalculateProjectionMatrix();

	protected:
		nlm::mat4 m_ProjectionMatrix = nlm::mat4(1.0f);

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = nlm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 0.0f;
	};
}