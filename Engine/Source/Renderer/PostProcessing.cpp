#include "nlpch.h"
#include "PostProcessing.h"

#include "Renderer/Renderer.h"
#include "Core/Misc/ConfigManager.h"

namespace NL
{
	Ref<Framebuffer> PostProcessing::m_Framebuffer = nullptr;
	Ref<Texture2D> PostProcessing::m_ColorTex = nullptr;
	Ref<Texture2D> PostProcessing::m_EntityTex = nullptr;
	Ref<VertexArray> PostProcessing::m_ScreenQuadVAO = nullptr;
	Ref<VertexBuffer> PostProcessing::m_ScreenQuadVBO = nullptr;
	Ref<IndexBuffer> PostProcessing::m_ScreenQuadIBO = nullptr;

	PostProcessing::PostProcessing(const PostProcessingType& type)
		: m_Type(type)
	{
	}

	void PostProcessing::Init()
	{
		/*m_ScreenQuadVAO = VertexArray::Create();
		float screenQuadVertices[] = {
			// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		uint32_t screenQuadIndices[] = {
			0, 1, 2, 0, 2, 3
		};
		m_ScreenQuadIBO = IndexBuffer::Create(screenQuadIndices, 6);
		m_ScreenQuadVBO = VertexBuffer::Create(screenQuadVertices, sizeof(screenQuadVertices));
		m_ScreenQuadVBO->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		m_ScreenQuadVAO->AddVertexBuffer(m_ScreenQuadVBO);
		m_ScreenQuadVAO->SetIndexBuffer(m_ScreenQuadIBO);

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RedInteger };
		fbSpec.Width = ConfigManager::GetInstance().GetViewportInitWidth();
		fbSpec.Height = ConfigManager::GetInstance().GetViewportInitHeight();
		m_Framebuffer = Framebuffer::Create(fbSpec);*/
	}

	void PostProcessing::Execute()
	{
		Renderer::DrawIndices(m_ScreenQuadVAO);
	}
}
