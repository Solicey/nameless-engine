#include "nlpch.h"

#include "ID.h"

#include <random>

namespace NL
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	ID::ID()
		: m_ID(s_UniformDistribution(s_Engine))
	{
	}

	ID::ID(uint64_t id)
		: m_ID(id)
	{
	}
}