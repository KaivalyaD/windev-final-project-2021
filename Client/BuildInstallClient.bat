@echo off

echo.
echo +-------------------------------------------------------------------+
echo +--------------- AMCMath Client Build/Install Script ---------------+
echo.+                                                                   +
echo + Project by Kaivalya Deshpande                                     +
echo + WinDev Batch 2021, AstroMediComp, Pune, Maharashtra, India        +
echo.+                                                                   +
echo +-------------------------------------------------------------------+

echo.
echo Building DotNetTrigServer.dll
echo.
csc.exe /t:library /out:dotnet\DotNetTrigServer.dll dotnet\DotNetTrigServer.cs
copy dotnet\DotNetTrigServer.dll .
RegAsm.exe dotnet\DotNetTrigServer.dll /tlb
mkdir dotnet\import
move dotnet\DotNetTrigServer.tlb dotnet\import\

echo =====================================================================
echo.
echo Building AMCMathServices.exe
echo.
cl.exe /c /EHsc AMCMathServices.cpp
rc.exe AMCMathServices_Resources.rc
link.exe AMCMathServices.obj AMCMathServices_Resources.res user32.lib gdi32.lib kernel32.lib ole32.lib oleaut32.lib advapi32.lib comctl32.lib /SUBSYSTEM:WINDOWS /MACHINE:x64

echo =====================================================================
echo.
echo Cleaning up...
del *.tlh *.obj *.res

echo.
