project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Ellis/vendor/spdlog/include",
		"%{wks.location}/Ellis/src",
		"%{wks.location}/Ellis/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Ellis"
	}

	filter "system:windows"
		systemversion "latest"

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