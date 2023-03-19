#pragma once

#include "Core/Log/Log.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"

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
			// NL_ENGINE_INFO("  Material Count: {0}", m_materialNames.size());

			int cnt = 0;
			for (const auto& mesh : m_Meshes)
			{
				NL_ENGINE_INFO("  Mesh {0}: {1} vertices, {2} faces", 
					cnt++, mesh->GetVertexCount(), mesh->GetIndexCount() / 3);
			}

			/*cnt = 0;
			for (const auto& mat : m_materialNames)
			{
				NL_ENGINE_INFO("  Material {0} name: {1}",
					cnt++, mat);
			}*/
		}

		const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
		// 获取当前mesh绑定的material
		const Ref<Material>& GetMaterial(const Ref<Mesh>& mesh) const
		{
			const std::string& name = mesh->GetMaterialName();
			NL_ENGINE_ASSERT(m_Materials.contains(name), "Model get shader failed!");
			return m_Materials.at(name);
		}
		// 获取当前mesh绑定的material绑定的shader
		const Ref<Shader>& GetShader(const Ref<Mesh>& mesh) const
		{
			return GetMaterial(mesh)->GetShader();
		}
		const std::string& GetPath() const { return m_Path; }
		const std::unordered_map<std::string, Ref<Material>>& GetMaterials() const { return m_Materials; }

	private:
		Model() = delete;
		Model(const std::string& path) : m_Path(path) {}

	private:
		std::string					m_Path;
		std::vector<Ref<Mesh>>		m_Meshes;
		std::unordered_map<std::string, Ref<Material>> m_Materials;
	};
}