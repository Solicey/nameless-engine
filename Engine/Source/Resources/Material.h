#pragma once

#include "Core/Log/Log.h"
#include "Resources/Shader.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Resources/Libraries/TextureLibrary.h"
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
        Material();
        Material(const std::string& shaderName, const std::string& matName = "Default Material");
        Material(const Material* src);

        void DeleteTexturesAndShadersReference();

        void AddTexture(TextureType type, Ref<Texture2D> texture)
        {
            // NL_ENGINE_ASSERT(m_TextureMap.find(type) == m_TextureMap.end(), "Texture of this type already exists!");
            m_TextureMap[type] = texture;
        }

        // Delete old tex to avoid memory leaking, update prop value BY YOURSELF!
        void ReplaceTexture(const std::string& name, Ref<Texture2D> texture);

        const Ref<Texture2D>& GetTexture(TextureType type) { return m_TextureMap[type]; }
        const Ref<Texture2D>& GetTexture(const std::string& name);
        const std::string& GetShaderName() const { return m_ShaderName; }
        std::string GetShaderNameNoSuffix() const { return m_ShaderName.substr(0, m_ShaderName.find_first_of(".")); }
        const Ref<Shader>& GetShader() const { return m_Shader; }
        const std::vector<ShaderProperty>& GetShaderProperties() const { return m_Properties; }
        std::vector<ShaderProperty>& GetShaderPropertiesNotConst() { return m_Properties; }
        void LoadShaderAndUpdateProps(const std::string name);
        const std::string& GetName() const { return m_Name; }

        // called after all textures are sent to lib, or LoadShaderAndUpdateProps
        void UpdateSampler2DinProperties();

        // void SetShaderName(const std::string& name) { m_ShaderName = name; }

        // called after user change a texture
        // void DeleteOldTextures(Ref<Texture2D> oldTex, Ref<Texture2D> newTex);
        void SetEnabled(bool enabled) { m_Enabled = enabled; }
        bool IsEnabled() const { return m_Enabled; }
        bool* GetEnabled() { return &m_Enabled; }

	private:
        std::string m_Name;
        std::string m_ShaderName;
		Ref<Shader>	m_Shader;
        std::vector<ShaderProperty> m_Properties;
        std::unordered_map<std::string, Ref<Texture2D>> m_CustomTextureMap;
        std::unordered_map<TextureType, Ref<Texture2D>, EnumClassHash> m_TextureMap;
        // std::unordered_map<TextureType, bool, EnumClassHash> m_UseDefaultTexture;
        static std::unordered_map<std::string, TextureType> s_String2TexTypeMap;

        // For post-processing
        bool m_Enabled = true;
	};
}