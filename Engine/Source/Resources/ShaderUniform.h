#pragma once

#include "Core/Math/Math.h"

#include <string>
#include <any>
#include <variant>

namespace NL
{
	enum class ShaderUniformType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Color1, Color3, Color4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool,
		Sampler2D, SamplerCube
	};

	typedef std::variant<std::string, nlm::vec3, float> ShaderPropValue;
	// variant TODO in SceneSerializer.cpp
	struct ShaderProperty
	{
		ShaderUniformType								Type;
		std::string										Name;
		ShaderPropValue									Value;
	};

	static const std::unordered_map<std::string, ShaderUniformType> s_ShaderUniformTypeMap =
	{
		{"float", ShaderUniformType::Float},
		{"vec2", ShaderUniformType::Float2},
		{"vec3", ShaderUniformType::Float3},
		{"vec4", ShaderUniformType::Float4},
		{"color1", ShaderUniformType::Color1},
		{"color3", ShaderUniformType::Color3},
		{"color4", ShaderUniformType::Color4},
		{"mat3", ShaderUniformType::Mat3},
		{"mat4", ShaderUniformType::Mat4},
		{"int", ShaderUniformType::Int},
		{"int2", ShaderUniformType::Int2},
		{"int3", ShaderUniformType::Int3},
		{"int4", ShaderUniformType::Int4},
		{"bool", ShaderUniformType::Bool},
		{"sampler2D", ShaderUniformType::Sampler2D},
		{"samplerCube", ShaderUniformType::SamplerCube}
	};
}