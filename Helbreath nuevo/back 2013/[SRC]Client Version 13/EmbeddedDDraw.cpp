#include <windows.h>
#include "MemoryModule.h"
#include "EmbeddedDDraw.h"
#include "Ressources/resource.h"

static HMEMORYMODULE g_hMemDDraw = NULL;

bool LoadEmbeddedDDraw()
{
	HMODULE hExe = GetModuleHandle(NULL);
	HRSRC hRes = FindResource(hExe, MAKEINTRESOURCE(IDR_DDRAW_DLL), RT_RCDATA);
	if (!hRes) return false;

	HGLOBAL hResLoad = LoadResource(hExe, hRes);
	if (!hResLoad) return false;

	void* pBytes = LockResource(hResLoad);
	DWORD size = SizeofResource(hExe, hRes);

	g_hMemDDraw = MemoryLoadLibrary((const unsigned char*)pBytes, size);
	return (g_hMemDDraw != NULL);
}

FARPROC GetEmbeddedDDrawProc(const char* name)
{
	if (!g_hMemDDraw) return NULL;
	return MemoryGetProcAddress(g_hMemDDraw, name);
}

void UnloadEmbeddedDDraw()
{
	if (g_hMemDDraw)
	{
		MemoryFreeLibrary(g_hMemDDraw);
		g_hMemDDraw = NULL;
	}
}
