#pragma once

#include "Core/Misc/PtrWrapper.h"

namespace NL
{
	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(uint32_t width, uint32_t height, int samples);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual int GetSamples() const = 0;

		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetColorTextureID() const = 0;
		virtual uint32_t GetEntityTextureID() const = 0;
		virtual uint32_t GetDepthStencilBufferID() const = 0;

		virtual void ClearEntityTexture(int color) = 0;
		virtual int ReadEntityTexture(int x, int y) = 0;
	};
}