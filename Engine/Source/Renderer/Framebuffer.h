#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Core/Math/Math.h"

#include <initializer_list>
#include <vector>

namespace NL
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RedInteger,
		
		// 
		RGBA16F,

		// Depth/stencil
		Depth24Stencil8,
		Depth32F3D,

		// Defaults
		Depth = Depth24Stencil8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void BindTex3D(uint32_t slot) = 0;
		// For post-processing
		virtual void BindOneColorOnly(uint32_t attachmentIndex) = 0;
		virtual void Unbind() = 0;
		virtual void UnbindTex3D(uint32_t slot) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, const nlm::vec4& color) = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;

		virtual void ColorBlit(uint32_t attachmentIndex, Ref<Framebuffer>& dst) = 0;
		virtual void ColorBlit(uint32_t srcAttachmentIndex, Ref<Framebuffer>& dst, uint32_t dstAttachmentIndex) = 0;
		virtual int GetColorAttachmentsCount() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}