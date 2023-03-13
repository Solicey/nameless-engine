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

	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		SetViewPort(0, 0, width, height);
	}
}