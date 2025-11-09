function(find_android_native_app_glue OUT_VAR)
	find_path(NATIVE_APP_GLUE_DIR android_native_app_glue.h
		HINTS
			$ENV{ANDROID_NDK}/sources/android/native_app_glue
			${ANDROID_NDK}/sources/android/native_app_glue
			${CMAKE_ANDROID_NDK}/sources/android/native_app_glue
	)
	
	if(NOT NATIVE_APP_GLUE_DIR)
		message(FATAL_ERROR "android_native_app_glue not found.")
	endif()
	
	add_library(android_native_app_glue STATIC 
		${NATIVE_APP_GLUE_DIR}/android_native_app_glue.c
	)

	target_include_directories(android_native_app_glue PUBLIC 
		${NATIVE_APP_GLUE_DIR}
	)
	
	set(${OUT_VAR} android_native_app_glue PARENT_SCOPE)
endfunction()
