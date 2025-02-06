#pragma once

#include <intrin.h>

#define PLATFORM_BREAK() (__nop(), __debugbreak())
