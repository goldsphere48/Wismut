project "Wismut"
kind "StaticLib"
language "C++"
cppdialect "C++20"
toolset("clang")

targetdir("%{wks.location}/bin/%{prj.name}/" .. outputdir)
objdir("%{wks.location}/obj/%{prj.name}/" .. outputdir)

files {
	"src/**.cpp",
	"src/**.h",
	"Resources/Shaders/**.shadercfg",
	"Resources/Shaders/**.glsl",
	HeaderOnlyLibraries.spdlog,
}

pchheader "wipch.h"

filter "action:vs*"
	pchsource "src/wipch.cpp"

includedirs {
	"src/",
	HeaderOnlyLibraries.spdlog,
	IncludeDirs.toml,
	IncludeDirs.glfw,
	IncludeDirs.Vulkan,
	IncludeDirs.magic_enum,
	IncludeDirs.glm,
}

links {
	"GLFW",
	Libraries.Vulkan
}

defines {
	"VULKAN_HPP_NO_CONSTRUCTORS"
}

filter "configurations:Debug"
	runtime "Debug"
	symbols "On"
	defines "WI_DEBUG"
	links {
		Libraries.SPRIV_Cross_Debug,
		Libraries.SPRIV_Tools_Debug,
		Libraries.SPRIV_Cross_GLSL_Debug,
		Libraries.shaderc_combined_Debug
	}

filter "configurations:Release"
	runtime "Release"
	optimize "On"
	defines "WI_RELEASE"
	links {
		Libraries.SPRIV_Cross_Release,
		Libraries.SPRIV_Tools_Release,
		Libraries.SPRIV_Cross_GLSL_Release,
		Libraries.shaderc_combined_Release
	}

filter "system:windows"
	systemversion "latest"
	defines "WI_PLATFORM_WIN"