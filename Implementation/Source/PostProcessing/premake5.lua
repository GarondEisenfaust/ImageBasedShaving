project "PostProcessing"
    kind "StaticLib"
    language "C++"
    targetdir "../../Bin/%{cfg.buildcfg}-%{cfg.architecture}"
    objdir "./obj/%{prj.name}-%{cfg.buildcfg}"
    debugdir "%{cfg.targetdir}"
    staticruntime "on"

    files
    {   
        "src/**.cpp",
        "src/**.h"
    } 
    
    filter{}
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        pchheader "ibspch.h"
        pchsource "src/ibspch.cpp"
        
        includedirs
        {
            "../Util/src/Exports",
            "../../Dependencies/OpenCV_Compiled/include",
            "../../Dependencies/spdlog/include",
        }

    filter{}
    filter{"Debug"}
        runtime ("Debug")
        optimize "Off"
        symbols "on"
        defines {"DEBUG"}

    filter{}
    filter{"Release"}
        runtime ("Release")
        optimize "On"
        defines {"NDEBUG"}

    filter{}
    filter{"ReleaseDev"}
        runtime ("Release")
        optimize "On"
        defines {"NDEBUGDEV"}
