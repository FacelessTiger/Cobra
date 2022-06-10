include "Dependencies.lua"

workspace "Ellis"
	architecture "x64"
	startproject "Sandbox"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Ellis/vendor/GLFW"
	include "Ellis/vendor/Glad"
	include "Ellis/vendor/imgui"
group ""

include "Ellis"
include "Sandbox"