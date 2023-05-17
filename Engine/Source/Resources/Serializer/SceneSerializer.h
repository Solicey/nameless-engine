#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "ECS/Scene/Scene.h"

#include <yaml-cpp/yaml.h>
#include <variant>

namespace NL
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene) {}

		void Serialize(const std::string& path);
		bool Deserialize(const std::string& path);

	private:
		void SerializeEntity(YAML::Emitter& out, Entity entity);
		// Call BeginSeq outside
		void SerializeShaderProperties(YAML::Emitter& out, const Ref<Material>& material);
		void DeserializeShaderProperties(YAML::Node& props, std::vector<ShaderProperty>& properties, Ref<Material>& material);

	private:
		Ref<Scene> m_Scene;
	};
}