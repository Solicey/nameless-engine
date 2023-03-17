#pragma once

#include "ECS/System/System.h"
#include "Resources/Shader.h"

namespace NL
{
	class RenderSystem : public System
	{
	public:
		RenderSystem(Scene* scene);

		virtual void OnStartRuntime();
		virtual void OnStopRuntime();
		virtual void OnUpdateRuntime(TimeStep ts);
		virtual void OnUpdateEditor(TimeStep ts, EditorCamera& camera);

	private:
		// Ref<Shader> m_TestShader;
	};
}