#pragma once

#include <string>
#include <any>

namespace NL
{
	enum class ShaderUniformType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool,
		Sampler2D, SamplerCube
	};

	struct ShaderUniform
	{
		ShaderUniformType	Type;
		std::string			Name;
		uint32_t			Location;
		std::any			Value;
	};
}