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