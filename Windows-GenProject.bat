@echo off
call Windows-Clean.bat
mkdir Build
cd Build
cmake -G "Visual Studio 15 2017 Win64" ..
cd ..