#pragma once

#include "Renderer/Framebuffer.h"

namespace NL
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;

		virtual void BindTex3D(uint32_t slot) override;

		virtual void BindOneColorOnly(uint32_t attachmentIndex) override;

		virtual void Unbind() override;

		virtual void UnbindTex3D(uint32_t slot) override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, const nlm::vec4& color) override;

		virtual const FramebufferSpecification& GetSpecification() const override;

		virtual uint32_t GetRendererID() const override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;

		virtual uint32_t GetDepthAttachmentRendererID() const override;

		virtual void ColorBlit(uint32_t attachmentIndex, Ref<Framebuffer>& dst) override;

		virtual void ColorBlit(uint32_t srcAttachmentIndex, Ref<Framebuffer>& dst, uint32_t dstAttachmentIndex) override;

		virtual int GetColorAttachmentsCount() const override { return m_ColorAttachments.size(); }

	private:
		/*
		* ÖØÐÂ°ó¶¨Framebuffer
		*/
		void Invalidate();

	private:
		uint32_t m_RendererID;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		std::vector<uint32_t> m_ColorAttachments;

		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		uint32_t m_DepthAttachment = 0;
	};
}