mkdir build
cd build
del OpenCVImageStitching.exe
"C:\Program Files (x86)\CMake 2.8\bin\cmake.exe" -G "MinGW Makefiles" ../source/
mingw32-make
OpenCVImageStitching.exe
pause