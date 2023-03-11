#pragma once

#include "Renderer/Buffers.h"
#include "Core/Misc/PtrWrapper.h"

namespace NL
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		/// <summary>
		/// ÃÌº”VBO
		/// </summary>
		/// <param name="vertexBuffer"></param>
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;

		/// <summary>
		/// …Ë÷√IBO
		/// </summary>
		/// <param name="indexBuffer"></param>
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
}