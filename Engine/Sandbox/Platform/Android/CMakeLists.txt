if (NOT ANDROID)
	return()
endif()

add_library(Sandbox SHARED "app/src/main/cpp/AndroidMain.cpp")

target_link_libraries(Sandbox PUBLIC
	Wismut::Runtime
	${WISMUT_GAME_LIB}
)
