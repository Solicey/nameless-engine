#include "nlpch.h"

#include "TransformFeedback.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTransformFeedback.h"

namespace NL
{
	Ref<TransformFeedback> TransformFeedback::Create()
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTransformFeedback>();
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
