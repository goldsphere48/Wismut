project "Launcher"
	kind "ConsoleApp"
	filter "configurations:Distribution"
		kind "WindowedApp"
	filter {}

	files {
		"Source/**.cpp",
		"Source/**.hpp",
		"Source/**.h",
		"premake5.lua"
	}

	includedirs {
		"%{wks.location}/Engine/Source/Runtime"
	}

	links {
		"Runtime"
	}

	vpaths {
		["*"] = { "**.cpp", "**.hpp", "**.h" }
	}
