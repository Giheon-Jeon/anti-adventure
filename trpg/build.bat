@echo off
setlocal
echo ========================================
echo [🛠 빌드 시작] 변경 사항을 반영 중입니다...
echo ========================================

:: 1. 기존 프로세스 종료 (실행 중일 경우)
taskkill /IM trpg.exe /F >nul 2>&1

:: 2. 컴파일 (모든 .c 파일 포함)
gcc -o trpg.exe src/*.c -Iinclude

if %errorlevel% neq 0 (
    echo.
    echo ❌ [오류] 컴파일에 실패했습니다! 코드를 확인해주세요.
    echo ========================================
    pause
    exit /b %errorlevel%
)

echo.
echo ✅ [성공] 빌드가 완료되었습니다!
echo 🚀 게임을 실행합니다...
echo ========================================

:: 3. 게임 실행
start trpg.exe
exit /b 0
