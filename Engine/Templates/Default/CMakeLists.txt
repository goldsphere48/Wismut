cmake_minimum_required(VERSION 3.20)

project({name}Project
	LANGUAGES C CXX
	DESCRIPTION "Wismut awesome game"
)

add_subdirectory({path_to_wismut})

set(GAME_TARGET_NAME {name})

wismut_add_game(
	TARGET ${GAME_TARGET_NAME}
	SOURCES Source/Game.cpp
	ANDROID_PACKAGE "com.{company_android_package}.{name_android_package}"
	ASSETS_DIR "Assets"
	CONFIG_DIR "Config"
)
