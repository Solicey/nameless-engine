#include "nlpch.h"
#include "ParticleSystemComponent.h"

namespace NL
{
	ParticleSystemComponent::ParticleSystemComponent()
//		: Pass1("Rising.glsl"), Pass2("ParticleSprite.glsl")
	{
		for (int i = 0; i < 2; i++)
		{
			TFB[i] = TransformFeedback::Create();
		}

		Pass1 = CreateRef<Material>("Rising.glsl", "Transform Feedback Pass");
		Pass2 = CreateRef<Material>("ParticleSprite.glsl", "Render Pass");

		auto& props = Pass2->GetShaderPropertiesNotConst();
		for (auto& prop : props)
		{
			if (prop.Name == "u_Sprite" && prop.Type == ShaderUniformType::Sampler2D)
			{
				std::string texPath = PathConfig::GetInstance().GetAssetsFolder().string() + "/Textures/DontModify/DefaultParticle.png";
				// Bad design, should be replaced later or so
				Pass2->ReplaceTexture("u_Sprite", Library<Texture2D>::GetInstance().Fetch(texPath));
				prop.Value = texPath;
			}
		}

		//Pass1 = Library<Shader>::GetInstance().Fetch("Rising.glsl");
		//Pass2 = Library<Shader>::GetInstance().Fetch("ParticleSprite.glsl");
		//Tex = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Textures/DontModify/DefaultParticle.png");

		Prop.LauncherNum = 10;
		Prop.SpawnAreaShape = ParticleSpawnAreaShape::Circle;
		Prop.SpawnPositionDistribution = ParticleSpawnDistribution::Uniform;
		Prop.MinVelocity = nlm::vec3(0.0f, 0.2f, 0.0f);
		Prop.MaxVelocity = nlm::vec3(0.0f, 2.0f, 0.0f);
		Prop.MaxTotalLifetime = 5;
		Prop.MinTotalLifetime = 3;
		Prop.SpawnAreaRadius = 2;
		Prop.InitSize = 1;
		Prop.InitColor = nlm::vec4(1.0f);

		Init();
	}

	ParticleSystemComponent::ParticleSystemComponent(const ParticleSystemComponent& comp) : Pass1(CreateRef<Material>(comp.Pass1.get())), Pass2(CreateRef<Material>(comp.Pass2.get())), Prop(comp.Prop)
	{
		TFB[0] = comp.TFB[0];
		TFB[1] = comp.TFB[1];

		Init();
	}

	void ParticleSystemComponent::Init()
	{
		static int randomTime = 10;
		Prop.IsFirstDraw = true;
		Prop.Input = 0;
		Prop.Output = 1;

		std::vector<Particle> particles;
		nlm::vec3 DelVelocity = Prop.MaxVelocity - Prop.MinVelocity;
		float DelTotalLifetime = Prop.MaxTotalLifetime - Prop.MinTotalLifetime;

		for (int i = 0; i < Prop.LauncherNum; i++)
		{
			Particle particle;
			nlm::vec3 pos(0.0f);

			switch (Prop.SpawnAreaShape)
			{
			case NL::ParticleSpawnAreaShape::Point:
				break;
			case NL::ParticleSpawnAreaShape::Circle:
			{
				nlm::vec2 dir = nlm::vec2(0);
				float mag = 0;
				switch (Prop.SpawnPositionDistribution)
				{
				case ParticleSpawnDistribution::Uniform:
				{
					mag = float(rand()) / float(RAND_MAX) * Prop.SpawnAreaRadius;
					break;
				}
				case ParticleSpawnDistribution::Normal:
				{
					for (int j = 0; j < randomTime; j++)
					{
						mag += (float(rand()) / float(RAND_MAX) * 2.0f - 1.0f);
					}
					mag = mag / randomTime * Prop.SpawnAreaRadius;
					break;
				}
				}
				dir.x = (float(rand()) / float(RAND_MAX) * 2.0f - 1.0f);
				dir.y = (float(rand()) / float(RAND_MAX) * 2.0f - 1.0f);
				dir = nlm::normalize(dir) * mag;
				pos.x = dir.x;
				pos.z = dir.y;
				break;
			}
			case NL::ParticleSpawnAreaShape::Sphere:
			{
				float mag = 0;
				switch (Prop.SpawnPositionDistribution)
				{
				case ParticleSpawnDistribution::Uniform:
				{
					mag = float(rand()) / float(RAND_MAX) * Prop.SpawnAreaRadius;
					break;
				}
				case ParticleSpawnDistribution::Normal:
				{
					for (int j = 0; j < randomTime; j++)
					{
						mag += (float(rand()) / float(RAND_MAX) * 2.0f - 1.0f);
					}
					mag = mag / randomTime * Prop.SpawnAreaRadius;
					break;
				}
				}
				pos.x = (float(rand()) / float(RAND_MAX) * 2.0f - 1.0f);
				pos.y = (float(rand()) / float(RAND_MAX) * 2.0f - 1.0f);
				pos.z = (float(rand()) / float(RAND_MAX) * 2.0f - 1.0f);
				pos = nlm::normalize(pos) * mag;
				break;
			}
			default:
				break;
			}

			particle.Type = 0;
			particle.Position = pos;
			
			nlm::vec3 vel;
			vel.x = DelVelocity.x * (float(rand()) / float(RAND_MAX)) + Prop.MinVelocity.x;
			vel.y = DelVelocity.y * (float(rand()) / float(RAND_MAX)) + Prop.MinVelocity.y;
			vel.z = DelVelocity.z * (float(rand()) / float(RAND_MAX)) + Prop.MinVelocity.z;
			particle.Velocity = vel;

			particle.Size = Prop.InitSize;
			particle.Color = Prop.InitColor;

			particle.TotalLifetime = DelTotalLifetime * (float(rand()) / float(RAND_MAX)) + Prop.MinTotalLifetime;
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

	void ParticleSystemComponent::UpdateShaderProperties(const std::string& shaderName)
	{
		if (Pass1->GetShaderName() == shaderName)
			Pass1->LoadShaderAndUpdateProps(shaderName);
		if (Pass2->GetShaderName() == shaderName)
			Pass2->LoadShaderAndUpdateProps(shaderName);
	}

	void ParticleSystemComponent::DeleteMaterialsReference()
	{
		if (Pass1 != nullptr)
			Pass1->DeleteTexturesAndShadersReference();
		if (Pass2 != nullptr)
			Pass2->DeleteTexturesAndShadersReference();
	}

}
