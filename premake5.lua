include "/vendor/premake/premake_customization/solution_items.lua"
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
	include "Ellis/vendor/msdf-atlas-gen"
	include "Ellis/vendor/imgui"
	include "Ellis/vendor/yaml-cpp"
group ""

group "Core"
	include "Ellis"
	include "Ellis-ScriptCore"
group ""

group "Tools"
	include "Ellis-Editor"
group ""

group "Misc"
	include "Sandbox"
group ""