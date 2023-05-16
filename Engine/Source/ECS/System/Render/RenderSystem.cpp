#include "nlpch.h"

#include "RenderSystem.h"

#include "Core/Misc/PathConfig.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"
#include "Renderer/Renderer.h"
#include "Resources/Libraries/ShaderLibrary.h"

namespace NL
{
	RenderSystem::RenderSystem(Scene* scene)
		: System(scene)
	{
		m_SkyboxShader = Library<Shader>::GetInstance().Fetch("Skybox.glsl");
		NL_ENGINE_ASSERT(m_SkyboxShader, "Skybox shader does NOT exists!");

		std::string modelsFolder = PathConfig::GetInstance().GetModelsFolder().string();
		m_Skybox = ModelLoader::Create(modelsFolder + "/DontModify/Skybox.obj", ModelLoaderFlags::Triangulate);
		m_SkyboxTextureCubemap = Library<TextureCubeMap>::GetInstance().FetchDefault();

		m_GizmosShader = Library<Shader>::GetInstance().Fetch("Gizmos.glsl");

		std::string assetFolder = PathConfig::GetInstance().GetAssetsFolder().string();
		m_PointGizmosTexture = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Icons/PointLight.png");
		m_DirGizmosTexture = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Icons/DirLight.png");

		m_SpriteShader = Library<Shader>::GetInstance().Fetch("Sprite.glsl");
	}

	void RenderSystem::OnStartRuntime()
	{
		m_TotalTime = m_DeltaTime = 0;
	}

	void RenderSystem::OnStopRuntime(Scene* editorScene)
	{
		m_TotalTime = m_DeltaTime = 0;
	}

	void RenderSystem::OnUpdateRuntime(TimeStep ts, Entity cameraEntity)
	{
		m_TotalTime += ts;
		m_DeltaTime = ts;

		if (!cameraEntity.HasComponent<CameraComponent>())
			return;

		if (!cameraEntity.HasComponent<TransformComponent>())
		{
			NL_ENGINE_ASSERT(false, "Entity with CameraComponent should also has TransformComponent!");
			return;
		}

		auto& camera = cameraEntity.GetComponent<CameraComponent>();
		auto& camTransform = cameraEntity.GetComponent<TransformComponent>();

		// Light Preparation
		//PointLightShadingData pointLightDatas[MAX_LIGHT_COUNT];
		//DirLightShadingData dirLightDatas[MAX_LIGHT_COUNT];
		//Entity pointEntities[MAX_LIGHT_COUNT], dirEntities[MAX_LIGHT_COUNT];

		//LightPreparation(pointLightDatas, dirLightDatas, pointEntities, dirEntities);

		// Update Light Data
		//Renderer::SetPointLightData(pointLightDatas);
		//Renderer::SetDirLightData(dirLightDatas);

		auto clearFlag = camera.mCamera.GetClearFlagType();
		// Camera Clear Color
		/*
		if (clearFlag == Camera::ClearFlagType::Color)
		{
			Renderer::SetClearColor(camera.ClearColor);
			Renderer::Clear();
		}*/

		Renderer::BeginScene(camera.mCamera, camTransform.GetTransform(), camTransform.GetTranslation());

		// Render Models (Opaque)
		{
			auto view = m_Scene->Registry.view<TransformComponent, ModelRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& model = entity.GetComponent<ModelRendererComponent>();

				if (model._Model != nullptr)
				{
					Renderer::DrawModel(model._Model, transform.GetTransform(), (int)(uint32_t)entity, m_Scene->GetPointLightShadingDataNotConst(), m_Scene->GetDirLightShadingDataNotConst());
				}
			}
		}

		// Render Sprites
		{
			// Renderer::SetCullFace(CullFace::Front);
			auto view = m_Scene->Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteRendererComponent>();

				if (sprite.SpriteTexture != nullptr)
				{
					Renderer::DrawSprite(m_SpriteShader, sprite.SpriteTexture, transform.GetTransform(), sprite.Color, sprite.Reaction, (int)(uint32_t)entity);
				}
			}
			// Renderer::SetCullFace(CullFace::Back);
		}

		if (clearFlag == Camera::ClearFlagType::Skybox)
		{
			m_SkyboxShader->Bind();

			Renderer::DepthFunc(DepthComp::Lequal);
			Renderer::SetCullFace(CullFace::Front);

			// tmp
			m_SkyboxTextureCubemap->Bind(0);
			m_SkyboxShader->SetUniformInt("u_Skybox", 0);
			NL_ENGINE_ASSERT(cameraEntity.HasComponent<TransformComponent>(), "Camera does NOT have transform!");
			Renderer::DrawModel(m_Skybox, m_SkyboxShader, nlm::translate(nlm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().GetTranslation()));

			Renderer::DepthFunc(DepthComp::Less);
			Renderer::SetCullFace(CullFace::Back);
		}

		UpdateParticleSystem();

		Renderer::EndScene();
	}

	void RenderSystem::OnStartEditor()
	{
		m_TotalTime = m_DeltaTime = 0;
	}

	void RenderSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera, Entity settings)
	{
		m_TotalTime += ts;
		m_DeltaTime = ts;

		Renderer::BeginScene(camera);

		bool renderGizmos = settings.GetComponent<SettingsComponent>().ShowGizmos;
		nlm::mat4 cameraRotation = nlm::mat4(camera.GetOrientation());

		// Light Preparation
		//PointLightShadingData pointLightDatas[MAX_LIGHT_COUNT];
		//DirLightShadingData dirLightDatas[MAX_LIGHT_COUNT];
		//Entity pointEntities[MAX_LIGHT_COUNT], dirEntities[MAX_LIGHT_COUNT];

		//LightPreparation(pointLightDatas, dirLightDatas, pointEntities, dirEntities);

		// Update Light Data
		//Renderer::SetPointLightData(pointLightDatas);
		//Renderer::SetDirLightData(dirLightDatas);

		// Render Entities (Opaque)
		{
			auto view = m_Scene->Registry.view<TransformComponent, ModelRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& model = entity.GetComponent<ModelRendererComponent>();

				if (model._Model != nullptr)
				{
					Renderer::DrawModel(model._Model, transform.GetTransform(), (int)(uint32_t)entity, m_Scene->GetPointLightShadingDataNotConst(), m_Scene->GetDirLightShadingDataNotConst());
				}
			}
		}

		// Render Sprites
		{
			// Renderer::SetCullFace(CullFace::Front);
			auto view = m_Scene->Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteRendererComponent>();

				if (sprite.SpriteTexture != nullptr)
				{
					Renderer::DrawSprite(m_SpriteShader, sprite.SpriteTexture, transform.GetTransform(), sprite.Color, sprite.Reaction, (int)(uint32_t)entity);
				}
			}
			// Renderer::SetCullFace(CullFace::Back);
		}

		if (renderGizmos)
		{
			Renderer::SetCullFace(CullFace::Front);
			// Render Camera Gizmos
			auto view = m_Scene->Registry.view<TransformComponent, CameraComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& camera = entity.GetComponent<CameraComponent>();

				if (camera.Gizmos != nullptr)
				{
					Renderer::DrawModel(camera.Gizmos, transform.GetTransform() * nlm::scale(nlm::mat4(1.0f), nlm::vec3(1.0f, 1.0f, -1.0f)), (int)(uint32_t)entity, m_Scene->GetPointLightShadingDataNotConst(), m_Scene->GetDirLightShadingDataNotConst());
				}
			}
			Renderer::SetCullFace(CullFace::Back);
		}

		// Render Skybox?
		//auto clearFlag = camera.GetClearFlagType();
		//if (clearFlag == Camera::ClearFlagType::Skybox)
		if (camera.GetClearFlagType() == Camera::ClearFlagType::Skybox)
		{
			m_SkyboxShader->Bind();

			Renderer::DepthFunc(DepthComp::Lequal);
			Renderer::SetCullFace(CullFace::Front);

			// tmp
			m_SkyboxTextureCubemap->Bind(0);
			m_SkyboxShader->SetUniformInt("u_Skybox", 0);
			Renderer::DrawModel(m_Skybox, m_SkyboxShader, nlm::translate(nlm::mat4(1.0f), camera.GetPosition()));

			Renderer::DepthFunc(DepthComp::Less);
			Renderer::SetCullFace(CullFace::Back);
		}

		// Gizmos
		if (renderGizmos)
		{
			Renderer::DepthMask(false);
			Renderer::EnableBlend(true);
			Renderer::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::One);

			auto& pointLightDatas = m_Scene->GetPointLightShadingDataNotConst();
			auto& dirLightDatas = m_Scene->GetDirLightShadingDataNotConst();

			for (auto& l : pointLightDatas)
			{
				Renderer::DrawSprite(m_GizmosShader, m_PointGizmosTexture, nlm::translate(nlm::mat4(1.0), l.Position) * cameraRotation * nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(l.Color, 0.7), SpriteCameraReaction::LookAt, (int)l.EntityId);
			}
			for (auto& l : dirLightDatas)
			{
				Renderer::DrawSprite(m_GizmosShader, m_DirGizmosTexture, nlm::translate(nlm::mat4(1.0), l.Position) * cameraRotation * nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(l.Color, 0.7), SpriteCameraReaction::LookAt, (int)l.EntityId);
			}
			Renderer::DepthMask(true);
			Renderer::EnableBlend(false);
			Renderer::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
		}
		

		UpdateParticleSystem();

		Renderer::EndScene();
	}

	void RenderSystem::UpdateParticleSystem()
	{
		Renderer::DepthMask(false);
		Renderer::EnableBlend(true);
		Renderer::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::One);

		auto view = m_Scene->Registry.view<TransformComponent, ParticleSystemComponent>();
		for (auto& e : view)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& particleSystem = entity.GetComponent<ParticleSystemComponent>();
			auto& prop = particleSystem.Prop;

			auto& inputBuffer = particleSystem.TFB[prop.Input];
			auto& outputBuffer = particleSystem.TFB[prop.Output];

			bool isFirstDraw = prop.IsFirstDraw;
			// if is playing
			if ((!prop.IsPaused && m_Scene->IsEditor()) || (m_Scene->IsPlaying() && !m_Scene->IsEditor()) || isFirstDraw)
			{
				// Pass 1
				auto& shader1 = particleSystem.Pass1->GetShader();
				shader1->Bind();
				// shader1->SetUniformMat4("u_View", viewMat);
				// shader1->SetUniformMat4("u_Projection", projMat);
				shader1->SetUniformMat4("u_Transform", transform.GetTransform());
				shader1->SetUniformFloat("u_DeltaTime", m_DeltaTime);
				shader1->SetUniformFloat("u_TotalTime", m_TotalTime);
				shader1->SetUniformFloat("u_Radius", prop.SpawnAreaRadius);
				shader1->SetUniformFloat3("u_MinVelocity", prop.MinVelocity);
				shader1->SetUniformFloat3("u_MaxVelocity", prop.MaxVelocity);
				shader1->SetUniformFloat("u_MinTotalLifetime", prop.MinTotalLifetime);
				shader1->SetUniformFloat("u_MaxTotalLifetime", prop.MaxTotalLifetime);

				Renderer::BindCustomShaderProperties(particleSystem.Pass1);

				inputBuffer->BindBuffer();
				outputBuffer->BindTransformFeedback();

				// Rasterizer discard on
				Renderer::RasterizerDiscard(true);

				Renderer::BeginTransformFeedback_Points();
				// test

				if (prop.IsFirstDraw)
				{
					Renderer::DrawArrays_Points(0, prop.LauncherNum);
					prop.IsFirstDraw = false;
				}
				else
				{
					inputBuffer->Draw_Points();
				}

				Renderer::EndTransformFeedback();

				// Rasterizer discard off
				Renderer::RasterizerDiscard(false);

				shader1->Unbind();
				inputBuffer->UnbindBuffer();
				outputBuffer->UnbindTransformFeedback();
			}

			// Pass 2 Sprite
			auto& shader2 = particleSystem.Pass2->GetShader();
			shader2->Bind();

			shader2->SetUniformMat4("u_Transform", transform.GetTransform());
			shader2->SetUniformInt("u_EntityId", (uint32_t)entity);
			//shader2->SetUniformInt("u_Sprite", 0);
			//particleSystem.Tex->Bind(0);
			Renderer::BindCustomShaderProperties(particleSystem.Pass2);

			outputBuffer->BindBuffer();

			outputBuffer->Draw_Points();

			shader2->Unbind();
			outputBuffer->UnbindBuffer();

			// Switch buffer
			if ((!prop.IsPaused && m_Scene->IsEditor()) || (m_Scene->IsPlaying() && !m_Scene->IsEditor()) || isFirstDraw)
			{
				prop.Input = prop.Output;
				prop.Output = 1 - prop.Output;
			}
		}

		Renderer::DepthMask(true);
		Renderer::EnableBlend(false);
		Renderer::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
	}

	
}