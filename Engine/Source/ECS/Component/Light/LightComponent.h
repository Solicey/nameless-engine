#pragma once

#include "ECS/Component/Component.h"
#include "Core/Math/Math.h"

namespace NL
{
	enum class LightType
	{
		Directional, Point
	};

	class LightComponent : public ComponentBase
	{
	public:
		LightComponent() = default;
		LightComponent(const LightComponent&) = default;
		LightComponent(LightType type, const nlm::vec3& color, float intensity)
			: Type(type), Color(color), Intensity(intensity) {}

		LightType Type;
		nlm::vec3 Color = nlm::vec3(1.0f, 1.0f, 1.0f);
		float Intensity = 1.0f;
		nlm::vec3 Attenuation = nlm::vec3(1.0f, 0.7f, 1.8f);
	};
}