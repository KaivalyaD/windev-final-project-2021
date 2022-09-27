@echo off

echo.
echo +-------------------------------------------------------------------+
echo +----------------- AMCMath Client Uninstall Script -----------------+
echo.+                                                                   +
echo + Project by Kaivalya Deshpande                                     +
echo + WinDev Batch 2021, AstroMediComp, Pune, Maharashtra, India        +
echo.+                                                                   +
echo +-------------------------------------------------------------------+

echo.
echo Uninstalling DotNetTrigServer
echo.
del DotNetTrigServer.dll
RegAsm.exe dotnet\DotNetTrigServer.dll /unregister
del dotnet\DotNetTrigServer.dll
del dotnet\import\DotNetTrigServer.tlb

echo =====================================================================
echo.
echo Cleaning up...
del *.exe
echo.
