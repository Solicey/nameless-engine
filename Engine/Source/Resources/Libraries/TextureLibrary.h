#pragma once

#include "Resources/Libraries/Library.h"
#include "Core/Misc/PathConfig.h"
#include "Resources/Texture.h"

namespace NL
{
	template <>
	class Library<Texture2D> : public LibraryBase<Library, Texture2D>
	{
	public:
		Library(Singleton::token)
		{
			Add(m_DefaultShaderName, Texture2D::Create(128, 128));
		}

		const std::string& GetDefaultTextureName() const { return m_DefaultShaderName; }

	private:
		const std::string& m_DefaultShaderName = "Default";
	};
}