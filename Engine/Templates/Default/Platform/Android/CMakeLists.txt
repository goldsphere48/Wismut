if (NOT ANDROID)
	return()
endif()

add_library(${WISMUT_GAME} SHARED "app/src/main/cpp/AndroidMain.cpp")

target_link_libraries(${WISMUT_GAME} PUBLIC
	Wismut::Runtime
	${WISMUT_GAME_LIB}
)
