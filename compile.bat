@echo off

set "PROJECT_TEMP=%CD%\project"
set "VERSION_URL=https://raw.githubusercontent.com/aveoxfd/winwindow/main/version.txt"
set "LIB_URL=https://raw.githubusercontent.com/aveoxfd/winwindow/main/libwinwindow.a"
set "H_URL=https://raw.githubusercontent.com/aveoxfd/winwindow/main/headers/winw.h"
set "LIB_TARGET=%CD%\lib\libwinwindow.a"
set "LOCAL_VERSION_FILE=%CD%\project\version.txt"
set "REMOTE_VERSION_FILE=%PROJECT_TEMP%\remote_version.txt"

if not exist "%PROJECT_TEMP%" (
    mkdir "%PROJECT_TEMP%"
)

set "LOCAL_VERSION=0"
if exist "%LOCAL_VERSION_FILE%" (
    for /f "tokens=2 delims==" %%A in ('findstr /i "lib_version" "%LOCAL_VERSION_FILE%"') do (
    set "LOCAL_VERSION=%%A"
    )
)

echo Local version = %LOCAL_VERSION%

curl.exe -s -L -o "%REMOTE_VERSION_FILE%" "%VERSION_URL%"
if errorlevel 1 (
    echo Error: failed to download remote version
    exit /b 1
)
for /f "tokens=2 delims==" %%A in (
    'findstr /i "lib_version" "%REMOTE_VERSION_FILE%"'
) do (
    set "REMOTE_VERSION=%%A"
)
echo Remote version = %REMOTE_VERSION%

::CONVERT TO NUMBERS
call :ver2num "%LOCAL_VERSION%" LOCAL_NUM
call :ver2num "%REMOTE_VERSION%" REMOTE_NUM

echo Local numeric  = %LOCAL_NUM%
echo Remote numeric = %REMOTE_NUM%

::COMPARE
if not %REMOTE_NUM% == %LOCAL_NUM% (
    echo Updating library...
    if exist "%LIB_TARGET%" del "%LIB_TARGET%"
    curl.exe -s -L -o "%LIB_TARGET%" "%LIB_URL%"
    del "%CD%\header\window.h"
    curl.exe -s -L -0 "%CD%\header\window.h" "%H_URL%"
    if errorlevel 1 (
        echo Error: failed to download library
        exit /b 2
    )
    >"%LOCAL_VERSION_FILE%" echo lib_version=%REMOTE_VERSION%
) else (
    echo Library is up to date.
)

g++ -O2 -DW_STATIC -static -static-libgcc -static-libstdc++ -o bin/grapp.exe eng.cpp wnd.cpp -Iheader -Llib -lwinwindow -lopengl32 -lgdi32 -luser32
::del "%PROJECT_TEMP%\remote_version.txt"
exit /b 0

::FUNCTION
:ver2num
:: %1 = version, %2 = output variable
setlocal EnableDelayedExpansion
set "v=%~1"
set "maj=0"
set "min=0"
for /f "tokens=1-2 delims=." %%a in ("!v!") do (
    set "maj=%%a"
    set "min=%%b"
)
if "!min!"=="" set "min=0"
set /a num=1000*!maj!+!min! 2>nul
endlocal & set "%2=%num%"
goto :eof