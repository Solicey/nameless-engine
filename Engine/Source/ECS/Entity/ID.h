#pragma once

#include <cstdint>

namespace NL
{
	class ID
	{
	public:
		ID();
		ID(uint64_t id);
		ID(const ID&) = default;

		uint64_t GetID() const { return m_ID; }

	private:
		uint64_t m_ID;
	};
}