@echo off

rem define installation directory
set INSTALL_DIR=%APPDATA%\ms_home

if not exist "%INSTALL_DIR%\" (
    echo making folder at "%INSTALL_DIR%"
    mkdir "%INSTALL_DIR%"
)

rem TODO: Package executable in release
make build_local

echo Installing at "%INSTALL_DIR%\"

rem Copy executable
echo "copying %~dp0ms_home.exe"
copy /Y "%~dp0build\ms_home.exe" "%INSTALL_DIR%\"

rem Copy dll
echo "copying %~dp0include\bin\lua54.dll"
copy /Y "%~dp0include\bin\lua54.dll" "%INSTALL_DIR\"

rem Copy path to a file
echo %PATH% > "%INSTALL_DIR%\path.bak"

set "INSTALL_DIR=%PATH%;%INSTALL_DIR%"
reg add HKEY_CURRENT_USER\Environment /v Path /t REG_EXPAND_SZ /d "%INSTALL_DIR%" /f

echo Installed ms-home.
