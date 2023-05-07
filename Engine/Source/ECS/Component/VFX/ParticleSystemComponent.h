#pragma once

#include "ECS/Component/Component.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Resources/Libraries/TextureLibrary.h"
#include "Renderer/TransformFeedback.h"

namespace NL
{
	struct Particle
	{
		float Type;
		nlm::vec3 Position;
		nlm::vec3 Velocity;
		float Lifetime;

		nlm::vec4 Color;
		float Size;
		float TotalLifetime;
	};

	enum class ParticleSpawnAreaShape
	{
		Point, Circle, Sphere
	};

	class ParticleSystemComponent : public ComponentBase
	{
	public:
		ParticleSystemComponent();

	private:
		void Init();

	public:
		Ref<TransformFeedback>	TFB[2];
		// Uses TFB in Pass1
		Ref<Shader>				Pass1, Pass2;
		Ref<Texture2D>			Tex;

		int						Input = 0, Output = 1;
		bool					IsFirstDraw = true;

		// Init Options
		int						LauncherNum;
		ParticleSpawnAreaShape	SpawnAreaShape;
		float					SpawnAreaRadius;

		nlm::vec3				MinVelocity;
		nlm::vec3				MaxVelocity;

		float					MinTotalLifetime;
		float					MaxTotalLifetime;

		nlm::vec4				InitColor;
		float					InitSize;
	};
}