#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "ECS/Component/Component.h"
#include "Resources/Libraries/TextureLibrary.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "ECS/System/Render/RenderSystem.h"

#include "Renderer/Buffers.h"
#include "Renderer/VertexArray.h"

namespace NL
{
	struct Particle
	{
		float		Type;
		nlm::vec3	Position;
		nlm::vec3	Velocity;
		float		LifetimeMillis;
	};

	// TODO: GC
	class ParticleSystemComponent : public ComponentBase
	{
	public:
		friend class RenderSystem;

		ParticleSystemComponent() = default;
		ParticleSystemComponent(const ParticleSystemComponent& comp) = default;
		ParticleSystemComponent(const std::string& texPath, const std::string& shaderName)
			: _Texture(Library<Texture2D>::GetInstance().Fetch(texPath)), _Shader(Library<Shader>::GetInstance().Fetch(shaderName)) 
		{
			Init();
		}

	private:
		void Init();

	public:
		// GC
		Ref<Texture2D>	_Texture;
		Ref<Shader>		_Shader;

		std::vector<Particle>	Particles;

	private:
		Ref<VertexArray>				m_VAO;
		Ref<TransformFeedbackBuffer>	m_TFB[2];
	};
}