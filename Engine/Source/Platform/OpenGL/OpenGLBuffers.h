#pragma once

#include "Renderer/Buffers.h"

namespace NL
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* vertices, uint32_t size, BufferAccessFrequency frequency, BufferAccessNature nature);
		OpenGLVertexBuffer(std::vector<float>& vertices, BufferAccessFrequency frequency, BufferAccessNature nature);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual void BindBufferBase(BindBufferBaseTarget target) override;

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		OpenGLIndexBuffer(std::vector<uint32_t>& indices);

		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLTransformFeedbackBuffer : public TransformFeedbackBuffer
	{
	public:
		OpenGLTransformFeedbackBuffer();

		virtual ~OpenGLTransformFeedbackBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void BindBufferBase(const Ref<VertexBuffer>& buffer) const override;

		virtual void Begin(TransformFeedbackPrimitiveMode primitiveMode) const override;
		virtual void End() const override;

		virtual void Draw(PrimitiveMode mode) const override;

	private:
		uint32_t m_RendererID;
	};
}