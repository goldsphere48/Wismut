function(wismut_project)
	set(options)
	set(oneValueArgs
		NAME
		VERSION
		DESCRIPTION
	)
	
	cmake_parse_arguments(
		WISMUT_PROJ
		"${options}"
		"${oneValueArgs}"
		"${multiValueArgs}"
		${ARGN}
	)
	
	# Проверка обязательных параметров
	if (NOT WISMUT_PROJ_NAME)
		message(FATAL_ERROR "wismut_project(): NAME is required")
	endif()
	
	# Значения по умолчанию
	if (NOT WISMUT_PROJ_VERSION)
		set(WISMUT_PROJ_VERSION "0.0.1")
	endif()
	
	if (NOT WISMUT_PROJ_DESCRIPTION)
		set(WISMUT_PROJ_DESCRIPTION "Wismut Game Project")
	endif()
	
	project(${WISMUT_PROJ_NAME}
		LANGUAGES C CXX
		VERSION ${WISMUT_PROJ_VERSION}
		DESCRIPTION "${WISMUT_PROJ_DESCRIPTION}"
	)
	
	set(CMAKE_CXX_STANDARD 20 PARENT_SCOPE)
	set(CMAKE_CXX_STANDARD_REQUIRED ON PARENT_SCOPE)
	
	message(STATUS "=================================")
	message(STATUS "Wismut Project: ${WISMUT_PROJ_NAME}")
	message(STATUS "Version: ${WISMUT_PROJ_VERSION}")
	message(STATUS "Description: ${WISMUT_PROJ_DESCRIPTION}")
	message(STATUS "=================================")
endfunction()

function(add_wismut_game TARGET_NAME)
	set(options)
	set(oneValueArgs
		LIB_TYPE
	)
	set(multiValueArgs
		SOURCES
	)

	cmake_parse_arguments(
		WISMUT_GAME
		"${options}"
		"${oneValueArgs}"
		"${multiValueArgs}"
		${ARGN}
	)

	# Default library type if not specified.
	if (NOT WISMUT_GAME_LIB_TYPE)
		set(WISMUT_GAME_LIB_TYPE STATIC)
	endif()

	# SOURCES is required.
	if (NOT WISMUT_GAME_SOURCES)
		message(FATAL_ERROR
			"add_wismut_game(${TARGET_NAME} ...): SOURCES not specified")
	endif()

	add_library(${TARGET_NAME}_LIB
		${WISMUT_GAME_LIB_TYPE}
		${WISMUT_GAME_SOURCES}
	)

	target_link_libraries(${TARGET_NAME}_LIB PRIVATE Runtime)

	if (NOT ANDROID)
		set(GENERATED_NULL_CPP "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_null.cpp")
		file(WRITE ${GENERATED_NULL_CPP} "")
		add_executable(${TARGET_NAME} ${GENERATED_NULL_CPP})
		target_link_libraries(${TARGET_NAME} PRIVATE Player ${TARGET_NAME}_LIB)
	endif()

endfunction()
