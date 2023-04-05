#include "nlpch.h"
#include "OpenGLPostProcessing.h"

#include "Renderer/Renderer.h"
#include "Resources/Libraries/ShaderLibrary.h"

#include <glad/glad.h>

namespace NL
{
    OpenGLPostProcessing::OpenGLPostProcessing()
    {
        Init();
    }

    uint32_t OpenGLPostProcessing::ExecutePostProcessingQueue(const std::vector<PostProcessingType>& queue, Ref<Framebuffer>& srcFramebuffer)
    {
        // uint32_t srcColorTex = srcFramebuffer->GetColorAttachmentRendererID(0);
        // uint32_t srcEntityIDTex = srcFramebuffer->GetColorAttachmentRendererID(1);
        
        const auto& srcSpec = srcFramebuffer->GetSpecification();
        const auto& spec = m_FBO->GetSpecification();
        if (srcSpec.Width != spec.Width || srcSpec.Height != spec.Height)
            m_FBO->Resize(srcSpec.Width, srcSpec.Height);

        srcFramebuffer->ColorBlit(0, m_FBO, 0);

        for (int i = 0; i < queue.size(); i++)
        {
            // srcAttachment texture attachment id
            uint32_t srcAttachment = (i % 2 == 0) ? 0 : 1;
            // dstAttachment texture attachment id
            uint32_t dstAttachment = (i % 2 == 0) ? 1 : 0;

            switch (queue[i])
            {
            case PostProcessingType::None:
            {
                m_FBO->ColorBlit(srcAttachment, m_FBO, dstAttachment);
                break;
            }
            case PostProcessingType::GrayScale:
            {
                GrayScale(srcAttachment, dstAttachment);
                break;
            }
            default:
                break;
            }
        }

        if (queue.size() % 2 == 0)
            return m_Tex0;
        return m_Tex1;
    }

    void OpenGLPostProcessing::Init()
	{
		m_QuadVAO = VertexArray::Create();
        float vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };
        uint32_t indices[] = {
            0, 1, 2, 0, 2, 3
        };

        m_QuadVBO = VertexBuffer::Create(vertices, sizeof(vertices));
        m_QuadVBO->SetLayout({
            { ShaderDataType::Float2, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoords" },
            });
        m_QuadVAO->AddVertexBuffer(m_QuadVBO);

        m_QuadIBO = IndexBuffer::Create(indices, 6);
        m_QuadVAO->SetIndexBuffer(m_QuadIBO);

        // Init Framebuffer
        FramebufferSpecification spec;
        spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8 };
        spec.Width = 1280;
        spec.Height = 720;
        m_FBO = Framebuffer::Create(spec);

        m_Tex0 = m_FBO->GetColorAttachmentRendererID(0);
        m_Tex1 = m_FBO->GetColorAttachmentRendererID(1);

        // Load shaders
        m_GrayScaleShader = Library<Shader>::GetInstance().LoadShader("Post_GrayScale.glsl");
	}

    void OpenGLPostProcessing::GrayScale(uint32_t srcAttachment, uint32_t dstAttachment)
    {
        uint32_t srcTex = m_FBO->GetColorAttachmentRendererID(srcAttachment);

        m_FBO->BindOneColorOnly(dstAttachment);
        glDisable(GL_DEPTH_TEST);

        m_GrayScaleShader->Bind();

        // Set shader uniform
        m_GrayScaleShader->SetUniformInt("u_ColorTex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, srcTex);

        m_QuadVAO->Bind();

        Renderer::DrawIndices(m_QuadVAO);

        m_QuadVAO->Unbind();
        m_GrayScaleShader->Unbind();
        m_FBO->Unbind();
        glEnable(GL_DEPTH_TEST);
    }
}
