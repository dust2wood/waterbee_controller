@echo off
setlocal

for %%I in ("%~dp0.") do set "ROOT=%%~fI"
set "BUILD_DIR=%ROOT%\\build"
set "TOOLCHAIN=%ROOT%\\arm-none-eabi.cmake"

for /f "delims=" %%I in ('where arm-none-eabi-gcc 2^>nul') do set "CC=%%I"
if not defined CC (
    echo arm-none-eabi-gcc not found in PATH.
    exit /b 1
)

where cmake >nul 2>nul
if errorlevel 1 (
    echo CMake not found in PATH.
    exit /b 1
)

where ninja >nul 2>nul
if errorlevel 1 (
    echo Ninja not found in PATH.
    exit /b 1
)

if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
mkdir "%BUILD_DIR%"

cmake -S "%ROOT%" -B "%BUILD_DIR%" -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="%TOOLCHAIN%"
if errorlevel 1 (
    echo CMake configure failed.
    exit /b 1
)

cmake --build "%BUILD_DIR%"
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build OK:
echo   %BUILD_DIR%\ID_Solution.elf
echo   %BUILD_DIR%\ID_Solution.hex
echo   %BUILD_DIR%\ID_Solution.bin
