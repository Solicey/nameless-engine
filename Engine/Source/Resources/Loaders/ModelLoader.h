#pragma once

#include "Resources/Model.h"
#include "Resources/Loaders/ModelLoaderFlags.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

namespace NL
{
	class ModelLoader
	{
	public:
		ModelLoader() = delete;

		static Ref<Model> Create(const std::string& path,
			ModelLoaderFlags flags = ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace | ModelLoaderFlags::PopulateArmatureData | ModelLoaderFlags::JoinIdenticalVertices | ModelLoaderFlags::GenSmoothNormals);
		
	private:
		static bool AssimpLoadModel(const std::string& path,
			std::vector<Ref<Mesh>>& meshes,
			std::unordered_map<std::string, Ref<Material>>& materials,
			std::unordered_map<std::string, int>& boneMap,
			std::map<int, BoneInfo>& bones,
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
			const std::string& modelPath);

		static void ProcessMesh(const struct aiScene* scene,
			void* transform,
			aiMesh* mesh,
			std::vector<Vertex>& vertices,
			std::vector<uint32_t>& indices
		);

		static void ProcessMesh(const struct aiScene* scene,
			void* transform,
			aiMesh* mesh,
			std::vector<SkinnedVertex>& vertices,
			std::vector<uint32_t>& indices
		);

		static void ProcessBoneHierarchy(
			std::unordered_map<std::string, int>& boneMap,
			std::map<int, BoneInfo>& bones,
			std::vector<std::pair<std::string, std::string>>& bonePairs,
			const aiNode* node
		);

		static void ProcessBonePreRotate(const std::string& rootName, const aiNode* node, aiMatrix4x4& matrix);
	};
}