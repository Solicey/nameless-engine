#include "nlpch.h"

#include "Shader.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Core/Misc/PtrWrapper.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace NL
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::filesystem::path& path)
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, path.string());
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}