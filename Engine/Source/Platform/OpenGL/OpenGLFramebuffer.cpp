#include "nlpch.h"

#include "OpenGLFramebuffer.h"
#include "Core/Log/Log.h"

#include <glad/glad.h>

namespace NL
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(bool isMultisample)
		{
			return isMultisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool isMultisample, uint32_t count, uint32_t* outID)
		{
			glCreateTextures(TextureTarget(isMultisample), count, outID);
		}

		static void BindTexture(bool isMultisample, uint32_t id)
		{
			glBindTexture(TextureTarget(isMultisample), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, GLenum type, uint32_t width, uint32_t height, int index, bool isRealColorTex)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);

				if (isRealColorTex)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool isMultisample = samples > 1;
			if (isMultisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(isMultisample), id, 0);
		}

		static void AttachDepthTexture3D(uint32_t id, GLenum format, uint32_t width, uint32_t height, int depth = 5)
		{
			glGenTextures(1, &id);

			glBindTexture(GL_TEXTURE_2D_ARRAY, id);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, width, height, depth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto tex : m_Specification.Attachments.Attachments)
		{
			switch (tex.TextureFormat)
			{
			case FramebufferTextureFormat::RGBA8:
			case FramebufferTextureFormat::RedInteger:
			case FramebufferTextureFormat::RGBA16F:
				m_ColorAttachmentSpecifications.emplace_back(tex);
				break;
			case FramebufferTextureFormat::Depth24Stencil8:
			case FramebufferTextureFormat::Depth32F3D:
				m_DepthAttachmentSpecification = tex;
				break;
			}
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		if (m_ColorAttachments.size() >= 1)
		{
			NL_ENGINE_ASSERT(m_ColorAttachments.size() <= 4, "Color attachments size greater than 4!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}
	}

	void OpenGLFramebuffer::BindTex3D(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_DepthAttachment);
	}

	void OpenGLFramebuffer::BindOneColorOnly(uint32_t attachmentIndex)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		if (m_ColorAttachments.size() >= 1)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		}
		else
		{
			glDrawBuffer(GL_NONE);
		}
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GLenum buffers[1] = { GL_FRONT_LEFT };
		glDrawBuffers(1, buffers);
	}

	void OpenGLFramebuffer::UnbindTex3D(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			NL_ENGINE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		NL_ENGINE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index invalid!");

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		// NL_ENGINE_TRACE("Read pixel data: {0}", pixelData);
		// There may be bugs...
		//if (pixelData > 0 && pixelData < 1)
			//return -1;
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		if (attachmentIndex >= m_ColorAttachments.size())
			return;
		// NL_ENGINE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index invalid!");

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];

		switch (spec.TextureFormat)
		{
		case FramebufferTextureFormat::RedInteger:
			// glClearTexImage(m_ColorAttachments[attachmentIndex], 0, GL_R32I, GL_INT, &value);
			glClearBufferiv(GL_COLOR, attachmentIndex, &value);
			break;
		case FramebufferTextureFormat::RGBA16F:
			glClearTexImage(m_ColorAttachments[attachmentIndex], 0, GL_RGBA16F, GL_INT, &value);
			break;
		case FramebufferTextureFormat::RGBA8:
			glClearTexImage(m_ColorAttachments[attachmentIndex], 0, GL_RGBA8, GL_INT, &value);
			break;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, const nlm::vec4& color)
	{
		if (attachmentIndex >= m_ColorAttachments.size())
			return;
		// NL_ENGINE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index invalid!");

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];

		switch (spec.TextureFormat)
		{
		case FramebufferTextureFormat::RGBA16F:
		case FramebufferTextureFormat::RGBA8:
			glClearBufferfv(GL_COLOR, attachmentIndex, nlm::value_ptr(color));
			break;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t OpenGLFramebuffer::GetRendererID() const
	{
		return m_RendererID;
	}

	const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const
	{
		return m_Specification;
	}

	uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const
	{
		NL_ENGINE_ASSERT(index < m_ColorAttachments.size(), "Attachment index invalid!");
		
		return m_ColorAttachments[index];
	}

	uint32_t OpenGLFramebuffer::GetDepthAttachmentRendererID() const
	{
		return m_DepthAttachment;
	}

	void OpenGLFramebuffer::ColorBlit(uint32_t attachmentIndex, Ref<Framebuffer>& dst)
	{
		if (attachmentIndex >= m_ColorAttachments.size() || attachmentIndex >= dst->GetColorAttachmentsCount())
			return;
		// NL_ENGINE_INFO("Color Blit: {0}", attachmentIndex);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->GetRendererID());
		glNamedFramebufferReadBuffer(m_RendererID, GL_COLOR_ATTACHMENT0 + attachmentIndex);
		glNamedFramebufferDrawBuffer(dst->GetRendererID(), GL_COLOR_ATTACHMENT0 + attachmentIndex);
		glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::ColorBlit(uint32_t srcAttachmentIndex, Ref<Framebuffer>& dst, uint32_t dstAttachmentIndex)
	{
		if (srcAttachmentIndex >= m_ColorAttachments.size() || dstAttachmentIndex >= dst->GetColorAttachmentsCount())
			return;
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->GetRendererID());
		glNamedFramebufferReadBuffer(m_RendererID, GL_COLOR_ATTACHMENT0 + srcAttachmentIndex);
		glNamedFramebufferDrawBuffer(dst->GetRendererID(), GL_COLOR_ATTACHMENT0 + dstAttachmentIndex);
		glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool isMultisample = m_Specification.Samples > 1;

		// Color Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(isMultisample, m_ColorAttachments.size(), m_ColorAttachments.data());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(isMultisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, m_Specification.Width, m_Specification.Height, i, true);
					break;
				case FramebufferTextureFormat::RedInteger:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, GL_INT, m_Specification.Width, m_Specification.Height, i, false);
					break;
				case FramebufferTextureFormat::RGBA16F:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA16F, GL_BGRA, GL_FLOAT, m_Specification.Width, m_Specification.Height, i, false);
					break;
				}
			}
		}

		// Depth Attachments
		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(isMultisample, 1, &m_DepthAttachment);
			Utils::BindTexture(isMultisample, m_DepthAttachment);

			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::Depth24Stencil8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			case FramebufferTextureFormat::Depth32F3D:
				Utils::AttachDepthTexture3D(m_DepthAttachment, GL_DEPTH_COMPONENT32, m_Specification.Width, m_Specification.Height);
				break;
			}
		}

		// ?
		if (m_ColorAttachments.size() > 1)
		{
			NL_ENGINE_ASSERT(m_ColorAttachments.size() <= 4, "Color attachments size greater than 4!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		NL_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}