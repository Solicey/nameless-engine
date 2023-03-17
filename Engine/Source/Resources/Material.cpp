#include "nlpch.h"

#include "Material.h"

#include "Resources/Libraries/ShaderLibrary.h"

namespace NL
{
	void Material::Init()
	{
		// Load default shader
		NL_ENGINE_TRACE("Material init: {0}", Library<Shader>::GetInstance().GetDefaultShader());
		m_Shader = Library<Shader>::GetInstance().LoadShader(Library<Shader>::GetInstance().GetDefaultShader());
		// Load properties
		m_Properties = m_Shader->GetShaderProperties();
	}
}