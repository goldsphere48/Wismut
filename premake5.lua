include "premake/solutionitems.lua"

workspace "Wismut"

	------------ Global Settings----------------

	configurations { "Debug", "Development", "Release", "Distribution" }

	targetdir("%{wks.location}/Engine/Intermediate/Binaries/%{prj.name}-%{cfg.platform}-%{cfg.buildcfg}/")
	objdir("%{wks.location}/Engine/Intermediate/Intermediate/%{prj.name}-%{cfg.platform}-%{cfg.buildcfg}/")

	characterset "Unicode"
	floatingpoint "Fast"

	language "C++"
	cppdialect "C++20"

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	------------ Platform Settings--------------

	filter "system:Windows"
		architecture "x64"
		staticruntime "Off"
		defines { "WI_PLATFORM_WIN" }
	filter {}

	filter "platforms:not Win64"
		excludes { "**/Windows/**.*" }
	filter {}

	------------ Toolset Settings----------------

	filter "toolset:msvc"
		buildoptions {
			"/EHsc",
			"/Zc:preprocessor",
			"/Zc:__cplusplus"
		}
	filter {}

	filter "action:vs*"
		flags { "MultiProcessorCompile" }
		solutionitems(".editorconfig")
	filter {}

	------------ Configurations Settings---------

	filter "configurations:Debug or Development"
		symbols "On"
		optimize "Off"
		inlining "Disabled"
		runtime "Debug"
		defines "WI_DEBUG"
	filter {}

	filter "configurations:Debug"
		editandcontinue "Off"
		sanitize "Address"
	filter {}

	filter "configurations:Release"
		symbols "Off"
		optimize "On"
		inlining "Auto"
		runtime "Release"
		defines "WI_RELEASE"
	filter {}

	filter "configurations:Distribution"
		symbols "Off"
		optimize "Full"
		inlining "Auto"
		runtime "Release"
		defines { "WI_RELEASE", "WI_DISTRIBUTION" }
	filter {}

	------------ Projects -----------------------

	startproject("Launcher")

	include "Engine/Source/Runtime"
	include "Engine/Source/Launcher"
