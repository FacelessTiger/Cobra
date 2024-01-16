include "/vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Cobra"
	architecture "x86_64"
	startproject "Cobra-Editor"
	
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
	include "vendor/premake"
	include "Cobra/vendor/Box2D"
	include "Cobra/vendor/GLFW"
	include "Cobra/vendor/Glad"
	include "Cobra/vendor/msdf-atlas-gen"
	include "Cobra/vendor/imgui"
	include "Cobra/vendor/yaml-cpp"
group ""

group "Core"
	include "Cobra"
	include "Cobra-ScriptCore"
group ""

group "Tools"
	include "Cobra-Editor"
	include "Cobra-ProjectManager"
group ""

group "Misc"
	include "Sandbox"
group ""