#include "nlpch.h"
#include "OpenGLPostprocessing.h"

#include "Resources/Libraries/ShaderLibrary.h"

#include <glad/glad.h>

namespace NL
{
	OpenGLPostProcessing::OpenGLPostProcessing(const PostProcessingType& type)
		: PostProcessing(type)
	{
		switch (m_Type)
		{
		case NL::PostProcessingType::None:
			break;
		case NL::PostProcessingType::Outline:
			m_Shader = Library<Shader>::GetInstance().LoadShader("Post_Outline.glsl");
			break;
		default:
			break;
		}
	}

	uint32_t OpenGLPostProcessing::ExecuteAndReturnFinalTex()
	{
		uint32_t rendererID = 0;

		switch (m_Type)
		{
		case NL::PostProcessingType::None:
			return 0;
			break;
		case NL::PostProcessingType::Outline:
			rendererID = ExecuteWithShader();
			break;
		default:
			break;
		}

		return rendererID;
	}

	uint32_t OpenGLPostProcessing::ExecuteWithShader()
	{
		if (m_Shader == nullptr)
			return 0;

		/*m_Framebuffer->CopyColorAttachment2Texture(0, m_ColorTex->GetRendererID());

		m_Framebuffer->BindDrawFramebuffer();
		m_Shader->Bind();

		m_Shader->SetUniformInt("u_ScreenTex", 0);
		m_Shader->SetUniformInt("u_EntityTex", 1);

		m_ColorTex->Bind(0);
		m_EntityTex->Bind(1);

		Execute();
		
		m_Framebuffer->UnbindDrawFramebuffer();
		m_Shader->Unbind();

		return m_Framebuffer->GetColorAttachmentRendererID(0);*/
		
	}
}
