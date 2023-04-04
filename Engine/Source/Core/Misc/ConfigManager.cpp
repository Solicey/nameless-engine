#include "nlpch.h"

#include "ConfigManager.h"

#include "Core/Misc/Directives.h"
#include "Core/Log/Log.h"

namespace NL
{
	ConfigManager::ConfigManager(token)
	{
		m_AssetsFolder = { NL_XSTRINGIFY_MACRO(NL_ASSETS_PATH) };
		m_MaterialsFolder = m_AssetsFolder / "Materials";
		m_ModelsFolder = m_AssetsFolder / "Models";
		m_ScriptsFolder = m_AssetsFolder / "Scripts";
		m_ShadersFolder = m_AssetsFolder / "Shaders";
		m_FontsFolder = m_AssetsFolder / "Fonts";
	}

	const std::filesystem::path& ConfigManager::GetAssetsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_AssetsFolder), "Assets folder does NOT exist!");
		return m_AssetsFolder;
	}

	const std::filesystem::path& ConfigManager::GetModelsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_ModelsFolder), "Models folder does NOT exist!");
		return m_ModelsFolder;
	}

	const std::filesystem::path& ConfigManager::GetShadersFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_ShadersFolder), "Shaders folder does NOT exist!");
		return m_ShadersFolder;
	}

	const std::filesystem::path& ConfigManager::GetMaterialsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_MaterialsFolder), "Materials folder does NOT exist!");
		return m_MaterialsFolder;
	}

	const std::filesystem::path& ConfigManager::GetScriptsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_ScriptsFolder), "Scripts folder does NOT exist!");
		return m_ScriptsFolder;
	}

	const std::filesystem::path& ConfigManager::GetFontsFolder() const
	{
		NL_ENABLE_ASSERTS(std::filesystem::exists(m_FontsFolder), "Fonts folder does NOT exist!");
		return m_FontsFolder;
	}
}