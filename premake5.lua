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

    project "ScnrySingle"
        language "C++"
        cppdialect "C++17"
        kind "consoleApp"
        includedirs
        {
            "./include",
             "./Dependencies/yaml-cpp/include/"

        }
        files {
            "./include/**.h",
            "./include/**.hpp",
            "./Dependencies/yaml-cpp/include/**.h",
            "./Dependencies/yaml-cpp/include/**.hpp",
            "./Dependencies/yaml-cpp/src/**.h",
            "./Dependencies/yaml-cpp/src/**.cpp",
            "./Scnry.cpp",
            "./MainTest.cpp"
        }
       
        filter "configurations:Debug"
        

        filter "configurations:Release"
        

        filter{}
        