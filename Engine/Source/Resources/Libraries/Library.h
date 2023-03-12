#pragma once

#include "Core/Misc/Singleton.h"
#include "Core/Log/Log.h"

#include <unordered_map>
#include <string>

namespace NL
{
	// CRTP
	template <template <typename> typename Derived, typename LibType>
	class LibraryBase : public Singleton<Derived<LibType>>
	{
	public:
		void Add(const std::string& name, const Ref<LibType>& item)
		{
			NL_ENGINE_ASSERT(m_Library.find(name) == m_Library.end(), "Item already exists in this library!");
			m_Library[name] = item;
		}

		void Set(const std::string& name, const Ref<LibType>& item)
		{
			NL_ENGINE_ASSERT(m_Library.find(name) != m_Library.end(), "Item does NOT exist in this library! Please use Add(...) instead!");
			m_Library[name] = item;
		}

		Ref<LibType> Get(const std::string& name)
		{
			NL_ENGINE_ASSERT(m_Library.find(name) != m_Library.end(), "Item does NOT exist in this library! Please use Add(...) first!");
			return m_Library[name];
		}

	protected:
		std::unordered_map<std::string, Ref<LibType>> m_Library;
	};

	template <typename LibType>
	class Library : public LibraryBase<Library, LibType>
	{
	public:
		Library();
	};
}