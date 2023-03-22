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

	private:
		Ref<Scene> m_Scene;
	};
}