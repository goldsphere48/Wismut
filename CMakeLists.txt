cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
