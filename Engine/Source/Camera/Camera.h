#pragma once

#include "Core/Math/Math.h"

namespace NL
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const nlm::mat4& projection) 
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const nlm::mat4& GetProjection() const { return m_Projection; }

	private:
		nlm::mat4 m_Projection = nlm::mat4(1.0f);
	};
}