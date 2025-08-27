#pragma once
#if WI_PLATFORM_WIN

#include <intrin.h>

#define PLATFORM_BREAK() (__nop(), __debugbreak())
#endif
