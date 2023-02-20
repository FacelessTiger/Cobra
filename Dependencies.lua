VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Ellis/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Ellis/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/Ellis/vendor/Box2D/include"
IncludeDir["filewatch"] = "%{wks.location}/Ellis/vendor/filewatch"
IncludeDir["GLFW"] = "%{wks.location}/Ellis/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Ellis/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Ellis/vendor/imgui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Ellis/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/Ellis/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Ellis/vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/Ellis/vendor/mono/include"
IncludeDir["shaderc"] = "%{wks.location}/Ellis/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Ellis/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["msdfgen"] = "%{wks.location}/Ellis/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Ellis/vendor/msdf-atlas-gen/msdf-atlas-gen"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["mono"] = "%{wks.location}/Ellis/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_combinedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_combined.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "BCrypt.lib"