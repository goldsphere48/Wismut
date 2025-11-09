if (NOT ANDROID)
	return()
endif()

add_library(Player SHARED "app/src/main/cpp/AndroidMain.cpp")

target_link_libraries(Player PUBLIC
	Runtime
	Sandbox_LIB
)
