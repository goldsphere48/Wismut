workspace "Wismut"
architecture "x64"
configurations { "Debug", "Release" }

startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies"

group "Dependencies"
	include "Wismut/vendor/glfw"
group ""

include "Wismut"
include "Sandbox"
