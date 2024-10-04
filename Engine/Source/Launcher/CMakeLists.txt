set(TARGET_NAME "Launcher")

add_executable(${TARGET_NAME}
	"Private/LauncherApplication.cpp"
)

target_link_libraries(${TARGET_NAME} Runtime)

file(GLOB PUBLIC_DIRS LIST_DIRECTORIES TRUE ${CMAKE_CURRENT_SOURCE_DIR}/*/Public)
file(GLOB INTERNAL_DIRS LIST_DIRECTORIES TRUE ${CMAKE_CURRENT_SOURCE_DIR}/*/Internal)

target_include_directories(${TARGET_NAME} PUBLIC ${PUBLIC_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${INTERNAL_DIRS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR})
