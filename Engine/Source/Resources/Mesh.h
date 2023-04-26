#pragma once

#include "Renderer/Buffers.h"
#include "Renderer/VertexArray.h"
#include "Resources/Vertex.h"

namespace NL
{
	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, 
			const std::vector<SkinnedVertex>& skinnedVertices,
			const std::vector<uint32_t>& indices,
			uint32_t matIndex, 
			const std::string& modelPath,
			const std::string& name,
			bool hasBones = false);

		// VAO actually
		void Bind();
		
		// VAO actually
		void Unbind();

		uint32_t GetVertexCount() const;
		
		uint32_t GetIndexCount() const;
		
		uint32_t GetMaterialIndex() const;

		inline const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

		const std::string& GetModelPath() const { return m_ModelPath; }
		const std::string& GetMeshName() const { return m_MeshName; }

	private:
		void CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		void CreateBuffers(const std::vector<SkinnedVertex>& skinnedVertices, const std::vector<uint32_t>& indices);

	private:
		uint32_t m_VertexCount;
		uint32_t m_IndexCount;
		uint32_t m_MaterialIndex;

		// includes buffers already
		Ref<VertexArray> m_VertexArray;

		std::string m_ModelPath;
		std::string m_MeshName;
		bool m_HasBones;

		// bounding sphere
	};
}