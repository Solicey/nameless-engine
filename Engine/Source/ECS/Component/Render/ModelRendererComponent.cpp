#include "nlpch.h"

#include "ModelRendererComponent.h"

namespace NL
{
	void ModelRendererComponent::RotateBone(int boneId, nlm::vec3* eulerAngles)
	{
		if (!mModel->HasBones())
			return;

		auto& bones = mModel->GetBonesNotConst();
		if (bones.find(boneId) == bones.end())
			return;

		auto& bone = bones[boneId];

		// NL_ENGINE_INFO("Rotate bone {0}, angles: {1}", bone.ID, nlm::to_string(*eulerAngles));
		bone.Rotation = nlm::radians(*eulerAngles);

		/*nlm::vec3 translate, skew, scale;
		nlm::vec4 persp;
		nlm::quat orientation;
		nlm::decompose(bone.Transformation, scale, orientation, translate, skew, persp);

		bone.Transformation = nlm::translate(nlm::mat4(1.0f), translate) * nlm::rotate(nlm::mat4(1.0f), angle, *axis) * nlm::mat4_cast(orientation) * nlm::scale(nlm::mat4(1.0f), scale);*/
	}

	void ModelRendererComponent::RecalculateFinalBoneMatrices()
	{
		if (mModel->HasBones())
			mModel->CalculateFinalBoneMatrices();
	}
}