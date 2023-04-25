#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Core/Log/Log.h"
#include "ECS/Component/Component.h"
#include "Resources/Model.h"
#include "Resources/Loaders/ModelLoader.h"

#include <regex>

namespace NL
{
	class ModelRendererComponent : public ComponentBase
	{
	public:
		ModelRendererComponent() {}
		ModelRendererComponent(const ModelRendererComponent&) = default;

		ModelRendererComponent(const std::string& path, ModelLoaderFlags flags = ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace | ModelLoaderFlags::PopulateArmatureData | ModelLoaderFlags::JoinIdenticalVertices | ModelLoaderFlags::GenSmoothNormals)
			: Path(std::regex_replace(path, std::regex("\\\\"), "/"))
		{
			mModel = ModelLoader::Create(path, flags);
		}

		void RotateBone(int boneId, nlm::vec3* eulerAngles);

		void RecalculateFinalBoneMatrices();

	public:
		// turn all "\" into "/" with std::regex_replace(Path, std::regex("\\\\"), "/");
		std::string Path = "";
		Ref<Model> mModel = nullptr;

	};
}