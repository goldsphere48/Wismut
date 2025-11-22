cmake_minimum_required(VERSION 3.19)
project(WismutCLI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)

add_executable(wismut-cli ${SOURCE_FILES})

target_include_directories(wismut-cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source)
