workspace "Nameless"
    architecture "x86_64"
    startproject "Editor"
    configurations
    {
        "Debug", 
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

-- Headers only
IncludeDir["glm"] = "3rdParty/glm"
IncludeDir["spdlog"] = "3rdParty/spdlog/include"

-- Need to compile
IncludeDir["Glad"] = "3rdParty/Glad/include"
IncludeDir["GLFW"] = "3rdParty/GLFW/include"
IncludeDir["ImGui"] = "3rdParty/imgui"

include "3rdParty/Glad"
include "3rdParty/GLFW"
include "3rdParty/imgui"

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    pchheader "nlpch.h"
    pchsource "%{prj.name}/Source/nlpch.cpp"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.imgui}"
    }

    links
    {
        "Glad",
        "GLFW",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "NL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "NL_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "NL_RELEASE"
        runtime "Release"
        optimize "on"


project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "Engine/Source",
        "%{IncludeDir.spdlog}"
    }

    links
    {
        "Engine"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "NL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "NL_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "NL_RELEASE"
        runtime "Release"
        optimize "on"

