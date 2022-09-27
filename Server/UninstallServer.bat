@echo off

echo.
echo +--------------------------------------------------------------------+
echo +--------------- AMCMath Server Uninstall Server Script -------------+
echo.+                                                                    +
echo + Project by Kaivalya Deshpande                                      +
echo + WinDev Batch 2021, AstroMediComp, Pune, Maharashtra, India         +
echo.+                                                                    +
echo +--------------------------------------------------------------------+

echo.
regsvr32.exe /u bin\\x64\\AMCMathProxyStub.dll
del C:\Windows\System32\AMCMathProxyStub.dll
echo Uninstalled Proxy-Stub DLL
echo.

regsvr32.exe /u bin\\x64\\AMCMathServer.dll
del C:\Windows\System32\AMCMathServer.dll
echo Uninstalled AMCMath Server DLL
echo.

del C:\Windows\System32\AMCMathServerTypeLib.tlb
echo Uninstalled AMCMath Type Library

echo.
echo Thank You!
echo.