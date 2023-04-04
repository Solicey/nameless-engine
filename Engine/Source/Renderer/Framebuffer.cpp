#include "nlpch.h"

#include "Framebuffer.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace NL
{
	Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height, int samples)
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(width, height, samples);
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}