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
IncludeDir["entt"] = "3rdParty/entt/src"
IncludeDir["stb"] = "3rdParty/stb"
IncludeDir["ImGuizmo"] = "3rdParty/ImGuizmo"

-- Need to compile
IncludeDir["assimp"] = "3rdParty/assimp/include"
IncludeDir["Glad"] = "3rdParty/Glad/include"
IncludeDir["GLFW"] = "3rdParty/GLFW/include"
IncludeDir["imgui"] = "3rdParty/imgui"
IncludeDir["yamlcpp"] = "3rdParty/yamlcpp"

include "3rdParty/assimp"
include "3rdParty/Glad"
include "3rdParty/GLFW"
include "3rdParty/imgui"
include "3rdParty/yamlcpp"

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    pchheader "nlpch.h"
    pchsource "%{prj.name}/Source/nlpch.cpp"

    targetdir ("%{prj.location}/bin/" .. outputdir )
    objdir ("%{prj.location}/bin-int/" .. outputdir )

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",
        "%{IncludeDir.ImGuizmo}/ImGuizmo.h",
        "%{IncludeDir.ImGuizmo}/ImGuizmo.cpp",
    }

    defines 
    {  
        "NL_ASSETS_PATH=%{wks.location}/Assets",
        "NL_ENGINE_PATH=%{prj.abspath}"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.yamlcpp}/include",
        "%{IncludeDir.entt}",
        "%{IncludeDir.stb}"
    }

    links
    {
        "assimp",
        "Glad",
        "GLFW",
        "imgui",
        "yamlcpp",
        "opengl32.lib"
    }

    filter "files:3rdParty/ImGuizmo/**.cpp"
	    flags { "NoPCH" }

    -- wks_abspath = os.getcwd("%{wks.location}")

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
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{prj.location}/bin/" .. outputdir )
    objdir ("%{prj.location}/bin-int/" .. outputdir )

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
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.yamlcpp}/include",
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
        postbuildcommands 
        { 
            "copy %{prj.location}\\bin\\" .. outputdir .. "\\*.exe %{wks.location}Debug\\"
        }
    
    filter "configurations:Release"
        defines "NL_RELEASE"
        runtime "Release"
        optimize "on"
        postbuildcommands 
        { 
            "copy %{prj.location}\\bin\\" .. outputdir .. "\\*.exe %{wks.location}Release\\"
        }

