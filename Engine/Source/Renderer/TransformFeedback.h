#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Renderer/Buffers.h"

namespace NL
{
	class TransformFeedback
	{
	public:
		virtual ~TransformFeedback() {}

		virtual void BindBuffer() const = 0;
		virtual void UnbindBuffer() const = 0;

		virtual void BindTransformFeedback() const = 0;
		virtual void UnbindTransformFeedback() const = 0;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;

		static Ref<TransformFeedback> Create();
	};
}