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
			uint32_t entityID = -1,
			ModelLoaderFlags flags = ModelLoaderFlags::None);
		
	private:
		static bool AssimpLoadModel(const std::string& path,
			std::vector<Ref<Mesh>>& meshes,
			std::vector<std::string>& materials,
			uint32_t entityID,
			ModelLoaderFlags flags = ModelLoaderFlags::None);

		static void ProcessMaterials(const struct aiScene* scene,
			std::vector<std::string>& materials);

		static void ProcessNode(const struct aiScene* scene,
			void* transform,
			aiNode* node,
			std::vector<Ref<Mesh>>& meshes, 
			uint32_t entityID);

		static void ProcessMesh(const struct aiScene* scene,
			void* transform,
			aiMesh* mesh,
			std::vector<Vertex>& vertices,
			std::vector<uint32_t>& indices,
			uint32_t entityID
		);
	};
}