@echo off

rem # RESTART SERVICE
rem # 2022, atgroup09@yandex.ru


rem ## Root Directory
rem ## (set this value)
set RootDir=C:\ZVV\workspace\wslogger\server\__test\win32


rem ## Path to Log-file
rem ## (set this value)
set LogFile=%RootDir%\server.log

rem ## Service Name
rem ## (set this value)
set ServiceName=wsSCADA


rem # Stop Service
sc stop "%ServiceName%"

rem # Delay 5 sec 
ping -n 6 127.0.0.1>nul

rem # Rotate Log-file
if exist "%LogFile%" (
    if exist "%LogFile%.arh" (
        del %LogFileArh%
    )
    ren %LogFile% %LogFile%.arh
)

rem # Delay 1 sec 
ping -n 2 127.0.0.1>nul

rem # Start Service
sc start "%ServiceName%"
