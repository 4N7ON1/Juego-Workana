// AntiSpeedHack.cpp
#include "../Headers/AntiSpeedHack.h"
#include <windows.h>

CAntiSpeedHack::CAntiSpeedHack()
{
	m_dwLastGameTick = timeGetTime();   // Tiempo según el juego
	m_dwLastRealTick = GetTickCount();  // Tiempo real del sistema
	m_dwTolerance = 150; // Tolerancia en ms (ajustable)
}

void CAntiSpeedHack::Update(HWND hWnd)
{
	DWORD dwCurrentGameTick = timeGetTime();
	DWORD dwCurrentRealTick = GetTickCount();

	DWORD deltaGame = dwCurrentGameTick - m_dwLastGameTick;
	DWORD deltaReal = dwCurrentRealTick - m_dwLastRealTick;

	// Detección: si el tiempo del juego va mucho más rápido que el real
	if (deltaGame > deltaReal + m_dwTolerance)
	{
		MessageBoxA(hWnd, "Speed hack detectado. El cliente se cerrará.", "AntiHack", MB_ICONERROR | MB_OK);
		ExitProcess(0);
	}

	m_dwLastGameTick = dwCurrentGameTick;
	m_dwLastRealTick = dwCurrentRealTick;
}

bool CAntiSpeedHack::IsSpeedHackDetected() const
{
	DWORD deltaGame = timeGetTime() - m_dwLastGameTick;
	DWORD deltaReal = GetTickCount() - m_dwLastRealTick;
	return (deltaGame > deltaReal + m_dwTolerance);
}
