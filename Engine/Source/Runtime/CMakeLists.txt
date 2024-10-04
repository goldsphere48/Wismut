set(TARGET_NAME "Runtime")

add_library(${TARGET_NAME}
	"Application/Private/GenericPlatform/GenericApplication.cpp"
	"Application/Private/Windows/WindowsApplication.cpp"
	"Application/Public/GenericPlatform/GenericApplication.h"
	"Application/Public/Windows/WindowsApplication.h"
	"Core/Public/GenericPlatform/GenericPlatform.h"
	"Core/Public/Windows/WindowsPlatform.h"
	"Core/Public/Core.h"
	"Launch/Public/EntryPoint.h"
	"Application/Public/GenericPlatform/GenericWindow.h"
	"Application/Public/Windows/WindowsWindow.h"
)

file(GLOB PUBLIC_DIRS LIST_DIRECTORIES TRUE ${CMAKE_CURRENT_SOURCE_DIR}/*/Public)
file(GLOB INTERNAL_DIRS LIST_DIRECTORIES TRUE ${CMAKE_CURRENT_SOURCE_DIR}/*/Internal)

if (WIN32)
	target_compile_definitions(${TARGET_NAME} PUBLIC WI_PLATFORM_WIN)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${PUBLIC_DIRS} PRIVATE ${INTERNAL_DIRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR})
