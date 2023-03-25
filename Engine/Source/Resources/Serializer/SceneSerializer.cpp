#include "nlpch.h"

#include "SceneSerializer.h"
#include "Core/Log/Log.h"
#include "Core/Math/Math.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"

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
}

namespace NL
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

			auto modelRendererComponent = entity["ModelRendererComponent"];
			if (modelRendererComponent)
			{
				std::string path = modelRendererComponent["ModelPath"].as<std::string>();
				auto& comp = deserializedEntity.AddComponent<ModelRendererComponent>(path, (uint32_t)deserializedEntity);
				auto& materialMap = comp.mModel->GetMaterialsNotConst();

				auto mats = modelRendererComponent["Materials"];
				if (!mats)
					continue;
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

		out << YAML::EndMap; // Entity
	}
}