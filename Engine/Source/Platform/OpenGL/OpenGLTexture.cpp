#include "nlpch.h"

#include "OpenGLTexture.h"
#include "Core/Log/Log.h"
#include "Core/Math/Math.h"

#include <stb_image.h>

namespace NL
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		// for OpenGL
		stbi_set_flip_vertically_on_load(0);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			m_IsLoaded = true;
			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (channels == 1)
			{
				internalFormat = GL_RGB;
				dataFormat = GL_UNSIGNED_BYTE;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			NL_ENGINE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// 1字节对齐读取数据，以适配各种通道的格式
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			GLenum type = internalFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE;
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, type, data);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			stbi_image_free(data);

			NL_ENGINE_TRACE("Load texture {0} successfully!", path);
		}
		else
		{
			NL_ENGINE_ASSERT(false, "Failed to load textures!");
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const std::vector<nlm::vec4>& data)
	{
		m_InternalFormat = GL_RGBA16F;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_FLOAT, &data[0]);

		// For Noise Textures
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		NL_ENGINE_TRACE("glDeleteTexture {0}", m_Path);
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE, 0);
	}


	OpenGLTextureCubeMap::OpenGLTextureCubeMap(const std::vector<std::string>& texturePaths)
		: m_Paths(texturePaths)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		int width = 0, height = 0, channels = 0;
		stbi_set_flip_vertically_on_load(0);

		for (int i = 0; i < m_Paths.size(); i++)
		{
			stbi_uc* data = stbi_load(m_Paths[i].c_str(), &width, &height, &channels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
				m_IsLoaded = true;
			}
			else
			{
				NL_ENGINE_ASSERT(false, "Failed to load cubemap texture!");
				stbi_image_free(data);
			}
		}

		m_Width = width;
		m_Height = height;

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	/*void OpenGLTextureCubeMap::AddTexturePath(CubeMapFace faceIndex, const std::string& path)
	{
		m_Paths[(uint32_t)faceIndex] = path;
	}*/

	void OpenGLTextureCubeMap::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void OpenGLTextureCubeMap::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}