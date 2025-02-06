#pragma once

#include "PreprocessorHelpers.h"
#include "Platform/Platform.h"

#define CORE_CHECK_IMPL(expr) \
		if (!BOOL_CAST(expr)) \
		{					  \
			PLATFORM_BREAK(); \
		}

#ifdef WI_DEBUG
	#define CORE_CHECK(expr) CORE_CHECK_IMPL(expr)
#else
	#define CORE_CHECK(expr)
#endif
