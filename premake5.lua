workspace "Nameless"
    architecture "x86_64"
    configurations
    {
        "Debug", 
        "Release"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
    location "Engine"
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