workspace "Wsimut"
architecture "x64"
configurations { "Debug", "Release" }

startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies"

include "Wismut"
include "Sandbox"
