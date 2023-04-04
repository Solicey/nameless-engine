#pragma once

#include "Resources/Texture.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Buffers.h"

// Credit to HEngine

namespace NL
{
	enum class PostProcessingType
	{
		None = 0,
		MSAA,
		Outline
	};

	class PostProcessing
	{
	public:
		PostProcessing(const PostProcessingType& type);
		virtual ~PostProcessing() {}
		static void Init();
		virtual uint32_t ExecuteAndReturnFinalTex() { return 0; }

	protected:
		void Execute();

	public:
		PostProcessingType m_Type;
		static Ref<Framebuffer> m_Framebuffer;
		static Ref<Texture2D> m_ColorTex;
		static Ref<Texture2D> m_EntityTex;

	protected:
		static Ref<VertexArray> m_ScreenQuadVAO;
		static Ref<VertexBuffer> m_ScreenQuadVBO;
		static Ref<IndexBuffer> m_ScreenQuadIBO;

	};
}