#pragma once

#include "Core/Math/Math.h"
#include "Core/Misc/PtrWrapper.h"
#include "Renderer/VertexArray.h"

namespace NL
{
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
		virtual void SetClearColor(const nlm::vec4& color) = 0;
		
		virtual void Clear() = 0;
		
		virtual void DrawIndices(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetCurrent() { return s_API; }

		static Scope<RendererAPI> Create();

	private:
		inline static API s_API = API::OpenGL;
	};
}