@echo off

set VSWHERE_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe

if not exist "%VSWHERE_PATH%" (
    echo vswhere.exe not found. Make sure Visual Studio 2017 or later is installed.
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE_PATH%" -latest -products * -requires Microsoft.Component.MSBuild -property installationVersion`) do (
    set VS_VERSION=%%i
)

if not defined VS_VERSION (
    echo Visual Studio not found.
    exit /b 1
)

if "%VS_VERSION:~0,2%" == "15" (
    set VS_NAME=vs2017
    echo Visual Studio 2017 detected.
) else if "%VS_VERSION:~0,2%" == "16" (
    set VS_NAME=vs2019
    echo Visual Studio 2019 detected.
) else if "%VS_VERSION:~0,2%" == "17" (
    set VS_NAME=vs2022
    echo Visual Studio 2022 detected.
) else (
    echo Visual Studio version %VS_VERSION% is not supported by this script.
    exit /b 1
)

set PREMAKE_PATH=%~dp0Engine\Build\Premake5\Binaries\Win64\premake5.exe

if not exist "%PREMAKE_PATH%" (
    echo premake5.exe not found in %PREMAKE_PATH%.
    exit /b 1
)

"%PREMAKE_PATH%" %VS_NAME%

if %errorlevel% neq 0 (
    echo Failed to run premake5.
    exit /b %errorlevel%
) else (
    echo premake5 executed successfully.
)
