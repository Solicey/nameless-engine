#include "nlpch.h"

#include "PathConfig.h"

#include "Core/Misc/Directives.h"
#include "Core/Log/Log.h"

namespace NL
{
	PathConfig::PathConfig(token)
	{
		m_AssetsFolder = { NL_XSTRINGIFY_MACRO(NL_ASSETS_PATH) };
		m_MaterialsFolder = m_AssetsFolder / "Materials";
		m_ModelsFolder = m_AssetsFolder / "Models";
		m_ScriptsFolder = m_AssetsFolder / "Scripts";
		m_ShadersFolder = m_AssetsFolder / "Shaders";
	}

	const std::filesystem::path& PathConfig::GetAssetsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_AssetsFolder), "Assets folder does NOT exist!");
		return m_AssetsFolder;
	}

	const std::filesystem::path& PathConfig::GetModelsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_ModelsFolder), "Models folder does NOT exist!");
		return m_ModelsFolder;
	}

	const std::filesystem::path& PathConfig::GetShadersFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_ShadersFolder), "Shaders folder does NOT exist!");
		return m_ShadersFolder;
	}

	const std::filesystem::path& PathConfig::GetMaterialsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_MaterialsFolder), "Materials folder does NOT exist!");
		return m_MaterialsFolder;
	}

	const std::filesystem::path& PathConfig::GetScriptsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_ScriptsFolder), "Scripts folder does NOT exist!");
		return m_ScriptsFolder;
	}
}