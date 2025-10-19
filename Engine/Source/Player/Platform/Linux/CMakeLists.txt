# Sandbox Linux executable

if(UNIX)
	add_executable(Player LinuxMain.cpp)

	include(sanitizers)
	enable_sanitizers(Player)

	target_include_directories(Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
	target_link_libraries(Player PRIVATE Runtime Game)

	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
endif()
