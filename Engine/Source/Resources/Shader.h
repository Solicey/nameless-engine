#pragma once

#include "Core/Math/Math.h"
#include "Resources/ShaderUniform.h"

#include <string>

namespace NL
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformInt(const std::string& name, int value);
		void SetUniformFloat(const std::string& name, float value);
		void SetUniformFloat2(const std::string& name, const nlm::vec2& value);
		void SetUniformFloat3(const std::string& name, const nlm::vec3& value);
		void SetUniformFloat4(const std::string& name, const nlm::vec4& value);
		void SetUniformMat4(const std::string& name, const nlm::mat4& value);

	private:
		uint32_t GetUniformLocation(const std::string& name);

	private:
		uint32_t								m_RendererID;
		std::vector<ShaderUniform>				m_uniforms;
		std::unordered_map<std::string, int>	m_uniformLocationMap;
	};
}