#pragma once

#include "Core/Misc/Singleton.h"
#include "Core/Math/Math.h"

#include <filesystem>

namespace NL
{
	class ConfigManager final : public Singleton<ConfigManager>
	{
	public:
		ConfigManager(token);
		~ConfigManager() = default;
		ConfigManager(const ConfigManager&) = delete;
		ConfigManager& operator = (const ConfigManager&) = delete;

	public:
		const std::filesystem::path& GetAssetsFolder() const;
		const std::filesystem::path& GetModelsFolder() const;
		const std::filesystem::path& GetShadersFolder() const;
		const std::filesystem::path& GetMaterialsFolder() const;
		const std::filesystem::path& GetScriptsFolder() const;
		const std::filesystem::path& GetFontsFolder() const;
		float GetViewportInitWidth() const { return m_ViewportInitWidth; }
		float GetViewportInitHeight() const { return m_ViewportInitHeight; }

	private:
		std::filesystem::path m_AssetsFolder;
		std::filesystem::path m_ModelsFolder;
		std::filesystem::path m_ShadersFolder;
		std::filesystem::path m_MaterialsFolder;
		std::filesystem::path m_ScriptsFolder;
		std::filesystem::path m_FontsFolder;

		float m_ViewportInitWidth = 1280;
		float m_ViewportInitHeight = 720;
	};
}