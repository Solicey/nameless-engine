#include "nlpch.h"

#include "ParticleSystemComponent.h"

namespace NL
{
	void ParticleSystemComponent::Init()
	{
		Particle p;
		p.Type = 0; // Spawner
		p.Position = nlm::vec3(0.0);
		p.Velocity = nlm::vec3(0.0f, 0.0001f, 0.0f);
		p.LifetimeMillis = 0.0f;

		Particles.clear();
		Particles.push_back(p);

		BufferLayout layout = {
			{ShaderDataType::Float, "a_Type"},
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Velocity"},
			{ShaderDataType::Float, "a_Age"}
		};

		m_VAO = VertexArray::Create();

		for (int i = 0; i < 2; i++)
		{
			m_TFB[i] = TransformFeedbackBuffer::Create();

			Ref<VertexBuffer> vbo = VertexBuffer::Create((void*)const_cast<Particle*>(Particles.data()), Particles.size() * layout.GetStride(), BufferAccessFrequency::Dynamic, BufferAccessNature::Draw);
			vbo->SetLayout(layout);
			m_VAO->AddVertexBuffer(vbo);

			m_TFB[i]->BindBufferBase(vbo);
		}
	}
}
