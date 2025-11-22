if(WIN32)
	set(DXC_PLATFORM "Windows")
elseif(UNIX AND NOT APPLE)
	set(DXC_PLATFORM "Linux")
else()
	message(FATAL_ERROR "Unsupported platform for DXC")
endif()

set(TARGET_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})
if(NOT TARGET_PROCESSOR)
	set(TARGET_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

if(TARGET_PROCESSOR MATCHES "aarch64|ARM64|arm64")
	set(DXC_ARCH "arm64")
elseif(TARGET_PROCESSOR MATCHES "x86_64|AMD64|x64")
	set(DXC_ARCH "x64")
elseif(TARGET_PROCESSOR MATCHES "i686|i386|x86|Win32")
	set(DXC_ARCH "x86")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(DXC_ARCH "x64")
else()
	set(DXC_ARCH "x86")
endif()

set(DXC_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${DXC_PLATFORM}/bin/${DXC_ARCH}" CACHE INTERNAL "")
set(DXC_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${DXC_PLATFORM}/lib/${DXC_ARCH}")
set(DXC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(dxc SHARED IMPORTED)

target_include_directories(dxc INTERFACE ${DXC_INCLUDE_DIR})

function(wismut_copy_dxc TARGET_NAME)
	if(WIN32)
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${DXC_BIN_DIR}/dxcompiler.dll"
			"$<TARGET_FILE_DIR:${TARGET_NAME}>"
			COMMENT "Copying dxcompiler.dll to output directory"
		)
		if(EXISTS "${DXC_BIN_DIR}/dxil.dll")
			add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${DXC_BIN_DIR}/dxil.dll"
				"$<TARGET_FILE_DIR:${TARGET_NAME}>"
				COMMENT "Copying dxil.dll to output directory"
			)
		endif()
	endif()
endfunction()
