@echo off
rmdir C:\dev\app\deploy\windows
mkdir C:\dev\app\deploy\windows
copy C:\dev\app\build-vs\app\app.exe C:\dev\app\deploy\windows\app.exe
C:\Qt\6.1.2\msvc2019_64\bin\windeployqt.exe C:\dev\app\deploy\windows\app.exe
