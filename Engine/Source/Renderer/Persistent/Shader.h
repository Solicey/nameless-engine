#pragma once

#include "Core/Math/Math.h"

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

		void UploadUniformMat4(const std::string& name, const nlm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};
}