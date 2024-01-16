#pragma once

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Cobra {

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
#define CB_CORE_TRACE(...)   ::Cobra::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CB_CORE_INFO(...)    ::Cobra::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CB_CORE_WARN(...)    ::Cobra::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CB_CORE_ERROR(...)   ::Cobra::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CB_CORE_FATAL(...)   ::Cobra::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define CB_TRACE(...)        ::Cobra::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CB_INFO(...)         ::Cobra::Log::GetClientLogger()->info(__VA_ARGS__)
#define CB_WARN(...)         ::Cobra::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CB_ERROR(...)        ::Cobra::Log::GetClientLogger()->error(__VA_ARGS__)
#define CB_FATAL(...)        ::Cobra::Log::GetClientLogger()->critical(__VA_ARGS__)