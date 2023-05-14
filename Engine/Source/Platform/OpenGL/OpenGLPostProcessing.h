#pragma once

#include "Renderer/PostProcessing.h"
#include "Resources/Shader.h"

namespace NL
{
	class OpenGLPostProcessing : public PostProcessing
	{
	public:
		OpenGLPostProcessing();
		// virtual uint32_t ExecutePostProcessingQueue(const std::vector<PostProcessingType>& queue, Ref<Framebuffer>& srcFramebuffer) override;
		virtual uint32_t ExecutePostProcessingQueue(const std::vector<Ref<Material>>& queue, Ref<Framebuffer>& srcFramebuffer) override;

	private:
		void Init();
		//void GrayScale(uint32_t src, uint32_t dst);
		//void EditorOutline(uint32_t src, uint32_t dst, uint32_t entityTex);

	private:
		//Ref<Shader> m_GrayScaleShader;
		//Ref<Shader> m_EditorOutlineShader;

		// tex0 & tex1 are color tex
		uint32_t m_Tex0;
		uint32_t m_Tex1;

		union EntityID
		{
			int intID;
			float floatID;
		};
	};
}