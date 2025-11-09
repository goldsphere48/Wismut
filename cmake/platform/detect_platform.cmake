set(LINUX False)

if (UNIX)
	set(LINUX True)
endif()

if (ANDROID OR APPLE)
	set(LINUX False)
endif()
