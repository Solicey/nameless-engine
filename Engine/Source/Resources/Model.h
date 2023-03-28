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
		std::unordered_map<std::string, Ref<Material>>& GetMaterialsNotConst() { return m_Materials; }

		void DeleteMaterialTexturesReference()
		{
			for (auto& item : m_Materials)
			{
				Ref<Material> mat = item.second;
				if (mat != nullptr)
					mat->DeleteMaterialTexturesReference();
			}
			Library<Texture2D>::GetInstance().TraverseDelete();
		}
		
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

		const std::vector<nlm::mat4>& CalculateFinalBoneMatrices()
		{
			m_FinalBoneMatrices.resize(m_Bones.size());

			for (auto& pair : m_Bones)
			{
				auto& bone = pair.second;
				if (bone.parentID == -1)
					CalculateFinalBoneMatrixRecursively(bone.ID, nlm::mat4(1.0f));
			}

			return m_FinalBoneMatrices;
		}

	private:
		Model() = delete;
		Model(const std::string& path) : m_Path(path) {}

		void CalculateFinalBoneMatrixRecursively(int boneId, const nlm::mat4& parentTransform)
		{
			auto& bone = m_Bones[boneId];
			nlm::mat4 globalTransform = parentTransform * bone.Transformation;
			m_FinalBoneMatrices[boneId] = globalTransform * bone.Offset;
			// NL_ENGINE_TRACE("finalBoneMatrix[{0}] = {1}", boneId, nlm::to_string(m_FinalBoneMatrices[boneId]));

			for (int childId : bone.Childrens)
			{
				CalculateFinalBoneMatrixRecursively(childId, globalTransform);
			}
		}

	private:
		std::string					m_Path;
		std::vector<Ref<Mesh>>		m_Meshes;
		std::unordered_map<std::string, Ref<Material>> m_Materials;

		std::unordered_map<std::string, int>	m_BoneMap;
		std::map<int, BoneInfo>					m_Bones;
		std::vector<nlm::mat4>					m_FinalBoneMatrices;
	};
}