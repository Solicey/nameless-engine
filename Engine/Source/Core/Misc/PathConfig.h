#pragma once

#include "Core/Misc/Singleton.h"
#include "Core/Math/Math.h"

#include <filesystem>

namespace NL
{
	class PathConfig final : public Singleton<PathConfig>
	{
	public:
		PathConfig(token);
		~PathConfig() = default;
		PathConfig(const PathConfig&) = delete;
		PathConfig& operator = (const PathConfig&) = delete;

	public:
		void Clear();

		inline const std::filesystem::path& GetAssetsFolder() const;
		inline const std::filesystem::path& GetModelsFolder() const;
		inline const std::filesystem::path& GetShadersFolder() const;
		inline const std::filesystem::path& GetMaterialsFolder() const;
		inline const std::filesystem::path& GetScriptsFolder() const;

	private:
		std::filesystem::path m_AssetsFolder;
		std::filesystem::path m_ModelsFolder;
		std::filesystem::path m_ShadersFolder;
		std::filesystem::path m_MaterialsFolder;
		std::filesystem::path m_ScriptsFolder;
	};
}