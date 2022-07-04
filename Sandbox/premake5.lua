project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Ellis"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
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