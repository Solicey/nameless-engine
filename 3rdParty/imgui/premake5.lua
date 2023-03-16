project "imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
	targetdir ("%{prj.location}/bin/" .. outputdir )
    objdir ("%{prj.location}/bin-int/" .. outputdir )

	files
	{
        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imgui_demo.cpp"
    }

    defines
    {
        -- "IMGUI_API=__declspec(dllexport)"
    }
    
	filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        
