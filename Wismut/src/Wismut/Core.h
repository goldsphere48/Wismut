#pragma once

#ifdef WI_PLATFORM_WINDOWS
	#ifdef WI_BUILD_DLL
		#define WI_API __declspec(dllexport)
	#else
		#define WI_API __declspec(dllimport)
	#endif
#else
	#error Wismut only supports Windows
#endif
