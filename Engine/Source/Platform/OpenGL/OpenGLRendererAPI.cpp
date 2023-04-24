#include "nlpch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
#include <gl/GL.h>

namespace NL
{
	namespace Utils
	{
		static GLenum DepthComparison2OpenGLDepthComparison(DepthComp comp)
		{
			switch (comp)
			{
			case DepthComp::EQUAL:
				return GL_EQUAL;
			case DepthComp::LEQUAL:
				return GL_LEQUAL;
			case DepthComp::LESS:
				return GL_LESS;
			}
			NL_ENGINE_ASSERT(false, "Unknown Depth Comp");
			return -1;
		}
	}

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		//glStencilMask(0xFF);
		glStencilMask(0x00); // forbidden to write in stencil

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_MULTISAMPLE);

		// glEnable(GL_ALPHA_TEST);
		// glAlphaFunc(GL_GREATER, 0.1);

	}

	void OpenGLRendererAPI::SetClearColor(const nlm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndices(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::DepthTest(bool enable)
	{
		if (enable) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DepthFunc(DepthComp comp)
	{
		glDepthFunc(Utils::DepthComparison2OpenGLDepthComparison(comp));
	}
}