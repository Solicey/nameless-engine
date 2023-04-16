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

		if (model != nullptr && model->HasBones())
		{
			model->CalculateFinalBoneMatrices();
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

		std::vector<std::pair<std::string, std::string>> bonePairs;
		ProcessNode(scene, &identity, scene->mRootNode, meshes, materials, boneMap, bones, bonePairs, entityID);
		if (!bonePairs.empty())
			ProcessBoneHierarchy(boneMap, bones, bonePairs, scene->mRootNode);

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

	void ModelLoader::ProcessNode(const struct aiScene* scene, void* transform, aiNode* node, std::vector<Ref<Mesh>>& meshes, std::unordered_map<std::string, Ref<Material>>& materials, std::unordered_map<std::string, int>& boneMap, std::map<int, BoneInfo>& bones, std::vector<std::pair<std::string, std::string>>& bonePairs, int entityID)
	{
		aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(transform) * node->mTransformation;

		NL_ENGINE_INFO("Mesh count: {0}", node->mNumMeshes);

		// Process all the node's meshes (if any)
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			std::vector<Vertex> vertices;
			std::vector<SkinnedVertex> skinnedVertices;

			std::vector<uint32_t> indices;
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			NL_ENGINE_INFO("Mesh {0} Vertices count: {1}", i, mesh->mNumVertices);

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
					boneInfo.parentID = -1;
					boneInfo.Name = boneName;
					if (aiBone->mNode != nullptr)
					{
						// NL_ENGINE_TRACE("aiBone {0} has mNode", boneName);
						if (aiBone->mNode->mParent != nullptr)
						{
							std::string parentNodeName = std::string(aiBone->mNode->mParent->mName.C_Str());
							// NL_ENGINE_TRACE("  parent node name {0}", parentNodeName);
							if (boneMap.find(parentNodeName) != boneMap.end())
							{
								boneInfo.parentID = boneMap[parentNodeName];
								bones[boneMap[parentNodeName]].Childrens.insert(boneID);
							}
							else
							{
								bonePairs.push_back(std::make_pair(parentNodeName, boneName));
							}
							// NL_ENGINE_TRACE("  parent Id {0}", boneInfo.parentID);
							// boneInfo.ParentTransformation = Utils::ConvertMatrixToNLMFormat(aiBone->mNode->mParent->mTransformation);
						}
						boneInfo.Transformation = aiBone->mNode->mTransformation;
						
					}
					boneInfo.Offset = aiBone->mOffsetMatrix;
					// NL_ENGINE_INFO("bone {0} offset: {1}", boneID, nlm::to_string(boneInfo.Offset));
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

					/*if (weight < 1e-6) continue;
					int minWeightIndex = 0;
					for (int k = 1; k < MAX_BONE_INFLUENCE; k++)
					{
						if (skinnedVertices[vertexID].Weights[k] < skinnedVertices[vertexID].Weights[minWeightIndex])
							minWeightIndex = k;
					}
					if (skinnedVertices[vertexID].Weights[minWeightIndex] < weight)
					{
						skinnedVertices[vertexID].BoneIDs[minWeightIndex] = boneID;
						skinnedVertices[vertexID].Weights[minWeightIndex] = weight;
					}*/

					for (int k = 0; k < MAX_BONE_INFLUENCE; k++)
					{
						NL_ENGINE_ASSERT(vertexID < skinnedVertices.size(), "");
						if (skinnedVertices[vertexID].Weights[k] <= 0)
						{
							skinnedVertices[vertexID].BoneIDs[k] = boneID;
							skinnedVertices[vertexID].Weights[k] = weight;
							break;
						}
					}
				}
			}

			// debug
			/*int id = 4503;
			if (skinnedVertices.size() > id)
			{
				std::cout << "id: " << id << std::endl;
				for (int k = 0; k < 4; k++)
				{
					std::cout << "  boneid: " << skinnedVertices[id].BoneIDs[k] << " boneweight: " << skinnedVertices[id].Weights[k] << std::endl;
				}
				std::cout << "  pos: " << nlm::to_string(skinnedVertices[id].Position) << std::endl;
			}*/
			//

			if (hasBones)
			{
				int cnt = 0;
				int cnt2 = 0;
				for (int v = 0; v < skinnedVertices.size(); v++)
				{
					bool flag = false;
					float add = 0.0f;
					for (int k = 0; k < MAX_BONE_INFLUENCE; k++)
					{
						if (skinnedVertices[v].BoneIDs[k] >= 0)
						{
							flag = true;
						}
						add += skinnedVertices[v].Weights[k];
					}
					if (!flag)
						cnt++;
					if (abs(add - 1.0f) > 0.01f)
						cnt2++;
				}
				NL_ENGINE_WARN("There are {0} vertices that aren't skinned yet!", cnt);
				NL_ENGINE_WARN("There are {0} vertices that are missing weights!", cnt2);
			}

			std::string matName = std::string(scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str());
			materials[matName] = nullptr;
			meshes.push_back(CreateRef<Mesh>(vertices, skinnedVertices, indices, mesh->mMaterialIndex, matName, hasBones)); // The model will handle mesh destruction
		}

		// Then do the same for each of its children
		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(scene, &nodeTransformation, node->mChildren[i], meshes, materials, boneMap, bones, bonePairs, entityID);
		}

		// If it is a tip
		/*if (node->mNumChildren == 0 && node->mParent != nullptr)
		{
			std::string parentNodeName = std::string(node->mParent->mName.C_Str());
			if (boneMap.find(parentNodeName) != boneMap.end())
			{
				BoneInfo boneInfo;
				int boneID = boneInfo.ID = bones.size();
				boneInfo.parentID = boneMap[parentNodeName];
				boneInfo.Name = node->mName.C_Str();
				boneInfo.Transformation = Utils::ConvertMatrixToNLMFormat(node->mTransformation);
				boneMap[boneInfo.Name] = boneID;
				bones[boneID] = boneInfo;
				bones[boneInfo.parentID].Childrens.insert(boneID);
				NL_ENGINE_TRACE("aiBone tip {0}", boneInfo.Name);
			}
		}*/
	}

	void ModelLoader::ProcessMesh(const struct aiScene* scene, void* transform, aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, int entityID)
	{
		// aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(transform);
		aiMatrix4x4 meshTransformation = aiMatrix4x4();

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
		//aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(transform);
		aiMatrix4x4 meshTransformation = aiMatrix4x4();

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
					nlm::ivec4(0, 0, 0, 0),
					nlm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
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

	void ModelLoader::ProcessBoneHierarchy(std::unordered_map<std::string, int>& boneMap, std::map<int, BoneInfo>& bones, std::vector<std::pair<std::string, std::string>>& bonePairs, const aiNode* node)
	{
		for (auto& pair : bonePairs)
		{
			if (boneMap.find(pair.first) != boneMap.end() && boneMap.find(pair.second) != boneMap.end())
			{
				int parentId = boneMap[pair.first];
				int childId = boneMap[pair.second];
				bones[childId].parentID = parentId;
				bones[parentId].Childrens.insert(childId);
			}
		}

		/*for (auto& pair : bones)
		{
			auto& bone = pair.second;
			if (bone.parentID == -1)
			{
				ProcessBonePreRotate(bone.Name, node, bone.RootPreRotation);
			}
		}*/
	}

	void ModelLoader::ProcessBonePreRotate(const std::string& rootName, const aiNode* sceneRoot, aiMatrix4x4& matrix)
	{
		const aiNode* root = sceneRoot->FindNode(aiString(rootName));

		root = root->mParent;
		if (root)
		{
			matrix = root->mTransformation;
			root = root->mParent;
		}

		while (root != nullptr)
		{
			matrix = root->mTransformation * matrix;
			root = root->mParent;
		}

		NL_ENGINE_TRACE("Process bone prerotate! bone: {0}, matrix: {1}", rootName, nlm::to_string(Utils::ConvertMatrixToNLMFormat(matrix)));
	}
}
