#include "nlpch.h"

#include "Core/Log/Log.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace NL
{
	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, int samples)
		: m_Width(width), m_Height(height), m_Samples(samples)
	{
		glGenFramebuffers(1, &m_RendererID);
		glGenTextures(1, &m_ColorTexID);
		glGenRenderbuffers(1, &m_EntityBufID);
		glGenRenderbuffers(1, &m_DepthBufID);

		// Setup color texture
		if (samples <= 1)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorTexID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorTexID);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}

		Resize(width, height);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(2, buffers);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GLenum buffers[] = { GL_FRONT_LEFT };
		glDrawBuffers(1, buffers);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		// Color texture
		if (m_Samples <= 1)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorTexID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexID, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorTexID);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Samples, GL_RGBA8, width, height, GL_TRUE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorTexID, 0);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}

		// Setup EntityId buffer
		glBindRenderbuffer(GL_RENDERBUFFER, m_EntityBufID);
		if (m_Samples < 1)
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_R32I, width, height);
		}
		else
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, GL_R32I, width, height);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, m_EntityBufID);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		// Setup depth-stencil buffer (24 + 8 bits)
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufID);
		if (m_Samples < 1)
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		}
		else
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, GL_DEPTH24_STENCIL8, width, height);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufID);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(2, buffers);

		NL_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	}

	void OpenGLFramebuffer::ClearEntityRenderBuffer(int color)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glClearBufferiv(GL_COLOR, 1, &color);
	}

	int OpenGLFramebuffer::ReadEntityRenderBuffer(int x, int y)
	{
		static bool hasInit = false;
		static unsigned int intFBO;
		static unsigned int intRBO;

		if (!hasInit)
		{
			glGenFramebuffers(1, &intFBO);
			glGenRenderbuffers(1, &intRBO);
			hasInit = true;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, intFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, intRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_R32I, m_Width, m_Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, intRBO);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intFBO);
		glNamedFramebufferReadBuffer(m_RendererID, GL_COLOR_ATTACHMENT1);
		glNamedFramebufferDrawBuffer(intFBO, GL_COLOR_ATTACHMENT1);
		glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, intFBO);
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		// Reset bind state
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		return pixelData;
	}
}
