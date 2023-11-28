@echo off

rem # Run Debug-release (not service)
rem # 2022, atgroup09@yandex.ru


rem ## Root Directory
rem ## (set this value)
set RootDir=C:\ZVV\workspace\wslogger\server\__test\win32

rem ## Release File
rem ## (set this value)
set ReleaseExe=%RootDir%\serverd.exe

rem ## Config file
rem ## (set this value)
set ConfigFile=%RootDir%\wsscada.conf.d\config.json

rem ## Forced Logout
rem ## = 0 - off
rem ## = 1 - on (%ReleaseExe%.log)
rem ## (set this value)
set ForcedLogOut=1


if not exist "%RootDir%" (
    echo RootDir '%RootDir%' is not exists!
    exit
)

if not exist "%ReleaseExe%" (
    echo ReleaseExe '%ReleaseExe%' is not exists!
    exit
)

if not exist "%ConfigFile%" (
    echo ReleaseExe '%ConfigFile%' is not exists!
    exit
)

if %ForcedLogOut% equ 1 (
    echo %ReleaseExe% --config %ConfigFile% --log %ReleaseExe%.log
    %ReleaseExe% --config %ConfigFile% --log %ReleaseExe%.log
) else (
    echo %ReleaseExe% --config %ConfigFile%
    %ReleaseExe% --config %ConfigFile%
)


