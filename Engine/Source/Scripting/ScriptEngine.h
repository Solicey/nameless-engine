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
	// C#�ű����������
	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};

	// C#�ű��е���
	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};

	// C#�ű������ʵ��
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

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	// C#�ű��е���
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* CallMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		const ScriptFieldMap& GetFieldMap() const { return m_Fields; }

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		ScriptFieldMap m_Fields;
		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
	};

	// C#�ű������ʵ��
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

		void OnRuntimeStart(Ref<Scene> scene);
		void OnRuntimeStop();

		bool EntityClassExists(const std::string& fullClassName);
		void OnCreateEntity(Entity entity);
		void OnUpdateEntity(Entity entity, TimeStep ts);

		// ========================

		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);

		void PrintAssemblyTypes(MonoAssembly* assembly);

		// ����C++��������class name, ���ض�Ӧ��MonoClass, �������C++�˴���C#�ϵĶ���, ��Ҫ������API
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);

		MonoObject* Instantiate(MonoClass* monoClass);

		// Mono gives us two ways of calling C# methods: mono_runtime_invoke and Unmanaged Method Thunks. 
		// This Api will only cover mono_runtime_invoke

		// Using mono_runtime_invoke is slower compared to Unmanaged Method Thunks, but it's also safe and more flexible. 
		// mono_runtime_invoke can invoke any method with any parameters, and from what I understand mono_runtime_invoke also does a lot more error checking and validation on the object you pass, as well as the parameters.

		// �ڱ����ڲ�֪��Methodǩ��ʱ, �ʺ���mono_runtime_invoke, ÿ���Ƶ�ʵ���(10fps)��Method�ʺ���Unmanaged Method Thunks, 
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

		Ref<Scene> m_Scene;
	};
}