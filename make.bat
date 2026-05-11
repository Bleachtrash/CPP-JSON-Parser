@echo off
g++ *.cpp -o main

if errorlevel 1 goto ERROR
if [%1] == [-r] main.exe
exit /b 0

:ERROR
cmd /k
exit /b 1