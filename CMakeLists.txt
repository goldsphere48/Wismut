cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(options)

project(Wismut
	LANGUAGES C CXX
	VERSION 0.0.1
	DESCRIPTION "Wismut Game Engine"
)

# General settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Modules
include(platform/detect_platform)
include(platform/utils)
include(compiler/compiler_interface)
include(wismut)

# Subdirectories
add_subdirectory(Engine/ThirdParty/OpenGL)
add_subdirectory(Engine/Source/Runtime)
add_subdirectory(Sandbox)
add_subdirectory(Engine/Source/Launch)

if(EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
	file(COPY_FILE
		${CMAKE_BINARY_DIR}/compile_commands.json
		${CMAKE_SOURCE_DIR}/compile_commands.json
	)
	message(STATUS "Copied compile_commands.json to project root ✅")
endif()

if(CLANG_TIDY_ENABLED)
	find_program(CLANG_TIDY_PROGRAM "clang-tidy")
	if(NOT CLANG_TIDY_PROGRAM)
		message(SEND_ERROR "Can't find clang-tidy")
	else()
		message(STATUS "Clang Tidy enabled: ${CLANG_TIDY_PROGRAM}")
		set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PROGRAM}" PARENT_SCOPE)
	endif()
endif()
