#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Core/Log/Log.h"
#include "Resources/Model.h"
#include "Resources/Loaders/ModelLoader.h"

namespace NL
{
	class ModelRendererComponent : public ComponentBase
	{
	public:
		ModelRendererComponent() : Path("") {}
		ModelRendererComponent(const std::string& path, int entityID, ModelLoaderFlags flags)
			: Path(path), mModel(ModelLoader::Create(path, entityID, flags))
		{
			// NL_ENGINE_TRACE("Entity entt id: {0}", entityID);
		}


	public:
		std::string Path = "";
		Ref<Model> mModel = nullptr;
	};
}