@echo off
setlocal

for %%I in ("%~dp0.") do set "ROOT=%%~fI"
set "BUILD_DIR=%ROOT%\\build"
set "IMAGE=%BUILD_DIR%\\ID_Solution.bin"

where STM32_Programmer_CLI >nul 2>nul
if errorlevel 1 (
    echo STM32_Programmer_CLI not found in PATH.
    exit /b 1
)

if not exist "%IMAGE%" (
    echo Firmware image not found: %IMAGE%
    echo Run build.bat first.
    exit /b 1
)

STM32_Programmer_CLI -c port=SWD mode=UR -w "%IMAGE%" 0x08000000 -v -rst
if errorlevel 1 (
    echo Flash failed.
    exit /b 1
)

echo Flash OK: %IMAGE%
