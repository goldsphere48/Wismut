#pragma once

#if defined(_MSC_VER)
	#define FORCEINLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
	#define FORCEINLINE inline __attribute__((always_inline))
#elif defined(__INTEL_COMPILER)
	#define FORCEINLINE __forceinline
#else
	#define FORCEINLINE inline
#endif

#if defined(_MSC_VER)
	#define NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
	#define NOINLINE __attribute__((noinline))
#else
	#define NOINLINE
#endif

#if defined(_MSC_VER)
	#define DEBUGBREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
	#define DEBUGBREAK() __builtin_trap()
#else
	#include <signal.h>
	#define DEBUGBREAK() raise(SIGTRAP)
#endif
