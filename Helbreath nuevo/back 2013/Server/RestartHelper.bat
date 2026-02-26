@echo off
echo [RestartHelper] Esperando que el servidor se cierre...
timeout /t 2 >nul

echo [RestartHelper] Cerrando CoreServer.exe...
taskkill /f /im CoreServer.exe >nul 2>&1

echo [RestartHelper] Esperando a que el sistema libere recursos...
timeout /t 2 >nul

echo [RestartHelper] Reiniciando CoreServer.exe...
start "" "CoreServer.exe"

echo [RestartHelper] Listo. Este script se cerrará en 2 segundos.
timeout /t 2 >nul
exit
