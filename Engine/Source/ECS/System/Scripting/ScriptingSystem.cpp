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
		//ScriptEngine::GetInstance().OnStopRuntime();
	}

	void ScriptingSystem::OnUpdateRuntime(TimeStep ts, Entity cameraEntity)
	{
		auto view = m_Scene->m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			auto& comp = entity.GetComponent<ScriptComponent>();

			if (!ScriptEngine::GetInstance().EntityClassExists(comp.ClassName))
			{
				comp.ScriptClassExists = false;
				continue;
			}
			else comp.ScriptClassExists = true;

			ScriptEngine::GetInstance().OnUpdateRuntime(entity, ts);
		}
	}

	void ScriptingSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		auto view = m_Scene->m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			auto& comp = entity.GetComponent<ScriptComponent>();

			if (!ScriptEngine::GetInstance().EntityClassExists(comp.ClassName))
			{
				comp.ScriptClassExists = false;
				continue;
			}
			else comp.ScriptClassExists = true;

			auto& hasInstantiate = comp.HasInstantiate;
			if (!hasInstantiate)
			{
				if (ScriptEngine::GetInstance().OnCreateEntity(entity))
					hasInstantiate = true;
			}
			ScriptEngine::GetInstance().OnUpdateEditor(entity, ts);
		}
	}

	void ScriptingSystem::SetAllHasInstantiateToFalse()
	{
		auto view = m_Scene->m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			entity.GetComponent<ScriptComponent>().HasInstantiate = false;
		}
	}
}