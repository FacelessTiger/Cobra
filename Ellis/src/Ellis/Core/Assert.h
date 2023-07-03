#pragma once

#include "Ellis/Core/Core.h"
#include "Ellis/Core/Log.h"

#include <filesystem>

#ifdef EL_ENABLE_ASSERTS
	#define EL_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { EL##type##ERROR(msg, __VA_ARGS__); EL_DEBUGBREAK(); } }
	#define EL_INTERNAL_ASSERT_WITH_MSG(type, check, ...) EL_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define EL_INTERNAL_ASSERT_NO_MSG(type, check) EL_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EL_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define EL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define EL_INTERNAL_ASSERT_GET_MACRO(...) EL_EXPAND_MACRO( EL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, EL_INTERNAL_ASSERT_WITH_MSG, EL_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define EL_ASSERT(...) EL_EXPAND_MACRO( EL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define EL_CORE_ASSERT(...) EL_EXPAND_MACRO( EL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define EL_ASSERT(...)
	#define EL_CORE_ASSERT(...)
#endif

#ifdef EL_ENABLE_VERIFY
	#define EL_INTERNAL_VERIFY_IMPL(type, check, msg, ...) { if(!(check)) { EL##type##ERROR(msg, __VA_ARGS__); EL_DEBUGBREAK(); } }
	#define EL_INTERNAL_VERIFY_WITH_MSG(type, check, ...) EL_INTERNAL_VERIFY_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define EL_INTERNAL_VERIFY_NO_MSG(type, check) EL_INTERNAL_VERIFY_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EL_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define EL_INTERNAL_VERIFY_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define EL_INTERNAL_VERIFY_GET_MACRO(...) EL_EXPAND_MACRO( EL_INTERNAL_VERIFY_GET_MACRO_NAME(__VA_ARGS__, EL_INTERNAL_VERIFY_WITH_MSG, EL_INTERNAL_VERIFY_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define EL_VERIFY(...) EL_EXPAND_MACRO( EL_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define EL_CORE_VERIFY(...) EL_EXPAND_MACRO( EL_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define EL_VERIFY(...)
	#define EL_CORE_VERIFY(...)
#endif