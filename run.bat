@echo off
cd build

make
..\bin\main.exe > ..\image\%1

cd ..

echo Build and run completed.