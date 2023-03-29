#include "nlpch.h"

#include "ScriptGlue.h"

#include "Scripting/ScriptEngine.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace NL
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define NL_ADD_INTERNAL_CALL(Name) mono_add_internal_call("NL.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(nlm::vec3* parameter, nlm::vec3* outResult)
	{
		// NL_ENGINE_INFO("Value: {0}", *parameter);
		*outResult = nlm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(nlm::vec3* parameter)
	{
		// NL_ENGINE_INFO("Value: {0}", *parameter);
		return nlm::dot(*parameter, *parameter);
	}

	static MonoObject* GetScriptInstance(ID entityID)
	{
		return ScriptEngine::GetInstance().GetManagedInstance(entityID);
	}

	static bool Entity_HasComponent(ID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		NL_ENGINE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(), "");
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetID();
	}

	static void TransformComponent_GetTranslation(ID entityID, nlm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(ID entityID, nlm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void ModelRendererComponent_RotateBone(ID entityID, int boneId, nlm::vec3* eulerAngles)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<ModelRendererComponent>())
		{
			auto& comp = entity.GetComponent<ModelRendererComponent>();
			comp.RotateBone(boneId, eulerAngles);
		}
	}

	static void ModelRendererComponent_RecalculateFinalBoneMatrices(ID entityID)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<ModelRendererComponent>())
		{
			auto& comp = entity.GetComponent<ModelRendererComponent>();
			comp.RecalculateFinalBoneMatrices();
		}
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]() {

		std::string_view typeName = typeid(Component).name();
		size_t pos = typeName.find_last_of(':');
		std::string_view structName = typeName.substr(pos + 1);
		std::string managedTypename = fmt::format("NL.{}", structName);

		MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetInstance().GetCoreAssemblyImage());
		if (!managedType)
		{
			NL_ENGINE_ERROR("Could not find component type {}", managedTypename);
			return;
		}

		s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };

		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(Components<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		NL_ADD_INTERNAL_CALL(NativeLog);
		NL_ADD_INTERNAL_CALL(NativeLog_Vector);
		NL_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		NL_ADD_INTERNAL_CALL(GetScriptInstance);

		NL_ADD_INTERNAL_CALL(Entity_HasComponent);
		NL_ADD_INTERNAL_CALL(Entity_FindEntityByName);

		NL_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		NL_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		NL_ADD_INTERNAL_CALL(Input_IsKeyDown);

		NL_ADD_INTERNAL_CALL(ModelRendererComponent_RotateBone);
		NL_ADD_INTERNAL_CALL(ModelRendererComponent_RecalculateFinalBoneMatrices);
	}
}
