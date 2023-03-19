#include "nlpch.h"

#include "ModelLoader.h"

#include "Core/Math/Math.h"
#include "Core/Log/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

namespace NL
{
	Ref<Model> ModelLoader::Create(const std::string& path, int entityID, ModelLoaderFlags flags)
	{
		Ref<Model> model = CreateRef<Model>(Model(path));

		if (!AssimpLoadModel(path, model->m_Meshes, model->m_Materials, entityID, flags))
		{
			NL_ENGINE_WARN("Failed to load model path: {0}", path);
			return nullptr;
		}

		NL_ENGINE_INFO("Model loaded successfully: {0}", path);

		model->DebugPrintModelInfo();

		return model;
	}

	bool ModelLoader::AssimpLoadModel(const std::string& path,
		std::vector<Ref<Mesh>>& meshes,
		std::unordered_map<std::string, Ref<Material>>& materials,
		int entityID,
		ModelLoaderFlags flags)
	{

		Assimp::Importer importer;
		const struct aiScene* scene = importer.ReadFile(path, static_cast<unsigned int>(flags));

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			return false;

		aiMatrix4x4 identity;

		ProcessNode(scene, &identity, scene->mRootNode, meshes, materials, entityID);

		ProcessMaterials(scene, materials);

		return true;
	}

	void ModelLoader::ProcessMaterials(const struct aiScene* scene, std::unordered_map<std::string, Ref<Material>>& materials)
	{
		for (uint32_t i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];
			if (material)
			{
				aiString name;
				aiGetMaterialString(material, AI_MATKEY_NAME, &name);
				std::string matName = std::string(name.C_Str());
				if (materials.contains(matName))
				{
					materials[matName] = CreateRef<Material>();
				}
			}
		}
	}

	void ModelLoader::ProcessNode(const struct aiScene* scene, void* transform, aiNode* node, std::vector<Ref<Mesh>>& meshes, std::unordered_map<std::string, Ref<Material>>& materials, int entityID)
	{
		aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(transform) * node->mTransformation;

		// Process all the node's meshes (if any)
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(scene, &nodeTransformation, mesh, vertices, indices, entityID);
			std::string matName = std::string(scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str());
			materials[matName] = nullptr;
			meshes.push_back(CreateRef<Mesh>(vertices, indices, mesh->mMaterialIndex, matName)); // The model will handle mesh destruction
		}

		// Then do the same for each of its children
		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(scene, &nodeTransformation, node->mChildren[i], meshes, materials, entityID);
		}
	}

	void ModelLoader::ProcessMesh(const struct aiScene* scene, void* transform, aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, int entityID)
	{
		aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(transform);

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
		{
			aiVector3D position = meshTransformation * mesh->mVertices[i];
			aiVector3D normal = meshTransformation * (mesh->mNormals ? mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
			aiVector3D texCoords = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
			aiVector3D tangent = mesh->mTangents ? meshTransformation * mesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
			aiVector3D bitangent = mesh->mBitangents ? meshTransformation * mesh->mBitangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);

			vertices.push_back
			(
				{
					nlm::vec3(position.x, position.y, position.z),
					nlm::vec2(texCoords.x, texCoords.y),
					nlm::vec3(normal.x, normal.y, normal.z),
					nlm::vec3(tangent.x, tangent.y, tangent.z),
					nlm::vec3(bitangent.x, bitangent.y, bitangent.z),
					entityID
				}
			);

			// NL_ENGINE_TRACE("entity entt id: {0} at ProcessMesh", entityID);
		}

		for (uint32_t faceID = 0; faceID < mesh->mNumFaces; ++faceID)
		{
			auto& face = mesh->mFaces[faceID];

			for (size_t indexID = 0; indexID < 3; ++indexID)
				indices.push_back(face.mIndices[indexID]);
		}

	}
}
