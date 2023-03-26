#pragma once

#include "ECS/System/System.h"

namespace NL
{
	class ScriptingSystem : public System
	{
	public:
		ScriptingSystem(Scene* scene) : System(scene) {}

		virtual void OnStartRuntime() override;
		virtual void OnStopRuntime() override;
		virtual void OnUpdateRuntime(TimeStep ts, Entity cameraEntity) override;
		virtual void OnUpdateEditor(TimeStep ts, EditorCamera& camera) override;

	private:
	};
}