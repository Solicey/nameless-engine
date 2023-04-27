#include "nlpch.h"

#include "Model.h"
#include "Resources/Libraries/MeshLibrary.h"

namespace NL
{
	const std::vector<nlm::mat4>& Model::CalculateFinalBoneMatrices()
	{
		m_FinalBoneMatrices.clear();
		m_FinalBoneMatrices.resize(m_Bones.size());

		for (auto& pair : m_Bones)
		{
			auto& bone = pair.second;
			if (bone.parentID == -1)
				CalculateFinalBoneMatrixRecursively(bone.ID, aiMatrix4x4(), bone.Transformation.Inverse());
		}

		return m_FinalBoneMatrices;
	}

	void Model::AddBonePair(const std::pair<std::string, std::string>& pair)
	{
		auto& parentName = pair.first;
		auto& childName = pair.second;
		NL_ENGINE_INFO("Bone pair: {0}, {1}", parentName, childName);
		if (m_BoneMap.find(childName) == m_BoneMap.end())
			return;
		int childId = m_BoneMap.at(childName);

		BoneInfo& child = m_Bones[childId];

		if (parentName == "null")
		{
			child.parentID = -1;
		}
		else
		{
			if (m_BoneMap.find(parentName) == m_BoneMap.end())
				return;
			int parentId = m_BoneMap.at(parentName);
			BoneInfo& parent = m_Bones[parentId];

			if (child.parentID != -1)
				m_Bones[child.parentID].Childrens.erase(childId);
			parent.Childrens.insert(childId);
			child.parentID = parentId;
		}
	}

	void Model::DeleteMeshesAndTexturesReference()
	{
		for (auto& mesh : m_Meshes)
		{
			if (mesh != nullptr)
			{
				std::string modelPath = mesh->GetModelPath(),
					meshName = mesh->GetMeshName();
				mesh.reset();
				Library<Mesh>::GetInstance().DeleteMesh(modelPath, meshName);
			}
		}

		for (auto& item : m_Materials)
		{
			Ref<Material> mat = item.second;
			if (mat != nullptr)
				mat->DeleteTexturesReference();
		}
		// Library<Texture2D>::GetInstance().TraverseDelete();
	}

	Model::Model(const Model* src)
		: m_Path(src->m_Path), m_Meshes(src->m_Meshes), m_BoneMap(src->m_BoneMap),
		m_Bones(src->m_Bones), m_FinalBoneMatrices(src->m_FinalBoneMatrices)
	{
		/*m_Path = src->m_Path;
		m_Meshes = src->m_Meshes;
		m_BoneMap = src->m_BoneMap;
		m_Bones = src->m_Bones;
		m_FinalBoneMatrices = src->m_FinalBoneMatrices;*/

		m_Materials.clear();
		for (const auto& mat : src->m_Materials)
		{
			m_Materials[mat.first] = CreateRef<Material>(mat.second.get());
		}

	}

	void Model::UpdateShaderProperties(const std::string& shaderName)
	{
		// NL_ENGINE_INFO("Update Shader Properties: {0}, {1}", m_Path, shaderName);
		for (auto& mat : m_Materials)
		{
			mat.second->LoadShaderAndUpdateProps(shaderName);
		}
	}

	void Model::CalculateFinalBoneMatrixRecursively(int boneId, const aiMatrix4x4& parentTransform, const aiMatrix4x4& invRootTransform)
	{
		const auto& bone = m_Bones[boneId];
		// NL_ENGINE_INFO("final. bone Id {0}, angles: {1}, originalAngles: {2}", boneId, nlm::to_string(bone.Rotation), nlm::to_string(bone.OriginalRotation));
		// nlm::quat q = nlm::quat(bone.OriginalRotation);
		aiMatrix4x4 rotation;
		auto trans = nlm::transpose(nlm::mat4_cast(nlm::quat(bone.Rotation)));
		memcpy(&rotation, &trans, 16 * sizeof(float));

		aiMatrix4x4 worldTransform = parentTransform * bone.Transformation * rotation;
		m_FinalBoneMatrices[boneId] = Utils::ConvertMatrixToNLMFormat(invRootTransform * worldTransform * bone.Offset);

		// NL_ENGINE_TRACE("invRootTransform[{0}] = {1}", boneId, nlm::to_string(invRootTransform));
		/*NL_ENGINE_TRACE("Transformation[{0}] = {1}", boneId, nlm::to_string(Utils::ConvertMatrixToNLMFormat(bone.Transformation)));
		NL_ENGINE_TRACE("boneMod[{0}] = {1}", boneId, nlm::to_string(Utils::ConvertMatrixToNLMFormat(rotation)));
		NL_ENGINE_TRACE("parentTransform[{0}] = {1}", boneId, nlm::to_string(Utils::ConvertMatrixToNLMFormat(parentTransform)));
		NL_ENGINE_TRACE("globalTransform[{0}] = {1}", boneId, nlm::to_string(Utils::ConvertMatrixToNLMFormat(worldTransform)));
		NL_ENGINE_TRACE("Offset[{0}] = {1}", boneId, nlm::to_string(Utils::ConvertMatrixToNLMFormat(bone.Offset)));
		NL_ENGINE_TRACE("finalBoneMatrix[{0}] = {1}", boneId, nlm::to_string(m_FinalBoneMatrices[boneId]));*/

		for (int childId : bone.Childrens)
		{
			CalculateFinalBoneMatrixRecursively(childId, worldTransform, invRootTransform);
		}
	}
}