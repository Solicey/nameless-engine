#pragma once

#include "Renderer/Buffers.h"
#include "Renderer/VertexArray.h"
#include "Resources/Vertex.h"

namespace NL
{
	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
			uint32_t matIndex);

		// VAO actually
		void Bind();
		
		// VAO actually
		void Unbind();

		uint32_t GetVertexCount() const;
		
		uint32_t GetIndexCount() const;
		
		uint32_t GetMaterialIndex() const;

	private:
		void CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	private:
		uint32_t m_VertexCount;
		uint32_t m_IndexCount;
		uint32_t m_MaterialIndex;

		// includes buffers already
		Ref<VertexArray> m_VertexArray;

		// bounding sphere
	};
}