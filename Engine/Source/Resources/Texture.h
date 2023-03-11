#pragma once

#include "Renderer/Alternatives.h"
#include <cstdint>

namespace NL
{
	class Texture
	{
	private:
		uint32_t m_RendererID;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}