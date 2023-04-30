#pragma once

#include "Core/Misc/Singleton.h"
#include "Core/Log/Log.h"

#include <unordered_map>
#include <set>
#include <string>
#include <regex>

namespace NL
{
	// CRTP
	template <template <typename> typename Derived, typename LibType>
	class LibraryBase : public Singleton<Derived<LibType>>
	{
	public:
		int GetSize() const { return m_Library.size(); }

	protected:
		void Add(const std::string& name, const Ref<LibType>& item)
		{
			auto path = std::regex_replace(name, std::regex("\\\\"), "/");
			// NL_ENGINE_INFO("Library: Add {0}", path);
			// NL_ENGINE_ASSERT(m_Library.find(name) == m_Library.end(), "Item already exists in this library!");
			m_Library[path] = item;
			m_Names.insert(path);
		}

		void Set(const std::string& name, const Ref<LibType>& item)
		{
			auto path = std::regex_replace(name, std::regex("\\\\"), "/");
			// NL_ENGINE_ASSERT(m_Library.find(name) != m_Library.end(), "Item does NOT exist in this library! Please use Add(...) instead!");
			// NL_ENGINE_INFO("Library: Set {0}", path);
			m_Library[path] = item;
			m_Names.insert(path);
		}

		Ref<LibType> Get(const std::string& name) const
		{
			auto path = std::regex_replace(name, std::regex("\\\\"), "/");
			NL_ENGINE_ASSERT(m_Names.contains(path), "Item does NOT exist in this library! Please use Add(...) first!");
			return m_Library.at(path);
			// NL_ENGINE_INFO("Library: Get {0}", path);
		}

		void Remove(const std::string& name)
		{
			auto path = std::regex_replace(name, std::regex("\\\\"), "/");
			NL_ENGINE_ASSERT(m_Names.contains(path), "Item does NOT exist in this library! Please use Add(...) first!");
			m_Library[path].reset();
			m_Library.erase(path);
			m_Names.erase(path);
			// NL_ENGINE_INFO("Library: Remove {0}", path);
		}

		bool Contains(const std::string& name) const
		{
			auto path = std::regex_replace(name, std::regex("\\\\"), "/");
			// NL_ENGINE_INFO("Library: Check Contains {0}", path);
			return m_Names.contains(path);
		}

		int GetUseCount(const std::string& name)
		{
			auto path = std::regex_replace(name, std::regex("\\\\"), "/");
			return m_Library[path].use_count();
		}

		const std::unordered_map<std::string, Ref<LibType>>& GetLibrary() const
		{
			return m_Library;
		}

	public:
		const std::set<std::string>& GetNames() const
		{
			return m_Names;
		}

	private:
		std::unordered_map<std::string, Ref<LibType>> m_Library;
		std::set<std::string> m_Names;
	};

	template <typename LibType>
	class Library : public LibraryBase<Library, LibType>
	{

	};
}