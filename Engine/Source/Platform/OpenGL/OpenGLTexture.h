#pragma once

#include "Resources/Texture.h"

#include <glad/glad.h>
#include <filesystem>

namespace NL
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::filesystem::path& path);
		~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat;
		std::filesystem::path m_Path;
	};
}