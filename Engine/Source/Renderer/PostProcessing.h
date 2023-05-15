#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/VertexArray.h"
#include "Resources/Texture.h"
#include "Resources/Material.h"

#include <vector>

namespace NL
{
	enum class PostProcessingType
	{
		None,
		EditorOutline,
		GrayScale,
	};

	class PostProcessing
	{
	public:
		// virtual uint32_t ExecutePostProcessingQueue(const std::vector<PostProcessingType>& queue, Ref<Framebuffer>& srcFramebuffer) = 0;
		virtual uint32_t ExecutePostProcessingQueue(const std::vector<Ref<Material>>& queue, Ref<Framebuffer>& srcFramebuffer, int entityId) = 0;

		static Ref<PostProcessing> Create();

	protected:
		// Ping-pong texturing
		Ref<Framebuffer> m_FBO;

		Ref<VertexArray> m_QuadVAO;
		Ref<VertexBuffer> m_QuadVBO;
		Ref<IndexBuffer> m_QuadIBO;
	};
}