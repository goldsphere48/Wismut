cmake_minimum_required(VERSION 3.28.0)

project(
	Wismut
	VERSION 1.0.0
	LANGUAGES CXX
	DESCRIPTION "Wismt Game Engine")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_subdirectory(Engine/Source/Runtime)
add_subdirectory(Engine/Source/Launcher)
