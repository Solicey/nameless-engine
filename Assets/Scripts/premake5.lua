local RootDir = '../..'

workspace "CSharpScripts"
	architecture "x86_64"
	startproject "CSharpScripts"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

project "CSharpScripts"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Binaries")
	objdir ("Intermediates")

	files 
	{
		"**.cs"
	}

	include (RootDir .. "/ScriptCore")

	links
	{
		"ScriptCore"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"


