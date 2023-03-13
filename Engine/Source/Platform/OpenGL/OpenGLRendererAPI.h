#pragma once

#include "Renderer/RendererAPI.h"

namespace NL
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const nlm::vec4& color) override;
		
		virtual void Clear() override;
		
		virtual void DrawIndices(const Ref<VertexArray>& vertexArray) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};
}