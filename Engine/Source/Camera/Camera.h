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
		Camera(int type, float pFov, float pNear, float pFar, float oSize, float oNear, float oFar, uint32_t width, uint32_t height) : m_ProjectionType((ProjectionType)type), m_PerspectiveFOV(pFov), m_PerspectiveNear(pNear), m_PerspectiveFar(pFar), m_OrthographicSize(oSize), m_OrthographicFar(oFar), m_OrthographicNear(oNear), m_ViewportWidth(width), m_ViewportHeight(height) { ReCalculateAspectRatio();  RecalculateProjectionMatrix(); }

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		void SetAspectRatio(uint32_t width, uint32_t height);
		float GetAspectRatio() const { return m_AspectRatio; }

		const nlm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjectionMatrix();
		}
		
		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjectionMatrix(); }

		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		void SetPerspectiveNear(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjectionMatrix();
		}

		float GetPerspectiveFar() const { return m_PerspectiveFar; }
		void SetPerspectiveFar(float farClip) { m_PerspectiveFar = farClip; RecalculateProjectionMatrix();
		}

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjectionMatrix();
		}

		float GetOrthographicNear() const { return m_OrthographicNear; }
		void SetOrthographicNear(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjectionMatrix();
		}

		float GetOrthographicFar() const { return m_OrthographicFar; }
		void SetOrthographicFar(float farClip) { m_OrthographicFar = farClip; RecalculateProjectionMatrix();
		}

		uint32_t GetViewportWidth() const { return m_ViewportWidth; }
		void SetViewportWidth(uint32_t width) { m_ViewportWidth = width; ReCalculateAspectRatio(); }

		uint32_t GetViewportHeight() const { return m_ViewportHeight; }
		void SetViewportHeight(uint32_t height) { m_ViewportHeight = height; ReCalculateAspectRatio(); }


	private:
		void RecalculateProjectionMatrix();		
		void ReCalculateAspectRatio() 
		{ 
			m_AspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight; 
			RecalculateProjectionMatrix();
		}

	protected:
		nlm::mat4 m_ProjectionMatrix = nlm::mat4(1.0f);

		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = 0.01f, m_OrthographicFar = 100.0f;

		float m_PerspectiveFOV = nlm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 1.7778f;
		uint32_t m_ViewportWidth = 1280;
		uint32_t m_ViewportHeight = 720;
	};
}