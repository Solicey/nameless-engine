#include "nlpch.h"
#include "OpenGLPostProcessing.h"

#include "Renderer/Renderer.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Resources/Libraries/TextureLibrary.h"

#include <glad/glad.h>
#include <random>

namespace NL
{
    #define SSAO_SAMPLE_COUNT 256

    OpenGLPostProcessing::OpenGLPostProcessing()
    {
        m_SSAONoiseTex = Library<Texture2D>::GetInstance().Fetch(Library<Texture2D>::GetInstance().GetSSAONoiseTextureName(), false);

        // SSAO Kernel Generate
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // Ëæ»ú¸¡µãÊý£¬·¶Î§0.0 - 1.0
        std::default_random_engine generator;
        const int sampleCount = SSAO_SAMPLE_COUNT;
        for (GLuint i = 0; i < sampleCount; ++i)
        {
            glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
            );
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = float(i) / sampleCount;
            scale = std::lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            m_SSAOKernel.push_back(sample);
        }

        m_SkyboxTextureCubemap = Library<TextureCubeMap>::GetInstance().FetchDefault();

        Init();
    }

    /*uint32_t OpenGLPostProcessing::ExecutePostProcessingQueue(const std::vector<PostProcessingType>& queue, Ref<Framebuffer>& srcFramebuffer)
    {
        // uint32_t srcColorTex = srcFramebuffer->GetColorAttachmentRendererID(0);
        uint32_t entityIDTex = srcFramebuffer->GetColorAttachmentRendererID(2);
        
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
            case PostProcessingType::EditorOutline:
            {
                EditorOutline(srcAttachment, dstAttachment, entityIDTex);
                break;
            }
            default:
                break;
            }
        }

        if (queue.size() % 2 == 0)
            return m_Tex0;
        return m_Tex1;
    }*/

    uint32_t OpenGLPostProcessing::ExecutePostProcessingQueue(const std::vector<Ref<Material>>& queue, Ref<Framebuffer>& srcFramebuffer, int entityId, const std::vector<PointLightShadingData>& points, const std::vector<DirLightShadingData>& dirs)
    {
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

            auto& mat = queue[i];
            if (mat->GetShaderName() == Library<Shader>::GetInstance().GetDefaultShaderName() || !mat->IsEnabled())
            {
                m_FBO->ColorBlit(srcAttachment, m_FBO, dstAttachment);
                continue;
            }

            auto& shader = mat->GetShader();
            if (shader == nullptr)
                continue;

            uint32_t srcTex = m_FBO->GetColorAttachmentRendererID(srcAttachment);

            m_FBO->BindOneColorOnly(dstAttachment);
            glDisable(GL_DEPTH_TEST);
            shader->Bind();

            shader->SetUniformInt("u_ScreenWidth", m_FBO->GetSpecification().Width);
            shader->SetUniformInt("u_ScreenHeight", m_FBO->GetSpecification().Height);
            shader->SetUniformInt("u_EntityId", entityId);

            // Renderer...
            if (shader->IsLightingRequired())
            {
                Renderer::BindLightsData(shader, points, dirs);
            }

            // WARNING: index should be changed accordingly
            Renderer::BindCustomShaderProperties(mat, 8);

            if (shader->CheckTag(ShaderTag::Skybox))
            {
                shader->SetUniformInt("u_Skybox", 7);
                m_SkyboxTextureCubemap->Bind(7);
            }

            if (shader->CheckTag(ShaderTag::SrcColor))
            {
                shader->SetUniformInt("u_SrcColorTex", 6);
                uint32_t srcTex = srcFramebuffer->GetColorAttachmentRendererID(0);
                glActiveTexture(GL_TEXTURE6);
                glBindTexture(GL_TEXTURE_2D, srcTex);
            }

            // SSAO Noise Tex
            // TODO: Check Shader Tag
            if (shader->CheckTag(ShaderTag::SSAO))
            {
                shader->SetUniformInt("u_NoiseTex", 5);
                m_SSAONoiseTex->Bind(5);
                for (int i = 0; i < SSAO_SAMPLE_COUNT; i++)
                {
                    shader->SetUniformFloat3("u_Samples[" + std::to_string(i) + "]", m_SSAOKernel[i]);
                }
            }

            int count = srcFramebuffer->GetColorAttachmentsCount();

            switch (count)
            {
            case 4:
            {
                // Bind normal tex
                uint32_t normalTex = srcFramebuffer->GetColorAttachmentRendererID(3);
                shader->SetUniformInt("u_NormalTex", 4);
                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, normalTex);
                //NL_ENGINE_INFO("Normal Tex");
            }
            case 3:
            {
                // Bind position tex
                uint32_t positionTex = srcFramebuffer->GetColorAttachmentRendererID(2);
                shader->SetUniformInt("u_PositionDepthTex", 3);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, positionTex);
            }
            case 2:
            {
                // Bind entity tex
                uint32_t entityIDTex = srcFramebuffer->GetColorAttachmentRendererID(1);
                shader->SetUniformInt("u_EntityTex", 2);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, entityIDTex);
            }
            case 1:
            {
                // Bind color tex
                shader->SetUniformInt("u_ColorTex", 1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, srcTex);
            }
            default:
                break;
            }

            if (shader->CheckTag(ShaderTag::Shadow))
            {
                shader->SetUniformInt("u_ShadowMaps", 0);
                Renderer::s_LightFBO->BindTex3D(0);
            }

            m_QuadVAO->Bind();
            Renderer::DrawIndices(m_QuadVAO);
            m_QuadVAO->Unbind();
            
            shader->Unbind();
            m_FBO->Unbind();
            glEnable(GL_DEPTH_TEST);
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
        //m_GrayScaleShader = Library<Shader>::GetInstance().Fetch("Post_GrayScale.glsl");
        // m_EditorOutlineShader_1 = Library<Shader>::GetInstance().Fetch("Post_EditorOutline_1.glsl");
        //m_EditorOutlineShader = Library<Shader>::GetInstance().Fetch("Post_EditorOutline.glsl");
	}

    /*void OpenGLPostProcessing::GrayScale(uint32_t srcAttachment, uint32_t dstAttachment)
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

    void OpenGLPostProcessing::EditorOutline(uint32_t srcAttachment, uint32_t dstAttachment, uint32_t entityTex)
    {
        uint32_t srcTex = m_FBO->GetColorAttachmentRendererID(srcAttachment);

        m_FBO->BindOneColorOnly(dstAttachment);
        glDisable(GL_DEPTH_TEST);
        m_EditorOutlineShader->Bind();

        m_EditorOutlineShader->SetUniformInt("u_ScreenWidth", m_FBO->GetSpecification().Width);
        m_EditorOutlineShader->SetUniformInt("u_ScreenHeight", m_FBO->GetSpecification().Height);

        // Bind entity tex
        m_EditorOutlineShader->SetUniformInt("u_EntityTex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, entityTex);

        // Bind color tex
        m_EditorOutlineShader->SetUniformInt("u_ColorTex", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, srcTex);     

        m_QuadVAO->Bind();
        Renderer::DrawIndices(m_QuadVAO);
        m_QuadVAO->Unbind();

        m_EditorOutlineShader->Unbind();
        m_FBO->Unbind();
        glEnable(GL_DEPTH_TEST);
    }*/
}
