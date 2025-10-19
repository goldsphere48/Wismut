# Game lib

add_library(Game STATIC SandboxApp.cpp)

include(sanitizers)
enable_sanitizers(Game)

target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Game PUBLIC Runtime)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
