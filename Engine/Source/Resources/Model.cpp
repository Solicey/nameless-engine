#include "nlpch.h"

#include "Model.h"

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

	void Model::CalculateFinalBoneMatrixRecursively(int boneId, const aiMatrix4x4& parentTransform, const aiMatrix4x4& invRootTransform)
	{
		const auto& bone = m_Bones[boneId];

		aiMatrix4x4 rotation;
		auto trans = nlm::transpose(nlm::mat4_cast(nlm::quat(bone.Rotation)));
		memcpy(&rotation, &trans, 16 * sizeof(float));

		aiMatrix4x4 worldTransform = parentTransform * bone.Transformation * rotation;
		m_FinalBoneMatrices[boneId] = Utils::ConvertMatrixToNLMFormat(invRootTransform * worldTransform * bone.Offset);

		for (int childId : bone.Childrens)
		{
			CalculateFinalBoneMatrixRecursively(childId, worldTransform, invRootTransform);
		}
	}
}