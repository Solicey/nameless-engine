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
		
#pragma region IK Relative. Temporary. Will be transport to another component in the future maybe

		void CalculateFinalBoneMatrices()
		{
			if (mModel->HasBones())
				mModel->CalculateFinalBoneMatrices();
		}

		// Return bone chain id
		int CreateBoneChain(int tipBoneId)
		{
			if (!mModel->HasBones() || tipBoneId < 0 || tipBoneId >= mModel->GetBones().size())
				return -1;

			auto& bones = mModel->GetBonesNotConst();
			int parentId = bones.at(tipBoneId).parentID;
			int chainId = BoneChains.size();
			
			ChainInfo chain;
			chain.ID = chainId;
			chain.Bones.push_back(&bones[tipBoneId]);

			while (parentId != -1)
			{
				chain.Bones.push_back(&bones[parentId]);
				parentId = bones.at(parentId).parentID;
			}

			chain.Root = chain.Bones.size() - 1;
			BoneChains.push_back(chain);
			return chainId;
		}

		void InitializeBoneChainLocalOffsetAndRotation(int chainId, float tipLocalOffset)
		{
			if (chainId < 0 || chainId >= BoneChains.size())
				return;

			auto& chain = BoneChains[chainId];
			auto& bones = mModel->GetBonesNotConst();

			nlm::mat4 globalTransform(1.0f);
			for (int i = chain.Root; i >= 0; i--)
			{
				auto& bone = chain.Bones[i];
				nlm::vec3 scale, skew;
				nlm::vec4 persp;
				globalTransform = globalTransform * bone->Transformation;
				nlm::decompose(globalTransform, scale, bone->LocalRotation, bone->LocalOffset, skew, persp);
				NL_ENGINE_TRACE("bone {0}: rotation: {1}, offset: {2}", i, nlm::to_string(bone->LocalRotation), nlm::to_string(bone->LocalOffset));
			}
			chain.TipLocalRotation = nlm::quat_cast(nlm::mat4(1.0));
			chain.TipLocalOffset = nlm::vec3(0.0f, 0.0f, tipLocalOffset);	// ??
		}

		bool ForwardKinematics(int chainId, int startId, const nlm::vec3& modelWorldPosition)
		{
			if (chainId < 0 || chainId >= BoneChains.size())
				return false;

			auto& chain = BoneChains[chainId];
			auto& bones = mModel->GetBonesNotConst();
			
			if (startId < 0 || startId > chain.Root)
				return false;

			if (startId == chain.Root)
			{
				auto& rootBone = chain.Bones[startId];
				rootBone->WorldPosition = rootBone->LocalOffset + modelWorldPosition;
				rootBone->WorldRotation = rootBone->LocalRotation;
				NL_ENGINE_TRACE("bone {0}: worldRot: {1}, worldPos: {2}", chain.Root, nlm::to_string(rootBone->WorldRotation), nlm::to_string(rootBone->WorldPosition));
				startId--;
			}

			for (int i = startId; i >= 0; i--)
			{
				auto& parentBone = chain.Bones[i + 1];
				auto& bone = chain.Bones[i];

				nlm::quat q = parentBone->WorldRotation;
				nlm::quat qInv = nlm::conjugate(q);
				nlm::quat tmp = q * nlm::quat(0, bone->LocalOffset) * qInv;
				bone->WorldPosition = nlm::vec3(tmp.x, tmp.y, tmp.z) + parentBone->WorldPosition;
				bone->WorldRotation = parentBone->WorldRotation * bone->LocalRotation;

				NL_ENGINE_TRACE("bone {0}: worldRot: {1}, worldPos: {2}", i, nlm::to_string(bone->WorldRotation), nlm::to_string(bone->WorldPosition));
			}

			// Tips
			auto& parentBone = chain.Bones[0];
			nlm::quat q = parentBone->WorldRotation;
			nlm::quat qInv = nlm::conjugate(q);
			nlm::quat tmp = q * nlm::quat(0, chain.TipLocalOffset) * qInv;
			chain.TipWorldPosition = nlm::vec3(tmp.x, tmp.y, tmp.z) + parentBone->WorldPosition;
			chain.TipWorldRotation = parentBone->WorldRotation * chain.TipLocalRotation;
			//

			return true;
		}

		bool InverseKinematicsCCD(int chainId, const nlm::vec3& modelWorldPosition, const nlm::vec3& targetWorldPosition, int maxCCDIKIteration, float eps)
		{
			if (chainId < 0 || chainId >= BoneChains.size())
				return false;

			auto& chain = BoneChains[chainId];
			if (!ForwardKinematics(chainId, chain.Root, modelWorldPosition))
				return false;

			// NL_ENGINE_TRACE("chain tip position: {0}; target position: {1}", nlm::to_string(chain.TipWorldPosition), nlm::to_string(targetWorldPosition));
			return true;

			for (int CCDIKIteration = 0; CCDIKIteration < maxCCDIKIteration && nlm::l2Norm(chain.TipWorldPosition - targetWorldPosition) > eps; CCDIKIteration++)
			{
				auto& tipWorldPosition = chain.TipWorldPosition;
				for (int i = 0; i <= chain.Root; i++)
				{
					auto& bone = chain.Bones[i];

					nlm::vec3 tipWorldDir = nlm::normalize(tipWorldPosition - bone->WorldPosition);
					nlm::vec3 targetWorldDir = nlm::normalize(targetWorldPosition - bone->WorldPosition);

					nlm::quat q = bone->WorldRotation;
					nlm::quat qInv = nlm::conjugate(q);
					
					auto tmp = qInv * nlm::quat(0, tipWorldDir) * q;
					nlm::vec3 tipLocalDir(tmp.x, tmp.y, tmp.z);

					tmp = qInv * nlm::quat(0, targetWorldDir) * q;
					nlm::vec3 targetLocalDir(tmp.x, tmp.y, tmp.z);

					float deltaAngle = nlm::acos(nlm::dot(nlm::normalize(tipLocalDir), nlm::normalize(targetLocalDir)));

					nlm::quat deltaLocalRotation = nlm::rotation(nlm::normalize(tipLocalDir), nlm::normalize(targetLocalDir));
					bone->LocalRotation = deltaLocalRotation * bone->LocalRotation;

					ForwardKinematics(chainId, i, modelWorldPosition);
				}
			}

			return true;
		}

		void RecalculateTransformationMatrices(int chainId)
		{
			if (chainId < 0 || chainId >= BoneChains.size())
				return;

			auto& chain = BoneChains[chainId];
			
			for (int i = 0; i <= chain.Root; i++)
			{
				auto& bone = chain.Bones[i];
				bone->Transformation = nlm::translate(nlm::mat4(1.0f), bone->LocalOffset) * nlm::mat4_cast(bone->LocalRotation);
			}
		}

#pragma endregion


	public:
		// turn all "\" into "/" with std::regex_replace(Path, std::regex("\\\\"), "/");
		std::string Path = "";
		Ref<Model> mModel = nullptr;

		std::vector<ChainInfo> BoneChains;
		
		ModelLoaderFlags Flags;
	};
}