#include "nlpch.h"

#include "OpenGLShader.h"

#include "Core/Log/Log.h"
#include "Core/Math/Math.h"

#include <glad/glad.h>


namespace NL
{
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		m_HasCompiledSuccessfully = CompileShader(vertexSrc, fragmentSrc);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& path)
		: m_Name(name), m_Path(path)
	{
		NL_ENGINE_TRACE("Loading Shader {0} ... path: {1}", name, path);
		std::string src = ReadShaderFile(path);
		std::string vertexSrc, fragmentSrc;
		
		if (ShaderFileParser(src, vertexSrc, fragmentSrc))
		{
			m_HasCompiledSuccessfully = CompileShader(vertexSrc, fragmentSrc);
		}
		else
		{
			m_HasCompiledSuccessfully = false;
			NL_ENGINE_ERROR("Failed to load shader: {0}", name);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const nlm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const nlm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const nlm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const nlm::mat4& value)
	{
		// GL_FALSE：列优先矩阵
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, nlm::value_ptr(value));
	}

	void OpenGLShader::SetUniformMat4Array(const std::string& name, const std::vector<nlm::mat4>& values)
	{
		// GL_FALSE：列优先矩阵
		glUniformMatrix4fv(GetUniformLocation(name), values.size(), GL_FALSE, (float*)values.data());
	}

	void OpenGLShader::Reload()
	{
		if (m_RendererID)
			glDeleteProgram(m_RendererID);
		m_Properties.clear();
		m_UniformLocationMap.clear();

		std::string src = ReadShaderFile(m_Path);
		std::string vertexSrc, fragmentSrc;

		if (ShaderFileParser(src, vertexSrc, fragmentSrc))
		{
			m_HasCompiledSuccessfully = CompileShader(vertexSrc, fragmentSrc);
		}
		else
		{
			m_HasCompiledSuccessfully = false;
		}
	}

	uint32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationMap.find(name) != m_UniformLocationMap.end())
			return m_UniformLocationMap.at(name);

		const int location = glGetUniformLocation(m_RendererID, name.c_str());

		if (location == -1)
			NL_ENGINE_WARN("Uniform: '{0}' doesn't exist!", name);

		m_UniformLocationMap[name] = location;

		return location;
	}

	std::string OpenGLShader::ReadShaderFile(const std::string& path)
	{
		std::string result;
		// binary是为了读取\r\n作为换行符
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			NL_ENGINE_ERROR("Could not open file '{0}'", path);
		}

		return result;
	}

	bool OpenGLShader::ShaderFileParser(const std::string& rawSrc, std::string& vertexSrc, std::string& fragmentSrc)
	{
		bool vertexSrcParsed = false, fragmentSrcParsed = false;

		// Parsing props
		{
			m_Properties.clear();

			const char* propToken = "#prop";
			const char* endToken = "#end";
			size_t propPos = rawSrc.find(propToken, 0);
			if (propPos != std::string::npos)
			{
				size_t endPos = rawSrc.find(endToken, propPos);
				NL_ENGINE_ASSERT(endPos != std::string::npos, "Shader syntax error!");
				propPos = rawSrc.find_first_of("\r\n", propPos);
				propPos = rawSrc.find_first_not_of("\r\n", propPos);
				while (propPos < endPos)
				{
					size_t propEnd = rawSrc.find_first_of(" ", propPos);
					size_t namePos = rawSrc.find_first_not_of(" ", propEnd);
					std::string propType = rawSrc.substr(propPos, propEnd - propPos);
					propPos = rawSrc.find_first_of(";", namePos);
					std::string propName = rawSrc.substr(namePos, propPos - namePos);
					propPos = rawSrc.find_first_of("\r\n", propPos);
					propPos = rawSrc.find_first_not_of("\r\n", propPos);
					NL_ENGINE_INFO("Shader prop type: {0}, name: {1}", propType, propName);
					AddProperty(propType, propName);
				}
			}
		}

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = rawSrc.find(typeToken, 0);
		// NL_ENGINE_TRACE("rawSrc: {0}", rawSrc);
		// NL_ENGINE_TRACE("pos: {0}", pos);
		while (pos != std::string::npos)
		{
			size_t eol = rawSrc.find_first_of("\r\n", pos);
			NL_ENGINE_ASSERT(eol != std::string::npos, "Shader syntax error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = rawSrc.substr(begin, eol - begin);
			size_t nextLinePos = rawSrc.find_first_not_of("\r\n", eol);
			NL_ENGINE_ASSERT(nextLinePos != std::string::npos, "Shader syntax error!");
			pos = rawSrc.find(typeToken, nextLinePos);
			
			if (type == "vertex")
			{
				vertexSrc = (pos == std::string::npos) ? rawSrc.substr(nextLinePos) : rawSrc.substr(nextLinePos, pos - nextLinePos);
				vertexSrcParsed = true;
				NL_ENGINE_TRACE("Vertex shader parsed!");
			}
			else if (type == "fragment")
			{
				fragmentSrc = (pos == std::string::npos) ? rawSrc.substr(nextLinePos) : rawSrc.substr(nextLinePos, pos - nextLinePos);
				fragmentSrcParsed = true;
				NL_ENGINE_TRACE("Fragment shader parsed!");
			}	
			else
			{
				NL_ENGINE_ASSERT(false, "Invalid shader type specified!");
			}

			// NL_ENGINE_TRACE("pos: {0}", pos);
		}

		return vertexSrcParsed && fragmentSrcParsed;
	}

	bool OpenGLShader::CompileShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			NL_ENGINE_ERROR("{0}", infoLog.data());
			// NL_ENGINE_ASSERT(false, "Vertex shader compilation error!");

			return false;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			NL_ENGINE_ERROR("{0}", infoLog.data());
			// NL_ENGINE_ASSERT(false, "Fragment shader compilation error!");

			// In this simple program, we'll just leave
			return false;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		m_RendererID = program;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			NL_ENGINE_ERROR("{0}", infoLog.data());
			// NL_ENGINE_ASSERT(false, "OpenGLShader link error!");

			// In this simple program, we'll just leave
			return false;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		return true;
	}

	void OpenGLShader::AddProperty(const std::string& type, const std::string& name)
	{
		if (s_ShaderUniformTypeMap.find(type) == s_ShaderUniformTypeMap.end())
		{
			return;
		}

		ShaderProperty prop;
		prop.Name = name;
		prop.Type = s_ShaderUniformTypeMap.at(type);
		m_Properties.emplace_back(prop);

	}

}