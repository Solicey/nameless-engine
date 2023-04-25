#pragma once

#include "Core/Math/Math.h"
#include "Core/Misc/PtrWrapper.h"
#include "Resources/ShaderUniform.h"

#include <string>
#include <filesystem>

namespace NL
{
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
		virtual const std::vector<ShaderProperty>& GetShaderProperties() const = 0;

	private:
		// Should ONLY be called by ShaderLibrary, call ShaderLibrary instead
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		// Should ONLY be called by ShaderLibrary, call ShaderLibrary instead
		static Ref<Shader> Create(const std::string& name, const std::filesystem::path& path);
	};
}