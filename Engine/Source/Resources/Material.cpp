#include "nlpch.h"

#include "Material.h"

#include "Resources/Libraries/ShaderLibrary.h"
#include "Resources/Libraries/TextureLibrary.h"

namespace NL
{
	std::unordered_map<std::string, TextureType> Material::s_String2TexTypeMap = {
			{ "u_Diffuse", TextureType::Diffuse },
			{ "u_Specular", TextureType::Specular },
			{ "u_Normals", TextureType::Normals },
			{ "u_Ambient", TextureType::Ambient },
			{ "u_Height", TextureType::Height },
			{ "u_Shininess", TextureType::Shininess },
			{ "u_Lightmap", TextureType::Lightmap },
			{ "u_Metalness", TextureType::Metalness },
			{ "u_Roughness", TextureType::Roughness }
	};

	Material::Material()
	{
		LoadShader(Library<Shader>::GetInstance().GetDefaultShaderName());
	}

	void Material::ReplaceTexture(const std::string& name, Ref<Texture2D> texture)
	{
		if (s_String2TexTypeMap.contains(name))
		{
			const auto& type = s_String2TexTypeMap[name];
			if (m_TextureMap.contains(type))
			{
				std::string oldTex = m_TextureMap[type]->GetPath();
				m_TextureMap[type].reset();
				Library<Texture2D>::GetInstance().Delete(oldTex);
			}
			m_TextureMap[type] = texture;
		}
	}

	const Ref<Texture2D>& Material::GetTexture(const std::string& name)
	{
		if (s_String2TexTypeMap.contains(name))
		{
			return m_TextureMap[s_String2TexTypeMap[name]];
		}
		else return nullptr;
	}

	void Material::LoadShader(const std::string name)
	{
		// Load default shader
		// NL_ENGINE_TRACE("Material init: {0}", Library<Shader>::GetInstance().GetDefaultShaderName());
		m_ShaderName = name;
		m_Shader = Library<Shader>::GetInstance().LoadShader(m_ShaderName);
		if (m_Shader == nullptr)
		{
			m_Shader = Library<Shader>::GetInstance().LoadShader(Library<Shader>::GetInstance().GetDefaultShaderName());
			NL_ENGINE_WARN("Couldn't find shader '{0}', use default instead", name);
		}
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
				if (s_String2TexTypeMap.find(prop.Name) != s_String2TexTypeMap.end())
				{
					auto type = s_String2TexTypeMap.at(prop.Name);
					if (m_TextureMap.contains(type))
					{
						prop.Value = std::string(m_TextureMap.at(type)->GetPath());
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

	void Material::DeleteOldTextures(Ref<Texture2D> oldTex, Ref<Texture2D> newTex)
	{
		for (auto& pair : m_TextureMap)
		{
			if (pair.second->GetPath().compare(oldTex->GetPath()) == 0)
			{
				pair.second = newTex;
			}
		}
	}

}