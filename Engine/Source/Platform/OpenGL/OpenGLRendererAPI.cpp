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
			case DepthComp::Equal:
				return GL_EQUAL;
			case DepthComp::Lequal:
				return GL_LEQUAL;
			case DepthComp::Less:
				return GL_LESS;
			}
			NL_ENGINE_ASSERT(false, "Unknown Depth Comp");
			return -1;
		}

		static GLenum CullFace2OpenGLCullFace(CullFace face)
		{
			switch (face)
			{
			case NL::CullFace::Front:
				return GL_FRONT;
			case NL::CullFace::Back:
				return GL_BACK;
			case NL::CullFace::FrontAndBack:
				return GL_FRONT_AND_BACK;
			}
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
		glEnable(GL_CULL_FACE);
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

	void OpenGLRendererAPI::SetCullFace(CullFace face)
	{
		glCullFace(Utils::CullFace2OpenGLCullFace(face));
	}

	void OpenGLRendererAPI::EnableCullFace(bool enable)
	{
		if (enable) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::BeginTransformFeedback_Points()
	{
		glBeginTransformFeedback(GL_POINTS);
	}

	void OpenGLRendererAPI::EndTransformFeedback()
	{
		glEndTransformFeedback();
	}

	void OpenGLRendererAPI::DrawArrays_Points(int first, uint32_t count)
	{
		glDrawArrays(GL_POINTS, first, count);
	}
}