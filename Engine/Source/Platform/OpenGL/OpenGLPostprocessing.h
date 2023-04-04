#pragma once

#include "Renderer/PostProcessing.h"
#include "Resources/Shader.h"

namespace NL
{
	class OpenGLPostProcessing : public PostProcessing
	{
	public:
		OpenGLPostProcessing(const PostProcessingType& type);
		virtual uint32_t ExecuteAndReturnFinalTex() override;

	private:
		uint32_t ExecuteWithShader();
		Ref<Shader> m_Shader;
	};
}