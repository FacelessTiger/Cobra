#pragma once

#include <memory>

#include "Cobra/Core/PlatformDetection.h"

#ifdef CB_PLATFORM_WINDOWS
#else
	#error Cobra only supports Windows!
#endif


#if defined(CB_PLATFORM_WINDOWS)
	#define CB_DEBUGBREAK() __debugbreak()
#elif defined(CB_PLATFORM_LINUX)
	#include <signal.h>
	#define CB_DEBUGBREAK() raise(SIGTRAP)
#else
	#error "Platform doesn't support debugbreak yet!"
#endif

#ifdef CB_DEBUG
	#define CB_ENABLE_ASSERTS
#endif

#ifndef CB_DIST
	#define CB_ENABLE_VERIFY
#endif

#define CB_EXPAND_MACRO(x) x
#define CB_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define CB_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Cobra {

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

#include "Cobra/Core/Log.h"
#include "Cobra/Core/Assert.h"