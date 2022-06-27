#pragma once

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Ellis {

	class Log
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

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
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