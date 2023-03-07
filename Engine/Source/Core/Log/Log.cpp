#include "nlpch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace NL
{
	Ref<spdlog::logger> Log::s_EngineLogger;
	Ref<spdlog::logger> Log::s_AppLogger;

	void Log::Init()
	{
		// ÉèÖÃÑùÊ½
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_AppLogger = spdlog::stdout_color_mt("APP");
		s_AppLogger->set_level(spdlog::level::trace);
	}
}