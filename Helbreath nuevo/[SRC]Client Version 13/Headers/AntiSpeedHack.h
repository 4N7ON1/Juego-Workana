// AntiSpeedHack.h
#pragma once
#include <windows.h>

class CAntiSpeedHack
{
private:
	DWORD m_dwLastGameTick;
	DWORD m_dwLastRealTick;
	DWORD m_dwTolerance;

public:
	CAntiSpeedHack();

	void Update(HWND hWnd);
	bool IsSpeedHackDetected() const;
};
