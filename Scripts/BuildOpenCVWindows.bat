echo off
PUSHD "..\Implementation\Dependencies\OpenCV"

mkdir "build_msvc2017_x64"
CD "build_msvc2017_x64" 
cmake ".." -G "Visual Studio 15 2017 Win64" ^
-DBUILD_SHARED_LIBS=OFF ^
-DOPENCV_EXTRA_MODULES_PATH="../../opencv_contrib/modules" ^
-DBUILD_LIST=core,face,highgui,imgcodecs,video,improc,objdetect,calib3d,features,videoio,flann,plot 

cmake --build "." --config Debug --target ALL_BUILD
cmake --build "." --config Release --target ALL_BUILD
cmake --build "." --config Debug --target INSTALL
cmake --build "." --config Release --target INSTALL

move "install" "..\..\OpenCV_Compiled"
POPD
pause
