#pragma once

#include "Core/Log/Log.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Animation/Bones.h"

#include <vector>
#include <map>

namespace NL
{
	class Model
	{
		friend class ModelLoader;

	public:

		inline void DebugPrintModelInfo()
		{
			/*NL_ENGINE_INFO("Model Info:");
			NL_ENGINE_INFO("  Path: {0}", m_Path);
			NL_ENGINE_INFO("  Mesh Count: {0}", m_Meshes.size());
			// NL_ENGINE_INFO("  Material Count: {0}", m_materialNames.size());

			int cnt = 0;
			for (const auto& mesh : m_Meshes)
			{
				NL_ENGINE_INFO("  Mesh {0}: {1} vertices, {2} faces", 
					cnt++, mesh->GetVertexCount(), mesh->GetIndexCount() / 3);
			}*/

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
			const std::string& name = mesh->GetMeshName();
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
		std::unordered_map<std::string, Ref<Material>>& GetMaterialsNotConst() { return m_Materials; }
		
		bool HasBones() const
		{
			return m_Bones.size() > 0;
		}

		const std::unordered_map<std::string, int>& GetBoneMap() const { return m_BoneMap; }
		std::map<int, BoneInfo>& GetBonesNotConst() { return m_Bones; }
		const std::map<int, BoneInfo>& GetBones() const { return m_Bones; }

		const std::vector<nlm::mat4>& GetFinalBoneMatrices() const
		{
			return m_FinalBoneMatrices;
		}

		const std::vector<nlm::mat4>& CalculateFinalBoneMatrices();

		void AddBonePair(const std::pair<std::string, std::string>& pair);

		// Called when the model is deleted (single delete operation)
		// For multiply delete operations (such as delete of a whole scene, use Scene::DestroyScene)
		void DeleteMeshesAndTexturesReference();

		Model(const Model* src);

		void UpdateShaderProperties(const std::string& shaderName);

		// Model(const Model& model) = default;

	private:
		Model() = delete;
		Model(const std::string& path) : m_Path(path) {}

		void CalculateFinalBoneMatrixRecursively(int boneId, const aiMatrix4x4& parentTransform, const aiMatrix4x4& invRootTransform);

	private:
		std::string					m_Path;
		std::vector<Ref<Mesh>>		m_Meshes;
		std::unordered_map<std::string, Ref<Material>> m_Materials;

		std::unordered_map<std::string, int>	m_BoneMap;
		std::map<int, BoneInfo>					m_Bones;
		std::vector<nlm::mat4>					m_FinalBoneMatrices;
	};
}