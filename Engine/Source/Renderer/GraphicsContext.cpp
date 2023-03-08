#include "nlpch.h"

#include "GraphicsContext.h"
#include "Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace NL
{
	Ref<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}