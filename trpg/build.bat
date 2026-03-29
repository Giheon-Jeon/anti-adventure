@echo off
echo Compiling TRPG Game...
gcc -o trpg_game.exe src/main.c src/character.c src/combat.c src/event.c src/shop.c src/utils.c src/village.c -Iinclude
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)
echo Compilation successful! trpg_game.exe created.
echo Running game...
trpg_game.exe
pause
