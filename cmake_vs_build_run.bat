mkdir vsbuild
cd vsbuild
del Release\OpenCVImageStitching.exe
cmake -G "Visual Studio 10 Win64" ../source/
C:\Windows\Microsoft.NET\Framework\v4.0.30319\msbuild.exe OpenCVImageStitching.sln /p:Configuration=Release
cd Release
Physics.exe
pause
