#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/VertexArray.h"
#include "Resources/Texture.h"

#include <vector>

namespace NL
{
	enum class PostProcessingType
	{
		None,
		GrayScale,

	};

	class PostProcessing
	{
	public:
		virtual uint32_t ExecutePostProcessingQueue(const std::vector<PostProcessingType>& queue, Ref<Framebuffer>& srcFramebuffer) = 0;

		static Ref<PostProcessing> Create();

	protected:
		// Ping-pong texturing
		Ref<Framebuffer> m_FBO;
		// tex0 & tex1 are color tex
		uint32_t m_Tex0;
		uint32_t m_Tex1;

		Ref<VertexArray> m_QuadVAO;
		Ref<VertexBuffer> m_QuadVBO;
		Ref<IndexBuffer> m_QuadIBO;
	};
}