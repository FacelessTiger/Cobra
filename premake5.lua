workspace "Ellis"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Ellis"
	location "Ellis"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EL_PLATFORM_WINDOWS",
			"EL_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "EL_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "EL_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "EL_DIST"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"Ellis/src"
	}

	links
	{
		"Ellis"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "EL_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "EL_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "EL_DIST"
		optimize "on"