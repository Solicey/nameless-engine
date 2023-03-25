#pragma once

#include "ECS/Scene/Scene.h"
#include "Core/Misc/TimeStep.h"
#include "Camera/EditorCamera.h"

namespace NL
{
	class System
	{
	public:
		System() = delete;
		System(Scene* scene) : m_Scene(scene) {}
		virtual ~System() = default;

		virtual void OnStartRuntime() = 0;
		virtual void OnStopRuntime() = 0;
		virtual void OnUpdateRuntime(TimeStep ts, Camera* camera) = 0;
		virtual void OnUpdateEditor(TimeStep ts, EditorCamera& camera) = 0;

	protected:
		Scene* m_Scene = nullptr;
	};
}