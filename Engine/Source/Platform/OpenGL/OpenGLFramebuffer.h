#pragma once

#include "Renderer/Framebuffer.h"

namespace NL
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height, int samples);

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual int GetSamples() const override { return m_Samples; }

		virtual uint32_t GetRendererID() const { return m_RendererID; }
		virtual uint32_t GetColorTextureID() const { return m_ColorTexID; }
		virtual uint32_t GetEntityTextureID() const { return m_EntityTexID; }
		virtual uint32_t GetDepthStencilBufferID() const { return m_DepthBufID; }

		virtual void ClearEntityTexture(int color) override;
		virtual int ReadEntityTexture(int x, int y) override;

	private:
		uint32_t m_Width;
		uint32_t m_Height;
		int m_Samples;

		uint32_t m_RendererID;
		uint32_t m_ColorTexID;
		uint32_t m_EntityTexID;
		uint32_t m_DepthBufID;
	};
}