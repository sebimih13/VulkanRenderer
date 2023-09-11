workspace "VulkanRenderer"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }



-- Ouput directories for bin and intermediate files
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glfw"] = "vendor/glfw/include"
IncludeDir["VulkanSDK"] = "vendor/VulkanSDK/include"
IncludeDir["glm"] = "vendor/glm"



project "glfw"
    location "bin-int/project-files"
    kind "StaticLib"
    language "C"
    
    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "vendor/glfw/include/GLFW/glfw3.h",
        "vendor/glfw/include/GLFW/glfw3native.h",
        "vendor/glfw/src/**.c",
        "vendor/glfw/src/**.h"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        defines 
        { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"



project "VulkanRenderer"
    location "VulkanRenderer"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.hpp",
        "%{prj.name}/**.vert",
        "%{prj.name}/**.frag"
    }

    includedirs
    {
        "%{IncludeDir.glfw}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.glm}"
    }

    libdirs
    {
        "vendor/VulkanSDK/lib"
    }

    links
    {
        "glfw",
        "vulkan-1.lib"
    }

    -- prebuild command to compile .vert and .frag files
    filter "files:**.vert or **.frag"
        -- A message to display while this build step is running (optional)
        buildmessage 'Compiling %{file.name}'

        -- One or more commands to run (required)
        buildcommands 
        {
            '"shaders/glslc.exe" "%{file.relpath}" -o "%{cfg.targetdir}/shaders/%{file.name}.spv"', -- TODO : compiled shaders for bin directory
            '"shaders/glslc.exe" "%{file.relpath}" -o "%{file.directory}/%{file.name}.spv"'
        }

        -- One or more outputs resulting from the build (required)
        buildoutputs
        { 
            '%{cfg.targetdir}/shaders/%{file.name}.spv',    -- TODO : compiled shaders for bin directory
            '%{file.directory}/%{file.name}.spv'
        }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

