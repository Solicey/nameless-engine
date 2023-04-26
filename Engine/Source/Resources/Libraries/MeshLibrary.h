#pragma once

#include "Resources/Mesh.h"
#include "Resources/Libraries/Library.h"

namespace NL
{
	template <>
	class Library<Mesh> : public LibraryBase<Library, Mesh>
	{
	public:
		Library(Singleton::token) {}

		bool HasMesh(const std::string& modelName, const std::string meshName)
		{
			if (Contains(modelName + ":" + meshName))
				return true;
			return false;
		}

		void AddMesh(const std::string& modelName, const std::string meshName, Ref<Mesh> mesh)
		{
			Add(modelName + ":" + meshName, mesh);
		}

		Ref<Mesh> GetMesh(const std::string& modelName, const std::string meshName)
		{
			return Get(modelName + ":" + meshName);
		}

		bool HasModel(const std::string& modelName)
		{
			std::string name = std::regex_replace(modelName, std::regex("\\\\"), "/");
			for (const auto& item : GetLibrary())
			{
				if (item.first.substr(0, item.first.find_first_of(":")).compare(name) == 0)
					return true;
			}
			return false;
		}

		void DeleteMesh(const std::string& modelName, const std::string meshName)
		{
			auto name = modelName + ":" + meshName;
			if (Contains(name))
			{
				const auto& item = Get(name);
				int useCount = item.use_count();
				if (useCount <= 1)
				{
					NL_ENGINE_TRACE("Mesh {0} reference no more than 1, will be deleted!", name);
					Remove(name);
					//Get(name).reset();
					//m_Library.erase(name);
				}
				else
				{
					NL_ENGINE_TRACE("Mesh {0} reference count is {1}, will not be deleted!", name, useCount);
				}
			}
		}

		/*void TraverseDelete()
		{
			NL_ENGINE_TRACE("Mesh Library Traverse Delete!");
			if (m_Library.size() <= 0)
				return;

			std::unordered_map<std::string, Ref<Mesh>>::iterator iter = m_Library.begin();
			while (iter != m_Library.end())
			{
				if (iter->second == nullptr)
				{
					m_Library.erase(iter++);
				}
				else if (iter->second.use_count() <= 1)
				{
					NL_ENGINE_TRACE("Mesh {0} reference no more than 1, will be deleted!", iter->first);
					iter->second.reset();
					m_Library.erase(iter++);
				}
				else iter++;
			}
		}*/

	
	};
}