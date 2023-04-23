#include "nlpch.h"

#include "Renderer.h"
#include "Resources/Libraries/TextureLibrary.h"

namespace NL
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
	Scope<RendererAPI> Renderer::s_RendererAPI = RendererAPI::Create();
	PointLightShadingData Renderer::s_PointLightDatas[MAX_LIGHT_COUNT] = {};

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewPositionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(EditorCamera& camera)
	{
		s_SceneData->ViewPositionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(Camera& camera, const nlm::mat4& transform)
	{
		s_SceneData->ViewPositionMatrix = camera.GetProjectionMatrix() * nlm::inverse(transform);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const nlm::mat4& transform, const std::vector<nlm::mat4>& finalMatrices, bool isSelected)
	{
		NL_ENGINE_ASSERT(material, "Material is nullptr!");

		const auto& shader = material->GetShader();

		NL_ENGINE_ASSERT(shader, "Shader is nullptr!");

		shader->Bind();

		// Typical
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewPositionMatrix);
		shader->SetUniformMat4("u_Transform", transform);
		shader->SetUniformInt("u_IsSelected", isSelected ? 1 : 0);

		// Lights
		for (int i = 0; i < MAX_LIGHT_COUNT; i++)
		{
			const auto& point = s_PointLightDatas[i];
			if (point.IsValid)
			{
				shader->SetUniformFloat3("u_PointLights[" + std::to_string(i) + "].Color", point.Color);
				shader->SetUniformFloat3("u_PointLights[" + std::to_string(i) + "].Position", point.Position);
			}
			else
			{
				shader->SetUniformFloat3("u_PointLights[" + std::to_string(i) + "].Color", nlm::vec3{ -1.0f });
			}
		}

		if (!finalMatrices.empty())
		{
			/*for (int i = 0; i < finalMatrices.size(); i++)
			{
				NL_ENGINE_TRACE("final[{0}] = {1}", i, nlm::to_string(finalMatrices[i]));
			}*/

			shader->SetUniformMat4Array("u_FinalBoneMatrices", finalMatrices);
		}

		// Custom
		int cntSampler2D = 0;
		for (const auto& prop : material->GetShaderPropertiesNotConst())
		{
			switch (prop.Type)
			{
			case ShaderUniformType::Color3:
				shader->SetUniformFloat3(prop.Name.c_str(), std::get<nlm::vec3>(prop.Value));
				break;

			case ShaderUniformType::Sampler2D:
				//NL_ENGINE_TRACE("Sampler2D name: {0}", prop.Name);
				//NL_ENGINE_TRACE("Sampler2D filepath: {0}", std::get<std::string>(prop.Value));
				shader->SetUniformInt(prop.Name.c_str(), cntSampler2D);
				// std::get<std::string>(prop.Value)
				Library<Texture2D>::GetInstance().Fetch(std::get<std::string>(prop.Value))->Bind(cntSampler2D);
				cntSampler2D++;
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

	void Renderer::DrawModel(const Ref<Model>& model, const nlm::mat4& transform, bool isSelected)
	{
		const auto& meshes = model->GetMeshes();
		bool hasBones = model->HasBones();
		std::vector<nlm::mat4> emptyMatrices = {};

		for (const auto& mesh : meshes)
		{
			const auto& material = model->GetMaterial(mesh);
			Submit(mesh->GetVertexArray(), material, transform, hasBones ? model->GetFinalBoneMatrices() : emptyMatrices, isSelected);
		}
	}

	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		SetViewPort(0, 0, width, height);
	}
}