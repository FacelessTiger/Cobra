#pragma once

#include "Cobra/Core/Core.h"
#include "Cobra/Core/Log.h"

#include <filesystem>

#ifdef CB_ENABLE_ASSERTS
	#define CB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { CB##type##ERROR(msg, __VA_ARGS__); CB_DEBUGBREAK(); } }
	#define CB_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define CB_INTERNAL_ASSERT_NO_MSG(type, check) CB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", CB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define CB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define CB_INTERNAL_ASSERT_GET_MACRO(...) CB_EXPAND_MACRO( CB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, CB_INTERNAL_ASSERT_WITH_MSG, CB_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define CB_ASSERT(...) CB_EXPAND_MACRO( CB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define CB_CORE_ASSERT(...) CB_EXPAND_MACRO( CB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define CB_ASSERT(...)
	#define CB_CORE_ASSERT(...)
#endif

#ifdef CB_ENABLE_VERIFY
	#define CB_INTERNAL_VERIFY_IMPL(type, check, msg, ...) { if(!(check)) { CB##type##ERROR(msg, __VA_ARGS__); CB_DEBUGBREAK(); } }
	#define CB_INTERNAL_VERIFY_WITH_MSG(type, check, ...) CB_INTERNAL_VERIFY_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define CB_INTERNAL_VERIFY_NO_MSG(type, check) CB_INTERNAL_VERIFY_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", CB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define CB_INTERNAL_VERIFY_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define CB_INTERNAL_VERIFY_GET_MACRO(...) CB_EXPAND_MACRO( CB_INTERNAL_VERIFY_GET_MACRO_NAME(__VA_ARGS__, CB_INTERNAL_VERIFY_WITH_MSG, CB_INTERNAL_VERIFY_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define CB_VERIFY(...) CB_EXPAND_MACRO( CB_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define CB_CORE_VERIFY(...) CB_EXPAND_MACRO( CB_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define CB_VERIFY(...)
	#define CB_CORE_VERIFY(...)
#endif