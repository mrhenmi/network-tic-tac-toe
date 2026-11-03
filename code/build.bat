@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

REM cl -nologo -FC -Z7 ..\code\Client_TicTacToe.cpp

cl -nologo -DSLOW=1 -DSERVER=1 -FC -Z7 ..\code\win32_server.cpp /link Ws2_32.lib
REM 64-BIT
cl -nologo -DSLOW=1 -DCLIENT=1 -FC -Z7 ..\code\win32_client.cpp /link Ws2_32.lib
popd