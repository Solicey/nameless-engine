#include "nlpch.h"

#include "Renderer.h"

namespace NL
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
	Scope<RendererAPI> Renderer::s_RendererAPI = RendererAPI::Create();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewPositionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(EditorCamera& camera)
	{
		s_SceneData->ViewPositionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const nlm::mat4& transform)
	{
		NL_ENGINE_ASSERT(material, "Material is nullptr!");

		const auto& shader = material->GetShader();

		NL_ENGINE_ASSERT(shader, "Shader is nullptr!");

		shader->Bind();

		// Typical
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewPositionMatrix);
		shader->SetUniformMat4("u_Transform", transform);

		// Custom
		for (const auto& prop : material->GetShaderPropertiesNotConst())
		{
			switch (prop.Type)
			{
			case ShaderUniformType::Color3:
				shader->SetUniformFloat3(prop.Name.c_str(), std::any_cast<nlm::vec3>(prop.Value));
				break;

			default:
				break;
			}
		}

		vertexArray->Bind();

		DrawIndices(vertexArray);

		shader->Unbind();
		vertexArray->Unbind();
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const nlm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewPositionMatrix);
		shader->SetUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		
		DrawIndices(vertexArray);

		shader->Unbind();
		vertexArray->Unbind();
	}

	void Renderer::DrawModel(const Ref<Model>& model, const Ref<Shader>& shader, const nlm::mat4& transform)
	{
		const auto& meshes = model->GetMeshes();

		for (const auto& mesh : meshes)
		{
			Submit(mesh->GetVertexArray(), shader, transform);
		}
	}

	void Renderer::DrawModel(const Ref<Model>& model, const nlm::mat4& transform)
	{
		const auto& meshes = model->GetMeshes();

		for (const auto& mesh : meshes)
		{
			const auto& material = model->GetMaterial(mesh);
			Submit(mesh->GetVertexArray(), material, transform);
		}
	}

	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		SetViewPort(0, 0, width, height);
	}
}