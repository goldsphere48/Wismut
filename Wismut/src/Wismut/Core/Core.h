#pragma once

#ifdef _MSC_VER
	#define WI_DEBUG_BREAK() __debugbreak();
#elif defined(__clang__)
	#define WI_DEBUG_BREAK() __builtin_trap();
#else
	#error Debug break for current environment undefined
#endif

inline uint64_t GetAligned(uint64_t operand, uint64_t granularity) {
	return ((operand + (granularity - 1)) & ~(granularity - 1));
}