#pragma once

#include "Core/Math/Math.h"
#include "Resources/Shader.h"
#include "Resources/ShaderUniform.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace NL
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& name, const std::string& path);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformInt(const std::string& name, int value) override;
		virtual void SetUniformFloat(const std::string& name, float value) override;
		virtual void SetUniformFloat2(const std::string& name, const nlm::vec2& value) override;
		virtual void SetUniformFloat3(const std::string& name, const nlm::vec3& value) override;
		virtual void SetUniformFloat4(const std::string& name, const nlm::vec4& value) override;
		virtual void SetUniformMat4(const std::string& name, const nlm::mat4& value) override;
		virtual const std::vector<ShaderProperty>& GetShaderProperties() const override { return m_Properties; }

	private:
		uint32_t GetUniformLocation(const std::string& name);
		std::string ReadShaderFile(const std::string& path);
		bool ShaderFileParser(const std::string& rawSrc, std::string& vertexSrc, std::string& fragmentSrc);
		void CompileShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		void AddProperty(const std::string& type, const std::string& name);

	private:
		uint32_t								m_RendererID;
		std::vector<ShaderProperty>				m_Properties;
		std::unordered_map<std::string, int>	m_UniformLocationMap;

		std::string								m_Name;
		std::string								m_Path;
	};
}