@echo off
REM Keil MDK flash download
REM Requires J-Link or ST-Link connected, Keil MDK-ARM installed
set UV4="C:\Keil_v5\UV4\UV4.exe"
if not exist %UV4% set UV4="C:\Keil\UV4\UV4.exe"
if not exist %UV4% (
    echo Keil UV4 not found. Install Keil MDK-ARM or edit flash.bat to set correct path.
    exit /b 1
)
cd /d "%~dp0Working"
%UV4% -f ID_Solution.uvprojx -j0
echo Flash download completed.
