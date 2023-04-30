#pragma once

#include "Renderer/TransformFeedback.h"
#include "Renderer/VertexArray.h"

namespace NL
{
	class OpenGLTransformFeedback : public TransformFeedback
	{
	public:
		OpenGLTransformFeedback();
		virtual ~OpenGLTransformFeedback() override;

		virtual void BindBuffer() const override;
		virtual void UnbindBuffer() const override;

		virtual void BindTransformFeedback() const override;
		virtual void UnbindTransformFeedback() const override;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;

		virtual void Draw_Points() const override;

	private:
		Ref<VertexArray> m_VertexArray;
		uint32_t m_RendererID;
	};
}