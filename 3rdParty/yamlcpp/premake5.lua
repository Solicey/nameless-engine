project "yamlcpp"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	targetdir ("%{prj.location}/bin/" .. outputdir )
    objdir ("%{prj.location}/bin-int/" .. outputdir )

	files
	{
		"src/**.h",
		"src/**.cpp",
		
		"include/**.h"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
