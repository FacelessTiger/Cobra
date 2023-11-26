#pragma once

#include <memory>

#include "Ellis/Core/PlatformDetection.h"

#ifdef EL_PLATFORM_WINDOWS
#else
	#error Ellis only supports Windows!
#endif


#if defined(EL_PLATFORM_WINDOWS)
	#define EL_DEBUGBREAK() __debugbreak()
#elif defined(EL_PLATFORM_LINUX)
	#include <signal.h>
	#define EL_DEBUGBREAK() raise(SIGTRAP)
#else
	#error "Platform doesn't support debugbreak yet!"
#endif

#ifdef EL_DEBUG
	#define EL_ENABLE_ASSERTS
#endif

#ifndef EL_DIST
	#define EL_ENABLE_VERIFY
#endif

#define EL_EXPAND_MACRO(x) x
#define EL_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define EL_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Ellis {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Ellis/Core/Log.h"
#include "Ellis/Core/Assert.h"