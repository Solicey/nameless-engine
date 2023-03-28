#include "nlpch.h"

#include "OpenGLVertexArray.h"
#include "Resources/ShaderDataType.h"

#include <glad/glad.h>

namespace NL
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case NL::ShaderDataType::Float:		return GL_FLOAT;
		case NL::ShaderDataType::Float2:	return GL_FLOAT;
		case NL::ShaderDataType::Float3:	return GL_FLOAT;
		case NL::ShaderDataType::Float4:	return GL_FLOAT;
		case NL::ShaderDataType::Mat3:		return GL_FLOAT;
		case NL::ShaderDataType::Mat4:		return GL_FLOAT;
		case NL::ShaderDataType::Int:		return GL_INT;
		case NL::ShaderDataType::Int2:		return GL_INT;
		case NL::ShaderDataType::Int3:		return GL_INT;
		case NL::ShaderDataType::Int4:		return GL_INT;
		case NL::ShaderDataType::Bool:		return GL_BOOL;
		}

		NL_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();

		NL_ENGINE_ASSERT(layout.GetElements().size(), "Vertex Buffer has no layout!");

		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			if (element.Type == ShaderDataType::Int || element.Type == ShaderDataType::Int2 || element.Type == ShaderDataType::Int3 || element.Type == ShaderDataType::Int4)
				glVertexAttribIPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), layout.GetStride(), (const void*)element.Offset);
			else
				glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}