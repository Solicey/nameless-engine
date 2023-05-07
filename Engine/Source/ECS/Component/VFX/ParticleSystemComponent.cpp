#include "nlpch.h"
#include "ParticleSystemComponent.h"

namespace NL
{
	ParticleSystemComponent::ParticleSystemComponent()
	{
		for (int i = 0; i < 2; i++)
		{
			TFB[i] = TransformFeedback::Create();
		}

		Pass1 = Library<Shader>::GetInstance().Fetch("Fire.glsl");
		Pass2 = Library<Shader>::GetInstance().Fetch("ParticleSprite.glsl");

		std::string assetFolder = PathConfig::GetInstance().GetAssetsFolder().string();
		Tex = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Textures/DontModify/DefaultParticle.png");

		LauncherNum = 10;
		SpawnAreaShape = ParticleSpawnAreaShape::Circle;
		MinVelocity = nlm::vec3(0.0f, 0.2f, 0.0f);
		MaxVelocity = nlm::vec3(0.0f, 2.0f, 0.0f);
		MaxTotalLifetime = 5;
		MinTotalLifetime = 3;
		SpawnAreaRadius = 0.3f;
		InitSize = 1;
		InitColor = nlm::vec4(1.0f);

		Init();
	}

	void ParticleSystemComponent::Init()
	{
		static int randomTime = 10;

		std::vector<Particle> particles;
		nlm::vec3 DelVelocity = MaxVelocity - MinVelocity;
		float DelTotalLifetime = MaxTotalLifetime - MinTotalLifetime;

		for (int i = 0; i < LauncherNum; i++)
		{
			Particle particle;
			nlm::vec3 pos(0.0f);

			switch (SpawnAreaShape)
			{
			case NL::ParticleSpawnAreaShape::Point:
				break;
			case NL::ParticleSpawnAreaShape::Circle:
			{
				for (int j = 0; j < randomTime; j++)
				{
					pos.x += (2.0 * float(rand()) / float(RAND_MAX) - 1.0f);
					pos.z += (2.0 * float(rand()) / float(RAND_MAX) - 1.0f);
				}
				pos.x *= SpawnAreaRadius;
				pos.z *= SpawnAreaRadius;
				break;
			}
			case NL::ParticleSpawnAreaShape::Sphere:
			{
				for (int j = 0; j < randomTime; j++)
				{
					pos.x += (2.0 * float(rand()) / float(RAND_MAX) - 1.0f);
					pos.y += (2.0 * float(rand()) / float(RAND_MAX) - 1.0f);
					pos.z += (2.0 * float(rand()) / float(RAND_MAX) - 1.0f);
				}
				pos *= SpawnAreaRadius;
				break;
			}
			default:
				break;
			}

			particle.Type = 0;
			particle.Position = pos;
			
			nlm::vec3 vel;
			vel.x = DelVelocity.x * (float(rand()) / float(RAND_MAX)) + MinVelocity.x;
			vel.y = DelVelocity.y * (float(rand()) / float(RAND_MAX)) + MinVelocity.y;
			vel.z = DelVelocity.z * (float(rand()) / float(RAND_MAX)) + MinVelocity.z;
			particle.Velocity = vel;

			particle.Size = InitSize;
			particle.Color = InitColor;

			particle.TotalLifetime = DelTotalLifetime * (float(rand()) / float(RAND_MAX)) + MinTotalLifetime;
			particle.Lifetime = particle.TotalLifetime;

			particles.push_back(particle);

			//NL_ENGINE_INFO("Spawn Particle {0}, pos: ({1}, {2}, {3})", i, pos.x, pos.y, pos.z);
		}

		BufferLayout layout = {
			{ShaderDataType::Float, "a_Type"},
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Velocity"},
			{ShaderDataType::Float, "a_Lifetime"},

			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float, "a_Size"},
			{ShaderDataType::Float, "a_TotalLifetime"},
		};

		for (int i = 0; i < 2; i++)
		{
			Ref<VertexBuffer> vbo = VertexBuffer::Create((void*)particles.data(), particles.size() * layout.GetStride(), BufferUsage::DynamicDraw);
			vbo->SetLayout(layout);
			TFB[i]->SetVertexBuffer(vbo);
		}

		NL_ENGINE_INFO("Particle System Init Done!");

	}

}
