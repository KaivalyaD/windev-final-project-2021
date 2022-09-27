@echo off

echo.
echo +-------------------------------------------------------------------+
echo +------------------- AMCMath Server Build Script -------------------+
echo.+                                                                   +
echo + Project by Kaivalya Deshpande                                     +
echo + WinDev Batch 2021, AstroMediComp, Pune, Maharashtra, India        +
echo.+                                                                   +
echo +-------------------------------------------------------------------+

echo.
echo Building C++ server DLL
echo.
cl.exe /c /EHsc *.cpp
link.exe *.obj /dll /def:AMCMathServer.def user32.lib gdi32.lib kernel32.lib ole32.lib oleaut32.lib advapi32.lib /SUBSYSTEM:WINDOWS /MACHINE:x64

echo =====================================================================
echo.
echo Building the Proxy-Stub DLL
echo.
midl.exe /h AMCMathProxyStubHeader.h /iid AMCMathProxyStubGuids.c /dlldata AMCMathProxyStubDllData.c /proxy AMCMathProxyStub.c AMCMathServerTypeLib.idl
cl.exe /c /EHsc /D REGISTER_PROXY_DLL AMCMathProxyStub.c AMCMathProxyStubDllData.c AMCMathProxyStubGuids.c
link.exe AMCMathProxyStub.obj AMCMathProxyStubDllData.obj AMCMathProxyStubGuids.obj /dll /def:AMCMathProxyStub.def user32.lib ole32.lib oleaut32.lib rpcns4.lib rpcrt4.lib advapi32.lib /SUBSYSTEM:WINDOWS /MACHINE:x64

echo =====================================================================
echo.
echo Moving binaries to .\bin\x64
echo.
mkdir bin\\x64
move /Y AMCMathServerTypeLib.tlb bin\\x64
move /Y AMCMathProxyStub.dll bin\\x64
move /Y AMCMathServer.dll bin\\x64

echo =====================================================================
echo.
echo Cleaning up...
del *.c
del AMCMathProxyStubHeader.h
del *.obj
del *.lib
del *.exp

echo.
regsvr32.exe bin\x64\AMCMathProxyStub.dll
copy bin\x64\AMCMathProxyStub.dll C:\Windows\System32
echo Installed Proxy-Stub DLL
echo.

regsvr32.exe bin\x64\AMCMathServer.dll
copy bin\x64\AMCMathServer.dll C:\Windows\System32
echo Installed AMCMath Server DLL
echo.

copy bin\x64\AMCMathServerTypeLib.tlb C:\Windows\System32
echo Installed AMCMath Type Library
echo.