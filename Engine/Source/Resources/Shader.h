#pragma once

#include "Core/Math/Math.h"
#include "Core/Misc/PtrWrapper.h"
#include "Resources/ShaderUniform.h"

#include <string>
#include <filesystem>

namespace NL
{
	enum class ShaderUse
	{
		Unknown, Model, PostProcess, Particle1, Particle2
	};

	enum class ShaderTag
	{
		None = 0x0,
		SSAO = 0x1,
		SrcColor = 0x2,		// Post processing pass requires original color texture
	};

	inline ShaderTag operator~ (ShaderTag a) { return (ShaderTag)~(int)a; }
	inline ShaderTag operator| (ShaderTag a, ShaderTag b) { return (ShaderTag)((int)a | (int)b); }
	inline ShaderTag operator& (ShaderTag a, ShaderTag b) { return (ShaderTag)((int)a & (int)b); }
	inline ShaderTag operator^ (ShaderTag a, ShaderTag b) { return (ShaderTag)((int)a ^ (int)b); }
	inline ShaderTag& operator|= (ShaderTag& a, ShaderTag b) { return (ShaderTag&)((int&)a |= (int)b); }
	inline ShaderTag& operator&= (ShaderTag& a, ShaderTag b) { return (ShaderTag&)((int&)a &= (int)b); }
	inline ShaderTag& operator^= (ShaderTag& a, ShaderTag b) { return (ShaderTag&)((int&)a ^= (int)b); }

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
		virtual void SetUniformMat3(const std::string& name, const nlm::mat3& value) = 0;
		virtual void SetUniformMat4(const std::string& name, const nlm::mat4& value) = 0;
		virtual void SetUniformMat4Array(const std::string& name, const std::vector<nlm::mat4>& values) = 0;
		virtual void Reload() = 0;

		const std::vector<ShaderProperty>& GetShaderProperties() const { return m_Properties; }
		bool HasCompiledSuccessfully() const { return m_HasCompiledSuccessfully; }
		ShaderUse GetShaderUse() const { return m_Use; }
		static ShaderUse ParseAndGetShaderUse(const std::string& path);
		bool IsLightingRequired() const { return m_IsLightingRequired; }
		int GetMaxLightsCount() const { return m_MaxLightsCount; }
		bool CheckTag(ShaderTag tag) const { return (bool)(m_Tag & tag); }

	protected:
		static std::string ReadShaderFile(const std::string& path);
		bool ShaderFileParser(const std::string& rawSrc, std::string& vertexSrc, std::string& fragmentSrc, std::string& geometrySrc);
		void AddProperty(const std::string& type, const std::string& name);

	private:
		// Should ONLY be called by ShaderLibrary, call ShaderLibrary instead
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		// Should ONLY be called by ShaderLibrary, call ShaderLibrary instead
		static Ref<Shader> Create(const std::string& name, const std::filesystem::path& path);

	protected:
		std::vector<ShaderProperty>				m_Properties;
		std::unordered_map<std::string, int>	m_UniformLocationMap;
		std::string								m_Name;
		std::string								m_Path;
		bool									m_HasCompiledSuccessfully = false;

		static std::unordered_map<std::string, ShaderUse> s_ShaderUseMap;
		static std::unordered_map<std::string, ShaderTag> s_ShaderTagMap;
		ShaderUse								m_Use = ShaderUse::Unknown;
		ShaderTag								m_Tag = ShaderTag::None;
		bool									m_IsLightingRequired = false;
		int										m_MaxLightsCount = 0;
	};
}