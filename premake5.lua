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
IncludeDir["assimp"] = "3rdParty/assimp/include"
IncludeDir["Glad"] = "3rdParty/Glad/include"
IncludeDir["GLFW"] = "3rdParty/GLFW/include"
IncludeDir["imgui"] = "3rdParty/imgui"

include "3rdParty/assimp"
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

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.imgui}"
    }

    links
    {
        "assimp",
        "Glad",
        "GLFW",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "NL_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
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

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "%{IncludeDir.assimp}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}",
        "Engine/Source",
        "%{prj.name}/Source"
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

