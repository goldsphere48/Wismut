# cmake/sanitizers.cmake
include_guard(GLOBAL)

option(ENABLE_ASAN	"Enable ASan"	OFF)
option(ENABLE_UBSAN "Enable UBSan"	OFF)

function(enable_sanitizers target)
	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
		set(san_list "")
		if (ENABLE_ASAN)
			list(APPEND san_list "address")
		endif()
		if (ENABLE_UBSAN)
			list(APPEND san_list "undefined")
		endif()
		if (san_list)
			string(REPLACE ";" "," san_joined "${san_list}")
			target_compile_options(${target} PRIVATE
				-fsanitize=${san_joined}
				-fno-omit-frame-pointer
			)
			target_link_options(${target} PRIVATE
				-fsanitize=${san_joined}
			)
		endif()

	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND MSVC) # clang-cl
		if (ENABLE_ASAN)
			target_compile_options(${target} PRIVATE
				-fsanitize=address
				-fno-omit-frame-pointer
			)
			target_link_options(${target} PRIVATE
				-fsanitize=address
			)
		endif()
	elseif (MSVC)
		if (ENABLE_ASAN)
			target_compile_options(${target} PRIVATE
				/fsanitize=address
				/Zi
				/EHsc
			)
			target_link_options(${target} PRIVATE
				/DEBUG
				/INCREMENTAL:NO
			)
			get_property(rt TARGET ${target} PROPERTY MSVC_RUNTIME_LIBRARY)
			if (NOT rt)
				set_property(TARGET ${target} PROPERTY MSVC_RUNTIME_LIBRARY
					"MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
			endif()
		endif()
	endif()
endfunction()
