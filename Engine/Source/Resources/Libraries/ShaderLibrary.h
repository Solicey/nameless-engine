#pragma once

#include "Resources/Libraries/Library.h"
#include "Resources/Shader.h"

namespace NL
{
	template <>
	class Library<Shader> : public LibraryBase<Library, Shader>
	{
	public:
		Library(Singleton::token);
		void TraverseShadersFolder(const std::filesystem::path&);
		Ref<Shader> LoadShader(const std::string& name);
		const std::unordered_map<std::string, std::filesystem::path>& GetShaderNameMap()
		{
			return m_ShaderNameMap;
		}
		std::string GetDefaultShader()
		{
			if (m_ShaderNameMap.contains("NormalTest.glsl"))
				return "NormalTest.glsl";
			return "";
		}

	private:
		std::filesystem::path m_ShadersFolder;
		std::unordered_map<std::string, std::filesystem::path> m_ShaderNameMap;
	};
}