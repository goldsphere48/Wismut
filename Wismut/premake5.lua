project "Wismut"
	kind "SharedLib"
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
		"vendor/spdlog/include"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"
		defines
		{
			"WI_PLATFORM_WINDOWS",
			"WI_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/bin/" .. outputdir .. "/Sandbox")
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
