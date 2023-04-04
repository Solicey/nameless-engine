#pragma once

#include "Resources/Libraries/Library.h"
#include "Core/Misc/ConfigManager.h"
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

		void Delete(const std::string& name)
		{
			if (m_Library.find(name) != m_Library.end())
			{
				if (m_Library[name].use_count() <= 1)
				{
					NL_ENGINE_TRACE("Texture {0} reference no more than 1, will be deleted!", name);
					m_Library[name].reset();
					m_Library.erase(name);
				}
			}
		}

		// Avoid memory leak, buggy
		void TraverseDelete()
		{
			if (m_Library.empty())
				return;

			std::unordered_map<std::string, Ref<Texture2D>>::iterator iter = m_Library.begin();
			while (iter != m_Library.end())
			{
				if (iter->second == nullptr)
				{
					m_Library.erase(iter++);
				}
				else if (iter->second.use_count() <= 1)
				{
					NL_ENGINE_TRACE("Texture {0} reference no more than 1, will be deleted!", iter->first);
					iter->second.reset();
					m_Library.erase(iter++);
				}
				else iter++;
			}
		}

	private:
		const std::string& m_DefaultShaderName = "Default";
	};
}