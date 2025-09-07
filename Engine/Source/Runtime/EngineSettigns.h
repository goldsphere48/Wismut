#pragma once

#if WI_DEBUG
#define ENABLE_MEMORY_TRACE 1
	#define WI_ENABLE_DEATH_TESTS 1
#else
	#define ENABLE_MEMORY_TRACE 0
#endif

#define ENABLE_CUSTOM_MEMORY_SYSTEM 1

#ifndef WI_EXCEPTIONS_ENABLED
	#if defined(_MSC_VER)
		#ifndef _HAS_EXCEPTIONS
			#define _HAS_EXCEPTIONS 0
		#endif
		#define WI_EXCEPTIONS_ENABLED (_HAS_EXCEPTIONS != 0)
	#else
		#ifdef __cpp_exceptions
			#define WI_EXCEPTIONS_ENABLED 1
		#else
			#define WI_EXCEPTIONS_ENABLED 0
		#endif
	#endif
#endif
