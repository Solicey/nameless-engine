#include "nlpch.h"

#include "ShaderLibrary.h"

#include "Core/Misc/PathConfig.h"

namespace NL
{
	Library<Shader>::Library(Singleton::token)
		: m_ShadersFolder(PathConfig::GetInstance().GetShadersFolder())
	{
		// Add("Unlit", Shader::Create());
		TraverseShadersFolder(m_ShadersFolder);
	}

	void Library<Shader>::TraverseShadersFolder(const std::filesystem::path& path)
	{
		// NL_ENGINE_TRACE("Traverse Shader Folder: {0}", path);
		for (auto& item : std::filesystem::directory_iterator(path))
		{
			if (std::filesystem::is_directory(item.status()))
			{
				TraverseShadersFolder(item.path());
			}
			else
			{
				if (item.path().extension().string() == ".glsl")
				{
					std::string name = item.path().filename().string();
					// NL_ENGINE_TRACE("Traverse shaders file: {0}", name);
					// Add(name, Shader::Create(name, item.path().string()));
					m_ShaderNameMap[name] = item.path();
				}
			}
		}
	}

	Ref<Shader> Library<Shader>::Reload(const std::string& name)
	{
		if (m_ShaderNameMap.contains(name))
		{
			if (Contains(name))
			{
				Get(name)->Reload();
				return Get(name);
			}
			else
			{
				Ref<Shader> shader = Shader::Create(name, m_ShaderNameMap[name]);
				Add(name, shader);
				NL_ENGINE_TRACE("Add shader to library: {0}", name);
				return shader;
			}
		}
		return nullptr;
	}

	Ref<Shader> Library<Shader>::Fetch(const std::string& name)
	{
		NL_ENGINE_TRACE("Load shader: {0}", name);
		if (m_ShaderNameMap.contains(name))
		{
			if (Contains(name))
			{
				NL_ENGINE_TRACE("Shader library contains: {0}", name);
				// Ref<Shader> shader = Shader::Create(name, m_ShaderNameMap[name]);
				// Set(name, shader);
				return Get(name);
			}
			else
			{
				Ref<Shader> shader = Shader::Create(name, m_ShaderNameMap[name]);
				Add(name, shader);
				NL_ENGINE_TRACE("Add shader to library: {0}", name);
				return shader;
			}
		}
		NL_ENGINE_WARN("Shader library failed to load shader {0}", name);
		return nullptr;
	}
}