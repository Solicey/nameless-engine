#include "nlpch.h"

#include "ScriptEngine.h"
#include "Scripting/ScriptGlue.h"

#include "Core/Log/Log.h"
#include "Core/Misc/PathConfig.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/tabledefs.h>
#include <fstream>

namespace NL
{
	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Char", ScriptFieldType::Char },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.Int64", ScriptFieldType::Long },
		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.UInt64", ScriptFieldType::ULong },

		{ "NL.Vector2", ScriptFieldType::Vector2 },
		{ "NL.Vector3", ScriptFieldType::Vector3 },
		{ "NL.Vector4", ScriptFieldType::Vector4 },

		{ "NL.Entity", ScriptFieldType::Entity },
	};

	namespace Utils
	{
		static char* ReadBytes(const std::string& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				NL_ENGINE_ERROR("Failed to open the file:" + filepath);
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (!size)
			{
				NL_ENGINE_WARN("File is empty:" + filepath);
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static MonoAssembly* LoadMonoAssembly(const std::string& filepath)
		{
			uint32_t fileSize = 0;
			// Read C# .dll Files
			char* fileData = Utils::ReadBytes(filepath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, true, &status, false);

			if (status != MONO_IMAGE_OK)
			{
				NL_ENGINE_ERROR("[Mono] Load image error: {0}", mono_image_strerror(status));
				delete[] fileData;
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, filepath.c_str(), &status, 0);
			mono_image_close(image);

			delete[] fileData;
			return assembly;
		}

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				NL_ENGINE_ERROR("Unknown type: {}", typeName);
				return ScriptFieldType::None;
			}

			return it->second;
		}
	}

	void ScriptEngine::Init()
	{
		// Let Mono know where the .NET libraries are located.
		mono_set_assemblies_path("../3rdParty/mono/DotNetLibs/4.5");

		m_RootDomain = mono_jit_init("NLJITRuntime");
		if (m_RootDomain == nullptr)
		{
			NL_ENGINE_ERROR("[Mono] Root domain init error!");
			return;
		}

		// mono_thread_set_main(mono_thread_current());

		ScriptGlue::RegisterFunctions();

		m_CoreAssemblyFilepath = "../ScriptCore/Binaries/ScriptCore.dll";
		bool status = LoadCoreAssembly(m_CoreAssemblyFilepath);
		if (!status)
		{
			NL_ENGINE_ERROR("[Mono] Could not load ScriptCore assembly!");
			return;
		}

		// all C# scripts in assets folder will be put into one app assembly
		// this may be temporary
		std::filesystem::path appPath = PathConfig::GetInstance().GetScriptsFolder() / "Binaries/CSharpScripts.dll";
		m_AppAssemblyFilepath = appPath.string();
		status = LoadAppAssembly(m_AppAssemblyFilepath);
		if (!status)
		{
			NL_ENGINE_ERROR("[Mono] Could not load App assembly!");
			return;
		}
		
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();

		// Retrieve and instantiate class
		m_EntityClass = ScriptClass("NL", "Entity", true);

	}

	void ScriptEngine::Shutdown()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(m_AppDomain);
		m_AppDomain = nullptr;

		mono_jit_cleanup(m_RootDomain);
		m_RootDomain = nullptr;
	}

	bool ScriptEngine::LoadCoreAssembly(const std::string& filepath)
	{
		char domain[20] = "NLScriptRuntime";
		m_AppDomain = mono_domain_create_appdomain(domain, nullptr);
		mono_domain_set(m_AppDomain, true);

		m_CoreAssembly = Utils::LoadMonoAssembly(filepath);
		if (m_CoreAssembly == nullptr)
			return false;

		m_CoreAssemblyImage = mono_assembly_get_image(m_CoreAssembly);
		return true;		
	}

	bool ScriptEngine::LoadAppAssembly(const std::string& filepath)
	{
		m_AppAssembly = Utils::LoadMonoAssembly(filepath);
		if (m_AppAssembly == nullptr)
			return false;

		m_AppAssemblyImage = mono_assembly_get_image(m_AppAssembly);

		// File Watcher

		return true;
	}

	void ScriptEngine::ReloadAssembly()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(m_AppDomain);

		LoadCoreAssembly(m_CoreAssemblyFilepath);
		LoadAppAssembly(m_AppAssemblyFilepath);
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();

		// Retrieve and instantiate class
		m_EntityClass = ScriptClass("NL", "Entity", true);
	}

	void ScriptEngine::SetSceneContext(Scene* scene)
	{
		//m_Scene.reset();
		m_Scene = scene;
	}

	void ScriptEngine::OnStopRuntime()
	{
		m_Scene = nullptr;
		//m_Scene.reset();
		m_EntityInstances.clear();
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return m_EntityClasses.find(fullClassName) != m_EntityClasses.end();
	}

	bool ScriptEngine::OnCreateEntity(Entity entity)
	{
		ID entityID = entity.GetID();
		const auto& comp = entity.GetComponent<ScriptComponent>();
		if (EntityClassExists(comp.ClassName))
		{
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(m_EntityClasses[comp.ClassName], entity);
			m_EntityInstances[entityID] = instance;

			// Copy 
			if (m_EntityScriptFieldInstances.find(entityID) != m_EntityScriptFieldInstances.end())
			{
				const ScriptFieldInstances& fieldInstances = m_EntityScriptFieldInstances.at(entityID);
				for (const auto& [name, fieldInstance] : fieldInstances)
					instance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
			}

			instance->CallOnCreate();

			return true;
		}
		else
		{
			if (m_EntityInstances.find(entityID) != m_EntityInstances.end())
				m_EntityInstances.erase(entityID);
			if (m_EntityScriptFieldInstances.find(entityID) != m_EntityScriptFieldInstances.end())
				m_EntityScriptFieldInstances.erase(entityID);
			return false;
		}
	}

	void ScriptEngine::OnUpdateRuntime(Entity entity, TimeStep ts)
	{
		ID entityID = entity.GetID();
		if (m_EntityInstances.find(entityID) != m_EntityInstances.end())
		{
			Ref<ScriptInstance> instance = m_EntityInstances[entityID];
			instance->CallOnUpdateRuntime((float)ts);
		}
		else
		{
			NL_ENGINE_ERROR("Could not find Script Instance for entity {0}", entityID);
		}
	}

	void ScriptEngine::OnUpdateEditor(Entity entity, TimeStep ts)
	{
		ID entityID = entity.GetID();
		if (m_EntityInstances.find(entityID) != m_EntityInstances.end())
		{
			Ref<ScriptInstance> instance = m_EntityInstances[entityID];
			instance->CallOnUpdateEditor((float)ts);
		}
		else
		{
			// NL_ENGINE_ERROR("Could not find Script Instance for entity {0}", entityID);
		}
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		m_EntityClasses.clear();

		// 从assembly的meta信息里读取meta data table, 这里读取的是Type对应的Table, 表里的每一行代表一个Type
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(m_AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityClass = mono_class_from_name(m_CoreAssemblyImage, "NL", "Entity");

		// 遍历Table里的每行, 这里的numTypes最小为1, 因为C#的DLL和EXEs默认都会有一个Module类型的Type, 代表整个assembly的module
		for (int32_t i = 1; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(m_AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(m_AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			NL_ENGINE_TRACE("[Mono] Namespace: {0}, Name: {1}", nameSpace, className);

			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(m_AppAssemblyImage, nameSpace, className);

			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (!isEntity)
				continue;

			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);
			m_EntityClasses[fullName] = scriptClass;

			// This routine is an iterator routine for retrieving the fields in a class.
			// You must pass a gpointer that points to zero and is treated as an opaque handle
			// to iterate over all of the elements. When no more values are available, the return value is NULL.

			int fieldCount = mono_class_num_fields(monoClass);
			NL_ENGINE_INFO("{} has {} fields:", className, fieldCount);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);
					NL_ENGINE_INFO("  {} ({})", fieldName, Utils::ScriptFieldTypeToString(fieldType));

					scriptClass->m_Fields[fieldName] = ScriptField(fieldType, fieldName, field);
				}
			}
		}
	}

	MonoAssembly* ScriptEngine::LoadCSharpAssembly(const std::string& assemblyPath)
	{
		// Let Mono know where the .NET libraries are located.
		mono_set_assemblies_path("../3rdParty/mono/DotNetLibs/4.5");

		MonoDomain* rootDomain = mono_jit_init("NLScriptRuntime");
		if (rootDomain == nullptr)
		{
			NL_ENGINE_ERROR("[Mono] Root domain init error!");
			return nullptr;
		}

		// Create an App Domain
		char domain[20] = "NLAppDomain";
		m_AppDomain = mono_domain_create_appdomain(domain, nullptr);
		mono_domain_set(m_AppDomain, true);

		uint32_t fileSize = 0;
		// Read C# .dll Files
		char* fileData = Utils::ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, true, &status, false);

		if (status != MONO_IMAGE_OK)
		{
			NL_ENGINE_ERROR("[Mono] Load image error: {0}", mono_image_strerror(status));
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}

	void ScriptEngine::PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);

		// 从assembly的meta信息里读取meta data table, 这里读取的是Type对应的Table, 表里的每一行代表一个Type
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		// 遍历Table里的每行, 这里的numTypes最小为1, 因为C#的DLL和EXEs默认都会有一个Module类型的Type, 代表整个assembly的module
		for (int32_t i = 1; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			NL_ENGINE_TRACE("[Mono] Namespace: {0}, Name: {1}", nameSpace, name);
		}

	}

	MonoClass* ScriptEngine::GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* monoClass = mono_class_from_name(image, namespaceName, className);

		if (!monoClass)
			return nullptr;

		return monoClass;
	}

	MonoObject* ScriptEngine::Instantiate(MonoClass* monoClass)
	{
		if (!monoClass)
			return nullptr;

		MonoObject* instance = mono_object_new(m_AppDomain, monoClass);

		mono_runtime_object_init(instance);

		return instance;
	}

	void ScriptEngine::CallMethod(MonoObject* instance, const char* methodName)
	{
		MonoClass* monoClass = mono_object_get_class(instance);

		MonoMethod* method = mono_class_get_method_from_name(monoClass, methodName, 0);

		if (!method)
		{
			NL_ENGINE_WARN("[Mono] Method '{0}' cannot be called!", methodName);
			return;
		}

		MonoObject* exception = nullptr;
		mono_runtime_invoke(method, instance, nullptr, &exception);

		// TODO: Handle exceptions
	}

	MonoClassField* ScriptEngine::GetFieldRef(MonoObject* instance, const char* fieldName)
	{
		MonoClass* monoClass = mono_object_get_class(instance);

		return mono_class_get_field_from_name(monoClass, fieldName);
	}

	MonoProperty* ScriptEngine::GetPropertyRef(MonoObject* instance, const char* propName)
	{
		MonoClass* monoClass = mono_object_get_class(instance);

		return mono_class_get_property_from_name(monoClass, propName);
	}

	Ref<ScriptInstance> ScriptEngine::GetScriptInstance(ID entityID)
	{
		auto it = m_EntityInstances.find(entityID);
		if (it == m_EntityInstances.end())
			return nullptr;

		return it->second;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
	{
		auto& engine = ScriptEngine::GetInstance();
		m_MonoClass = mono_class_from_name(isCore ? engine.GetCoreAssemblyImage() : engine.GetAppAssemblyImage(), classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::GetInstance().Instantiate(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::CallMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		MonoObject* exception = nullptr;
		return mono_runtime_invoke(method, instance, params, &exception);
	}


	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();

		m_Constructor = ScriptEngine::GetInstance().GetClassEntity()->GetMethod(".ctor", 1);
		m_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateRuntimeMethod = scriptClass->GetMethod("OnUpdateRuntime", 1);
		m_OnUpdateEditorMethod = scriptClass->GetMethod("OnUpdateEditor", 1);

		// Call Entity Constructor
		{
			ID entityID = entity.GetID();
			void* param = &entityID;
			m_ScriptClass->CallMethod(m_Instance, m_Constructor, &param);
		}
	}

	void ScriptInstance::CallOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->CallMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::CallOnUpdateRuntime(float ts)
	{
		if (m_OnUpdateRuntimeMethod)
		{
			void* param = &ts;
			m_ScriptClass->CallMethod(m_Instance, m_OnUpdateRuntimeMethod, &param);
		}
	}

	void ScriptInstance::CallOnUpdateEditor(float ts)
	{
		if (m_OnUpdateEditorMethod)
		{
			void* param = &ts;
			m_ScriptClass->CallMethod(m_Instance, m_OnUpdateEditorMethod, &param);
		}
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);

		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_Instance, field.ClassField, (void*)value);

		return true;
	}

}