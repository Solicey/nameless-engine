#include "nlpch.h"

#include "RenderSystem.h"

#include "Core/Misc/ConfigManager.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"
#include "Renderer/Renderer.h"
#include "Resources/Libraries/ShaderLibrary.h"

namespace NL
{
	RenderSystem::RenderSystem(Scene* scene)
		: System(scene)
	{
		Library<Shader>::GetInstance();
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
		Renderer::SetClearColor(camera.ClearColor);
		Renderer::Clear();

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

		Renderer::EndScene();
	}

	void RenderSystem::OnStartEditor()
	{
	}

	void RenderSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		Renderer::BeginScene(camera);

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

		// Render Camera Gizmos
		auto view2 = m_Scene->m_Registry.view<TransformComponent, CameraComponent>();
		for (auto& e : view2)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& camera = entity.GetComponent<CameraComponent>();

			if (camera.Gizmos != nullptr)
			{
				Renderer::DrawModel(camera.Gizmos, transform.GetTransform() * nlm::scale(nlm::mat4(1.0f), nlm::vec3(1.0f, 1.0f, -1.0f)));
			}
		}

		Renderer::EndScene();
	}
}