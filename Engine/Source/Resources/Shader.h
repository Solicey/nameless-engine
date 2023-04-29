#pragma once

#include "Core/Math/Math.h"
#include "Core/Misc/PtrWrapper.h"
#include "Resources/ShaderUniform.h"

#include <string>
#include <filesystem>

namespace NL
{
	enum class ShaderUsage
	{
		Unknown, Model, PostProcessing, ParticalSystem
	};

	class Shader
	{

	public:
		template <typename LibType>
		friend class Library;

		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(const std::string& name, int value) = 0;
		virtual void SetUniformFloat(const std::string& name, float value) = 0;
		virtual void SetUniformFloat2(const std::string& name, const nlm::vec2& value) = 0;
		virtual void SetUniformFloat3(const std::string& name, const nlm::vec3& value) = 0;
		virtual void SetUniformFloat4(const std::string& name, const nlm::vec4& value) = 0;
		virtual void SetUniformMat4(const std::string& name, const nlm::mat4& value) = 0;
		virtual void SetUniformMat4Array(const std::string& name, const std::vector<nlm::mat4>& values) = 0;
		virtual void Reload() = 0;

		const std::vector<ShaderProperty>& GetShaderProperties() const { return m_Properties; }
		bool HasCompiledSuccessfully() const { return m_HasCompiledSuccessfully; }
		ShaderUsage GetShaderUsage() const { return m_Usage; }

	protected:
		static std::string ReadShaderFile(const std::string& path);
		bool ShaderFileParser(const std::string& rawSrc, std::string& vertexSrc, std::string& fragmentSrc, std::string& geometrySrc);
		void AddProperty(const std::string& type, const std::string& name);

	private:
		// Should ONLY be called by ShaderLibrary, call ShaderLibrary instead
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		// Should ONLY be called by ShaderLibrary, call ShaderLibrary instead
		static Ref<Shader> Create(const std::string& name, const std::filesystem::path& path);
		
		static ShaderUsage ParseShaderUsage(const std::string& path);

	protected:
		static std::unordered_map<std::string, ShaderUsage> s_ShaderUsageMap;
		ShaderUsage m_Usage = ShaderUsage::Unknown;
		std::vector<ShaderProperty>				m_Properties;
		std::unordered_map<std::string, int>	m_UniformLocationMap;

		std::string								m_Name;
		std::string								m_Path;

		bool									m_HasCompiledSuccessfully = false;
	};
}