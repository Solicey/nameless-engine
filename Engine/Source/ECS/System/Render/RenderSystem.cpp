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
		m_Gizmos = ModelLoader::Create(modelsFolder + "/DontModify/Gizmos.obj", ModelLoaderFlags::Triangulate);

		std::string assetFolder = PathConfig::GetInstance().GetAssetsFolder().string();
		m_PointGizmosTexture = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Icons/PointLight.png");
		m_DirGizmosTexture = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Icons/DirLight.png");
	}

	void RenderSystem::OnStartRuntime()
	{
	}

	void RenderSystem::OnStopRuntime(Scene* editorScene)
	{
	}

	void RenderSystem::OnUpdateRuntime(TimeStep ts, Entity cameraEntity)
	{
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

		Renderer::BeginScene(camera.mCamera, camTransform.GetTransform());

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

		if (clearFlag == Camera::ClearFlagType::Skybox)
		{
			m_SkyboxShader->Bind();

			Renderer::DepthFunc(DepthComp::Lequal);

			// tmp
			m_SkyboxTextureCubemap->Bind(0);
			m_SkyboxShader->SetUniformInt("u_Skybox", 0);
			NL_ENGINE_ASSERT(cameraEntity.HasComponent<TransformComponent>(), "Camera does NOT have transform!");
			Renderer::DrawModel(m_Skybox, m_SkyboxShader, nlm::translate(nlm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().GetTranslation()));

			Renderer::DepthFunc(DepthComp::Less);
		}

		Renderer::EndScene();
	}

	void RenderSystem::OnStartEditor()
	{
	}

	void RenderSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera, Entity selectedEntity)
	{
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

		if (renderGizmos)
		{
			// Render Camera Gizmos
			auto view2 = m_Scene->Registry.view<TransformComponent, CameraComponent>();
			for (auto& e : view2)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& camera = entity.GetComponent<CameraComponent>();

				if (camera.Gizmos != nullptr)
				{
					Renderer::DrawModel(camera.Gizmos, transform.GetTransform() * nlm::scale(nlm::mat4(1.0f), nlm::vec3(1.0f, 1.0f, -1.0f)), (int)(uint32_t)entity, selectedEntity == entity);
				}
			}
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

		// Gizsmo
		if (renderGizmos)
		{
			for (int i = 0; i < MAX_LIGHT_COUNT; i++)
			{
				if (pointLightDatas[i].IsValid)
				{
					auto& entity = pointEntites[i];
					const auto& transform = entity.GetComponent<TransformComponent>();
					Renderer::DrawSprite(m_GizmosShader, m_PointGizmosTexture, nlm::translate(nlm::mat4(1.0), transform.GetTranslation())* cameraRotation* nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(pointLightDatas[i].Color, 0.7), (int)(uint32_t)entity, selectedEntity == entity);
				}
				if (dirLightDatas[i].IsValid)
				{
					auto& entity = dirEntites[i];
					const auto& transform = entity.GetComponent<TransformComponent>();
					Renderer::DrawSprite(m_GizmosShader, m_DirGizmosTexture, nlm::translate(nlm::mat4(1.0), transform.GetTranslation())* cameraRotation* nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(dirLightDatas[i].Color, 0.7), (int)(uint32_t)entity, selectedEntity == entity);
				}
			}
		}

		Renderer::EndScene();
	}
}