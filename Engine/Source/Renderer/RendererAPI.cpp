#include "nlpch.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace NL
{
	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case NL::RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case NL::RendererAPI::API::OpenGL:
			return CreateScope<OpenGLRendererAPI>();
		case NL::RendererAPI::API::DX11:
			NL_ENGINE_ASSERT(false, "RendererAPI::DX11 is currently not supported!");
			return nullptr;
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}