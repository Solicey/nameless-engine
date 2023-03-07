#pragma once

/*
Nameless日志系统，调用第三方单文件库spdlog，只需在premake中includedir即可
*/

#include "Core/Misc/PtrWrapper.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace NL
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
		inline static Ref<spdlog::logger>& GetAppLogger() { return s_AppLogger; }

	private:
		static Ref<spdlog::logger> s_EngineLogger;
		static Ref<spdlog::logger> s_AppLogger;
	};
}

// 面向引擎 
#define NL_ENGINE_ERROR(...)		::NL::Log::GetEngineLogger()->error(__VA_ARGS__)
#define NL_ENGINE_FATAL(...)		::NL::Log::GetEngineLogger()->fatal(__VA_ARGS__)
#define NL_ENGINE_INFO(...)			::NL::Log::GetEngineLogger()->info(__VA_ARGS__)
#define NL_ENGINE_TRACE(...)		::NL::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define NL_ENGINE_WARN(...)			::NL::Log::GetEngineLogger()->warn(__VA_ARGS__)

// 面向应用
#define NL_ERROR(...)				::NL::Log::GetAppLogger()->error(__VA_ARGS__)
#define NL_FATAL(...)				::NL::Log::GetAppLogger()->fatal(__VA_ARGS__)
#define NL_INFO(...)				::NL::Log::GetAppLogger()->info(__VA_ARGS__)
#define NL_TRACE(...)				::NL::Log::GetAppLogger()->trace(__VA_ARGS__)
#define NL_WARN(...)				::NL::Log::GetAppLogger()->warn(__VA_ARGS__)