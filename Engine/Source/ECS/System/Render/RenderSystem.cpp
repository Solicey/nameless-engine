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
		m_SkyboxShader = Library<Shader>::GetInstance().LoadShader("Skybox.glsl");
		NL_ENGINE_ASSERT(m_SkyboxShader, "Skybox shader does NOT exists!");

		std::string modelsFolder = PathConfig::GetInstance().GetModelsFolder().string();
		m_Skybox = ModelLoader::Create(modelsFolder + "/DontModify/Skybox.obj", -1, ModelLoaderFlags::Triangulate);
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

		auto view = m_Scene->m_Registry.view<TransformComponent, ModelRendererComponent>();
		for (auto& e : view)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& model = entity.GetComponent<ModelRendererComponent>();

			if (model.mModel != nullptr)
			{
				Renderer::DrawModel(model.mModel, transform.GetTransform());
			}
		}

		if (clearFlag == Camera::ClearFlagType::Skybox)
		{
			m_SkyboxShader->Bind();

			Renderer::DepthFunc(DepthComp::LEQUAL);

			// tmp
			Library<TextureCubeMap>::GetInstance().Get("DefaultSkybox")->Bind(0);
			m_SkyboxShader->SetUniformInt("u_Skybox", 0);
			NL_ENGINE_ASSERT(cameraEntity.HasComponent<TransformComponent>(), "Camera does NOT have transform!");
			Renderer::DrawModel(m_Skybox, m_SkyboxShader, nlm::translate(nlm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().GetTranslation()));

			Renderer::DepthFunc(DepthComp::LESS);
		}

		Renderer::EndScene();
	}

	void RenderSystem::OnStartEditor()
	{
	}

	void RenderSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera, Entity selectedEntity)
	{
		Renderer::BeginScene(camera);

		// Render Entities
		auto view = m_Scene->m_Registry.view<TransformComponent, ModelRendererComponent>();
		for (auto& e : view)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& model = entity.GetComponent<ModelRendererComponent>();

			if (model.mModel != nullptr)
			{
				Renderer::DrawModel(model.mModel, transform.GetTransform(), selectedEntity == entity);
			}
		}

		// Render Camera Gizmos
		auto view2 = m_Scene->m_Registry.view<TransformComponent, CameraComponent>();
		for (auto& e : view2)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& camera = entity.GetComponent<CameraComponent>();

			if (camera.Gizmos != nullptr)
			{
				Renderer::DrawModel(camera.Gizmos, transform.GetTransform() * nlm::scale(nlm::mat4(1.0f), nlm::vec3(1.0f, 1.0f, -1.0f)), selectedEntity == entity);
			}
		}

		// Render Skybox?
		auto clearFlag = camera.GetClearFlagType();
		if (clearFlag == Camera::ClearFlagType::Skybox)
		{
			m_SkyboxShader->Bind();

			Renderer::DepthFunc(DepthComp::LEQUAL);

			// tmp
			Library<TextureCubeMap>::GetInstance().Get("DefaultSkybox")->Bind(0);
			m_SkyboxShader->SetUniformInt("u_Skybox", 0);
			Renderer::DrawModel(m_Skybox, m_SkyboxShader, nlm::translate(nlm::mat4(1.0f), camera.GetPosition()));

			Renderer::DepthFunc(DepthComp::LESS);
		}

		Renderer::EndScene();
	}
}