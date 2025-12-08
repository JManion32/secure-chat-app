@echo off
setlocal


REM =======================================================
REM MOVE TO PROJECT ROOT
REM =======================================================
cd /d "%~dp0.."


REM =======================================================
REM CONFIG
REM =======================================================
set SRV_SRC=server\src
set CLI_SRC=client\src
set COMMON=common\src
set BIN=bin\windows


if not exist "%BIN%" mkdir "%BIN%"


REM Ensure MinGW g++ exists
where g++ >nul 2>nul
if errorlevel 1 (
   echo ERROR: g++ not found! Install MinGW-w64 and add to PATH.
   exit /b 1
)


REM Ensure Qt MinGW directory is set
if "%QT_DIR%"=="" (
   echo ERROR: Set QT_DIR to your Qt MinGW directory.
   echo Example:  set QT_DIR=C:\Qt\6.6.0\mingw_64
   exit /b 1
)


set CFLAGS=-std=c++17 -O2
set LIBS=-lws2_32 -luser32 -lgdi32




REM =======================================================
REM CLEAN
REM =======================================================
if /I "%1"=="clean" goto clean




REM =======================================================
REM CLIENT ONLY
REM =======================================================
if /I "%1"=="client" goto build_client




REM =======================================================
REM SERVER ONLY
REM =======================================================
if /I "%1"=="server" goto build_server_only




REM =======================================================
REM DEFAULT: BUILD BOTH
REM =======================================================
goto build_both






:build_server
echo Building server...


g++ %CFLAGS% ^
   "%SRV_SRC%\main.cpp" ^
   "%SRV_SRC%\client_thread.cpp" ^
   "%SRV_SRC%\server.cpp" ^
   "%COMMON%\socket_handler.cpp" ^
   "%COMMON%\thread_handler.cpp" ^
   "%COMMON%\protocol.cpp" ^
   -o "%BIN%\server.exe" ^
   %LIBS%


if errorlevel 1 exit /b 1
echo Server built successfully.
goto :EOF






:build_client
echo Building client...


g++ %CFLAGS% ^
   -I "%QT_DIR%\include" ^
   -I "%QT_DIR%\include\QtWidgets" ^
   -I "%QT_DIR%\include\QtCore" ^
   -I "%QT_DIR%\include\QtGui" ^
   -I "%QT_DIR%\include\QtNetwork" ^
   -I "client\include" ^
   -L "%QT_DIR%\lib" ^
   "%CLI_SRC%\client.cpp" ^
   "%CLI_SRC%\main.cpp" ^
   "%COMMON%\socket_handler.cpp" ^
   "%COMMON%\thread_handler.cpp" ^
   "%COMMON%\protocol.cpp" ^
   -o "%BIN%\client.exe" ^
   -lQt6Core -lQt6Gui -lQt6Widgets -lQt6Network ^
   %LIBS%


if errorlevel 1 exit /b 1
echo Client built successfully.
goto :EOF








:build_server_only
call :build_server
goto end




:build_both
call :build_server
call :build_client
goto end






:clean
echo Cleaning...
del /q "%BIN%\*.exe" 2>nul
echo Clean done.
goto end






:end
echo.
echo Done.
exit /b 0
