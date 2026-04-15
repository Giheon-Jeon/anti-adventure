@echo off
setlocal
echo ========================================
echo [BUILD] Rebuilding TRPG Game...
echo ========================================

:: 1. Terminate existing process if running
taskkill /IM trpg.exe /F >nul 2>&1

:: 2. Compile (All source files with warnings enabled)
gcc -Wall -Wextra -o trpg.exe src/main.c src/character.c src/combat.c src/event.c src/shop.c src/utils.c src/village.c src/crafting.c src/ability.c src/skill.c src/encyclopedia.c -Iinclude

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation failed! Please check your code.
    echo ========================================
    pause
    exit /b %errorlevel%
)

echo.
echo [SUCCESS] Build completed!
echo [RUN] Launching trpg.exe...
echo ========================================

:: 3. Run the game
start trpg.exe
exit /b 0
