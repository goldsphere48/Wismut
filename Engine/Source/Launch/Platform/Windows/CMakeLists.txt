if (NOT WIN32)
	return()
endif()

add_library(Player STATIC WindowsMain.cpp)

target_include_directories(Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Player PRIVATE Runtime)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
