@echo off
gcc -std=c11 -Wall -Wextra -O2 main.c -o program.exe -lm
if errorlevel 1 (
    echo Kompilacja nie powiodla sie.
    exit /b 1
)
echo Kompilacja zakonczona powodzeniem.
