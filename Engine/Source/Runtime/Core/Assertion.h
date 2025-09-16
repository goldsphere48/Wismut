#pragma once

#include <format>

#include "Utils/PreprocessorHelpers.h"
#include "Utils/CompilerMacros.h"

namespace Wi
{
	void ErrorMessage(const char* str);

	FORCEINLINE void AssertFail(const char* expr, const char* file, int line, std::string_view msg = {})
	{
		std::string message = std::format("[ASSERT] {}:{} '{}' failed. {}", file, line, expr, msg);
		ErrorMessage(message.c_str());
	}
}


#ifdef WI_DEBUG
	#define WI_ASSERT_IMPL(true_condition, ...)																			\
		if (!BOOL_CAST(true_condition))																				\
		{	\
			::Wi::AssertFail(#true_condition, __FILE__, __LINE__ __VA_OPT__(, std::format(__VA_ARGS__)));																\
			DEBUGBREAK();																							\
		}

	#define WI_ASSERT(true_condition, ...) WI_ASSERT_IMPL(true_condition, __VA_ARGS__)
#else
	#define WI_ASSERT(expr)
#endif
