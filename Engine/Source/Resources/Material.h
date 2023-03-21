#pragma once

#include "Core/Log/Log.h"
#include "Resources/Shader.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Resources/Texture.h"

#include <unordered_map>

namespace NL
{
    // from https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

	class Material
	{
    public:
        Material() { LoadShader(Library<Shader>::GetInstance().GetDefaultShaderName()); }

        void AddTexture(TextureType type, Ref<Texture2D> texture)
        {
            // NL_ENGINE_ASSERT(m_TextureMap.find(type) == m_TextureMap.end(), "Texture of this type already exists!");
            m_TextureMap[type] = texture;
        }

        const Ref<Texture2D>& GetTexture(TextureType type) { return m_TextureMap[type]; }
        const std::string& GetShaderName() const { return m_ShaderName; }
        const Ref<Shader>& GetShader() const { return m_Shader; }
        std::vector<ShaderProperty>& GetShaderPropertiesNotConst() { return m_Properties; }
        void LoadShader(const std::string name);

        // called after all textures are sent to lib
        void UpdateSampler2DinProperties();

	private:
        std::string m_Name;
        std::string m_ShaderName;
		Ref<Shader>	m_Shader;
        std::vector<ShaderProperty> m_Properties;
        std::unordered_map<TextureType, Ref<Texture2D>, EnumClassHash> m_TextureMap;
        std::unordered_map<TextureType, bool, EnumClassHash> m_UseDefaultTexture;
	};
}