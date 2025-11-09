include_guard(GLOBAL)

function(filter_files_by_platform OUT_VAR FILE_LIST)
	if(WIN32)
		set(CURRENT_PLATFORM "Windows")
	elseif(APPLE)
		if(IOS)
			set(CURRENT_PLATFORM "iOS")
		else()
			set(CURRENT_PLATFORM "Mac")
		endif()
	elseif(ANDROID)
		set(CURRENT_PLATFORM "Android")
	elseif(LINUX)
		set(CURRENT_PLATFORM "Linux")
	else()
		set(CURRENT_PLATFORM "Unknown")
	endif()

	set(ALL_PLATFORMS "Windows" "Linux" "iOS" "Mac" "Android")
	
	set(EXCLUDE_PLATFORMS ${ALL_PLATFORMS})
	list(REMOVE_ITEM EXCLUDE_PLATFORMS ${CURRENT_PLATFORM})
	
	set(FILTERED_FILES)
	foreach(FILE ${FILE_LIST})
		set(SHOULD_EXCLUDE FALSE)
		
		string(REPLACE "\\" "/" NORMALIZED_FILE "${FILE}")
		
		foreach(PLATFORM ${EXCLUDE_PLATFORMS})
			if(NORMALIZED_FILE MATCHES "/(${PLATFORM})/|/(${PLATFORM})$")
				set(SHOULD_EXCLUDE TRUE)
				break()
			endif()
		endforeach()
		
		if(NOT SHOULD_EXCLUDE)
			list(APPEND FILTERED_FILES ${FILE})
		endif()
	endforeach()
	
	set(${OUT_VAR} ${FILTERED_FILES} PARENT_SCOPE)
endfunction()
