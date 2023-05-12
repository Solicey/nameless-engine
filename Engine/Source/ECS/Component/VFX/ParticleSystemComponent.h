#pragma once

#include "ECS/Component/Component.h"
#include "Resources/Material.h"
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

	enum class ParticleSpawnDistribution
	{
		Uniform, Normal
	};

	class ParticleSystemComponent : public ComponentBase
	{
	public:
		ParticleSystemComponent();
		ParticleSystemComponent(const ParticleSystemComponent& comp);
		void Init();
		void UpdateShaderProperties(const std::string& shaderName);
		void DeleteTexturesReference();

	public:
		Ref<TransformFeedback>	TFB[2];
		// Uses TFB in Pass1
		Ref<Material>			Pass1, Pass2;
		// Ref<Shader>				Pass1, Pass2;
		//Ref<Texture2D>			Tex;

		struct Property
		{
			int						Input = 0, Output = 1;
			bool					IsFirstDraw = true;
			bool					IsPaused = false;

			// Init Options
			int						LauncherNum;
			ParticleSpawnAreaShape	SpawnAreaShape;
			float					SpawnAreaRadius;
			ParticleSpawnDistribution	SpawnPositionDistribution;

			nlm::vec3				MinVelocity;
			nlm::vec3				MaxVelocity;

			float					MinTotalLifetime;
			float					MaxTotalLifetime;

			nlm::vec4				InitColor;
			float					InitSize;
		} Prop;

	};
}