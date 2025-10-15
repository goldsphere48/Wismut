set(GLAD_SOURCE_FILES glad/src/glad.c)

if (WIN32)
	list(APPEND GLAD_SOURCE_FILES wgl/src/glad_wgl.c)
endif()

add_library(glad STATIC ${GLAD_SOURCE_FILES})

target_include_directories(glad PUBLIC glad/include wgl/include)
