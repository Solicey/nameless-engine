#pragma once

/*
该文件声明VBO与IBO(EBO)
FBO在Framebuffer.h
*/

#include "Resources/ShaderDataType.h"

#include "Core/Log/Log.h"

#include <string>

namespace NL
{
	enum class BindBufferBaseTarget
	{
		AtomicCounterBuffer,
		TransformFeedbackBuffer,
		UniformBuffer,
		ShaderStorageBuffer
	};

	enum class TransformFeedbackPrimitiveMode
	{
		Points,
		Lines,
		Triangles
	};

	enum class PrimitiveMode
	{
		Points,
		LineStrip, LineLoop, Lines, LineStripAdjacency, LinesAdjacency,
		TriangleStrip, TriangleFan, Triangles, TriangleStripAdjacency, TrianglesAdjacency,
		Patches
	};

	enum class BufferAccessFrequency
	{
		Stream, Static, Dynamic
	};

	enum class BufferAccessNature
	{
		Draw, Read, Copy
	};

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset = 0;
		bool Normalized;

		BufferElement() {}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(Type)), Normalized(normalized) {}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 9;
			case ShaderDataType::Mat4:		return 16;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
			}

			NL_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

	};

	/// <summary>
	/// 缓冲对象的布局
	/// </summary>
	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	/// <summary>
	/// 顶点缓冲类，对应VBO
	/// </summary>
	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		// Called after buffer base target is binded
		virtual void BindBufferBase(BindBufferBaseTarget target) = 0;

		static Ref<VertexBuffer> Create(void* vertices, uint32_t size, BufferAccessFrequency frequency = BufferAccessFrequency::Static, BufferAccessNature nature = BufferAccessNature::Draw);

	};

	/// <summary>
	///  索引缓冲类，对应IBO
	/// </summary>
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};

	class TransformFeedbackBuffer
	{
	public:
		virtual ~TransformFeedbackBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void BindBufferBase(const Ref<VertexBuffer>& buffer) const = 0;

		virtual void Begin(TransformFeedbackPrimitiveMode primitiveMode) const = 0;
		virtual void End() const = 0;

		virtual void Draw(PrimitiveMode mode) const = 0;

		static Ref<TransformFeedbackBuffer> Create();
	};
}