#include "nlpch.h"

#include "OpenGLBuffers.h"

#include <glad/glad.h>

namespace NL
{
	namespace Utils
	{
		static GLenum BindBufferBaseTarget2OpenGLTarget(BindBufferBaseTarget target)
		{
			switch (target)
			{
			case NL::BindBufferBaseTarget::AtomicCounterBuffer:
				return GL_ATOMIC_COUNTER_BUFFER;
			case NL::BindBufferBaseTarget::TransformFeedbackBuffer:
				return GL_TRANSFORM_FEEDBACK_BUFFER;
			case NL::BindBufferBaseTarget::UniformBuffer:
				return GL_UNIFORM_BUFFER;
			case NL::BindBufferBaseTarget::ShaderStorageBuffer:
				return GL_SHADER_STORAGE_BUFFER;
			default:
				break;
			}
			return GL_NONE;
		}

		static GLenum TransformFeedbackPrimitiveMode2OpenGLPrimitiveMode(TransformFeedbackPrimitiveMode mode)
		{
			switch (mode)
			{
			case NL::TransformFeedbackPrimitiveMode::Points:
				return GL_POINTS;
			case NL::TransformFeedbackPrimitiveMode::Lines:
				return GL_LINES;
			case NL::TransformFeedbackPrimitiveMode::Triangles:
				return GL_TRIANGLES;
			default:
				break;
			}
		}

		static GLenum PrimitiveMode2OpenGLPrimitiveMode(PrimitiveMode mode)
		{
			switch (mode)
			{
			case NL::PrimitiveMode::Points:
				return GL_POINTS;
			case NL::PrimitiveMode::LineStrip:
				return GL_LINE_STRIP;
			case NL::PrimitiveMode::LineLoop:
				return GL_LINE_LOOP;
			case NL::PrimitiveMode::Lines:
				return GL_LINES;
			case NL::PrimitiveMode::LineStripAdjacency:
				return GL_LINE_STRIP_ADJACENCY;
			case NL::PrimitiveMode::LinesAdjacency:
				return GL_LINES_ADJACENCY;
			case NL::PrimitiveMode::TriangleStrip:
				return GL_TRIANGLE_STRIP;
			case NL::PrimitiveMode::TriangleFan:
				return GL_TRIANGLE_FAN;
			case NL::PrimitiveMode::Triangles:
				return GL_TRIANGLES;
			case NL::PrimitiveMode::TriangleStripAdjacency:
				return GL_TRIANGLE_STRIP_ADJACENCY;
			case NL::PrimitiveMode::TrianglesAdjacency:
				return GL_TRIANGLES_ADJACENCY;
			case NL::PrimitiveMode::Patches:
				return GL_PATCHES;
			default:
				break;
			}
			return GL_POINTS;
		}

		static GLenum BufferAccessFreqAndNature2OpenGLEnums(BufferAccessFrequency frequency, BufferAccessNature nature)
		{
			switch (frequency)
			{
			case NL::BufferAccessFrequency::Stream:
			{
				switch (nature)
				{
				case NL::BufferAccessNature::Draw:
					return GL_STREAM_DRAW;
				case NL::BufferAccessNature::Read:
					return GL_STREAM_READ;
				case NL::BufferAccessNature::Copy:
					return GL_STREAM_COPY;
				default:
					break;
				}
			}
			case NL::BufferAccessFrequency::Static:
			{
				switch (nature)
				{
				case NL::BufferAccessNature::Draw:
					return GL_STATIC_DRAW;
				case NL::BufferAccessNature::Read:
					return GL_STATIC_READ;
				case NL::BufferAccessNature::Copy:
					return GL_STATIC_COPY;
				default:
					break;
				}
			}
			case NL::BufferAccessFrequency::Dynamic:
			{
				switch (nature)
				{
				case NL::BufferAccessNature::Draw:
					return GL_DYNAMIC_DRAW;
				case NL::BufferAccessNature::Read:
					return GL_DYNAMIC_READ;
				case NL::BufferAccessNature::Copy:
					return GL_DYNAMIC_COPY;
				default:
					break;
				}
			}
			default:
				break;
			}
		}
	}

	// VertexBuffer /////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size, BufferAccessFrequency frequency, BufferAccessNature nature)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, Utils::BufferAccessFreqAndNature2OpenGLEnums(frequency, nature));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<float>& vertices, BufferAccessFrequency frequency, BufferAccessNature nature)
		: OpenGLVertexBuffer(vertices.data(), vertices.size(), frequency, nature)
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

	void OpenGLVertexBuffer::BindBufferBase(BindBufferBaseTarget target)
	{
		glBindBufferBase(Utils::BindBufferBaseTarget2OpenGLTarget(target), 0, m_RendererID);
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

	OpenGLTransformFeedbackBuffer::OpenGLTransformFeedbackBuffer()
	{
		glCreateTransformFeedbacks(1, &m_RendererID);
	}

	OpenGLTransformFeedbackBuffer::~OpenGLTransformFeedbackBuffer()
	{
		glDeleteTransformFeedbacks(1, &m_RendererID);
	}

	void OpenGLTransformFeedbackBuffer::Bind() const
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_RendererID);
	}

	void OpenGLTransformFeedbackBuffer::Unbind() const
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	}

	void OpenGLTransformFeedbackBuffer::BindBufferBase(const Ref<VertexBuffer>& buffer) const
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_RendererID);
		buffer->BindBufferBase(BindBufferBaseTarget::TransformFeedbackBuffer);
	}

	void OpenGLTransformFeedbackBuffer::Begin(TransformFeedbackPrimitiveMode primitiveMode) const
	{
		glBeginTransformFeedback(Utils::TransformFeedbackPrimitiveMode2OpenGLPrimitiveMode(primitiveMode));
	}

	void OpenGLTransformFeedbackBuffer::End() const
	{
		glEndTransformFeedback();
	}

	void OpenGLTransformFeedbackBuffer::Draw(PrimitiveMode mode) const
	{
		glDrawTransformFeedback(Utils::PrimitiveMode2OpenGLPrimitiveMode(mode), m_RendererID);
	}
}