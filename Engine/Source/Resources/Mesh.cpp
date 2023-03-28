#include "nlpch.h"

#include "Mesh.h"

namespace NL
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<SkinnedVertex>& skinnedVertices, const std::vector<uint32_t>& indices, uint32_t matIndex, const std::string& name, bool hasBones) :
		m_VertexCount(static_cast<uint32_t>(vertices.size())),
		m_IndexCount(static_cast<uint32_t>(indices.size())),
		m_MaterialIndex(matIndex),
		m_MaterialName(name),
		m_HasBones(hasBones)
	{
		if (m_HasBones)
			CreateBuffers(skinnedVertices, indices);
		else
			CreateBuffers(vertices, indices);

		// BoundingSphere
	}

	void Mesh::Bind()
	{
		m_VertexArray->Bind();
	}
	
	void Mesh::Unbind()
	{
		m_VertexArray->Unbind();
	}

	uint32_t Mesh::GetVertexCount() const
	{
		return m_VertexCount;
	}

	uint32_t Mesh::GetIndexCount() const
	{
		return m_IndexCount;
	}

	uint32_t Mesh::GetMaterialIndex() const
	{
		return m_MaterialIndex;
	}

	void Mesh::CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		// std::vector<float> rawVertices;
		// std::vector<unsigned int> rawIndices;

		/*for (const auto& vertex : vertices)
		{
			rawVertices.push_back(vertex.Position.x);
			rawVertices.push_back(vertex.Position.y);
			rawVertices.push_back(vertex.Position.z);

			rawVertices.push_back(vertex.TexCoord.x);
			rawVertices.push_back(vertex.TexCoord.y);

			rawVertices.push_back(vertex.Normal.x);
			rawVertices.push_back(vertex.Normal.y);
			rawVertices.push_back(vertex.Normal.z);

			rawVertices.push_back(vertex.Tangent.x);
			rawVertices.push_back(vertex.Tangent.y);
			rawVertices.push_back(vertex.Tangent.z);

			rawVertices.push_back(vertex.Bitangent.x);
			rawVertices.push_back(vertex.Bitangent.y);
			rawVertices.push_back(vertex.Bitangent.z);

			// NL_ENGINE_TRACE("    vertex pos: ({0}, {1}, {2})", vertex.Position.x, vertex.Position.y, vertex.Position.z);
		}*/

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float3, "a_Tangent"},
			{ShaderDataType::Float3, "a_Bitangent"},
			{ShaderDataType::Int, "a_EntityID"}
		};

		m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create((void*)const_cast<Vertex*>(vertices.data()), vertices.size() * layout.GetStride());
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		/*for (int i = 0; i < indices.size(); i += 3)
		{
			NL_ENGINE_TRACE("    index: ({0}, {1}, {2})", indices[i], indices[i + 1], indices[i + 2]);
		}*/

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(const_cast<uint32_t*>(indices.data()), indices.size());
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// NL_ENGINE_TRACE("    indices counts: {0}", indexBuffer->GetCount());

	}

	void Mesh::CreateBuffers(const std::vector<SkinnedVertex>& vertices, const std::vector<uint32_t>& indices)
	{
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float3, "a_Tangent"},
			{ShaderDataType::Float3, "a_Bitangent"},
			{ShaderDataType::Int, "a_EntityID"},
			{ShaderDataType::Int4, "a_BoneIndex"},
			{ShaderDataType::Float4, "a_BoneWeight"}
		};

		m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create((void*)const_cast<SkinnedVertex*>(vertices.data()), vertices.size() * layout.GetStride());
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		/*for (int i = 0; i < indices.size(); i += 3)
		{
			NL_ENGINE_TRACE("    index: ({0}, {1}, {2})", indices[i], indices[i + 1], indices[i + 2]);
		}*/

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(const_cast<uint32_t*>(indices.data()), indices.size());
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}
}
