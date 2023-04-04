#pragma once

#include "Renderer/Framebuffer.h"
#include "Renderer/PostProcessing.h"

namespace NL
{
	class RenderPass
	{
	public:
		RenderPass();
		// const RenderPassSpecification& GetSpecification() const { return m_Specification; }
		void AddPostprocessing(PostProcessingType type);
		uint32_t ExecuteAndReturnFinalTex(const Ref<Framebuffer>& srcBuffer);
	
	private:
		std::vector<Scope<PostProcessing>> m_PostProcessings;

	};
}