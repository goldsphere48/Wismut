add_subdirectory(Game)

if (WIN32)
	add_subdirectory(Platform/Windows)
elseif (UNIX)
	add_subdirectory(Platform/Linux)
endif()
