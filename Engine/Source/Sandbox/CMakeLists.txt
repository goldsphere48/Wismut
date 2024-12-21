# Sandbox executable

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(Sandbox ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Sandbox PRIVATE Runtime)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
