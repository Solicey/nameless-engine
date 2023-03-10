#pragma once

#include "Resources/Model.h"
#include "Resources/Loaders/ModelLoaderFlags.h"

extern struct aiScene;
extern struct aiNode;
extern struct aiMesh;

namespace NL
{
	class ModelLoader
	{
	public:
		ModelLoader() = delete;

		static Ref<Model> Create(const std::string& path, 
			ModelLoaderFlags flags = ModelLoaderFlags::None);
		
	private:
		static bool AssimpLoadModel(const std::string& path,
			std::vector<Ref<Mesh>>& meshes,
			std::vector<std::string>& materials,
			ModelLoaderFlags flags = ModelLoaderFlags::None);

		static void ProcessMaterials(const struct aiScene* scene,
			std::vector<std::string>& materials);

		static void ProcessNode(const struct aiScene* scene,
			void* transform,
			aiNode* node,
			std::vector<Ref<Mesh>>& meshes);

		static void ProcessMesh(const struct aiScene* scene,
			void* transform,
			aiMesh* mesh,
			std::vector<Vertex>& vertices,
			std::vector<uint32_t>& indices
		);
	};
}