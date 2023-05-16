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

		/*for (auto& pair : customInt)
		{
			out << YAML::Key << pair.first << YAML::Value << pair.second;
		}*/

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->Registry.each([&](auto entityID)
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

		/*for (auto& pair : customInt)
		{
			pair.second = data[pair.first].as<int>();
		}*/

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

			auto settingsComponent = entity["SettingsComponent"];
			if (settingsComponent)
			{
				auto& comp = deserializedEntity.AddComponent<SettingsComponent>();
				comp.AntiAliasingType = (AntiAliasingType)settingsComponent["AntiAliasingType"].as<int>();
				comp.EditorCameraClearFlag = (Camera::ClearFlagType)settingsComponent["EditorCameraClearFlag"].as<int>();
				comp.MSAASamples = settingsComponent["MSAASamples"].as<int>();
				comp.ShowGizmos = settingsComponent["ShowGizmos"].as<bool>();
				comp.RenderMode = (RenderMode)settingsComponent["RenderMode"].as<int>();
			}

			// Model
			auto modelRendererComponent = entity["ModelRendererComponent"];
			if (modelRendererComponent)
			{
				std::string path = modelRendererComponent["ModelPath"].as<std::string>();
				auto& comp = deserializedEntity.AddComponent<ModelRendererComponent>(path);
				auto& materialMap = comp._Model->GetMaterialsNotConst();

				auto mats = modelRendererComponent["Materials"];
				for (auto mat : mats)
				{
					std::string matName = mat["MaterialName"].as<std::string>();
					if (!materialMap.contains(matName))
						continue;

					auto& material = materialMap[matName];

					material->LoadShaderAndUpdateProps(mat["ShaderName"].as<std::string>());

					auto props = mat["ShaderProperties"];
					if (!props)
						continue;

					auto& properties = material->GetShaderPropertiesNotConst();

					DeserializeShaderProperties(props, properties, material);
				}

				auto bones = modelRendererComponent["Bones"];
				for (auto bone : bones)
				{
					std::string parent = bone["Parent"].as<std::string>();
					std::string child = bone["Child"].as<std::string>();
					//comp.Bones.push_back(std::make_pair(parent, child));
					comp._Model->AddBonePair(std::make_pair(parent, child));
				}
				//comp.UpdateBoneInfos();
			}

			auto spriteRendererComponent = entity["SpriteRendererComponent"];
			if (spriteRendererComponent)
			{
				auto& comp = deserializedEntity.AddComponent<SpriteRendererComponent>();
				comp.Color = spriteRendererComponent["Color"].as<nlm::vec4>();
				comp.Reaction = (SpriteCameraReaction)spriteRendererComponent["Reaction"].as<int>();
				std::string path = spriteRendererComponent["Path"].as<std::string>();
				comp.ReplaceTexture(path);
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
					cameraComponent["ViewportHeight"].as<uint32_t>(),
					cameraComponent["ClearFlagType"].as<int>()
				);
			}

			auto postProcessingComponent = entity["PostProcessingComponent"];
			if (postProcessingComponent)
			{
				auto& comp = deserializedEntity.AddComponent<PostProcessingComponent>();
				auto mats = postProcessingComponent["Materials"];
				for (auto mat : mats)
				{
					Ref<Material> material = CreateRef<Material>(mat["ShaderName"].as<std::string>());

					auto props = mat["ShaderProperties"];
					if (props)
					{
						auto& properties = material->GetShaderPropertiesNotConst();
						DeserializeShaderProperties(props, properties, material);
					}

					comp.Queue.push_back(material);
				}
			}

			auto lightComponent = entity["LightComponent"];
			if (lightComponent)
			{
				auto& comp = deserializedEntity.AddComponent<LightComponent>();
				comp.Type = (LightType)lightComponent["LightType"].as<int>();
				comp.Color = lightComponent["Color"].as<nlm::vec3>();
				comp.Intensity = lightComponent["Intensity"].as<float>();
				comp.Attenuation = lightComponent["Attenuation"].as<nlm::vec3>();
			}

			auto particleSystemComponent = entity["ParticleSystemComponent"];
			if (particleSystemComponent)
			{
				auto& comp = deserializedEntity.AddComponent<ParticleSystemComponent>();
				auto& prop = comp.Prop;
				prop.LauncherNum = particleSystemComponent["LauncherNum"].as<int>();
				prop.SpawnAreaShape = (ParticleSpawnAreaShape)particleSystemComponent["SpawnAreaShape"].as<int>();
				prop.SpawnPositionDistribution = (ParticleSpawnDistribution)particleSystemComponent["SpawnPositionDistribution"].as<int>();
				prop.SpawnAreaRadius = particleSystemComponent["SpawnAreaRadius"].as<float>();
				prop.MaxVelocity = particleSystemComponent["MaxVelocity"].as<nlm::vec3>();
				prop.MinVelocity = particleSystemComponent["MinVelocity"].as<nlm::vec3>();
				prop.MaxTotalLifetime = particleSystemComponent["MaxTotalLifetime"].as<float>();
				prop.MinTotalLifetime = particleSystemComponent["MinTotalLifetime"].as<float>();
				prop.InitColor = particleSystemComponent["InitColor"].as<nlm::vec4>();
				prop.InitSize = particleSystemComponent["InitSize"].as<float>();
				
				comp.Pass1->LoadShaderAndUpdateProps(particleSystemComponent["Pass1Name"].as<std::string>());
				auto pass1Props = particleSystemComponent["Pass1Props"];
				DeserializeShaderProperties(pass1Props, comp.Pass1->GetShaderPropertiesNotConst(), comp.Pass1);

				comp.Pass2->LoadShaderAndUpdateProps(particleSystemComponent["Pass2Name"].as<std::string>());
				auto pass2Props = particleSystemComponent["Pass2Props"];
				DeserializeShaderProperties(pass2Props, comp.Pass2->GetShaderPropertiesNotConst(), comp.Pass2);

				comp.Init();
			}

			// Always at the bottom!!!!
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

		if (entity.HasComponent<SettingsComponent>())
		{
			out << YAML::Key << "SettingsComponent";
			out << YAML::BeginMap; // SettingsComponent

			auto& comp = entity.GetComponent<SettingsComponent>();
			out << YAML::Key << "AntiAliasingType" << YAML::Value << (int)comp.AntiAliasingType;
			out << YAML::Key << "EditorCameraClearFlag" << YAML::Value << (int)comp.EditorCameraClearFlag;
			out << YAML::Key << "MSAASamples" << YAML::Value << comp.MSAASamples;
			out << YAML::Key << "ShowGizmos" << YAML::Value << comp.ShowGizmos;
			out << YAML::Key << "RenderMode" << YAML::Value << (int)comp.RenderMode;

			out << YAML::EndMap; // SettingsComponent
		}

		if (entity.HasComponent<ModelRendererComponent>())
		{
			out << YAML::Key << "ModelRendererComponent";
			out << YAML::BeginMap; // ModelRendererComponent

			auto& comp = entity.GetComponent<ModelRendererComponent>();
			out << YAML::Key << "ModelPath" << YAML::Value << comp.Path;

			out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq; // Materials
			const auto& mats = comp._Model->GetMaterials();
			for (const auto& item : mats)
			{
				const auto& matName = item.first;
				const auto& mat = item.second;

				out << YAML::BeginMap; // Material

				out << YAML::Key << "MaterialName" << YAML::Value << matName;
				out << YAML::Key << "ShaderName" << YAML::Value << mat->GetShaderName();

				out << YAML::Key << "ShaderProperties" << YAML::Value << YAML::BeginSeq; // ShaderProperties

				SerializeShaderProperties(out, mat);

				out << YAML::EndSeq; // ShaderProperties

				out << YAML::EndMap; // Material
			}
			out << YAML::EndSeq; // Materials


			out << YAML::Key << "Bones" << YAML::Value << YAML::BeginSeq; // Bones
			auto& bones = comp._Model->GetBonesNotConst();
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

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& comp = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << comp.Color;
			out << YAML::Key << "Reaction" << YAML::Value << (int)comp.Reaction;
			out << YAML::Key << "Path" << YAML::Value << comp.Path;

			out << YAML::EndMap; // SpriteRendererComponent
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
			out << YAML::Key << "ClearFlagType" << YAML::Value << (int)cam.GetClearFlagType();

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<PostProcessingComponent>())
		{
			out << YAML::Key << "PostProcessingComponent";
			out << YAML::BeginMap; // PostProcessingComponent

			auto& comp = entity.GetComponent<PostProcessingComponent>();

			out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq; // Materials
			for (auto& mat : comp.Queue)
			{
				out << YAML::BeginMap; // Material

				out << YAML::Key << "ShaderName" << YAML::Value << mat->GetShaderName();

				out << YAML::Key << "ShaderProperties" << YAML::Value << YAML::BeginSeq; // ShaderProperties
				SerializeShaderProperties(out, mat);
				out << YAML::EndSeq; // ShaderProperties

				out << YAML::EndMap; // Material
			}
			out << YAML::EndSeq; // Materials

			out << YAML::EndMap; // PostProcessingComponent
		}

		// Light Component
		if (entity.HasComponent<LightComponent>())
		{
			out << YAML::Key << "LightComponent";
			out << YAML::BeginMap; // LightComponent

			auto& comp = entity.GetComponent<LightComponent>();
			out << YAML::Key << "LightType" << YAML::Value << (int)comp.Type;
			out << YAML::Key << "Color" << YAML::Value << comp.Color;
			out << YAML::Key << "Intensity" << YAML::Value << comp.Intensity;
			out << YAML::Key << "Attenuation" << YAML::Value << comp.Attenuation;

			out << YAML::EndMap; // LightComponent
		}

		// Particle System Component
		if (entity.HasComponent<ParticleSystemComponent>())
		{
			out << YAML::Key << "ParticleSystemComponent";
			out << YAML::BeginMap; // ParticleSystemComponent

			auto& comp = entity.GetComponent<ParticleSystemComponent>();
			auto& prop = comp.Prop;
			out << YAML::Key << "LauncherNum" << YAML::Value << prop.LauncherNum;
			out << YAML::Key << "SpawnAreaShape" << YAML::Value << (int)prop.SpawnAreaShape;
			out << YAML::Key << "SpawnPositionDistribution" << YAML::Value << (int)prop.SpawnPositionDistribution;
			out << YAML::Key << "SpawnAreaRadius" << YAML::Value << prop.SpawnAreaRadius;
			out << YAML::Key << "MaxVelocity" << YAML::Value << prop.MaxVelocity;
			out << YAML::Key << "MinVelocity" << YAML::Value << prop.MinVelocity;
			out << YAML::Key << "MaxTotalLifetime" << YAML::Value << prop.MaxTotalLifetime;
			out << YAML::Key << "MinTotalLifetime" << YAML::Value << prop.MinTotalLifetime;
			out << YAML::Key << "InitColor" << YAML::Value << prop.InitColor;
			out << YAML::Key << "InitSize" << YAML::Value << prop.InitSize;

			out << YAML::Key << "Pass1Name" << YAML::Value << comp.Pass1->GetShaderName();
			out << YAML::Key << "Pass1Props" << YAML::Value << YAML::BeginSeq; // Pass1Props
			SerializeShaderProperties(out, comp.Pass1);
			out << YAML::EndSeq; // Pass1Props
			
			out << YAML::Key << "Pass2Name" << YAML::Value << comp.Pass2->GetShaderName();
			out << YAML::Key << "Pass2Props" << YAML::Value << YAML::BeginSeq; // Pass2Props
			SerializeShaderProperties(out, comp.Pass2);
			out << YAML::EndSeq; // Pass2Props

			out << YAML::EndMap; // LightComponent
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

	void SceneSerializer::SerializeShaderProperties(YAML::Emitter& out, const Ref<Material>& mat)
	{
		for (const auto& prop : mat->GetShaderProperties())
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
			case 2:		// float
				out << YAML::Key << "Value" << YAML::Value << std::get<2>(prop.Value);
				break;
			default:
				break;
			}
			out << YAML::EndMap; // ShaderProperty
		}
	}

	void SceneSerializer::DeserializeShaderProperties(YAML::Node& props, std::vector<ShaderProperty>& properties, Ref<Material>& material)
	{
		std::unordered_map<std::string, ShaderPropValue> oldProps;
		for (auto prop : props)
		{
			ShaderProperty newProp;
			newProp.Type = static_cast<ShaderUniformType>(prop["Type"].as<int>());
			newProp.Name = prop["Name"].as<std::string>();
			switch (newProp.Type)
			{
			case ShaderUniformType::Color3:
				newProp.Value = prop["Value"].as<nlm::vec3>();
				break;
			case ShaderUniformType::Sampler2D:
			{
				newProp.Value = prop["Value"].as<std::string>();
				const std::string& path = std::get<std::string>(newProp.Value);

				Ref<Texture2D> newTex = Library<Texture2D>::GetInstance().Fetch(path);

				material->ReplaceTexture(newProp.Name, newTex);

				newProp.Value = path;

				// Avoid memory leak

				break;
			}
			case ShaderUniformType::Float:
			{
				newProp.Value = prop["Value"].as<float>();
				break;
			}
			default:
				break;
			}

			oldProps.insert(std::make_pair(newProp.Name + std::to_string((int)newProp.Type), newProp.Value));
		}

		for (auto& prop : properties)
		{
			int type = (int)prop.Type;
			std::string key = prop.Name + std::to_string(type);
			if (oldProps.contains(key))
			{
				prop.Value = oldProps[key];
				continue;
			}

			switch (prop.Type)
			{
			case ShaderUniformType::Color3:
				prop.Value = nlm::vec3(1.0, 1.0, 1.0);
				break;

			case ShaderUniformType::Float:
				prop.Value = 0.0f;
				break;

			default:
				break;
			}
		}
	}

}