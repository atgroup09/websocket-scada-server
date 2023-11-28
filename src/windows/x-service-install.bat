@echo off

rem # INSTALL SERVICE
rem # 2022, atgroup09@yandex.ru


rem ## Root Directory
rem ## (set this value)
set RootDir=C:\ZVV\workspace\wslogger\server\__test\win32

rem ## Release File
rem ## (set this value)
set ReleaseExe=%RootDir%\server-service.exe

rem ## Config file
rem ## (set this value)
set ConfigFile=%RootDir%\wsscada.conf.d\config.json


rem ## Path to Log-file
rem ## (set this value)
set LogFile=%RootDir%\server.log

rem ## Service Name
rem ## (set this value)
set ServiceName=wsSCADA

rem ## Service Title
rem ## (set this value)
set ServiceTitle="wsSCADA"


rem # Rotate Log-file
if exist "%LogFile%" (
    if exist "%LogFile%.arh" (
        del %LogFileArh%
    )
    ren %LogFile% %LogFile%.arh
)

rem # Delay 1 sec 
ping -n 2 127.0.0.1>nul

rem # Install Service
sc create "%ServiceName%" binpath= "%ReleaseExe% --config %ConfigFile% --sname %ServiceName%" DisplayName= %ServiceTitle% start= "auto"

rem # Delay 5 sec 
ping -n 6 127.0.0.1>nul

rem # Start Service
sc start "%ServiceName%"                                                                                                                                                                                                           
