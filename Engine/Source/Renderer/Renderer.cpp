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

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const nlm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewPositionMatrix);
		shader->SetUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		
		s_RendererAPI->DrawIndexed(vertexArray);
	}
}