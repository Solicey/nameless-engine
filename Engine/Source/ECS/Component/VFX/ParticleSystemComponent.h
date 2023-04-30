#pragma once

#include "ECS/Component/Component.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Renderer/TransformFeedback.h"

namespace NL
{
	struct Particle
	{
		float Type;
		nlm::vec3 Position;
		nlm::vec3 Velocity;
		float Lifetime;
	};

	class ParticleSystemComponent : public ComponentBase
	{
	public:
		ParticleSystemComponent();

	public:
		Ref<TransformFeedback>	TFB[2];
		// Uses TFB in Pass1
		Ref<Shader>				Pass1, Pass2;

		int						Input = 0, Output = 1;
		bool					IsFirstDraw = true;
	};
}