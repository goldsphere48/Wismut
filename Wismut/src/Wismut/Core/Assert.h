#pragma once

#include "Core.h"

#define WI_CORE_ASSERT(condition, message, ...) if (!(condition)) { WI_CORE_CRITICAL(message, __VA_ARGS__); WI_DEBUG_BREAK() }
#define WI_ASSERT(condition, message, ...) if (!(condition)) { WI_CRITICAL(message, __VA_ARGS__); WI_DEBUG_BREAK() }