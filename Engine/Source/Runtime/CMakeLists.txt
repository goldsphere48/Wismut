option(COMPILE_OPENGL_RENDERER "Compile open gl renderer" ON)

file(GLOB_RECURSE ALL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE ALL_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(ALL_FILES ${ALL_SOURCE_FILES} ${ALL_HEADER_FILES})

if(WIN32)
	list(APPEND RUNTIME_COMPILE_DEFINITIONS WI_PLATFORM_WIN)
elseif(UNIX)
	find_package(X11 REQUIRED)
	find_library(X11_XCB_LIB NAMES X11-xcb REQUIRED)

	list(APPEND RUNTIME_COMPILE_DEFINITIONS WI_PLATFORM_LINUX)
	list(APPEND RUNTIME_LINK_LIBS
		xcb
		${X11_XCB_LIB}
		xcb-keysyms
		X11::X11
	)
endif()

include(platform-filter-files)
filter_files_by_platform(PLATFORM_FILTERED_FILES "${ALL_FILES}")

add_library(Runtime ${PLATFORM_FILTERED_FILES})

include(sanitizers)
enable_sanitizers(Runtime)

set(RUNTIME_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
set(RUNTIME_COMPILE_DEFINITIONS
	$<$<CONFIG:Debug>:WI_DEBUG>
	$<$<CONFIG:Release>:WI_RELEASE>
	${RUNTIME_COMPILE_DEFINITIONS}
	UNICODE
)

target_include_directories(Runtime PUBLIC ${RUNTIME_INCLUDE_DIRS})
target_compile_definitions(Runtime PUBLIC ${RUNTIME_COMPILE_DEFINITIONS})
target_link_libraries(Runtime PRIVATE ${RUNTIME_LINK_LIBS})

if (COMPILE_OPENGL_RENDERER)
	find_package(OpenGL REQUIRED)
	target_link_libraries(Runtime PRIVATE glad ${OPENGL_LIBRARIES})
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
