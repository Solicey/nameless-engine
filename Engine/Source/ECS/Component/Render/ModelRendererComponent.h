#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Core/Log/Log.h"
#include "Resources/Model.h"
#include "Resources/Loaders/ModelLoader.h"

#include <regex>

namespace NL
{
	class ModelRendererComponent : public ComponentBase
	{
	public:
		ModelRendererComponent() : Flags(ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace | ModelLoaderFlags::PopulateArmatureData) {}

		ModelRendererComponent(const std::string& path, int entityID, ModelLoaderFlags flags = ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace | ModelLoaderFlags::PopulateArmatureData)
			: Path(std::regex_replace(path, std::regex("\\\\"), "/")), Flags(flags), mModel(ModelLoader::Create(path, entityID, flags))
		{
			// NL_ENGINE_TRACE("Entity entt id: {0}", entityID);
		}

		void UpdateBoneInfos()
		{
			auto& bones = mModel->GetBonesNotConst();
			auto& boneMap = mModel->GetBoneMap();
			for (auto& pair : Bones)
			{
				auto& parentName = pair.first;
				auto& childName = pair.second;
				NL_ENGINE_INFO("Bone pair: {0}, {1}", parentName, childName);
				if (boneMap.find(childName) == boneMap.end())
					continue;
				int childId = boneMap.at(childName);

				BoneInfo& child = bones[childId];

				if (parentName == "null")
				{
					child.parentID = -1;
				}
				else
				{
					if (boneMap.find(parentName) == boneMap.end())
						continue;
					int parentId = boneMap.at(parentName);
					BoneInfo& parent = bones[parentId];

					parent.Childrens.insert(childId);
					child.parentID = parentId;
				}
			}

			mModel->CalculateFinalBoneMatrices();
		}

		void GetBoneLocalTransform(int boneId, nlm::vec3& translation, nlm::vec3& rotation) const
		{
			const auto& bones = mModel->GetBones();
			if (bones.find(boneId) != bones.end())
			{
				const auto& transform = bones.at(boneId).Transformation;
				nlm::vec3 scale;
				Utils::DecomposeTransform(transform, translation, rotation, scale);
			}
		}

		void SetBoneLocalTransform(int boneId, nlm::vec3& translation, nlm::vec3& rotation)
		{
			auto& bones = mModel->GetBonesNotConst();
			if (bones.find(boneId) != bones.end())
			{
				bones[boneId].Transformation = nlm::translate(nlm::mat4(1.0f), translation) * nlm::toMat4(nlm::quat(rotation));
			}
		}

	public:
		// turn all "\" into "/" with std::regex_replace(Path, std::regex("\\\\"), "/");
		std::string Path = "";
		Ref<Model> mModel = nullptr;
		// parent:child
		std::vector<std::pair<std::string, std::string>> Bones;
		
		ModelLoaderFlags Flags;
	};
}