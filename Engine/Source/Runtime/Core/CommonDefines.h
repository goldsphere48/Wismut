#pragma once

#include "PreprocessorHelpers.h"

#if defined(WI_PLATFORM_WIN)
	#define PLATFORM_SOURCE_FOLDER Windows
#elif defined(WI_PLATFORM_LINUX)
	#define PLATFORM_SOURCE_FOLDER Linux
#elif
	#error "Unsupported platform"
#endif


#define PLATFORM_HEADER(header) PREPROCESSOR_STRINGIFY(PREPROCESSOR_CONCAT(PLATFORM_SOURCE_FOLDER/PLATFORM_SOURCE_FOLDER, header))

#define RULE_OF_FIVE(ClassName)						\
	ClassName() = default;							\
	ClassName(const ClassName&) = delete;			\
	ClassName(ClassName&&) = delete;				\
	ClassName& operator=(const ClassName&) = delete;\
	ClassName& operator=(ClassName&&) = delete;		\
