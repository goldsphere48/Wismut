IncludeDirs = {}
Libraries = {}
HeaderOnlyLibraries = {}

VULKAN_SDK = os.getenv("VULKAN_SDK")

HeaderOnlyLibraries["spdlog"] = "%{wks.location}/Wismut/vendor/spdlog/include"

IncludeDirs["magic_enum"] = "%{wks.location}/Wismut/vendor/magic_enum/include"
IncludeDirs["toml"] = "%{wks.location}/Wismut/vendor/tomlplusplus"
IncludeDirs["glfw"] = "%{wks.location}/Wismut/vendor/glfw/glfw/include"
IncludeDirs["Vulkan"] = "%{VULKAN_SDK}/Include"
IncludeDirs["magic_enum"] = "%{wks.location}/Wismut/vendor/magic_enum/include"

--Libraries--
Libraries["Vulkan"] = "%{VULKAN_SDK}/Lib/vulkan-1.lib"
--ShaderC--
Libraries["shaderc_combined_Release"] = "%{VULKAN_SDK}/Lib/shaderc_combined.lib"
Libraries["shaderc_combined_Debug"] = "%{VULKAN_SDK}/Lib/shaderc_combinedd.lib"

--SPIRV--
Libraries["SPRIV_Cross_Release"] = "%{VULKAN_SDK}/Lib/spirv-cross-core.lib"
Libraries["SPRIV_Tools_Release"] = "%{VULKAN_SDK}/Lib/SPIRV-Tools.lib"

Libraries["SPRIV_Cross_Debug"] = "%{VULKAN_SDK}/Lib/spirv-cross-cored.lib"
Libraries["SPRIV_Tools_Debug"] = "%{VULKAN_SDK}/Lib/SPIRV-Toolsd.lib"
Libraries["SPRIV_Cross_GLSL_Debug"] = "%{VULKAN_SDK}/Lib/spirv-cross-glsld.lib"