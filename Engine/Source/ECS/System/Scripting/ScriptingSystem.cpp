#include "nlpch.h"

#include "ScriptingSystem.h"
#include "Scripting/ScriptEngine.h"

namespace NL
{
	void ScriptingSystem::OnStartRuntime()
	{
		ScriptEngine::GetInstance().SetSceneContext(m_Scene);
		// Instantiate all script entities
		auto view = m_Scene->m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			ScriptEngine::GetInstance().OnCreateEntity(entity);
		}
	}

	void ScriptingSystem::OnStopRuntime()
	{
	}

	void ScriptingSystem::OnUpdateRuntime(TimeStep ts, Entity cameraEntity)
	{
		auto view = m_Scene->m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			ScriptEngine::GetInstance().OnUpdateRuntime(entity, ts);
		}
	}

	void ScriptingSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		auto view = m_Scene->m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };

			auto& hasInstantiate = entity.GetComponent<ScriptComponent>().HasInstantiate;
			if (!hasInstantiate)
			{
				if (ScriptEngine::GetInstance().OnCreateEntity(entity))
					hasInstantiate = true;
			}
			ScriptEngine::GetInstance().OnUpdateEditor(entity, ts);
		}
	}
}