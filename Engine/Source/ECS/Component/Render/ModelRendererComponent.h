#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Resources/Model.h"
#include "Resources/Loaders/ModelLoader.h"

namespace NL
{
	class ModelRendererComponent : public ComponentBase
	{
	public:
		ModelRendererComponent() = default;
		ModelRendererComponent(const std::string& path, ModelLoaderFlags flags)
			: Path(path), mModel(ModelLoader::Create(path, flags)) {}


	public:
		std::string Path = "";
		Ref<Model> mModel = nullptr;
	};
}