project "Runtime"
	kind "StaticLib"

	files {
		"./**.cpp",
		"./**.hpp",
		"./**.h",
		"premake5.lua"
	}

	includedirs {
		"%{wks.location}/Engine/Source/Runtime"
	}

	vpaths {
		["*"] = {"**.cpp", "**.hpp", "**.h"}
	}
