project "yaml-cpp"
    kind "StaticLib"
    language "C++"
    targetdir "./bin/%{cfg.buildcfg}"  -- Ensure target output is categorized by configuration (Debug/Release)
    objdir "bin-int/%{cfg.buildcfg}"  -- Ensuring intermediate objects are separated by configuration
    files
    {
        --"src/**.h",
        "include/**.h",
        "src/**.h",
        "src/**.cpp"
    }
    includedirs
    {
        "include"
    }
    
    defines {"YAML_CPP_STATIC_DEFINE"}
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
