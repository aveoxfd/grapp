@echo off
setlocal

set "VERS_URL=https://raw.githubusercontent.com/aveoxfd/winwindow/main/vers"
set "LIB_URL=https://raw.githubusercontent.com/aveoxfd/winwindow/main/libwinwindow.a"
set "LIB_TARGET=%CD%\lib\libwinwindow.a"
set "LOCAL_VER_FILE=%CD%\local_version.txt"

:: read local version (if present)
set "LOCAL_VER=0.0"
if exist "%LOCAL_VER_FILE%" (
  set /p LOCAL_VER=<"%LOCAL_VER_FILE%"
  if "%LOCAL_VER%"=="" set "LOCAL_VER=0.0"
)

:: download remote vers to a temporary file
set "TMP=%TEMP%\vers_tmp_%RANDOM%.txt"
curl -s -L -o "%TMP%" "%VERS_URL%"
if errorlevel 1 (
  echo Error: failed to download vers from %VERS_URL%
  del "%TMP%" >nul 2>&1
  exit /b 2
)

:: find the line containing dot_a_version
for /f "usebackq delims=" %%L in ("%TMP%") do (
  echo %%L | findstr /i "dot_a_version" >nul
  if not errorlevel 1 (
    set "line=%%L"
    goto :gotline
  )
)
echo Error: dot_a_version not found in vers file.
del "%TMP%" >nul 2>&1
exit /b 3

:gotline
:: extract value after '=' and remove quotes/spaces
for /f "tokens=2 delims==" %%A in ("%line%") do set "REMOTE_VER=%%~A"
set "REMOTE_VER=%REMOTE_VER:"=%"
set "REMOTE_VER=%REMOTE_VER: =%"

:: convert versions to numbers for comparison
call :ver2num "%LOCAL_VER%" LOCAL_NUM
call :ver2num "%REMOTE_VER%" REMOTE_NUM

echo Local numeric: %LOCAL_NUM%
echo Remote numeric: %REMOTE_NUM%

if %REMOTE_NUM% GTR %LOCAL_NUM% (
  echo Remote is newer - downloading library...
  if not exist "%CD%\lib" mkdir "%CD%\lib"
  curl -L -o "%LIB_TARGET%" "%LIB_URL%"
  if errorlevel 1 (
    echo Error: failed to download library.
    del "%TMP%" >nul 2>&1
    exit /b 5
  )
  echo Downloaded: %LIB_TARGET%
  >"%LOCAL_VER_FILE%" echo %REMOTE_VER%
) else (
  echo Local is up-to-date. No download.
)

del "%TMP%" >nul 2>&1
exit /b 0

::  function: version -> number (major*1000 + minor) 
:ver2num
:: args: %1 = version (e.g. 1.2.3), %2 = name of output variable
setlocal EnableDelayedExpansion
set "v=%~1"
for /f "tokens=1-3 delims=." %%a in ("!v!") do (
  set "maj=%%a"
  set "min=%%b"
  set "patch=%%c"
)
if "!maj!"=="" set "maj=0"
if "!min!"=="" set "min=0"
set /a num=(1000 * !maj!) + !min!
endlocal & set "%2=%num%"
goto :eof
