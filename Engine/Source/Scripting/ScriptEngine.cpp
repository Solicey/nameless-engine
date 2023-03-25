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
	}

	void ScriptEngine::Init()
	{
		// Let Mono know where the .NET libraries are located.
		mono_set_assemblies_path("../3rdParty/mono/DotNetLibs/4.5");

		MonoDomain* rootDomain = mono_jit_init("NLJITRuntime");
		if (rootDomain == nullptr)
		{
			NL_ENGINE_ERROR("[Mono] Root domain init error!");
			return;
		}

		// mono_thread_set_main(mono_thread_current());

		ScriptGlue::RegisterFunctions();

		bool status = LoadCoreAssembly("../ScriptCore/Binaries/ScriptCore.dll");
		if (!status)
		{
			NL_ENGINE_ERROR("[Mono] Could not load ScriptCore assembly!");
			return;
		}

		// all C# scripts in assets folder will be put into one app assembly
		// this may be temporary
		std::filesystem::path appPath = PathConfig::GetInstance().GetScriptsFolder() / "Binaries/CSharpScripts.dll";
		status = LoadAppAssembly(appPath.string());
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

	bool ScriptEngine::LoadCoreAssembly(const std::string& filepath)
	{
		m_AppAssembly = Utils::LoadMonoAssembly(filepath);
		if (m_AppAssembly == nullptr)
			return false;

		m_AppAssemblyImage = mono_assembly_get_image(m_CoreAssembly);

		// File Watcher

		return true;
	}

	bool ScriptEngine::LoadAppAssembly(const std::string& filepath)
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

	void ScriptEngine::OnRuntimeStart(Ref<Scene> scene)
	{
		m_Scene.reset();
		m_Scene = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		m_Scene.reset();
		m_EntityInstances.clear();
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return m_EntityClasses.find(fullClassName) != m_EntityClasses.end();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, TimeStep ts)
	{
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

			MonoClass* monoClass = mono_class_from_name(m_CoreAssemblyImage, nameSpace, className);

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

}