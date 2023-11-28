@echo off

rem # UNINSTALL SERVICE
rem # 2022, atgroup09@yandex.ru


rem ## Service Name
rem ## (set this value)
set ServiceName=wsSCADA


rem # Stop Service
sc stop "%ServiceName%"

rem # Delay 5 sec 
ping -n 6 127.0.0.1>nul

rem # Delete Service
sc delete "%ServiceName%" 
