#pragma once
#if WI_PLATFORM_LINUX
	#if defined(__GNUC__) || defined(__clang__)
		#define PLATFORM_BREAK() __builtin_trap()
	#else
		#include <signal.h>
		#define PLATFORM_BREAK() raise(SIGTRAP)
	#endif
#endif
