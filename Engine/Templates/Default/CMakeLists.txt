cmake_minimum_required(VERSION 3.19)

set(WISMUT_GAME_LIB {name}_LIB)
set(WISMUT_GAME {name})

project(${WISMUT_GAME}Project
	LANGUAGES C CXX
	DESCRIPTION "${WISMUT_GAME_LIB} Game"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory({path_to_wismut})

add_library(${WISMUT_GAME_LIB} Source/Game.cpp)
target_link_libraries(${WISMUT_GAME_LIB} PUBLIC Wismut::Runtime)

if (WIN32)
	add_subdirectory(Platform/Windows)
elseif (ANDROID)
	add_subdirectory(Platform/Android)
elseif (UNIX)
	add_subdirectory(Platform/Linux)
endif()


