#include "nlpch.h"

#include "Shader.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Core/Misc/PtrWrapper.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace NL
{
	std::unordered_map<std::string, ShaderUse> Shader::s_ShaderUseMap =
	{
		{"model", ShaderUse::Model},
		{"post", ShaderUse::PostProcess},
		{"particle1", ShaderUse::Particle1},
		{"particle2", ShaderUse::Particle2}
	};

	std::unordered_map<std::string, ShaderTag> Shader::s_ShaderTagMap =
	{
		{"none", ShaderTag::None},
		{"ssao", ShaderTag::SSAO},
		{"src", ShaderTag::SrcColor},
		{"skybox", ShaderTag::Skybox}
	};

	ShaderUse Shader::ParseAndGetShaderUse(const std::string& path)
	{
		std::string rawSrc = ReadShaderFile(path);
		ShaderUse shaderUse = ShaderUse::Unknown;

		const char* useToken = "#use";
		size_t useTokenLength = strlen(useToken);
		size_t usePos = rawSrc.find(useToken, 0);
		if (usePos != std::string::npos)
		{
			size_t eol = rawSrc.find_first_of("\r\n", usePos);
			size_t begin = rawSrc.find_first_not_of(" ", usePos + useTokenLength);
			std::string use = rawSrc.substr(begin, eol - begin);
			if (s_ShaderUseMap.contains(use))
			{
				shaderUse = s_ShaderUseMap[use];
			}
		}

		return shaderUse;
	}

	std::string Shader::ReadShaderFile(const std::string& path)
	{
		std::string result;
		// binary是为了读取\r\n作为换行符
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			NL_ENGINE_ERROR("Could not open file '{0}'", path);
		}

		return result;
	}

	bool Shader::ShaderFileParser(const std::string& rawSrc, std::string& vertexSrc, std::string& fragmentSrc, std::string& geometrySrc)
	{
		bool vertexSrcParsed = false, fragmentSrcParsed = false;

		// Parsing use
		{
			const char* useToken = "#use";
			size_t useTokenLength = strlen(useToken);
			size_t usePos = rawSrc.find(useToken, 0);
			if (usePos != std::string::npos)
			{
				size_t eol = rawSrc.find_first_of("\r\n", usePos);
				size_t begin = rawSrc.find_first_not_of(" ", usePos + useTokenLength);
				std::string use = rawSrc.substr(begin, eol - begin);
				if (s_ShaderUseMap.contains(use))
				{
					m_Use = s_ShaderUseMap[use];
				}
			}
		}

		// Parsing Tag
		{
			const char* tagToken = "#tag";
			size_t tagTokenLength = strlen(tagToken);
			size_t tagPos = rawSrc.find(tagToken, 0);
			if (tagPos != std::string::npos)
			{
				size_t eol = rawSrc.find_first_of("\r\n", tagPos);
				size_t begin = rawSrc.find_first_not_of(" ", tagPos + tagTokenLength);
				std::string tags = rawSrc.substr(begin, eol - begin);
				while (begin < eol)
				{
					size_t frac = rawSrc.find_first_of(";", begin);
					if (frac == std::string::npos || frac > eol)
						frac = eol;
					std::string tag = rawSrc.substr(begin, frac - begin);
					if (s_ShaderTagMap.contains(tag))
					{
						NL_ENGINE_INFO("Tag Found!: {0}", tag);
						m_Tag = m_Tag | s_ShaderTagMap[tag];
					}
					begin = frac + 1;
				}
			}
		}

		// Check Lighting
		{
			const char* litToken = "#lit";
			size_t litPos = rawSrc.find(litToken, 0);
			size_t litTokenLength = strlen(litToken);
			if (litPos != std::string::npos)
			{
				m_IsLightingRequired = true;
				size_t eol = rawSrc.find_first_of("\r\n", litPos);
				size_t begin = rawSrc.find_first_not_of(" ", litPos + litTokenLength);
				std::string litCount = rawSrc.substr(begin, eol - begin);
				// Temp
				m_MaxLightsCount = std::min(std::max(std::stoi(litCount), 0), 128);
			}
		}

		// Parsing props
		{
			m_Properties.clear();

			const char* propToken = "#prop";
			const char* endToken = "#end";
			size_t propPos = rawSrc.find(propToken, 0);
			if (propPos != std::string::npos)
			{
				size_t endPos = rawSrc.find(endToken, propPos);
				NL_ENGINE_ASSERT(endPos != std::string::npos, "Shader syntax error!");
				propPos = rawSrc.find_first_of("\r\n", propPos);
				propPos = rawSrc.find_first_not_of("\r\n", propPos);
				while (propPos < endPos)
				{
					size_t propEnd = rawSrc.find_first_of(" ", propPos);
					size_t namePos = rawSrc.find_first_not_of(" ", propEnd);
					std::string propType = rawSrc.substr(propPos, propEnd - propPos);
					propPos = rawSrc.find_first_of(";", namePos);
					std::string propName = rawSrc.substr(namePos, propPos - namePos);
					propPos = rawSrc.find_first_of("\r\n", propPos);
					propPos = rawSrc.find_first_not_of("\r\n", propPos);
					NL_ENGINE_INFO("Shader prop type: {0}, name: {1}", propType, propName);
					AddProperty(propType, propName);
				}
			}
		}

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = rawSrc.find(typeToken, 0);
		// NL_ENGINE_TRACE("rawSrc: {0}", rawSrc);
		// NL_ENGINE_TRACE("pos: {0}", pos);
		while (pos != std::string::npos)
		{
			size_t eol = rawSrc.find_first_of("\r\n", pos);
			NL_ENGINE_ASSERT(eol != std::string::npos, "Shader syntax error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = rawSrc.substr(begin, eol - begin);
			size_t nextLinePos = rawSrc.find_first_not_of("\r\n", eol);
			NL_ENGINE_ASSERT(nextLinePos != std::string::npos, "Shader syntax error!");
			pos = rawSrc.find(typeToken, nextLinePos);

			if (type == "vertex")
			{
				vertexSrc = (pos == std::string::npos) ? rawSrc.substr(nextLinePos) : rawSrc.substr(nextLinePos, pos - nextLinePos);
				vertexSrcParsed = true;
				NL_ENGINE_TRACE("Vertex shader parsed!");
			}
			else if (type == "fragment")
			{
				fragmentSrc = (pos == std::string::npos) ? rawSrc.substr(nextLinePos) : rawSrc.substr(nextLinePos, pos - nextLinePos);
				fragmentSrcParsed = true;
				NL_ENGINE_TRACE("Fragment shader parsed!");
			}
			else if (type == "geometry")
			{
				geometrySrc = (pos == std::string::npos) ? rawSrc.substr(nextLinePos) : rawSrc.substr(nextLinePos, pos - nextLinePos);
				NL_ENGINE_TRACE("Geometry shader parsed!");
			}
			else
			{
				NL_ENGINE_ASSERT(false, "Invalid shader type specified!");
			}

			// NL_ENGINE_TRACE("pos: {0}", pos);
		}

		return vertexSrcParsed; // && fragmentSrcParsed;
	}

	void Shader::AddProperty(const std::string& type, const std::string& name)
	{
		if (s_ShaderUniformTypeMap.find(type) == s_ShaderUniformTypeMap.end())
		{
			return;
		}

		ShaderProperty prop;
		prop.Name = name;
		prop.Type = s_ShaderUniformTypeMap.at(type);
		m_Properties.emplace_back(prop);
	}

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