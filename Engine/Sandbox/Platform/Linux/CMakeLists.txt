if (NOT LINUX)
	return()
endif()

add_executable(${WISMUT_GAME} LinuxMain.cpp)

target_include_directories(${WISMUT_GAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${WISMUT_GAME} PRIVATE Wismut::Runtime ${WISMUT_GAME_LIB})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
