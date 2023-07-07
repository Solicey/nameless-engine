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

	Ref<Framebuffer> Renderer::s_LightFBO = nullptr;

	void Renderer::SetUniformBuffer(Ref<EditorCamera> camera)
	{
		//s_SceneData->ViewPositionMatrix = camera.GetViewProjectionMatrix();
		Ref<UniformBuffer> cameraUniform = Library<UniformBuffer>::GetInstance().GetCameraUniformBuffer();
		nlm::mat4 ViewMatrix = camera->GetViewMatrix();
		nlm::mat4 ProjMatrix = camera->GetProjectionMatrix();
		nlm::vec3 CamPosition = camera->GetPosition();
		auto type = camera->GetProjectionType();
		float nearClip, farClip;
		if (type == Camera::ProjectionType::Orthographic)
		{
			nearClip = camera->GetOrthographicNear();
			farClip = camera->GetOrthographicFar();
		}
		else
		{
			nearClip = camera->GetPerspectiveNear();
			farClip = camera->GetPerspectiveFar();
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

	void Renderer::SetUniformBuffer(Ref<Camera> camera, const nlm::mat4& transform, const nlm::vec3& position)
	{
		//s_SceneData->ViewPositionMatrix = camera.GetProjectionMatrix() * nlm::inverse(transform);
		Ref<UniformBuffer> cameraUniform = Library<UniformBuffer>::GetInstance().GetCameraUniformBuffer();
		nlm::mat4 ViewMatrix = nlm::inverse(transform);
		nlm::mat4 ProjMatrix = camera->GetProjectionMatrix();
		auto type = camera->GetProjectionType();
		float nearClip, farClip;
		if (type == Camera::ProjectionType::Orthographic)
		{
			nearClip = camera->GetOrthographicNear();
			farClip = camera->GetOrthographicFar();
		}
		else
		{
			nearClip = camera->GetPerspectiveNear();
			farClip = camera->GetPerspectiveFar();
		}
		cameraUniform->SetData(&ViewMatrix, 64, 0);
		cameraUniform->SetData(&ProjMatrix, 64, 64);
		cameraUniform->SetData(&position, 12, 128);
		cameraUniform->SetData(&nearClip, 4, 140);
		cameraUniform->SetData(&farClip, 4, 144);
	}

	void Renderer::BindCustomShaderProperties(const Ref<Material>& material, int sampler2DStartIndex)
	{
		const auto& shader = material->GetShader();
		// Custom
		int cntSampler2D = sampler2DStartIndex;
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
				Library<Texture2D>::GetInstance().Fetch(std::get<std::string>(prop.Value), true)->Bind(cntSampler2D);
				cntSampler2D++;
				break;

			case ShaderUniformType::Float:
				shader->SetUniformFloat(prop.Name.c_str(), std::get<float>(prop.Value));
				break;

			case ShaderUniformType::Int:
				shader->SetUniformInt(prop.Name.c_str(), std::get<int>(prop.Value));
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

	void Renderer::BindLightsData(const Ref<Shader>& shader, const std::vector<PointLightShadingData>& points, const std::vector<DirLightShadingData>& dirs)
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

	
}