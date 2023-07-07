#pragma once

// Tmp
#include "Camera/OrthographicCamera.h"
#include "Camera/EditorCamera.h"
#include "Core/Misc/PtrWrapper.h"
#include "Resources/Shader.h"
#include "Resources/Model.h"
#include "Renderer/VertexArray.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Framebuffer.h"

namespace NL
{
	enum class SpriteCameraReaction
	{
		Normal,
		LookAt,
		Billboarding
	};

	struct PointLightShadingData
	{
		nlm::vec3 Position;
		nlm::vec3 Color;
		nlm::vec3 Attenuation;
		uint32_t EntityId;
	};

	struct DirLightShadingData
	{
		nlm::vec3 Position;
		nlm::vec3 Direction;
		nlm::vec3 Color;
		uint32_t EntityId;
	};

	class Renderer
	{
	public:
		static void SetUniformBuffer(Ref<EditorCamera> camera);
		static void SetUniformBuffer(Ref<Camera> camera, const nlm::mat4& transform, const nlm::vec3& position);

		// Bind shader first!
		static void BindCustomShaderProperties(const Ref<Material>& mat, int sampler2DStartIndex = 0);

		static void OnWindowResize(unsigned int width, unsigned int height);

		// Bind shader first!
		static void BindLightsData(const Ref<Shader>& shader, const std::vector<PointLightShadingData>& points, const std::vector<DirLightShadingData>& dirs);


#pragma region Commands

		inline static void Init()
		{
			s_RendererAPI->Init();

			FramebufferSpecification spec;
			spec.Attachments = { FramebufferTextureFormat::Depth32F3D };
			spec.Width = 4096;
			spec.Height = 4096;
			s_LightFBO = Framebuffer::Create(spec);
		}

		inline static void SetClearColor(const nlm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndices(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndices(vertexArray);
		}

		inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void DepthTest(bool enable)
		{
			s_RendererAPI->DepthTest(enable);
		}

		inline static void DepthFunc(DepthComp comp)
		{
			s_RendererAPI->DepthFunc(comp);
		}

		inline static void SetCullFace(CullFace face)
		{
			s_RendererAPI->SetCullFace(face);
		}

		inline static void EnableCullFace(bool enable)
		{
			s_RendererAPI->EnableCullFace(enable);
		}

		// tmp
		inline static void BeginTransformFeedback_Points()
		{
			s_RendererAPI->BeginTransformFeedback_Points();
		}

		inline static void EndTransformFeedback()
		{
			s_RendererAPI->EndTransformFeedback();
		}

		inline static void DrawArrays_Points(int first, uint32_t count)
		{
			s_RendererAPI->DrawArrays_Points(first, count);
		}

		inline static void RasterizerDiscard(bool enable)
		{
			s_RendererAPI->RasterizerDiscard(enable);
		}

		inline static void EnableBlend(bool enable)
		{
			s_RendererAPI->EnableBlend(enable);
		}

		/// <summary>
		/// enable write into depth buffer
		/// </summary>
		/// <param name="enable"></param>
		inline static void DepthMask(bool enable)
		{
			s_RendererAPI->DepthMask(enable);
		}

		inline static void BlendFunc(BlendFactor src, BlendFactor dst)
		{
			s_RendererAPI->BlendFunc(src, dst);
		}

		inline static void BlendEquation()
		{

		}

		inline static int GetDrawFramebuffer()
		{
			return s_RendererAPI->GetDrawFramebuffer();
		}

		inline static void BindFramebuffer(uint32_t framebufferID)
		{
			s_RendererAPI->BindFramebuffer(framebufferID);
		}

#pragma endregion

	public:
		static Ref<Framebuffer> s_LightFBO;

	private:
		// static Scope<SceneData> s_SceneData;
		static Scope<RendererAPI> s_RendererAPI;

	};
}