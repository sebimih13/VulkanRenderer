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
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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

    links
    {
        "glfw",
        "vulkan-1.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        libdirs
        {
            "vendor/VulkanSDK/lib"
        }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

