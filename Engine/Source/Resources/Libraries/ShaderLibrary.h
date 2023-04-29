#pragma once

#include "Resources/Libraries/Library.h"
#include "Core/Misc/PathConfig.h"
#include "Resources/Shader.h"

#include <map>

namespace NL
{
	template <>
	class Library<Shader> : public LibraryBase<Library, Shader>
	{
	public:
		Library(Singleton::token);
		void TraverseShadersFolder(const std::filesystem::path& path = PathConfig::GetInstance().GetShadersFolder());
		// Don't use Library Class's Get() directly, use Fetch instead
		Ref<Shader> Fetch(const std::string& name);
		Ref<Shader> Reload(const std::string& name);
		const std::unordered_map<std::string, std::filesystem::path>& GetShaderNameMap() const { return m_ShaderPathMap; }
		const std::map<std::string, ShaderUse>& GetShaderUseMap() const { return m_ShaderUseMap; }
		std::string GetDefaultShaderName()
		{
			if (m_ShaderPathMap.contains("Default.glsl"))
				return "Default.glsl";
			return "";
		}

	private:
		std::filesystem::path m_ShadersFolder;
		std::unordered_map<std::string, std::filesystem::path> m_ShaderPathMap;
		std::map<std::string, ShaderUse> m_ShaderUseMap;
	};
}