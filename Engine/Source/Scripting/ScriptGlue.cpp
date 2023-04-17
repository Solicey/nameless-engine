#include "nlpch.h"

#include "ScriptGlue.h"

#include "Core/App/Application.h"
#include "Scripting/ScriptEngine.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"
#include "Events/MiscEvent.h"

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

	static void Math_EulerAnglesToQuat(nlm::vec3* eulerAngles, nlm::vec4* orientation)
	{
		nlm::quat q = nlm::quat(nlm::radians(*eulerAngles));
		*orientation = nlm::vec4(q.x, q.y, q.z, q.w);
	}

	static void Math_QuatToEulerAngles(nlm::vec4* orientation, nlm::vec3* eulerAngles)
	{
		nlm::quat q = nlm::quat(orientation->w, orientation->x, orientation->y, orientation->z);
		//nlm::extractEulerAngleYXZ(nlm::toMat4(q), eulerAngles->y, eulerAngles->x, eulerAngles->z);
		*eulerAngles = nlm::degrees(nlm::eulerAngles(q));
	}

	static void Math_QuatNormalize(nlm::vec4* orientation)
	{
		nlm::quat q = nlm::quat(orientation->w, orientation->x, orientation->y, orientation->z);
		q = nlm::normalize(q);
		*orientation = nlm::vec4(q.x, q.y, q.z, q.w);
	}

	// ! WARNING !
	// We use radians here, but use degrees in C# Script!
	// Do all the conversions here!

	static void TransformComponent_GetTranslation(ID entityID, nlm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_GetRotation(ID entityID, nlm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		*outRotation = nlm::degrees(entity.GetComponent<TransformComponent>().Rotation);
	}

	static void TransformComponent_SetTranslation(ID entityID, nlm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void TransformComponent_SetRotation(ID entityID, nlm::vec3* rotation)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		entity.GetComponent<TransformComponent>().Rotation = nlm::radians(*rotation);
	}

	static void TransformComponent_GetForward(ID entityID, nlm::vec3* forward)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<TransformComponent>())
		{
			*forward = entity.GetComponent<TransformComponent>().GetForward();
		}
	}

	static void TransformComponent_GetRight(ID entityID, nlm::vec3* right)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<TransformComponent>())
		{
			*right = entity.GetComponent<TransformComponent>().GetRight();
		}
	}

	static void TransformComponent_GetUp(ID entityID, nlm::vec3* up)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<TransformComponent>())
		{
			*up = entity.GetComponent<TransformComponent>().GetUp();
		}
	}

	static void TransformComponent_Translate(ID entityID, nlm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<TransformComponent>())
		{
			auto& comp = entity.GetComponent<TransformComponent>();
			comp.Translation += *translation;
		}
	}

	static void TransformComponent_Rotate(ID entityID, nlm::vec3* rotateAxis, float angle)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<TransformComponent>())
		{
			auto& comp = entity.GetComponent<TransformComponent>();
			comp.Rotate(*rotateAxis, nlm::radians(angle));
		}
	}

	static void TransformComponent_LookAt(ID entityID, nlm::vec3* dest)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<TransformComponent>())
		{
			auto& comp = entity.GetComponent<TransformComponent>();
			comp.LookAt(*dest);
		}
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

	static void Input_GetCursorPos(nlm::vec2* pos)
	{
		*pos = Application::GetInstance().GetCursorPos();
		// NL_ENGINE_INFO("Cursor pos: {0}, {1}", (*pos).x, (*pos).y);
	}

	static void CameraComponent_SetAsRuntimeCamera(ID entityID)
	{
		Scene* scene = ScriptEngine::GetInstance().GetSceneContext();
		NL_ENGINE_ASSERT(scene, "");
		Entity entity = scene->GetEntityWithID(entityID);
		NL_ENGINE_ASSERT(entity, "");

		if (entity.HasComponent<CameraComponent>())
		{
			RuntimeCameraSwitchedEvent event(entity);
			ScriptGlue::GetInstance().CallEventCallback(event);
		}
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

		NL_ADD_INTERNAL_CALL(Math_EulerAnglesToQuat);
		NL_ADD_INTERNAL_CALL(Math_QuatToEulerAngles);
		NL_ADD_INTERNAL_CALL(Math_QuatNormalize);

		NL_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		NL_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		NL_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		NL_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		NL_ADD_INTERNAL_CALL(TransformComponent_GetForward);
		NL_ADD_INTERNAL_CALL(TransformComponent_GetRight);
		NL_ADD_INTERNAL_CALL(TransformComponent_GetUp);
		NL_ADD_INTERNAL_CALL(TransformComponent_Translate);
		NL_ADD_INTERNAL_CALL(TransformComponent_Rotate);
		NL_ADD_INTERNAL_CALL(TransformComponent_LookAt);

		NL_ADD_INTERNAL_CALL(Input_IsKeyDown);
		NL_ADD_INTERNAL_CALL(Input_GetCursorPos);

		NL_ADD_INTERNAL_CALL(ModelRendererComponent_RotateBone);
		NL_ADD_INTERNAL_CALL(ModelRendererComponent_RecalculateFinalBoneMatrices);

		NL_ADD_INTERNAL_CALL(CameraComponent_SetAsRuntimeCamera);
		
	}

	void ScriptGlue::SetEventCallback(const EventCallbackFn& callback)
	{
		s_EventCallback = callback;
	}
}
