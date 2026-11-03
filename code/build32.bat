@echo off

IF NOT EXIST ..\build\32bit mkdir ..\build\32bit
pushd ..\build\32bit
REM 32-BIT
cl -nologo -FC -Z7 ..\..\code\win32_client.cpp /link -subsystem:console,5.2 Ws2_32.lib 
popd
