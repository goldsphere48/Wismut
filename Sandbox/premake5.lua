project "Sandbox"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
toolset("clang")

targetdir("%{wks.location}/bin/%{prj.name}/" .. outputdir)
objdir("%{wks.location}/obj/%{prj.name}/" .. outputdir)

files {
	"src/**.cpp",
	"src/**.h",
}

pchheader "wipch.h"

filter "action:vs*"
	pchsource "src/wipch.cpp"

links {
	"Wismut"
}

includedirs {
	"%{wks.location}/Wismut/src",
	HeaderOnlyLibraries.spdlog,
	IncludeDirs.toml,
	IncludeDirs.magic_enum,
	IncludeDirs.glm,
}

filter "configurations:Debug"
	runtime "Debug"
	symbols "On"
	defines "WI_DEBUG"

filter "configurations:Release"
	runtime "Release"
	optimize "On"
	defines "WI_RELEASE"

filter "system:windows"
	systemversion "latest"