add_library(WismutCompilerSettings INTERFACE)
add_library(Wismut::CompilerSettings ALIAS WismutCompilerSettings)

if (MSVC)
	target_compile_definitions(WismutCompilerSettings INTERFACE
		_CRT_SECURE_NO_WARNINGS
		UNICODE
		_UNICODE
	)

	target_compile_options(WismutCompilerSettings INTERFACE
		/Zc:preprocessor
		/permissive-
		$<$<CONFIG:Debug>:/MDd>
		$<$<CONFIG:Release>:/MD>
	)

	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" PARENT_SCOPE)

	add_compile_options(/Zc:preprocessor)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(WismutCompilerSettings INTERFACE
		-Wall
		-Wextra
		$<$<CONFIG:Debug>:-g>
		$<$<CONFIG:Release>:-O3>
	)
endif()

if(ENABLE_ASAN OR ENABLE_UBSAN OR ENABLE_TSAN OR ENABLE_MSAN)

	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
		set(sanitizer_flags "")
		
		if(ENABLE_ASAN)
			list(APPEND sanitizer_flags "address")
		endif()
		
		if(ENABLE_UBSAN)
			list(APPEND sanitizer_flags "undefined")
		endif()
		
		if(ENABLE_TSAN)
			list(APPEND sanitizer_flags "thread")
		endif()
		
		if(ENABLE_MSAN)
			list(APPEND sanitizer_flags "memory")
		endif()
		
		if(sanitizer_flags)
			string(REPLACE ";" "," sanitizers_joined "${sanitizer_flags}")
			
			target_compile_options(WismutCompilerSettings INTERFACE
				$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-fsanitize=${sanitizers_joined}>
				$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-fno-omit-frame-pointer>
			)
			
			target_link_options(WismutCompilerSettings INTERFACE
				$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-fsanitize=${sanitizers_joined}>
			)
			
			message(STATUS "Wismut: Sanitizers enabled (${sanitizers_joined}) for Debug/RelWithDebInfo")
		endif()
		
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND MSVC)
		if(ENABLE_ASAN)
			target_compile_options(WismutCompilerSettings INTERFACE
				$<$<CONFIG:Debug>:-fsanitize=address>
				$<$<CONFIG:Debug>:-fno-omit-frame-pointer>
			)
			
			target_link_options(WismutCompilerSettings INTERFACE
				$<$<CONFIG:Debug>:-fsanitize=address>
			)
			
			message(STATUS "Wismut: AddressSanitizer enabled (clang-cl) for Debug")
		endif()
		
	elseif(MSVC)
		if(ENABLE_ASAN)
			target_compile_options(WismutCompilerSettings INTERFACE
				$<$<CONFIG:Debug>:/fsanitize=address>
				$<$<CONFIG:Debug>:/Zi>
				$<$<CONFIG:Debug>:/EHsc>
			)
			
			target_link_options(WismutCompilerSettings INTERFACE
				$<$<CONFIG:Debug>:/DEBUG>
				$<$<CONFIG:Debug>:/INCREMENTAL:NO>
			)
			
			message(STATUS "Wismut: AddressSanitizer enabled (MSVC) for Debug")
		endif()
	endif()
	
endif()

if(WIN32)
	target_compile_definitions(WismutCompilerSettings INTERFACE WI_PLATFORM_WIN)
elseif(ANDROID)
	target_compile_definitions(WismutCompilerSettings INTERFACE WI_PLATFORM_ANDROID)
elseif(LINUX)
	target_compile_definitions(WismutCompilerSettings INTERFACE WI_PLATFORM_LINUX)
endif()

target_compile_definitions(WismutCompilerSettings INTERFACE
	$<$<CONFIG:Debug>:WI_DEBUG>
	$<$<CONFIG:Release>:WI_RELEASE>
)
