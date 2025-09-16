# Sandbox executable

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(ALL_FILES ${SOURCE_FILES} ${HEADER_FILES})

include(platform-filter-files)
filter_files_by_platform(PLATFORM_FILTERED_FILES  "${ALL_FILES}")

add_executable(Player ${PLATFORM_FILTERED_FILES})

include(sanitizers)
enable_sanitizers(Player)

target_include_directories(Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Player PRIVATE Runtime)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
