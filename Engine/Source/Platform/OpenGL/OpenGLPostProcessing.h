#pragma once

#include "Renderer/PostProcessing.h"
#include "Resources/Shader.h"

namespace NL
{
	class OpenGLPostProcessing : public PostProcessing
	{
	public:
		OpenGLPostProcessing();
		virtual uint32_t ExecutePostProcessingQueue(const std::vector<PostProcessingType>& queue, Ref<Framebuffer>& srcFramebuffer) override;

	private:
		void Init();
		void GrayScale(uint32_t src, uint32_t dst);

	private:
		Ref<Shader> m_GrayScaleShader;
	};
}