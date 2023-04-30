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
			Add(m_DefaultTextureName, Texture2D::Create(1, 1));
		}

		// Use this function instead of Add or Get 
		Ref<Texture2D> Fetch(const std::string& texPath)
		{
			Ref<Texture2D> tex = nullptr;
			if (Library<Texture2D>::GetInstance().Contains(texPath))
			{
				tex = Library<Texture2D>::GetInstance().Get(texPath);
			}
			else
			{
				tex = Texture2D::Create(texPath);
				Library<Texture2D>::GetInstance().Add(texPath, tex);
			}
			return tex;
		}

		const std::string& GetDefaultTextureName() const { return m_DefaultTextureName; }

		void Delete(const std::string& texPath)
		{
			if (Contains(texPath))
			{
				int useCount = GetUseCount(texPath);
				if (useCount <= 1 && texPath.compare(m_DefaultTextureName) != 0)
				{
					NL_ENGINE_TRACE("Texture {0} reference no more than 1, will be deleted!", texPath);
					Remove(texPath);
					//m_Library.erase(texPath);
				}
				else
				{
					NL_ENGINE_TRACE("Texture {0} reference count is {1}, will not be deleted!", texPath, useCount);
				}
			}
			NL_ENGINE_TRACE("Texture {0} not found in library!", texPath);
		}

		// Avoid memory leak, buggy
		/*void TraverseDelete()
		{
			NL_ENGINE_TRACE("Texture Library Traverse Delete!");

			if (m_Library.size() <= 0)
				return;

			std::unordered_map<std::string, Ref<Texture2D>>::iterator iter = m_Library.begin();
			while (iter != m_Library.end())
			{
				if (iter->second == nullptr)
				{
					m_Library.erase(iter++);
				}
				else if (iter->second.use_count() <= 1 && iter->first.compare(m_DefaultTextureName) != 0)
				{
					NL_ENGINE_TRACE("Texture {0} reference no more than 1, will be deleted!", iter->first);
					iter->second.reset();
					m_Library.erase(iter++);
				}
				else iter++;
			}
		}*/

	private:
		const std::string& m_DefaultTextureName = "Default";
	};

	template <>
	class Library<TextureCubeMap> : public LibraryBase<Library, TextureCubeMap>
	{
	public:
		Library(Singleton::token)
		{
			std::string texturesFolderPath = PathConfig::GetInstance().GetTexturesFolder().string();
			std::vector<std::string> paths
			{
				texturesFolderPath + "/DontModify/DefaultSkybox/right.jpg",
				texturesFolderPath + "/DontModify/DefaultSkybox/left.jpg",
				texturesFolderPath + "/DontModify/DefaultSkybox/top.jpg",
				texturesFolderPath + "/DontModify/DefaultSkybox/bottom.jpg",
				texturesFolderPath + "/DontModify/DefaultSkybox/front.jpg",
				texturesFolderPath + "/DontModify/DefaultSkybox/back.jpg"
			};
			Add("DefaultSkybox", TextureCubeMap::Create(paths));
		}

		Ref<TextureCubeMap> FetchDefault()
		{
			if (Contains("DefaultSkybox"))
				return Get("DefaultSkybox");
			else return nullptr;
		}
	};
}