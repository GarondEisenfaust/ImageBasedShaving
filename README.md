# ImageBasedShaving

Implementation of the beard removal algorithm described in the paper [Image-based Shaving](http://graphics.cs.cmu.edu/projects/imageshaving/nguyen_eurographics_08.pdf) by Minh Hoai Nguyen, Jean-Francois Lalonde, Alexei A. Efros and Fernando De la Torre. Currently only Visual Studio 2017 and 64 bit systems are supported but other systems may be supported in the future.

## Getting Started

### Prerequisites
- Visual Studio 17
- CMake

### Installing
1. Clone repository including submodules
```
git clone --recurse-submodules -j8 https://github.com/GarondEisenfaust/ImageBasedShaving.git
```
2. To build the necessary OpenCV modules run
```
Scripts/BuildOpenCVProjectWindows.bat
```
3. For Visual Studio 2017 solutionfiles run
```
Scripts/BuildProjectFilesWindows.bat
```
4. Open solution and build with Visual Studio 2017
