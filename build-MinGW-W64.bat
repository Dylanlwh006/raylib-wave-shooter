@echo off
setlocal

pushd "%~dp0build"
premake5.exe gmake
if errorlevel 1 goto :error
popd

pushd "%~dp0"
mingw32-make clean
if errorlevel 1 goto :error
mingw32-make config=debug_x64
if errorlevel 1 goto :error
popd

echo Build completed: bin\Debug\raylib-wave-shooter.exe
pause
exit /b 0

:error
popd 2>nul
echo Build failed.
pause
exit /b 1
