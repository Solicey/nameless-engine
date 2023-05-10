#pragma once

#include "Core/Math/Math.h"
#include "Core/Misc/PtrWrapper.h"
#include "Renderer/VertexArray.h"

namespace NL
{
	enum class DepthComp
	{
		Equal,
		Lequal,
		Less
	};

	enum class CullFace
	{
		Front,
		Back,
		FrontAndBack
	};

	enum class BlendFactor
	{
		One,
		SrcAlpha,
		OneMinusSrcAlpha
	};

	enum class BlendOp
	{
		Add
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, 
			OpenGL,
			DX11
		};

	public:
		virtual void Init() = 0;

		virtual void SetClearColor(const nlm::vec4& color) = 0;
		
		virtual void Clear() = 0;
		
		virtual void DrawIndices(const Ref<VertexArray>& vertexArray) = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DepthTest(bool enable) = 0;

		virtual void DepthFunc(DepthComp comp) = 0;

		virtual void SetCullFace(CullFace face) = 0;

		virtual void EnableCullFace(bool enable) = 0;

		virtual void BeginTransformFeedback_Points() = 0;

		virtual void EndTransformFeedback() = 0;

		virtual void DrawArrays_Points(int first, uint32_t count) = 0;

		virtual void RasterizerDiscard(bool enable) = 0;

		virtual void DepthMask(bool enable) = 0;

		virtual void BlendFunc(BlendFactor srcFactor, BlendFactor dstFactor) = 0;

		inline static API GetCurrent() { return s_API; }

		static Scope<RendererAPI> Create();

	private:
		inline static API s_API = API::OpenGL;
	};
}