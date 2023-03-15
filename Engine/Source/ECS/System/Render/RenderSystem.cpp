#include "nlpch.h"

#include "RenderSystem.h"

#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"
#include "Renderer/Renderer.h"

namespace NL
{
	RenderSystem::RenderSystem(Scene* scene)
		: System(scene)
	{
		std::string normalShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 2) in vec3 a_Normal;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;
			out vec3 v_Normal;
			
			void main()
			{
				v_Position = a_Position;
				v_Normal = a_Normal;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string normalShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec3 v_Normal;
			
			void main()
			{
				color = vec4(v_Normal.x * 0.5 + 0.5, v_Normal.y * 0.5 + 0.5, v_Normal.z * 0.5 + 0.5, 1.0);
			}
		)";

		m_TestShader = CreateRef<Shader>(normalShaderVertexSrc, normalShaderFragmentSrc);
	}

	void RenderSystem::OnStartRuntime()
	{
	}

	void RenderSystem::OnStopRuntime()
	{
	}

	void RenderSystem::OnUpdateRuntime(TimeStep ts)
	{
	}

	void RenderSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		// Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		// Renderer::Clear();

		Renderer::BeginScene(camera);

		auto view = m_Scene->m_Registry.view<TransformComponent, ModelRendererComponent>();
		for (auto& e : view)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& model = entity.GetComponent<ModelRendererComponent>();

			Renderer::DrawModel(model.mModel, m_TestShader, transform.GetTransform());
		}

		Renderer::EndScene();
	}
}