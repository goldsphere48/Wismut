workspace "Wismut"
	architecture "x64"
	configurations 
	{ 
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Wismut"
include "Sandbox"