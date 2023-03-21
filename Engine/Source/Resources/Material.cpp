#include "nlpch.h"

#include "Material.h"

#include "Resources/Libraries/ShaderLibrary.h"
#include "Resources/Libraries/TextureLibrary.h"

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

	void Material::UpdateSampler2DinProperties()
	{
		for (auto& prop : m_Properties)
		{
			if (prop.Type == ShaderUniformType::Sampler2D)
			{
				// ËÀÍöif else if else
				if (prop.Name == "u_Diffuse")
				{
					if (m_TextureMap.contains(TextureType::Diffuse))
					{
						NL_ENGINE_TRACE("Diffuse texPath: {0}", m_TextureMap.at(TextureType::Diffuse)->GetPath());
						prop.Value = std::string(m_TextureMap.at(TextureType::Diffuse)->GetPath());
					}
					else
					{
						prop.Value = std::string(Library<Texture2D>::GetInstance().GetDefaultTextureName());
					}	
				}
				else if (prop.Name == "u_Specular")
				{
					if (m_TextureMap.contains(TextureType::Specular))
					{
						NL_ENGINE_TRACE("Specular texPath: {0}", m_TextureMap.at(TextureType::Specular)->GetPath());
						prop.Value = std::string(m_TextureMap.at(TextureType::Specular)->GetPath());
					}
					else
					{
						prop.Value = std::string(Library<Texture2D>::GetInstance().GetDefaultTextureName());
					}
				}
				else
				{
					prop.Value = std::string(Library<Texture2D>::GetInstance().GetDefaultTextureName());
				}
			}
		}
	}

}