@echo off

rem # Qt-exec deployment process

rem ## Path to file windeployqt.exe
set DeployExe=C:\Qt\Qt5.9.2\5.9.2\mingw53_32\bin\windeployqt.exe

rem ## Path to directory of Qt-release
set ReleaseDir=C:\ZVV\workspace\wslogger\server\__test\win32


if not exist "%DeployExe%" (
    echo File %DeployExe% is not exists!
    exit
)

if not exist "%ReleaseDir%" (
    echo Directory %ReleaseDir% is not exists!
    exit
)

%DeployExe% %ReleaseDir%
