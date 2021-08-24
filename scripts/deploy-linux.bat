@echo off
rmdir C:\dev\app\deploy\linux
mkdir C:\dev\app\deploy\linux
mkdir C:\dev\app\deploy\linux\rhel
mkdir C:\dev\app\deploy\linux\deb
copy C:\dev\app\scripts\setup-rhel.sh C:\dev\app\deploy\linux\rhel\setup-rhel.sh
copy C:\dev\app\scripts\setup-deb.sh C:\dev\app\deploy\linux\deb\setup-deb.sh
copy C:\dev\app\build-deb\app\app C:\dev\app\deploy\linux\deb\app
copy C:\dev\app\build-rhel\app\app C:\dev\app\deploy\linux\rhel\app
