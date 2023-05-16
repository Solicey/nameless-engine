#include "nlpch.h"

#include "Renderer.h"
#include "Resources/Loaders/ModelLoader.h"
#include "Resources/Libraries/TextureLibrary.h"
#include "Resources/Libraries/UniformBufferLibrary.h"
#include "ECS/Entity/Entity.h"

namespace NL
{
	// Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
	Scope<RendererAPI> Renderer::s_RendererAPI = RendererAPI::Create();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		//s_SceneData->ViewPositionMatrix = camera.GetViewProjectionMatrix();
		Ref<UniformBuffer> cameraUniform = Library<UniformBuffer>::GetInstance().GetCameraUniformBuffer();
		nlm::mat4 ViewMatrix = camera.GetViewMatrix();
		nlm::mat4 ProjMatrix = camera.GetProjectionMatrix();
		nlm::vec3 CamPosition = camera.GetPosition();
		cameraUniform->SetData(&ViewMatrix, 64, 0);
		cameraUniform->SetData(&ProjMatrix, 64, 64);
		cameraUniform->SetData(&CamPosition, 16, 128);
	}

	void Renderer::BeginScene(EditorCamera& camera)
	{
		//s_SceneData->ViewPositionMatrix = camera.GetViewProjectionMatrix();
		Ref<UniformBuffer> cameraUniform = Library<UniformBuffer>::GetInstance().GetCameraUniformBuffer();
		nlm::mat4 ViewMatrix = camera.GetViewMatrix();
		nlm::mat4 ProjMatrix = camera.GetProjectionMatrix();
		nlm::vec3 CamPosition = camera.GetPosition();
		auto type = camera.GetProjectionType();
		float nearClip, farClip;
		if (type == Camera::ProjectionType::Orthographic)
		{
			nearClip = camera.GetOrthographicNear();
			farClip = camera.GetOrthographicFar();
		}
		else
		{
			nearClip = camera.GetPerspectiveNear();
			farClip = camera.GetPerspectiveFar();
		}
		//nearClip = 0.3;
		//farClip = 0.6;
		//NL_ENGINE_INFO("Near: {0}, Far: {1}", nearClip, farClip);
		cameraUniform->SetData(&ViewMatrix, 64, 0);
		cameraUniform->SetData(&ProjMatrix, 64, 64);
		cameraUniform->SetData(&CamPosition, 12, 128);
		cameraUniform->SetData(&nearClip, 4, 140);
		cameraUniform->SetData(&farClip, 4, 144);
	}

	void Renderer::BeginScene(Camera& camera, const nlm::mat4& transform, const nlm::vec3& position)
	{
		//s_SceneData->ViewPositionMatrix = camera.GetProjectionMatrix() * nlm::inverse(transform);
		Ref<UniformBuffer> cameraUniform = Library<UniformBuffer>::GetInstance().GetCameraUniformBuffer();
		nlm::mat4 ViewMatrix = nlm::inverse(transform);
		nlm::mat4 ProjMatrix = camera.GetProjectionMatrix();
		auto type = camera.GetProjectionType();
		float nearClip, farClip;
		if (type == Camera::ProjectionType::Orthographic)
		{
			nearClip = camera.GetOrthographicNear();
			farClip = camera.GetOrthographicFar();
		}
		else
		{
			nearClip = camera.GetPerspectiveNear();
			farClip = camera.GetPerspectiveFar();
		}
		cameraUniform->SetData(&ViewMatrix, 64, 0);
		cameraUniform->SetData(&ProjMatrix, 64, 64);
		cameraUniform->SetData(&position, 12, 128);
		cameraUniform->SetData(&nearClip, 4, 140);
		cameraUniform->SetData(&farClip, 4, 144);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Material>& material,
		const std::vector<PointLightShadingData>& points, const std::vector<DirLightShadingData>& dirs, const nlm::mat4& transform, const std::vector<nlm::mat4>& finalMatrices, int entityId)
	{
		NL_ENGINE_ASSERT(material, "Material is nullptr!");

		const auto& shader = material->GetShader();
		if (!shader->HasCompiledSuccessfully())
			return;

		NL_ENGINE_ASSERT(shader, "Shader is nullptr!");

		shader->Bind();

		// Typical
		// shader->SetUniformMat4("u_View", s_SceneData->ViewMatrix);
		// shader->SetUniformMat4("u_Projection", s_SceneData->ProjectionMatrix);
		shader->SetUniformMat4("u_Transform", transform);
		shader->SetUniformMat4("u_NormalMatrix", nlm::transpose(nlm::inverse(transform)));
		//shader->SetUniformInt("u_IsSelected", isSelected ? 1 : 0);
		shader->SetUniformInt("u_EntityId", entityId);

		// Lights
		// Post-processing...
		if (shader->IsLightingRequired())
		{
			int cnt = 0;
			const int maxCnt = shader->GetMaxLightsCount();
			for (auto& l : points)
			{
				shader->SetUniformFloat3("u_PointLights[" + std::to_string(cnt) + "].Color", l.Color);
				shader->SetUniformFloat3("u_PointLights[" + std::to_string(cnt) + "].Position", l.Position);
				shader->SetUniformFloat3("u_PointLights[" + std::to_string(cnt) + "].Attenuation", l.Attenuation);
				cnt++;
				if (cnt >= maxCnt)
					break;
			}
			if (cnt < maxCnt)
			{
				for (int i = cnt; i < maxCnt; i++)
				{
					shader->SetUniformFloat3("u_PointLights[" + std::to_string(i) + "].Color", nlm::vec3(-1.0));
				}
			}

			cnt = 0;
			for (auto& l : dirs)
			{
				shader->SetUniformFloat3("u_DirLights[" + std::to_string(cnt) + "].Color", l.Color);
				shader->SetUniformFloat3("u_DirLights[" + std::to_string(cnt) + "].Direction", l.Direction);
				cnt++;
				if (cnt >= maxCnt)
					break;
			}
			if (cnt < maxCnt)
			{
				for (int i = cnt; i < maxCnt; i++)
				{
					shader->SetUniformFloat3("u_DirLights[" + std::to_string(i) + "].Color", nlm::vec3(-1.0));
				}
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

		BindCustomShaderProperties(material);

		vertexArray->Bind();

		DrawIndices(vertexArray);

		shader->Unbind();
		vertexArray->Unbind();
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const nlm::mat4& transform)
	{
		if (!shader->HasCompiledSuccessfully())
			return;

		shader->Bind();
		// shader->SetUniformMat4("u_View", s_SceneData->ViewMatrix);
		// shader->SetUniformMat4("u_Projection", s_SceneData->ProjectionMatrix);
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

	void Renderer::DrawSprite(const Ref<Shader>& shader,
		const Ref<Texture2D>& texture,
		const nlm::mat4& transform,
		const nlm::vec4& color,
		SpriteCameraReaction camReact,
		int entityId)
	{
		static Ref<VertexArray> vao = nullptr;
		if (vao == nullptr)
		{
			vao = VertexArray::Create();
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" }
			};
			float vertices[] = { 0.0f, 0.0f, 0.0f };
			Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
			vbo->SetLayout(layout);
			vao->AddVertexBuffer(vbo);
		}

		NL_ENGINE_ASSERT(shader, "Shader is nullptr!");
		NL_ENGINE_ASSERT(texture, "Texture is nullptr!");

		if (!shader->HasCompiledSuccessfully())
			return;

		shader->Bind();

		// Typical
		// shader->SetUniformMat4("u_View", s_SceneData->ViewMatrix);
		// shader->SetUniformMat4("u_Projection", s_SceneData->ProjectionMatrix);
		shader->SetUniformMat4("u_Transform", transform);
		//shader->SetUniformInt("u_IsSelected", isSelected ? 1 : 0);
		shader->SetUniformInt("u_EntityId", entityId);
		shader->SetUniformFloat4("u_Color", color);
		shader->SetUniformInt("u_Sprite", 0);
		shader->SetUniformInt("u_CameraReaction", (int)camReact);
		texture->Bind(0);

		vao->Bind();

		Renderer::DrawArrays_Points(0, 1);

		shader->Unbind();
		vao->Unbind();

	}

	void Renderer::DrawModel(const Ref<Model>& model, const nlm::mat4& transform, int entityId, const std::vector<PointLightShadingData>& points, const std::vector<DirLightShadingData>& dirs)
	{
		const auto& meshes = model->GetMeshes();
		bool hasBones = model->HasBones();
		std::vector<nlm::mat4> emptyMatrices = {};

		for (const auto& mesh : meshes)
		{
			const auto& material = model->GetMaterial(mesh);
			Submit(mesh->GetVertexArray(), material, points, dirs, transform, hasBones ? model->GetFinalBoneMatrices() : emptyMatrices, entityId);
		}
	}

	void Renderer::BindCustomShaderProperties(const Ref<Material>& material)
	{
		const auto& shader = material->GetShader();
		// Custom
		int cntSampler2D = 0;
		for (const auto& prop : material->GetShaderProperties())
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

			case ShaderUniformType::Float:
				shader->SetUniformFloat(prop.Name.c_str(), std::get<float>(prop.Value));
				break;

			default:
				break;
			}
		}
	}

	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		SetViewPort(0, 0, width, height);
	}

	
}