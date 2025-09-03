cmake_minimum_required(VERSION 3.19)

option(CLANG_TIDY_ENABLED "Enable clang tidy checks" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CLANG_TIDY_ENABLED)
	message(STATUS "Clang Tidy Enabled")
	find_program(CLANG_TIDY_PROGRAM "clang-tidy")
	if (NOT CLANG_TIDY_PROGRAM)
		message(SEND_ERROR "Can't find clang-tidy")
	else()
		set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
	endif()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(
	Wismut
	LANGUAGES CXX
	VERSION 0.0.1
	DESCRIPTION "Wismut Game Engine"
)

if(MSVC)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_subdirectory(Engine/Source/Runtime)
add_subdirectory(Engine/Source/Sandbox)

add_custom_target(
	COPY_COMPILE_COMMANDS ALL
	${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_BINARY_DIR}/compile_commands.json
	${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
)

if(EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_BINARY_DIR}/compile_commands.json
		${CMAKE_SOURCE_DIR}/compile_commands.json
		COMMAND_ERROR_IS_FATAL ANY
	)
	message(STATUS "Copied compile_commands.json to the project root. ✅")
endif()
