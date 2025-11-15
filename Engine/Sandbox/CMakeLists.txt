cmake_minimum_required(VERSION 3.19)

set(WISMUT_GAME Sandbox)
set(WISMUT_GAME_LIB Sandbox_LIB)

add_library(${WISMUT_GAME_LIB} SandboxApp.cpp)
target_link_libraries(${WISMUT_GAME_LIB} PUBLIC Wismut::Runtime)

if (WIN32)
	add_subdirectory(Platform/Windows)
elseif (ANDROID)
	add_subdirectory(Platform/Android)
elseif (UNIX)
	add_subdirectory(Platform/Linux)
endif()
