workspace "Ellis"
	architecture "x64"
	startproject "Sandbox"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Ellis/vendor/GLFW/include"
IncludeDir["Glad"] = "Ellis/vendor/Glad/include"
IncludeDir["ImGui"] = "Ellis/vendor/imgui"
IncludeDir["glm"] = "Ellis/vendor/glm"
IncludeDir["stb_image"] = "Ellis/vendor/stb_image"

group "Dependencies"
	include "Ellis/vendor/GLFW"
	include "Ellis/vendor/Glad"
	include "Ellis/vendor/imgui"
group ""

project "Ellis"
	location "Ellis"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "elpch.h"
	pchsource "Ellis/src/elpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EL_PLATFORM_WINDOWS",
			"EL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "EL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EL_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Ellis/vendor/spdlog/include",
		"Ellis/src",
		"Ellis/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Ellis"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "EL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EL_DIST"
		runtime "Release"
		optimize "on"