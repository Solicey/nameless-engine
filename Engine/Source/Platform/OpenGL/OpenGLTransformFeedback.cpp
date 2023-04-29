#include "nlpch.h"

#include "OpenGLTransformFeedback.h"
#include "Resources/ShaderDataType.h"
#include <glad/glad.h>

namespace NL
{
	namespace Utils
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
	}

	OpenGLTransformFeedback::OpenGLTransformFeedback()
	{
		glCreateTransformFeedbacks(1, &m_RendererID);
	}

	OpenGLTransformFeedback::~OpenGLTransformFeedback()
	{
		glDeleteTransformFeedbacks(1, &m_RendererID);
	}

	void OpenGLTransformFeedback::BindBuffer() const
	{
		if (m_VertexArray != nullptr)
			m_VertexArray->Bind();
	}

	void OpenGLTransformFeedback::UnbindBuffer() const
	{
		if (m_VertexArray != nullptr)
			m_VertexArray->Unbind();
	}

	void OpenGLTransformFeedback::BindTransformFeedback() const
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_RendererID);
	}

	void OpenGLTransformFeedback::UnbindTransformFeedback() const
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	}

	void OpenGLTransformFeedback::SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		if (m_VertexArray != nullptr)
			m_VertexArray.reset();

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Bind vbo to this tbo
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_RendererID);
		vertexBuffer->SetTransformFeedbackTarget();

	}
}