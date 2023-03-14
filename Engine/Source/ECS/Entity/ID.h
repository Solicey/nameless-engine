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

	private:
		uint64_t m_ID;
	};
}