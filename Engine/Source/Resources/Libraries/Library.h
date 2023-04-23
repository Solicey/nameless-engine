#pragma once

#include "Core/Misc/Singleton.h"
#include "Core/Log/Log.h"

#include <unordered_map>
#include <string>
#include <regex>

namespace NL
{
	// CRTP
	template <template <typename> typename Derived, typename LibType>
	class LibraryBase : public Singleton<Derived<LibType>>
	{
	protected:
		void Add(const std::string& name, const Ref<LibType>& item)
		{
			auto path = name;
			std::regex_replace(path, std::regex("\\\\"), "/");
			// NL_ENGINE_ASSERT(m_Library.find(name) == m_Library.end(), "Item already exists in this library!");
			m_Library[path] = item;
		}

		void Set(const std::string& name, const Ref<LibType>& item)
		{
			auto path = name;
			std::regex_replace(path, std::regex("\\\\"), "/");
			// NL_ENGINE_ASSERT(m_Library.find(name) != m_Library.end(), "Item does NOT exist in this library! Please use Add(...) instead!");
			m_Library[path] = item;
		}

		Ref<LibType> Get(const std::string& name)
		{
			auto path = name;
			std::regex_replace(path, std::regex("\\\\"), "/");
			NL_ENGINE_ASSERT(m_Library.find(name) != m_Library.end(), "Item does NOT exist in this library! Please use Add(...) first!");
			return m_Library[path];
		}

		bool Contains(const std::string& name)
		{
			return m_Library.find(name) != m_Library.end();
		}

	protected:
		std::unordered_map<std::string, Ref<LibType>> m_Library;
	};

	template <typename LibType>
	class Library : public LibraryBase<Library, LibType>
	{

	};
}