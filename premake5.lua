include "Dependencies.lua"

workspace "Ellis"
	architecture "x86_64"
	startproject "Ellis-Editor"
	
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
	include "Ellis/vendor/Box2D"
	include "Ellis/vendor/GLFW"
	include "Ellis/vendor/Glad"
	include "Ellis/vendor/imgui"
	include "Ellis/vendor/yaml-cpp"
group ""

include "Ellis"
include "Sandbox"
include "Ellis-Editor"