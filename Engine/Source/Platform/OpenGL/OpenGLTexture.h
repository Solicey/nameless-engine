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
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height, const std::vector<nlm::vec4>& data);
		~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat;
		std::string m_Path;
	};

	class OpenGLTextureCubeMap : public TextureCubeMap
	{
	public:
		OpenGLTextureCubeMap(const std::vector<std::string>& texturePaths);
		// virtual void AddTexturePath(CubeMapFace faceIndex, const std::string& path) override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		// virtual const std::string& GetPath() const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		bool m_IsLoaded = false;
		std::vector<std::string> m_Paths;
	};
}