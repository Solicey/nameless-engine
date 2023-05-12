#include "nlpch.h"

#include "ScriptingSystem.h"
#include "Scripting/ScriptEngine.h"

namespace NL
{
	void ScriptingSystem::OnStartRuntime()
	{
		ScriptEngine::GetInstance().ClearAllInstances();
		ScriptEngine::GetInstance().SetSceneContext(m_Scene);
		// Instantiate all script entities for runtime scene
		auto view = m_Scene->Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			ScriptEngine::GetInstance().OnCreateEntity(entity);
		}
	}

	void ScriptingSystem::OnStopRuntime(Scene* editorScene)
	{
		ScriptEngine::GetInstance().ClearAllInstances();
		ScriptEngine::GetInstance().SetSceneContext(editorScene);
		// OnUpdateEditor will handle the instantiate
	}

	void ScriptingSystem::OnUpdateRuntime(TimeStep ts, Entity cameraEntity)
	{
		if (m_Scene->IsPaused())
			return;

		auto view = m_Scene->Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			auto& comp = entity.GetComponent<ScriptComponent>();

			// Check whether Script class exists
			if (!ScriptEngine::GetInstance().EntityClassExists(comp.ClassName))
			{
				comp.ScriptClassExists = false;
				ScriptEngine::GetInstance().OnDeleteEntity(entity);
				continue;
			}
			else comp.ScriptClassExists = true;

			// Check whether instance has been instantiate
			if (!ScriptEngine::GetInstance().OnUpdateRuntime(entity, ts))
			{
				if (ScriptEngine::GetInstance().OnCreateEntity(entity))
				{
					ScriptEngine::GetInstance().OnUpdateRuntime(entity, ts);
				}
			}
		}
	}

	void ScriptingSystem::OnStartEditor()
	{
		ScriptEngine::GetInstance().ClearAllInstances();
		ScriptEngine::GetInstance().SetSceneContext(m_Scene);
		// Instantiate all script entities for editor scene
		auto view = m_Scene->Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			ScriptEngine::GetInstance().OnCreateEntity(entity);
		}
	}

	void ScriptingSystem::OnUpdateEditor(TimeStep ts, EditorCamera& camera, Entity selectedEntity)
	{
		auto view = m_Scene->Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			auto& comp = entity.GetComponent<ScriptComponent>();

			// Check whether Script class exists
			if (!ScriptEngine::GetInstance().EntityClassExists(comp.ClassName))
			{
				comp.ScriptClassExists = false;
				ScriptEngine::GetInstance().OnDeleteEntity(entity);
				continue;
			}
			else comp.ScriptClassExists = true;

			// Check whether instance has been instantiate
			if (!ScriptEngine::GetInstance().OnUpdateEditor(entity, ts))
			{
				if (ScriptEngine::GetInstance().OnCreateEntity(entity))
				{
					ScriptEngine::GetInstance().OnUpdateEditor(entity, ts);
				}
			}
		}
	}

}