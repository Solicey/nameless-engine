#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Resources/Model.h"
#include "Resources/Loaders/ModelLoader.h"

namespace NL
{
	class ModelRendererComponent : public ComponentBase
	{
	public:
		ModelRendererComponent() : Path("") {}
		ModelRendererComponent(const std::string& path, uint32_t entityID, ModelLoaderFlags flags)
			: Path(path), mModel(ModelLoader::Create(path, entityID, flags)) {}


	public:
		std::string Path = "";
		Ref<Model> mModel = nullptr;
	};
}