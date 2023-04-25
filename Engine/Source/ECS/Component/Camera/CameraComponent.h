#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Core/Math/Math.h"
#include "ECS/Component/Component.h"
#include "Resources/Loaders/ModelLoader.h"
#include "Camera/Camera.h"

#include <string>

namespace NL
{
	class CameraComponent : public ComponentBase
	{
	public:
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void LoadGizmosModel(int entityId)
		{
			Gizmos = ModelLoader::Create(PathConfig::GetInstance().GetModelsFolder().string() + "/DontModify/Camera.obj", entityId);
		}

		bool FixedAspectRatio = false;
		nlm::vec4 ClearColor = nlm::vec4(0.192f, 0.302f, 0.475f, 1.0f);

		Camera mCamera;
		Ref<Model> Gizmos = nullptr;
	};
}