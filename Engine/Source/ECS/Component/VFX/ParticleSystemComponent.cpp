#include "nlpch.h"
#include "ParticleSystemComponent.h"

namespace NL
{
	ParticleSystemComponent::ParticleSystemComponent()
	{
		Pass1 = Library<Shader>::GetInstance().Fetch("SingleParticle.glsl");

		std::vector<Particle> particles;

		Particle p;
		p.Type = 0;
		p.Position = nlm::vec3(0);
		p.Velocity = nlm::vec3(0);
		p.Lifetime = 0;

		particles.push_back(p);

		BufferLayout layout = {
			{ShaderDataType::Float, "a_Type"},
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Velocity"},
			{ShaderDataType::Float, "a_Lifetime"},
		};

		for (int i = 0; i < 2; i++)
		{
			TFB[i] = TransformFeedback::Create();
			
			Ref<VertexBuffer> vbo = VertexBuffer::Create((void*)particles.data(), particles.size() * layout.GetStride());
			vbo->SetLayout(layout);
			TFB[i]->SetVertexBuffer(vbo);
		}

	}
}
