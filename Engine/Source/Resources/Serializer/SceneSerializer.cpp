#include "nlpch.h"

#include "SceneSerializer.h"
#include "Core/Log/Log.h"
#include "Core/Math/Math.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"
#include "Scripting/ScriptEngine.h"

#include <fstream>

namespace YAML
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const NL::nlm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const NL::nlm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const NL::nlm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	template<>
	struct convert<NL::nlm::vec2>
	{
		static Node encode(const NL::nlm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, NL::nlm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<NL::nlm::vec3>
	{
		static Node encode(const NL::nlm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, NL::nlm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<NL::nlm::vec4>
	{
		static Node encode(const NL::nlm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, NL::nlm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<NL::ID>
	{
		static Node encode(const NL::ID& id)
		{
			Node node;
			node.push_back((uint64_t)id);
			return node;
		}

		static bool decode(const Node& node, NL::ID& id)
		{
			id = node.as<uint64_t>();
			return true;
		}
	};
}

namespace NL

#define WRITE_SCRIPT_FIELD(FieldType, Type)           \
			case ScriptFieldType::FieldType:          \
				out << fieldInstance.GetValue<Type>();\
				break

#define READ_SCRIPT_FIELD(FieldType, Type)             \
	case ScriptFieldType::FieldType:                   \
	{                                                  \
		Type data = scriptField["Data"].as<Type>();    \
		fieldInstance.SetValue(data);                  \
		break;                                         \
	}
{
	void SceneSerializer::Serialize(const std::string& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{

		Entity entity = { entityID, m_Scene.get() };
		if (!entity)
			return;

		//Serialize Entity
		SerializeEntity(out, entity);

		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::string& path)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path);
		}
		catch (YAML::ParserException e)
		{
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		NL_ENGINE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (!entities)
			return true;

		for (auto entity : entities)
		{
			uint64_t id = entity["ID"].as<uint64_t>();
			std::string name = entity["Name"].as<std::string>();

			NL_ENGINE_TRACE("Deserialized entity with ID = {0}, name = {1}", id, name);

			Entity deserializedEntity = m_Scene->CreateEntityWithID(id, name);

			auto transformComponent = entity["TransformComponent"];
			if (transformComponent)
			{
				// Entities always have transforms when created
				auto& comp = deserializedEntity.GetComponent<TransformComponent>();
				comp.Translation = transformComponent["Translation"].as<nlm::vec3>();
				comp.Rotation = transformComponent["Rotation"].as<nlm::vec3>();
				comp.Scale = transformComponent["Scale"].as<nlm::vec3>();
			}

			// Model
			auto modelRendererComponent = entity["ModelRendererComponent"];
			if (modelRendererComponent)
			{
				std::string path = modelRendererComponent["ModelPath"].as<std::string>();
				auto& comp = deserializedEntity.AddComponent<ModelRendererComponent>(path, (uint32_t)deserializedEntity);
				auto& materialMap = comp.mModel->GetMaterialsNotConst();

				auto mats = modelRendererComponent["Materials"];
				for (auto mat : mats)
				{
					std::string matName = mat["MaterialName"].as<std::string>();
					if (!materialMap.contains(matName))
						continue;

					auto& material = materialMap[matName];

					material->LoadShader(mat["ShaderName"].as<std::string>());

					auto props = mat["ShaderProperties"];
					if (!props)
						continue;

					auto& properties = material->GetShaderPropertiesNotConst();
					properties.clear();
					for (auto prop : props)
					{
						ShaderProperty newProp;
						newProp.Type = static_cast<ShaderUniformType>(prop["Type"].as<int>());
						newProp.Name = prop["Name"].as<std::string>();
						// variant TODO
						switch (newProp.Type)
						{
						case ShaderUniformType::Color3:
							newProp.Value = prop["Value"].as<nlm::vec3>();
							break;
						case ShaderUniformType::Sampler2D:
							newProp.Value = prop["Value"].as<std::string>();
							break;
						default:
							break;
						}
						properties.emplace_back(newProp);
					}
				}

				auto bones = modelRendererComponent["Bones"];
				for (auto bone : bones)
				{
					std::string parent = bone["Parent"].as<std::string>();
					std::string child = bone["Child"].as<std::string>();
					comp.Bones.push_back(std::make_pair(parent, child));
				}
				comp.UpdateBoneInfos();
			}

			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent)
			{
				auto& comp = deserializedEntity.AddComponent<CameraComponent>();
				comp.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				comp.ClearColor = cameraComponent["ClearColor"].as<nlm::vec4>();
				comp.mCamera = Camera(
					cameraComponent["ProjectionType"].as<int>(),
					cameraComponent["PerspFOV"].as<float>(),
					cameraComponent["PerspNear"].as<float>(),
					cameraComponent["PerspFar"].as<float>(),
					cameraComponent["OrthoSize"].as<float>(),
					cameraComponent["OrthoNear"].as<float>(),
					cameraComponent["OrthoFar"].as<float>(),
					cameraComponent["ViewportWidth"].as<uint32_t>(),
					cameraComponent["ViewportHeight"].as<uint32_t>()
				);
			}

			auto scriptComponent = entity["ScriptComponent"];
			if (scriptComponent)
			{
				auto& comp = deserializedEntity.AddComponent<ScriptComponent>();
				comp.ClassName = scriptComponent["ClassName"].as<std::string>();

				auto scriptFields = scriptComponent["ScriptFields"];
				if (scriptFields)
				{
					Ref<ScriptClass> entityClass = ScriptEngine::GetInstance().GetEntityClass(comp.ClassName);
					if (entityClass)
					{
						const auto& fields = entityClass->GetFields();
						auto& fieldInstances = ScriptEngine::GetInstance().GetScriptFieldInstances(deserializedEntity);

						for (auto scriptField : scriptFields)
						{
							std::string name = scriptField["Name"].as<std::string>();
							std::string typeString = scriptField["Type"].as<std::string>();
							ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);

							ScriptFieldInstance& fieldInstance = fieldInstances[name];

							// NL_ENGINE_ASSERT(fields.find(name) != fields.end(), "");

							if (fields.find(name) == fields.end())
								continue;

							fieldInstance.Field = fields.at(name);

							switch (type)
							{
								READ_SCRIPT_FIELD(Float, float);
								READ_SCRIPT_FIELD(Double, double);
								READ_SCRIPT_FIELD(Bool, bool);
								READ_SCRIPT_FIELD(Char, char);
								READ_SCRIPT_FIELD(Byte, int8_t);
								READ_SCRIPT_FIELD(Short, int16_t);
								READ_SCRIPT_FIELD(Int, int32_t);
								READ_SCRIPT_FIELD(Long, int64_t);
								READ_SCRIPT_FIELD(UByte, uint8_t);
								READ_SCRIPT_FIELD(UShort, uint16_t);
								READ_SCRIPT_FIELD(UInt, uint32_t);
								READ_SCRIPT_FIELD(ULong, uint64_t);
								READ_SCRIPT_FIELD(Vector2, glm::vec2);
								READ_SCRIPT_FIELD(Vector3, glm::vec3);
								READ_SCRIPT_FIELD(Vector4, glm::vec4);
								READ_SCRIPT_FIELD(Entity, ID);
							}
						}
					}
				}

			}
		}

		return true;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		NL_ENGINE_ASSERT(entity.HasComponent<IdentityComponent>(), "Entity should at least have IdentityComponent!");

		out << YAML::BeginMap; // Entity

		out << YAML::Key << "ID" << YAML::Value << entity.GetID().GetID();
		out << YAML::Key << "Name" << YAML::Value << entity.GetName();

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& comp = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << comp.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << comp.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << comp.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<ModelRendererComponent>())
		{
			out << YAML::Key << "ModelRendererComponent";
			out << YAML::BeginMap; // ModelRendererComponent

			auto& comp = entity.GetComponent<ModelRendererComponent>();
			out << YAML::Key << "ModelPath" << YAML::Value << comp.Path;

			out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq; // Materials
			const auto& mats = comp.mModel->GetMaterials();
			for (const auto& item : mats)
			{
				const auto& matName = item.first;
				const auto& mat = item.second;

				out << YAML::BeginMap; // Material

				out << YAML::Key << "MaterialName" << YAML::Value << matName;
				out << YAML::Key << "ShaderName" << YAML::Value << mat->GetShaderName();

				out << YAML::Key << "ShaderProperties" << YAML::Value << YAML::BeginSeq; // ShaderProperties

				for (const auto& prop : mat->GetShaderPropertiesNotConst())
				{
					out << YAML::BeginMap; // ShaderProperty
					out << YAML::Key << "Type" << YAML::Value << (int)prop.Type;
					out << YAML::Key << "Name" << YAML::Value << prop.Name;
					int index = prop.Value.index();
					// ShaderUniform.h
					// variant TODO
					switch (index)
					{
					case 0:		// std::string
						out << YAML::Key << "Value" << YAML::Value << std::get<0>(prop.Value);
						break;
					case 1:		// nlm::vec3
						out << YAML::Key << "Value" << YAML::Value << std::get<1>(prop.Value);
						break;
					default:
						break;
					}
					out << YAML::EndMap; // ShaderProperty
				}

				out << YAML::EndSeq; // ShaderProperties

				out << YAML::EndMap; // Material
			}
			out << YAML::EndSeq; // Materials


			out << YAML::Key << "Bones" << YAML::Value << YAML::BeginSeq; // Bones
			auto& bones = comp.mModel->GetBonesNotConst();
			std::string empty = "null";
			for (auto& pair : bones)
			{
				int id = pair.first;
				int parentId = pair.second.parentID;

				out << YAML::BeginMap; // BonePair
				if (parentId == -1)
					out << YAML::Key << "Parent" << YAML::Value << empty;
				else
					out << YAML::Key << "Parent" << YAML::Value << bones[parentId].Name;
				out << YAML::Key << "Child" << YAML::Value << bones[id].Name;
				out << YAML::EndMap; // BonePair
			}
			out << YAML::EndSeq; // Bones

			out << YAML::EndMap; // ModelRendererComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& comp = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << comp.FixedAspectRatio;
			out << YAML::Key << "ClearColor" << YAML::Value << comp.ClearColor;

			auto& cam = comp.mCamera;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)cam.GetProjectionType();
			out << YAML::Key << "OrthoSize" << YAML::Value << cam.GetOrthographicSize();
			out << YAML::Key << "OrthoFar" << YAML::Value << cam.GetOrthographicFar();
			out << YAML::Key << "OrthoNear" << YAML::Value << cam.GetOrthographicNear();
			out << YAML::Key << "PerspFOV" << YAML::Value << cam.GetPerspectiveFOV();
			out << YAML::Key << "PerspFar" << YAML::Value << cam.GetPerspectiveFar();
			out << YAML::Key << "PerspNear" << YAML::Value << cam.GetPerspectiveNear();
			out << YAML::Key << "ViewportWidth" << YAML::Value << cam.GetViewportWidth();
			out << YAML::Key << "ViewportHeight" << YAML::Value << cam.GetViewportHeight();

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			auto& comp = entity.GetComponent<ScriptComponent>();

			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; // ScriptComponent
			out << YAML::Key << "ClassName" << YAML::Value << comp.ClassName;

			// Fields
			Ref<ScriptClass> entityClass = ScriptEngine::GetInstance().GetEntityClass(comp.ClassName);
			if (entityClass != nullptr)
			{
				const auto& fields = entityClass->GetFields();
				if (fields.size() > 0)
				{
					out << YAML::Key << "ScriptFields" << YAML::Value;
					auto& fieldInstances = ScriptEngine::GetInstance().GetScriptFieldInstances(entity);
					out << YAML::BeginSeq;
					for (const auto& [name, field] : fields)
					{
						if (fieldInstances.find(name) == fieldInstances.end())
							continue;

						out << YAML::BeginMap; // ScriptField
						out << YAML::Key << "Name" << YAML::Value << name;
						out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

						out << YAML::Key << "Data" << YAML::Value;
						ScriptFieldInstance& fieldInstance = fieldInstances.at(name);

						switch (field.Type)
						{
							WRITE_SCRIPT_FIELD(Float, float);
							WRITE_SCRIPT_FIELD(Double, double);
							WRITE_SCRIPT_FIELD(Bool, bool);
							WRITE_SCRIPT_FIELD(Char, char);
							WRITE_SCRIPT_FIELD(Byte, int8_t);
							WRITE_SCRIPT_FIELD(Short, int16_t);
							WRITE_SCRIPT_FIELD(Int, int32_t);
							WRITE_SCRIPT_FIELD(Long, int64_t);
							WRITE_SCRIPT_FIELD(UByte, uint8_t);
							WRITE_SCRIPT_FIELD(UShort, uint16_t);
							WRITE_SCRIPT_FIELD(UInt, uint32_t);
							WRITE_SCRIPT_FIELD(ULong, uint64_t);
							WRITE_SCRIPT_FIELD(Vector2, glm::vec2);
							WRITE_SCRIPT_FIELD(Vector3, glm::vec3);
							WRITE_SCRIPT_FIELD(Vector4, glm::vec4);
							WRITE_SCRIPT_FIELD(Entity, ID);
						}
						out << YAML::EndMap; // ScriptFields
					}
					out << YAML::EndSeq;
				}
			}

			out << YAML::EndMap; // ScriptComponent
		}

		out << YAML::EndMap; // Entity
	}
}