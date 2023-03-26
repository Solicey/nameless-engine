#pragma once

#include "Core/Misc/Singleton.h"
#include "Core/Misc/TimeStep.h"
#include "ECS/Entity/Entity.h"

#include <mono/metadata/image.h>
#include <mono/jit/jit.h>

#include <string>
#include <unordered_map>

namespace NL
{
	// C#脚本中域的类型
	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};

	// C#脚本中的域
	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};

	// C#脚本中域的实例
	struct ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		ScriptFieldInstance(const ScriptField& field) : Field(field)
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}

	private:
		uint8_t m_Buffer[16];

		friend class ScriptEngine;
		friend class ScriptInstance;
	};

	using ScriptFields = std::unordered_map<std::string, ScriptField>;
	using ScriptFieldInstances = std::unordered_map<std::string, ScriptFieldInstance>;

	// C#脚本中的类
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* CallMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		const ScriptFields& GetFields() const { return m_Fields; }

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		ScriptFields m_Fields;
		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
	};

	// C#脚本中类的实例
	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void CallOnCreate();
		void CallOnUpdate(float ts);

		Ref<ScriptClass> GetScriptClass(){ return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			SetFieldValueInternal(name, &value);
		}

		MonoObject* GetManagedObject() { return m_Instance; }

	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);

	private:
		friend class ScriptEngine;

		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		inline static char s_FieldValueBuffer[16];
	};


	// Credit to hwx0000: https://blog.csdn.net/alexhu2010q/article/details/126960468?spm=1001.2014.3001.5506
	class ScriptEngine : public Singleton<ScriptEngine>
	{
	public:
		ScriptEngine(token) {}

		void Init();
		bool LoadCoreAssembly(const std::string& filepath);
		bool LoadAppAssembly(const std::string& filepath);

		void ReloadAssembly();

		void OnStartRuntime(Scene* scene);
		void OnStopRuntime();

		bool EntityClassExists(const std::string& fullClassName);
		void OnCreateEntity(Entity entity);
		void OnUpdateEntity(Entity entity, TimeStep ts);

#pragma region Deprecated

		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);

		void PrintAssemblyTypes(MonoAssembly* assembly);

		// 根据C++这边输入的class name, 返回对应的MonoClass, 如果想在C++端创建C#上的对象, 需要借助此API
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);

		MonoObject* Instantiate(MonoClass* monoClass);

		// Mono gives us two ways of calling C# methods: mono_runtime_invoke and Unmanaged Method Thunks. 
		// This Api will only cover mono_runtime_invoke

		// Using mono_runtime_invoke is slower compared to Unmanaged Method Thunks, but it's also safe and more flexible. 
		// mono_runtime_invoke can invoke any method with any parameters, and from what I understand mono_runtime_invoke also does a lot more error checking and validation on the object you pass, as well as the parameters.

		// 在编译期不知道Method签名时, 适合用mono_runtime_invoke, 每秒高频率调用(10fps)的Method适合用Unmanaged Method Thunks, 
		void CallMethod(MonoObject* instance, const char* methodName);

		MonoClassField* GetFieldRef(MonoObject* instance, const char* fieldName);

		template<class T>
		const T& GetFieldValue(MonoObject* instance, MonoClassField* field)
		{
			T value;
			mono_field_get_value(instance, field, &value);
			return value;
		}

		MonoProperty* GetPropertyRef(MonoObject* instance, const char* propName);

		template<class T>
		const T& GetPropertyValue(MonoObject* instance, MonoProperty* prop)
		{
			T value;
			mono_property_get_value(instance, prop, &value);
			return value;
		}

#pragma endregion

		MonoImage* GetCoreAssemblyImage() { return m_CoreAssemblyImage; }
		MonoImage* GetAppAssemblyImage() { return m_AppAssemblyImage; }
		ScriptClass* GetClassEntity() { return &m_EntityClass; }
		Ref<ScriptClass> GetEntityClass(const std::string& name)
		{
			if (m_EntityClasses.find(name) == m_EntityClasses.end())
				return nullptr;

			return m_EntityClasses.at(name);
		}

		MonoObject* GetManagedInstance(ID id)
		{
			NL_ENGINE_ASSERT(m_EntityInstances.find(id) != m_EntityInstances.end(), "");
			return m_EntityInstances.at(id)->GetManagedObject();
		}
		Scene* GetSceneContext() { return m_Scene; }
		Ref<ScriptInstance> GetScriptInstance(ID entityID);
		ScriptFieldInstances& GetScriptFieldInstances(Entity entity)
		{
			NL_ENGINE_ASSERT(entity, "");

			ID entityID = entity.GetID();
			return m_EntityScriptFieldInstances[entityID];
		}

	private:

		void LoadAssemblyClasses();

	private:
		MonoDomain* m_AppDomain = nullptr;

		MonoAssembly* m_CoreAssembly = nullptr;
		MonoImage* m_CoreAssemblyImage = nullptr;
		std::string m_CoreAssemblyFilepath;

		MonoAssembly* m_AppAssembly = nullptr;
		MonoImage* m_AppAssemblyImage = nullptr;
		std::string m_AppAssemblyFilepath;

		ScriptClass m_EntityClass;
		std::unordered_map<std::string, Ref<ScriptClass>> m_EntityClasses;
		std::unordered_map<ID, Ref<ScriptInstance>> m_EntityInstances;
		std::unordered_map<ID, ScriptFieldInstances> m_EntityScriptFieldInstances;

		Scene* m_Scene;
	};

	namespace Utils
	{
		inline const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
		{
			switch (fieldType)
			{
			case ScriptFieldType::None:    return "None";
			case ScriptFieldType::Float:   return "Float";
			case ScriptFieldType::Double:  return "Double";
			case ScriptFieldType::Bool:    return "Bool";
			case ScriptFieldType::Char:    return "Char";
			case ScriptFieldType::Byte:    return "Byte";
			case ScriptFieldType::Short:   return "Short";
			case ScriptFieldType::Int:     return "Int";
			case ScriptFieldType::Long:    return "Long";
			case ScriptFieldType::UByte:   return "UByte";
			case ScriptFieldType::UShort:  return "UShort";
			case ScriptFieldType::UInt:    return "UInt";
			case ScriptFieldType::ULong:   return "ULong";
			case ScriptFieldType::Vector2: return "Vector2";
			case ScriptFieldType::Vector3: return "Vector3";
			case ScriptFieldType::Vector4: return "Vector4";
			case ScriptFieldType::Entity:  return "Entity";
			}
			NL_ENGINE_ASSERT(false, "Unknown ScriptFieldType");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")    return ScriptFieldType::None;
			if (fieldType == "Float")   return ScriptFieldType::Float;
			if (fieldType == "Double")  return ScriptFieldType::Double;
			if (fieldType == "Bool")    return ScriptFieldType::Bool;
			if (fieldType == "Char")    return ScriptFieldType::Char;
			if (fieldType == "Byte")    return ScriptFieldType::Byte;
			if (fieldType == "Short")   return ScriptFieldType::Short;
			if (fieldType == "Int")     return ScriptFieldType::Int;
			if (fieldType == "Long")    return ScriptFieldType::Long;
			if (fieldType == "UByte")   return ScriptFieldType::UByte;
			if (fieldType == "UShort")  return ScriptFieldType::UShort;
			if (fieldType == "UInt")    return ScriptFieldType::UInt;
			if (fieldType == "ULong")   return ScriptFieldType::ULong;
			if (fieldType == "Vector2") return ScriptFieldType::Vector2;
			if (fieldType == "Vector3") return ScriptFieldType::Vector3;
			if (fieldType == "Vector4") return ScriptFieldType::Vector4;
			if (fieldType == "Entity")  return ScriptFieldType::Entity;

			NL_ENGINE_ASSERT(false, "Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}
	}
}