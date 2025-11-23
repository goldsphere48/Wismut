macro(wismut_add_game)
	set(options "")
	set(oneValueArgs TARGET ASSETS_DIR CONFIG_DIR ANDROID_PACKAGE GAME_NAME)
	set(multiValueArgs SOURCES)

	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if(NOT ARG_TARGET)
		message(FATAL_ERROR "wismut_add_game: TARGET argument is required!")
	endif()

	set(GAME_LIB ${ARG_TARGET}Lib)
	set(GAME_NAME ${ARG_GAME_NAME})
	set(USER_ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${ARG_ASSETS_DIR}")

	add_library(${GAME_LIB} STATIC ${ARG_SOURCES})

	target_link_libraries(${GAME_LIB} PUBLIC Wismut::Runtime)

	target_include_directories(${GAME_LIB} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/Source"
	)

	set_target_properties(${GAME_LIB} PROPERTIES
		CXX_STANDARD 20
	)

	if (ANDROID)
		message(STATUS "Configuring Wismut Game for Android: ${ARG_TARGET}")

		add_library(${ARG_TARGET} SHARED
			${WISMUT_ROOT}/Engine/Source/Launch/Android/AndroidMain.cpp
		)

		set(ANDROID_TEMPLATE_DIR "${WISMUT_ROOT}/Engine/Templates/AndroidProject")
		set(ANDROID_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
		set(USER_MANIFEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${ARG_CONFIG_DIR}/Android/AndroidManifest.xml")
		set(GRADLE_ASSETS_DIR "${ANDROID_BUILD_DIR}/AndroidProject/app/src/main/assets")

		if(EXISTS "${ANDROID_TEMPLATE_DIR}")
			file(COPY "${ANDROID_TEMPLATE_DIR}" DESTINATION "${ANDROID_BUILD_DIR}")
			message(STATUS "Copied Android Template to ${ANDROID_BUILD_DIR}")
		else()
			message(WARNING "Wismut Android Template not found at ${ANDROID_TEMPLATE_DIR}")
		endif()

		string(REPLACE "\\" "/" WISMUT_GAME_ROOT_CMAKE_PATH "${CMAKE_SOURCE_DIR}/CMakeLists.txt")
		string(REPLACE "\\" "/" ASSETS_SRC_PATH "${USER_ASSETS_SRC_DIR}")
		set(ANDROID_ID ${ARG_ANDROID_PACKAGE})

		set(GRADLE_TEMPLATE_IN_FILE "${ANDROID_BUILD_DIR}/AndroidProject/app/build.gradle.in")
		set(GRADLE_OUTPUT_FILE	"${ANDROID_BUILD_DIR}/AndroidProject/app/build.gradle")

		file(GLOB_RECURSE TEMPLATE_IN_FILES "${ANDROID_BUILD_DIR}/AndroidProject/*.in")

		foreach(TEMPLATE_FILE ${TEMPLATE_IN_FILES})
			string(REGEX REPLACE "\\.in$" "" OUTPUT_FILE "${TEMPLATE_FILE}")

			configure_file("${TEMPLATE_FILE}" "${OUTPUT_FILE}" @ONLY)

			message(STATUS ${TEMPLATE_FILE})
			file(REMOVE "${TEMPLATE_FILE}")

			message(STATUS "Configured: ${TEMPLATE_FILE}")
		endforeach()

		message(STATUS "Configured Android build.gradle with path: ${WISMUT_GAME_ROOT_CMAKE_PATH}")

		if(EXISTS ${USER_MANIFEST_PATH})
			file(COPY ${USER_MANIFEST_PATH} DESTINATION "${ANDROID_BUILD_DIR}/AndroidProject/app/src/main")
			message(STATUS "Copying user AndroidManifest.xml to Gradle project...")
		endif()

		target_sources(${ARG_TARGET} PRIVATE 
			"${WISMUT_ROOT}/Engine/Source/Launch/Android/AndroidMain.cpp"
		)

		if(CMAKE_HOST_WIN32)
			set(GRADLEW_EXECUTABLE "gradlew.bat")
		else()
			set(GRADLEW_EXECUTABLE "./gradlew")
		endif()
		
		set(GRADLEW_PATH "${ANDROID_BUILD_DIR}/AndroidProject/${GRADLEW_EXECUTABLE}")

		find_program(ADB_EXECUTABLE adb)

		set(RUN_COMMANDS
			COMMAND ${CMAKE_COMMAND} -E chdir "${ANDROID_BUILD_DIR}/AndroidProject" ${GRADLEW_PATH} installDebug
			COMMAND ${CMAKE_COMMAND} -E echo "Cleaning Gradle cache..."
			COMMAND ${CMAKE_COMMAND} -E chdir "${ANDROID_BUILD_DIR}/AndroidProject" ${GRADLEW_PATH} clean
		)

		if(ADB_EXECUTABLE AND ANDROID_ID)
			list(APPEND RUN_COMMANDS COMMAND ${ADB_EXECUTABLE} shell am start -n ${ANDROID_ID}/android.app.NativeActivity)
		endif()

		add_custom_target(
			run_${ARG_TARGET}_android
			${RUN_COMMANDS}
			COMMENT "Deploying and Running '${ARG_TARGET}'..."
			DEPENDS ${ARG_TARGET}
		)

		set(DEBUG_COMMANDS COMMAND ${CMAKE_COMMAND} -E chdir "${ANDROID_BUILD_DIR}/AndroidProject" ${GRADLEW_PATH} installDebug)
		if(ADB_EXECUTABLE AND ANDROID_ID)
			list(APPEND DEBUG_COMMANDS 
				COMMAND ${CMAKE_COMMAND} -E echo "Launching with Wait-For-Debugger (-D)... Attach your debugger NOW!"
				COMMAND ${ADB_EXECUTABLE} shell am start -D -n ${ANDROID_ID}/android.app.NativeActivity
			)
		endif()

		add_custom_target(
			debug_${ARG_TARGET}_android
			${DEBUG_COMMANDS}
			COMMENT "Deploying and Launching '${ARG_TARGET}' in WAIT-FOR-DEBUGGER mode..."
			DEPENDS ${ARG_TARGET}
		)

		message(STATUS "Added targets: 'run_${ARG_TARGET}_android' and 'debug_${ARG_TARGET}_android'.")

	elseif(WIN32)
		message(STATUS "Configuring Wismut Game for Windows: ${ARG_TARGET}")

		add_executable(${ARG_TARGET}
			${WISMUT_ROOT}/Engine/Source/Launch/Windows/WindowsMain.cpp
		)

		if(ARG_ASSETS_DIR AND EXISTS "${USER_ASSETS_SRC_DIR}")
			add_custom_command(TARGET ${ARG_TARGET} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_directory
						"${CMAKE_CURRENT_SOURCE_DIR}/${ARG_ASSETS_DIR}"
						"$<TARGET_FILE_DIR:${ARG_TARGET}>/Assets"
				COMMENT "Copying Assets to Output Directory..."
			)
		endif()
	elseif(LINUX)
		message(STATUS "Configuring Wismut Game for Linux: ${ARG_TARGET}")

		add_executable(${ARG_TARGET}
			${WISMUT_ROOT}/Engine/Source/Launch/Linux/LinuxMain.cpp
		)

		if(ARG_ASSETS_DIR AND EXISTS "${USER_ASSETS_SRC_DIR}")
			add_custom_command(TARGET ${ARG_TARGET} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_directory
						"${CMAKE_CURRENT_SOURCE_DIR}/${ARG_ASSETS_DIR}"
						"$<TARGET_FILE_DIR:${ARG_TARGET}>/Assets"
				COMMENT "Copying Assets to Output Directory..."
			)
		endif()
	endif()

	target_link_libraries(${ARG_TARGET} PRIVATE ${GAME_LIB})

	set(BUILD_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/${WI_PLATFORM_NAME}")
	
	set_target_properties(${ARG_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/bin"
		LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/lib"
	)
	
	set_target_properties(${GAME_LIB} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/lib"
	)

	message(STATUS "Set output directories for ${ARG_TARGET} to: ${BUILD_OUTPUT_DIR}")
	target_include_directories(${ARG_TARGET} PRIVATE 
		"${WISMUT_ROOT}/Source"
	)

	set_target_properties(${ARG_TARGET} PROPERTIES CXX_STANDARD 20)
endmacro()
