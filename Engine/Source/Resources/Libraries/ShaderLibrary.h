#pragma once

#include "Resources/Libraries/Library.h"
#include "Core/Misc/ConfigManager.h"
#include "Resources/Shader.h"

namespace NL
{
	template <>
	class Library<Shader> : public LibraryBase<Library, Shader>
	{
	public:
		Library(Singleton::token);
		void TraverseShadersFolder(const std::filesystem::path& path = ConfigManager::GetInstance().GetShadersFolder());
		Ref<Shader> LoadShader(const std::string& name);
		const std::unordered_map<std::string, std::filesystem::path>& GetShaderNameMap()
		{
			return m_ShaderNameMap;
		}
		std::string GetDefaultShaderName()
		{
			if (m_ShaderNameMap.contains("Default.glsl"))
				return "Default.glsl";
			return "";
		}

	private:
		std::filesystem::path m_ShadersFolder;
		std::unordered_map<std::string, std::filesystem::path> m_ShaderNameMap;
	};
}