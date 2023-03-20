#include "nlpch.h"

#include "Material.h"

#include "Resources/Libraries/ShaderLibrary.h"

namespace NL
{
	void Material::LoadShader(const std::string name)
	{
		// Load default shader
		// NL_ENGINE_TRACE("Material init: {0}", Library<Shader>::GetInstance().GetDefaultShaderName());
		m_ShaderName = name;
		m_Shader = Library<Shader>::GetInstance().LoadShader(m_ShaderName);
		// Load properties
		m_Properties = m_Shader->GetShaderProperties();

		for (auto& prop : m_Properties)
		{
			switch (prop.Type)
			{
			case ShaderUniformType::Color3:
				prop.Value = nlm::vec3(1.0, 1.0, 1.0);
				break;

			default:
				break;
			}
		}
	}

}