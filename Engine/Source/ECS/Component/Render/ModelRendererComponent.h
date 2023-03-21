#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Core/Log/Log.h"
#include "Resources/Model.h"
#include "Resources/Loaders/ModelLoader.h"

#include <regex>

namespace NL
{
	class ModelRendererComponent : public ComponentBase
	{
	public:
		ModelRendererComponent() {}
		ModelRendererComponent(const std::string& path, int entityID, ModelLoaderFlags flags)
			: Path(std::regex_replace(path, std::regex("\\\\"), "/")), Flags(flags), mModel(ModelLoader::Create(path, entityID, flags))
		{
			// NL_ENGINE_TRACE("Entity entt id: {0}", entityID);
		}

	public:
		// turn all "\" into "/" with std::regex_replace(Path, std::regex("\\\\"), "/");
		std::string Path = "";
		Ref<Model> mModel = nullptr;
		ModelLoaderFlags Flags = ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace;
	};
}