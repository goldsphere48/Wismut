# Runtime library
option(COMPILE_OPENGL_RENDERER "Compile OpenGL renderer" ON)

# Сбор файлов
file(GLOB_RECURSE ALL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE ALL_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

filter_files_by_platform(PLATFORM_FILTERED_FILES "${ALL_SOURCE_FILES};${ALL_HEADER_FILES}")

add_library(WismutRuntime STATIC ${PLATFORM_FILTERED_FILES})
add_library(Wismut::Runtime ALIAS WismutRuntime)

# OpenGL Renderer
if(COMPILE_OPENGL_RENDERER)
	target_link_libraries(WismutRuntime PRIVATE glad)
	
	if(ANDROID)
		find_library(GLES_LIBRARY
			NAMES GLESv3
			DOC "OpenGL ES library"
		)
		
		if(NOT GLES_LIBRARY)
			message(FATAL_ERROR "OpenGL ES library not found.")
		endif()
		
		target_link_libraries(WismutRuntime  PRIVATE ${GLES_LIBRARY} EGL)
	else()
		find_package(OpenGL REQUIRED)
		target_link_libraries(WismutRuntime PRIVATE ${OPENGL_LIBRARIES})
	endif()
endif()

set(PLATFORM_LIBRARIES "")

# Platform dependencies
if(ANDROID)
	list(APPEND PLATFORM_LIBRARIES android log)
	include(platform/android/android_native_app_glue)
	find_android_native_app_glue(NATIVE_APP_GLUE)
	list(APPEND PLATFORM_LIBRARIES ${NATIVE_APP_GLUE})
elseif(LINUX)
	find_package(X11 REQUIRED)
	find_library(X11_XCB_LIB NAMES X11-xcb REQUIRED)
	list(APPEND PLATFORM_LIBRARIES 
		xcb ${X11_XCB_LIB} xcb-keysyms X11::X11
	)
endif()

target_include_directories(WismutRuntime PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(WismutRuntime PUBLIC
	${PLATFORM_LIBRARIES}
	Wismut::CompilerSettings
	glm::glm
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PLATFORM_FILTERED_FILES})
