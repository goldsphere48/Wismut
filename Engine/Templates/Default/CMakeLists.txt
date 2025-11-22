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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${CMAKE_BUILD_TYPE}/${WI_PLATFORM_NAME}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${CMAKE_BUILD_TYPE}/${WI_PLATFORM_NAME}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${CMAKE_BUILD_TYPE}/${WI_PLATFORM_NAME}/lib)

add_subdirectory({path_to_wismut})

add_library(${WISMUT_GAME_LIB} Source/Game.cpp)

target_link_libraries(${WISMUT_GAME_LIB} PUBLIC Wismut::Runtime)

if (WIN32)
	add_executable(${WISMUT_GAME} ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsMain.cpp)
elseif (ANDROID)
	add_library(${WISMUT_GAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Android/app/src/main/cpp/AndroidMain.cpp)
elseif (LINUX)
	add_executable(${WISMUT_GAME} ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Linux/LinuxMain.cpp)
endif()

target_link_libraries(${WISMUT_GAME} PRIVATE Wismut::Runtime ${WISMUT_GAME_LIB})
