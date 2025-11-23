cmake_minimum_required(VERSION 3.20)

project(SandboxProject
	LANGUAGES C CXX
	DESCRIPTION "Wismut awesome game"
)

set(GAME_TARGET_NAME Sandbox)

wismut_add_game(
	TARGET ${GAME_TARGET_NAME}
	SOURCES Source/Game.cpp
	ASSETS_DIR "Assets"
	CONFIG_DIR "Config"
)
