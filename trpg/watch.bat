@echo off
title TRPG Auto-Dev Watcher
cls
echo ======================================================
echo    🛡️ TRPG 전자동 개발 모드 가동 중...
echo    코드(.c, .h)를 저장하면 자동으로 재빌드됩니다.
echo ======================================================
echo.

:: PowerShell을 이용한 실시간 파일 감시 루프
powershell -NoProfile -Command ^
    "$watcher = New-Object System.IO.FileSystemWatcher; " ^
    "$watcher.Path = '.'; " ^
    "$watcher.IncludeSubdirectories = $true; " ^
    "$watcher.Filter = '*.*'; " ^
    "$watcher.EnableRaisingEvents = $true; " ^
    "Write-Host '>> 감시 시작! (종료하려면 Ctrl+C)' -ForegroundColor Cyan; " ^
    "$action = { " ^
    "    $path = $Event.SourceEventArgs.FullPath; " ^
    "    if ($path -match '\.(c|h)$') { " ^
    "        Write-Host ('[' + (Get-Date -Format 'HH:mm:ss') + '] 변경 감지: ' + $Event.SourceEventArgs.Name) -ForegroundColor Yellow; " ^
    "        cmd /c build.bat; " ^
    "    } " ^
    "}; " ^
    "Register-ObjectEvent $watcher 'Changed' -Action $action | Out-Null; " ^
    "Register-ObjectEvent $watcher 'Created' -Action $action | Out-Null; " ^
    "while($true) { Start-Sleep -Seconds 1 }"
