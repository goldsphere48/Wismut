# Runtime library

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(Runtime ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(Runtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
	target_compile_definitions(Runtime PUBLIC WI_PLATFORM_WIN)
elseif(UNIX)
	target_compile_definitions(Runtime PUBLIC WI_PLATFORM_LINUX)
	target_link_libraries(Runtime PRIVATE xcb xcb-keysyms)
endif()

target_compile_definitions(Runtime PUBLIC 
	$<$<CONFIG:Debug>:WI_DEBUG>
	$<$<CONFIG:Release>:WI_RELEASE>
	UNICODE
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
