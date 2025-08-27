#pragma once

#include "PreprocessorHelpers.h"

#if defined(WI_PLATFORM_WIN)
	#define PLATFORM_SOURCE_FOLDER Windows
#elif defined(WI_PLATFORM_LINUX)
	#define PLATFORM_SOURCE_FOLDER Linux
#elif
	#error "Unsupported platform"
#endif

#define DELETE_COPYING(ClassName)						\
	ClassName() = default;							\
	ClassName(const ClassName&) = delete;			\
	ClassName(ClassName&&) = delete;				\
	ClassName& operator=(const ClassName&) = delete;\
	ClassName& operator=(ClassName&&) = delete;		\
