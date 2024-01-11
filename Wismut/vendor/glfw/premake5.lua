project "GLFW"
	language "C"
	kind "StaticLib"
	staticruntime "on"

	targetdir("%{wks.location}/bin/%{prj.name}/" .. outputdir)
	objdir("%{wks.location}/obj/%{prj.name}/" .. outputdir)
	
	files
	{
		"glfw/include/glfw3.h",
		"glfw/include/glfw3native.h",
		"glfw/src/glfw_config.h",
		"glfw/src/context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/mappings.h",
		"glfw/src/monitor.c",
		"glfw/src/vulkan.c",
		"glfw/src/window.c"
	}

	filter "system:windows"
		systemversion "latest"
		defines 
		{
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
		files
		{
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_joystick.h",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_platform.h",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"