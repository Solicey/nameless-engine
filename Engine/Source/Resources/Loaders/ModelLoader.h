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
			int entityID = -1,
			ModelLoaderFlags flags = ModelLoaderFlags::None);
		
	private:
		static bool AssimpLoadModel(const std::string& path,
			std::vector<Ref<Mesh>>& meshes,
			std::unordered_map<std::string, Ref<Material>>& materials,
			std::unordered_map<std::string, int>& boneMap,
			std::map<int, BoneInfo>& bones,
			int entityID,
			ModelLoaderFlags flags = ModelLoaderFlags::None);

		static void ProcessMaterials(const std::string& path, const struct aiScene* scene,
			std::unordered_map<std::string, Ref<Material>>& materials);

		static void ProcessNode(const struct aiScene* scene,
			void* transform,
			aiNode* node,
			std::vector<Ref<Mesh>>& meshes, 
			std::unordered_map<std::string, Ref<Material>>& materials,
			std::unordered_map<std::string, int>& boneMap,
			std::map<int, BoneInfo>& bones,
			std::vector<std::pair<std::string, std::string>>& bonePairs,
			int entityID);

		static void ProcessMesh(const struct aiScene* scene,
			void* transform,
			aiMesh* mesh,
			std::vector<Vertex>& vertices,
			std::vector<uint32_t>& indices,
			int entityID
		);

		static void ProcessMesh(const struct aiScene* scene,
			void* transform,
			aiMesh* mesh,
			std::vector<SkinnedVertex>& vertices,
			std::vector<uint32_t>& indices,
			int entityID
		);

		static void ProcessBoneHierarchy(
			std::unordered_map<std::string, int>& boneMap,
			std::map<int, BoneInfo>& bones,
			std::vector<std::pair<std::string, std::string>>& bonePairs
		);
	};
}