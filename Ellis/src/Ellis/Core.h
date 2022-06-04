#pragma once

#ifdef EL_PLATFORM_WINDOWS
	#ifdef EL_BUILD_DLL
		#define ELLIS_API __declspec(dllexport)
	#else
		#define ELLIS_API __declspec(dllimport)
	#endif
#else
	#error Ellis only supports Windows!
#endif

#define BIT(x) (1 << x)