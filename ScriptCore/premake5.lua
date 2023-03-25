project "ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{prj.location}/Binaries" )
    objdir ("%{prj.location}/Intermediates" )

	files 
	{
		"Source/**.cs",
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

