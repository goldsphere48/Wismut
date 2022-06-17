project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Wismut/vendor/spdlog/include",
		"%{wks.location}/Wismut/src"
	}

	links
	{
		"Wismut"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"
		defines
		{
			"WI_PLATFORM_WINDOWS",
			"WI_BUILD_DLL"
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines "WI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines "WI_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		defines "WI_DIST"
		optimize "On"
