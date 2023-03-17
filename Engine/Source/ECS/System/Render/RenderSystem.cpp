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
		std::string normalShaderVertexSrc = R"(
			#version 450 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 2) in vec3 a_Normal;
			layout(location = 5) in int a_EntityID;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			layout (location = 0) out vec3 v_Position;
			layout (location = 1) out vec3 v_Normal;
			layout (location = 2) out flat int v_EntityID;
			
			void main()
			{
				v_Position = a_Position;
				v_Normal = a_Normal;
				v_EntityID = a_EntityID;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string normalShaderFragmentSrc = R"(
			#version 450 core

			out vec4 color;
			out int color2;

			layout (location = 0) in vec3 v_Position;
			layout (location = 1) in vec3 v_Normal;
			layout (location = 2) in flat int v_EntityID;
			
			void main()
			{
				color = vec4(v_Normal.x * 0.5 + 0.5, v_Normal.y * 0.5 + 0.5, v_Normal.z * 0.5 + 0.5, 1.0);
				color2 = v_EntityID;
			}
		)";

		// m_TestShader = Shader::Create("Test Shader", PathConfig::GetInstance().GetShadersFolder() / "Debug/NormalTest.glsl");

		Library<Shader>::GetInstance();
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

			if (model.mModel != nullptr)
			{
				Renderer::DrawModel(model.mModel, transform.GetTransform());
			}
		}

		Renderer::EndScene();
	}
}