#include "nlpch.h"

#include "ModelLoader.h"

#include "Core/Math/Math.h"
#include "Core/Log/Log.h"
#include "Resources/Libraries/TextureLibrary.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>
#include <regex>

namespace NL
{
	namespace Utils
	{
		// Credit to HEngine
		static nlm::mat4 ConvertMatrixToNLMFormat(const aiMatrix4x4& from)
		{
			nlm::mat4 to;
			//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
			to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
			to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
			to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
			return to;
		}
	}

	Ref<Model> ModelLoader::Create(const std::string& path, int entityID, ModelLoaderFlags flags)
	{
		NL_ENGINE_INFO("Before regex: {0}", path);
		std::string normPath = std::regex_replace(path, std::regex("\\\\"), "/");
		NL_ENGINE_INFO("After regex: {0}", normPath);

		Ref<Model> model = CreateRef<Model>(Model(normPath));

		if (!AssimpLoadModel(normPath, model->m_Meshes, model->m_Materials, model->m_BoneMap, model->m_Bones, entityID, flags))
		{
			NL_ENGINE_WARN("Failed to load model path: {0}", normPath);
			return nullptr;
		}

		NL_ENGINE_INFO("Model loaded successfully: {0}", normPath);

		model->DebugPrintModelInfo();

		return model;
	}

	bool ModelLoader::AssimpLoadModel(const std::string& path,
		std::vector<Ref<Mesh>>& meshes,
		std::unordered_map<std::string, Ref<Material>>& materials,
		std::unordered_map<std::string, int>& boneMap,
		std::map<int, BoneInfo>& bones,
		int entityID,
		ModelLoaderFlags flags)
	{

		Assimp::Importer importer;
		const struct aiScene* scene = importer.ReadFile(path, static_cast<unsigned int>(flags));

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			return false;

		aiMatrix4x4 identity;

		ProcessNode(scene, &identity, scene->mRootNode, meshes, materials, boneMap, bones, entityID);

		ProcessMaterials(path, scene, materials);

		return true;
	}

	void ModelLoader::ProcessMaterials(const std::string& path, const struct aiScene* scene, std::unordered_map<std::string, Ref<Material>>& materials)
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
					Ref<Material> mat = CreateRef<Material>();
					materials[matName] = mat;

					const auto& loadTexture = [&](aiTextureType type)
					{
						for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
						{
							aiString str;
							material->GetTexture(type, i, &str);

							std::string texName = std::string(str.C_Str());
							std::string texPath = path;
							NL_ENGINE_INFO("Model path: {0}", path);
							texPath.replace(path.find_last_of("/") + 1, path.length(), texName);

							NL_ENGINE_INFO("Load Texture: {0}, Type: {1}", texPath, type);

							Ref<Texture2D> tex;

							if (Library<Texture2D>::GetInstance().Contains(texPath))
							{
								tex = Library<Texture2D>::GetInstance().Get(texPath);
							}
							else
							{
								tex = Texture2D::Create(texPath);
								Library<Texture2D>::GetInstance().Add(texPath, tex);
							}

							switch (type)
							{
							case aiTextureType_NONE:
								break;
							case aiTextureType_DIFFUSE:
								mat->AddTexture(TextureType::Diffuse, tex);
								break;
							case aiTextureType_SPECULAR:
								mat->AddTexture(TextureType::Specular, tex);
								break;
							case aiTextureType_AMBIENT:
								mat->AddTexture(TextureType::Ambient, tex);
								break;
							case aiTextureType_EMISSIVE:
								break;
							case aiTextureType_HEIGHT:
								mat->AddTexture(TextureType::Height, tex);
								break;
							case aiTextureType_NORMALS:
								mat->AddTexture(TextureType::Normals, tex);
								break;
							case aiTextureType_SHININESS:
								break;
							case aiTextureType_OPACITY:
								break;
							case aiTextureType_DISPLACEMENT:
								break;
							case aiTextureType_LIGHTMAP:
								break;
							case aiTextureType_REFLECTION:
								break;
							case aiTextureType_BASE_COLOR:
								mat->AddTexture(TextureType::Diffuse, tex);
								break;
							case aiTextureType_NORMAL_CAMERA:
								mat->AddTexture(TextureType::Normals, tex);
								break;
							case aiTextureType_EMISSION_COLOR:
								break;
							case aiTextureType_METALNESS:
								break;
							case aiTextureType_DIFFUSE_ROUGHNESS:
								break;
							case aiTextureType_AMBIENT_OCCLUSION:
								break;
							case aiTextureType_SHEEN:
								break;
							case aiTextureType_CLEARCOAT:
								break;
							case aiTextureType_TRANSMISSION:
								break;
							case aiTextureType_UNKNOWN:
								break;
							case _aiTextureType_Force32Bit:
								break;
							default:
								break;
							}

							// Hold One Only
							if (tex)
								break;
						}
					};

					// Load Textures
					for (uint32_t type = aiTextureType_NONE; type <= aiTextureType_AMBIENT_OCCLUSION; type++)
					{
						loadTexture(static_cast<aiTextureType>(type));
					}

					// After loading textures
					mat->UpdateSampler2DinProperties();
				}
			}
		}
	}

	void ModelLoader::ProcessNode(const struct aiScene* scene, void* transform, aiNode* node, std::vector<Ref<Mesh>>& meshes, std::unordered_map<std::string, Ref<Material>>& materials, std::unordered_map<std::string, int>& boneMap,
		std::map<int, BoneInfo>& bones, int entityID)
	{
		aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(transform) * node->mTransformation;

		// Process all the node's meshes (if any)
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			std::vector<Vertex> vertices;
			std::vector<SkinnedVertex> skinnedVertices;

			std::vector<uint32_t> indices;
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			bool hasBones = false;
			if (mesh->mNumBones > 0)
			{
				hasBones = true;
				ProcessMesh(scene, &nodeTransformation, mesh, skinnedVertices, indices, entityID);
			}
			else
			{
				ProcessMesh(scene, &nodeTransformation, mesh, vertices, indices, entityID);
			}

			for (uint32_t j = 0; j < mesh->mNumBones; j++)
			{
				const auto& aiBone = mesh->mBones[j];
				std::string boneName = aiBone->mName.C_Str();
				int boneID = -1;
				if (boneMap.find(boneName) == boneMap.end())
				{
					BoneInfo boneInfo;
					boneID = boneInfo.ID = bones.size();
					boneInfo.Offset = Utils::ConvertMatrixToNLMFormat(aiBone->mOffsetMatrix);
					boneMap[boneName] = boneID;
					bones[boneID] = boneInfo;
				}
				else
				{
					boneID = boneMap[boneName];
				}
				NL_ENGINE_ASSERT(boneID != -1, "");

				const auto& weights = mesh->mBones[j]->mWeights;
				int numWeights = mesh->mBones[j]->mNumWeights;

				for (int v = 0; v < numWeights; v++)
				{
					int vertexID = weights[v].mVertexId;
					float weight = weights[v].mWeight;

					for (int k = 0; k < MAX_BONE_INFLUENCE; k++)
					{
						NL_ENGINE_ASSERT(vertexID < skinnedVertices.size(), "");
						if (skinnedVertices[vertexID].BoneIDs[k] < 0)
						{
							skinnedVertices[vertexID].BoneIDs[k] = boneID;
							skinnedVertices[vertexID].Weights[k] = weight;
						}
					}
				}
			}

			std::string matName = std::string(scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str());
			materials[matName] = nullptr;
			meshes.push_back(CreateRef<Mesh>(vertices, skinnedVertices, indices, mesh->mMaterialIndex, matName, hasBones)); // The model will handle mesh destruction
		}

		// Then do the same for each of its children
		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(scene, &nodeTransformation, node->mChildren[i], meshes, materials, boneMap, bones, entityID);
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

	void ModelLoader::ProcessMesh(const struct aiScene* scene, void* transform, aiMesh* mesh, std::vector<SkinnedVertex>& vertices, std::vector<uint32_t>& indices, int entityID)
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
					entityID,
					{-1, -1, -1, -1},
					{0, 0, 0, 0}
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
