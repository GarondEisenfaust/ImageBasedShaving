project "PCA"
    kind "ConsoleApp"
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
    
    debugargs
    {
      
    }
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        pchheader "ibspch.h"
        pchsource "src/ibspch.cpp"
        
        includedirs
        {
            "../../Dependencies/OpenCV_Compiled/Include",
            "../../Dependencies/spdlog/include",
            "../Util/src/Exports"
        }

        libdirs 
        { 
            "../../Dependencies/OpenCV_Compiled/x64/vc15/staticlib"
        }

    filter{}
    filter{"Debug"}
        runtime ("Debug")
        optimize "Off"
        symbols "on"
        defines {"DEBUG"}
        links 
        {
            "opencv_core410d",
            "opencv_face410d",
            "opencv_highgui410d",
            "opencv_imgcodecs410d",
            "opencv_video410d",
            "opencv_imgproc410d",
            "opencv_objdetect410d",
            "opencv_calib3d410d",
            "opencv_features2d410d",
            "opencv_videoio410d",
            "opencv_flann410d",
            "ippiwd",
            "ippicvmt",
            "ittnotifyd",
            "libjpeg-turbod",
            "zlibd",
            "libpngd",
            "libtiffd",
            "libjasperd",
            "IlmImfd",
            "libwebpd",
            "Util"
        }

    filter{}
    filter{"Release or ReleaseDev"}
        runtime ("Release")
        optimize "On"
        defines {"NDEBUG"}
        links 
        {
            "opencv_core410",
            "opencv_face410",
            "opencv_highgui410",
            "opencv_imgcodecs410",
            "opencv_video410",
            "opencv_imgproc410",
            "opencv_objdetect410",
            "opencv_calib3d410",
            "opencv_features2d410",
            "opencv_videoio410",
            "opencv_flann410",
            "ippiw",
            "ippicvmt",
            "ittnotify",
            "libjpeg-turbo",
            "zlib",
            "libpng",
            "libtiff",
            "libjasper",
            "IlmImf",
            "libwebp",
            "Util"
        }

    filter{}
    filter{"ReleaseDev"}
        runtime ("Release")
        optimize "On"
        defines {"NDEBUGDEV"}
        links 
        {
        }