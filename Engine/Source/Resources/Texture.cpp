#include "nlpch.h"

#include "Texture.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace NL
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, const std::vector<nlm::vec4>& data)
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height, data);
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<TextureCubeMap> TextureCubeMap::Create(const std::vector<std::string>& texturePaths)
	{
		switch (RendererAPI::GetCurrent())
		{
		case RendererAPI::API::None:
			NL_ENGINE_ASSERT(false, "RendererAPI::None not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTextureCubeMap>(texturePaths);
		}

		NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}