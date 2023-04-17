#include "nlpch.h"

#include "PostProcessing.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLPostProcessing.h"

namespace NL
{
	Ref<PostProcessing> PostProcessing::Create()
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLPostProcessing>();
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
