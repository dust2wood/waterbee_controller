@echo off
REM Keil MDK command-line build
REM Requires Keil MDK-ARM installed (typically C:\Keil_v5\UV4\UV4.exe)
set UV4="C:\Keil_v5\UV4\UV4.exe"
if not exist %UV4% set UV4="C:\Keil\UV4\UV4.exe"
if not exist %UV4% (
    echo Keil UV4 not found. Install Keil MDK-ARM or edit build.bat to set correct path.
    exit /b 1
)
cd /d "%~dp0Working"
%UV4% -b ID_Solution.uvprojx -j0 -o build.log
if %ERRORLEVEL% neq 0 (
    echo Build failed.
    exit /b 1
)
echo Build OK: Working\Out\ID_Solution.axf
