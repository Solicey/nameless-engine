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
		CameraComponent() { _Camera = CreateRef<Camera>(); }
		CameraComponent(const CameraComponent& comp)  : _Camera(CreateRef<Camera>(*comp._Camera.get())), FixedAspectRatio(comp.FixedAspectRatio), ClearColor(comp.ClearColor) {}

		void LoadGizmosModel(int entityId)
		{
			Gizmos = ModelLoader::Create(PathConfig::GetInstance().GetModelsFolder().string() + "/DontModify/Camera.obj");
		}

		bool FixedAspectRatio = false;
		nlm::vec4 ClearColor = nlm::vec4(0.192f, 0.302f, 0.475f, 1.0f);

		Ref<Camera> _Camera;
		Ref<Model> Gizmos = nullptr;
	};
}