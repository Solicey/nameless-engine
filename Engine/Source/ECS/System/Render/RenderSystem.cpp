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

		// Camera Clear Color
		auto clearFlag = camera.mCamera.GetClearFlagType();
		if (clearFlag == Camera::ClearFlagType::Color)
		{
			Renderer::SetClearColor(camera.ClearColor);
			Renderer::Clear();
		}

		Renderer::BeginScene(camera.mCamera, camTransform.GetTransform(), camTransform.GetTranslation());

		// Render Models
		{
			auto view = m_Scene->Registry.view<TransformComponent, ModelRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& model = entity.GetComponent<ModelRendererComponent>();

				if (model._Model != nullptr)
				{
					Renderer::DrawModel(model._Model, transform.GetTransform(), (int)(uint32_t)entity, false);
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

	void RenderSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera, Entity selectedEntity)
	{
		m_TotalTime += ts;
		m_DeltaTime = ts;

		Renderer::BeginScene(camera);

		bool renderGizmos = camera.IsRenderGizmos();
		nlm::mat4 cameraRotation = nlm::mat4(camera.GetOrientation());

		// Light Preparation
		PointLightShadingData pointLightDatas[MAX_LIGHT_COUNT];
		DirLightShadingData dirLightDatas[MAX_LIGHT_COUNT];
		Entity pointEntites[MAX_LIGHT_COUNT], dirEntites[MAX_LIGHT_COUNT];

		for (int i = 0; i < MAX_LIGHT_COUNT; i++)
			pointLightDatas[i].IsValid = dirLightDatas[i].IsValid = false;

		{
			auto view = m_Scene->Registry.view<TransformComponent, LightComponent>();
			int pointId = 0, dirId = 0;
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);
				auto [transform, light] = view.get<TransformComponent, LightComponent>(e);

				if (light.Type == LightType::Point && pointId < MAX_LIGHT_COUNT)
				{
					nlm::vec3 color = light.Color * light.Intensity;

					PointLightShadingData info = {
						transform.GetTranslation(),
						color,
						true
					};
					pointEntites[pointId] = entity;
					pointLightDatas[pointId++] = info;
				}
				else if (light.Type == LightType::Directional && dirId < MAX_LIGHT_COUNT)
				{
					nlm::vec3 color = light.Color * light.Intensity;

					DirLightShadingData info = {
						transform.GetForward(),
						color,
						true
					};
					dirEntites[dirId] = entity;
					dirLightDatas[dirId++] = info;
				}
			}
		}

		// Update Light Data
		Renderer::SetPointLightData(pointLightDatas);
		Renderer::SetDirLightData(dirLightDatas);

		// Render Entities
		{
			auto view = m_Scene->Registry.view<TransformComponent, ModelRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& model = entity.GetComponent<ModelRendererComponent>();

				if (model._Model != nullptr)
				{
					Renderer::DrawModel(model._Model, transform.GetTransform(), (int)(uint32_t)entity, selectedEntity == entity);
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
					Renderer::DrawSprite(m_SpriteShader, sprite.SpriteTexture, transform.GetTransform(), sprite.Color, sprite.Reaction, (int)(uint32_t)entity, selectedEntity == entity);
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
					Renderer::DrawModel(camera.Gizmos, transform.GetTransform() * nlm::scale(nlm::mat4(1.0f), nlm::vec3(1.0f, 1.0f, -1.0f)), (int)(uint32_t)entity, selectedEntity == entity);
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
		Renderer::DepthMask(false);
		if (renderGizmos)
		{
			for (int i = 0; i < MAX_LIGHT_COUNT; i++)
			{
				if (pointLightDatas[i].IsValid)
				{
					auto& entity = pointEntites[i];
					const auto& transform = entity.GetComponent<TransformComponent>();
					Renderer::DrawSprite(m_GizmosShader, m_PointGizmosTexture, nlm::translate(nlm::mat4(1.0), transform.GetTranslation())* cameraRotation* nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(pointLightDatas[i].Color, 0.7), SpriteCameraReaction::LookAt, (int)(uint32_t)entity, selectedEntity == entity);
				}
				if (dirLightDatas[i].IsValid)
				{
					auto& entity = dirEntites[i];
					const auto& transform = entity.GetComponent<TransformComponent>();
					Renderer::DrawSprite(m_GizmosShader, m_DirGizmosTexture, nlm::translate(nlm::mat4(1.0), transform.GetTranslation())* cameraRotation* nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(dirLightDatas[i].Color, 0.7), SpriteCameraReaction::LookAt, (int)(uint32_t)entity, selectedEntity == entity);
				}
			}
		}
		Renderer::DepthMask(true);

		UpdateParticleSystem(selectedEntity);

		Renderer::EndScene();
	}

	void RenderSystem::UpdateParticleSystem(Entity selectedEntity)
	{
		auto view = m_Scene->Registry.view<TransformComponent, ParticleSystemComponent>();
		Renderer::DepthMask(false);
		for (auto& e : view)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& particleSystem = entity.GetComponent<ParticleSystemComponent>();

			auto& inputBuffer = particleSystem.TFB[particleSystem.Input];
			auto& outputBuffer = particleSystem.TFB[particleSystem.Output];

			if (!particleSystem.IsPaused)
			{
				// Pass 1
				auto& shader1 = particleSystem.Pass1->GetShader();
				shader1->Bind();
				// shader1->SetUniformMat4("u_View", viewMat);
				// shader1->SetUniformMat4("u_Projection", projMat);
				shader1->SetUniformMat4("u_Transform", transform.GetTransform());
				shader1->SetUniformFloat("u_DeltaTime", m_DeltaTime);
				shader1->SetUniformFloat("u_TotalTime", m_TotalTime);
				shader1->SetUniformFloat("u_Radius", particleSystem.SpawnAreaRadius);
				shader1->SetUniformFloat3("u_MinVelocity", particleSystem.MinVelocity);
				shader1->SetUniformFloat3("u_MaxVelocity", particleSystem.MaxVelocity);
				shader1->SetUniformFloat("u_MinTotalLifetime", particleSystem.MinTotalLifetime);
				shader1->SetUniformFloat("u_MaxTotalLifetime", particleSystem.MaxTotalLifetime);

				inputBuffer->BindBuffer();
				outputBuffer->BindTransformFeedback();

				// Rasterizer discard on
				Renderer::RasterizerDiscard(true);

				Renderer::BeginTransformFeedback_Points();
				// test

				if (particleSystem.IsFirstDraw)
				{
					Renderer::DrawArrays_Points(0, particleSystem.LauncherNum);
					particleSystem.IsFirstDraw = false;
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
			shader2->SetUniformInt("u_IsSelected", entity == selectedEntity);
			shader2->SetUniformInt("u_EntityId", (uint32_t)entity);
			shader2->SetUniformInt("u_Sprite", 0);
			particleSystem.Tex->Bind(0);

			outputBuffer->BindBuffer();

			outputBuffer->Draw_Points();

			shader2->Unbind();
			outputBuffer->UnbindBuffer();

			// Switch buffer
			if (!particleSystem.IsPaused)
			{
				particleSystem.Input = particleSystem.Output;
				particleSystem.Output = 1 - particleSystem.Output;
			}
		}
		Renderer::DepthMask(true);
	}
}