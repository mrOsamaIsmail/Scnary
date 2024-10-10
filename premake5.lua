workspace "Scnry"
    configurations {"Debug","Release"}
    architecture "x86_64"
    include "Dependencies/yaml-cpp"
    project "Scnry"
        language "C++"
        cppdialect "C++17"
        kind "ConsoleApp"
        files {
            "./include/**.h",
            "./include/**.hpp",
            "./Dependencies/yaml-cpp/include/**.h",
            "./Dependencies/yaml-cpp/include/**.hpp",
            "./Scnry.cpp",
            "./MainTest.cpp"
        }
        includedirs
        {
            "./Dependencies/yaml-cpp/include",
            "./include"
        }
        filter "configurations:Debug"
        libdirs
        {
            "./Dependencies/yaml-cpp/bin/Debug"
        }
        links
            "yaml-cpp.lib"
        

        filter "configurations:Release"
        libdirs
        {
            "./Dependencies/yaml-cpp/bin/Release/"
        }
        links{
            "yaml-cpp.lib"
        }

        filter{}
        