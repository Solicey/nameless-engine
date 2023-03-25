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

		operator uint64_t() { return m_ID; }
		operator const uint64_t() const { return m_ID; }
		uint64_t GetID() const { return m_ID; }

	private:
		uint64_t m_ID;
	};
}

namespace std
{
	template<>
	struct hash<NL::ID>
	{
		std::size_t operator()(const NL::ID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}