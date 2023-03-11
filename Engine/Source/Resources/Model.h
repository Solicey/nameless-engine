#pragma once

#include "Core/Log/Log.h"
#include "Resources/Mesh.h"

#include <vector>

namespace NL
{
	class Model
	{
		friend class ModelLoader;

	public:
		inline void DebugPrintModelInfo()
		{
			NL_ENGINE_INFO("Model Info:");
			NL_ENGINE_INFO("  Path: {0}", m_Path);
			NL_ENGINE_INFO("  Mesh Count: {0}", m_Meshes.size());
			NL_ENGINE_INFO("  Material Count: {0}", m_materialNames.size());

			int cnt = 0;
			for (const auto& mesh : m_Meshes)
			{
				NL_ENGINE_INFO("  Mesh {0}: {1} vertices, {2} faces", 
					cnt++, mesh->GetVertexCount(), mesh->GetIndexCount());
			}

			cnt = 0;
			for (const auto& mat : m_materialNames)
			{
				NL_ENGINE_INFO("  Material {0} name: {1}",
					cnt++, mat);
			}
		}

	private:
		Model() = delete;
		Model(const std::string& path) : m_Path(path) {}

	private:
		std::string					m_Path;
		std::vector<Ref<Mesh>>		m_Meshes;
		std::vector<std::string>	m_materialNames;
	};
}