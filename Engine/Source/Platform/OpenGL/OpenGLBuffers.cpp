#include "nlpch.h"

#include "OpenGLBuffers.h"

#include <glad/glad.h>

namespace NL
{
	// VertexBuffer /////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size, BufferUsage usage)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		GLenum glUsage = GL_STATIC_DRAW;
		switch (usage)
		{
		case NL::BufferUsage::StaticDraw:
			glUsage = GL_STATIC_DRAW;
			break;
		case NL::BufferUsage::DynamicDraw:
			glUsage = GL_DYNAMIC_DRAW;
			break;
		default:
			break;
		}
		glBufferData(GL_ARRAY_BUFFER, size, vertices, glUsage);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<float>& vertices, BufferUsage usage)
		: OpenGLVertexBuffer(vertices.data(), vertices.size(), usage)
	{
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetTransformFeedbackTarget() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_RendererID);
	}

	// IndexBuffer /////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		// Tmp
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t>& indices)
		: OpenGLIndexBuffer(indices.data(), indices.size())
	{
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}