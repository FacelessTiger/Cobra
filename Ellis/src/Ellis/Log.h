#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Ellis {

	class ELLIS_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};

}

// Core log macros
#define EL_CORE_TRACE(...)   ::Ellis::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EL_CORE_INFO(...)    ::Ellis::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EL_CORE_WARN(...)    ::Ellis::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EL_CORE_ERROR(...)   ::Ellis::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EL_CORE_FATAL(...)   ::Ellis::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define EL_TRACE(...)        ::Ellis::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EL_INFO(...)         ::Ellis::Log::GetClientLogger()->info(__VA_ARGS__)
#define EL_WARN(...)         ::Ellis::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EL_ERROR(...)        ::Ellis::Log::GetClientLogger()->error(__VA_ARGS__)
#define EL_FATAL(...)        ::Ellis::Log::GetClientLogger()->critical(__VA_ARGS__)