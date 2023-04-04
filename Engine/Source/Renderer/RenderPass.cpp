#include "nlpch.h"

#include "RenderPass.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLPostprocessing.h"

#include <glad/glad.h>

namespace NL
{
	RenderPass::RenderPass()
	{
		PostProcessing::Init();
	}

	void RenderPass::AddPostprocessing(PostProcessingType type)
	{
		switch (RendererAPI::GetCurrent())
		{
		case NL::RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return;
		case NL::RendererAPI::API::OpenGL:
			m_PostProcessings.emplace_back(CreateScope<OpenGLPostProcessing>(type));
			return;
		case NL::RendererAPI::API::DX11:
			NL_ENGINE_ASSERT(false, "RendererAPI::DX11 is currently not supported!");
			return;
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
	}

	uint32_t RenderPass::ExecuteAndReturnFinalTex(const Ref<Framebuffer>& srcBuffer)
	{
		/*uint32_t width = srcBuffer->GetSpecification().Width;
		uint32_t height = srcBuffer->GetSpecification().Height;

		PostProcessing::m_ColorTex = Texture2D::Create(width, height);
		PostProcessing::m_EntityTex = Texture2D::Create(width, height);

		// Blit
		srcBuffer->BindReadFramebuffer();
		PostProcessing::m_Framebuffer->BindDrawFramebuffer();
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		srcBuffer->UnbindReadFramebuffer();
		PostProcessing::m_Framebuffer->UnbindDrawFramebuffer();

		// Copy textures except main color
		srcBuffer->CopyColorAttachment2Texture(1, PostProcessing::m_EntityTex->GetRendererID());

		uint32_t final = 0;
		for (auto& effect : m_PostProcessings)
		{
			final = effect->ExecuteAndReturnFinalTex();
		}

		return final;*/
		return 0;
	}
}