@echo off

rem define installation directory
set INSTALL_DIR=%APPDATA%\ms_home

if not exist "%INSTALL_DIR%\" (
    echo making folder at "%INSTALL_DIR%"
    mkdir "%INSTALL_DIR%"
)

rem TODO: Package executable in release
REM make build_local

rem Copy executable
echo Installing at "%INSTALL_DIR%\"
copy /Y "%~dp0ms_home.exe" "%INSTALL_DIR%\"
copy /Y "%~dp0/include/bin/lua54.dll" "%INSTALL_DIR\"

rem Copy path to a file
echo %PATH% > "%INSTALL_DIR%\path.bak"

set "INSTALL_DIR=%PATH%;%INSTALL_DIR%"
reg add HKEY_CURRENT_USER\Environment /v Path /t REG_EXPAND_SZ /d "%INSTALL_DIR%" /f

echo Installed creal.
