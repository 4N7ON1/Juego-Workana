// -------------------------------------------------------------- 
//                      Helbreath Client 						  
//
//                      1998.10 by Soph
//
// --------------------------------------------------------------


#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h> 
#include <winbase.h>
#include <mmsystem.h>
#include <process.h>
#include "../Ressources/resource.h"
#include "../Headers/XSocket.h"
#include "../Headers/winmain.h"
#include "../Headers/Game.h"
#include "../Headers/Event.h"
#include <fstream>
#include <iostream>
#include <iosfwd>
#include <vector>
#include <sstream>
//added resolution
#include "..\Resolution\Resolution.h"
#include "../Headers/AntiSpeedHack.h"
#include "..\\MemoryModule.h"
#include "..\\EmbeddedDDraw.h"
#include "../Headers/commonds.h"
#define CNC_REG_PATH "Software\\cnc-ddraw"


// ========================================================
// FUNCIONES AUXILIARES DE REGISTRO
// ========================================================

static void WriteRegString(const char* key, const std::string& value)
{
	HKEY hKey;
	DWORD disp;
	if (RegCreateKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &disp) == ERROR_SUCCESS)
	{
		RegSetValueExA(hKey, key, 0, REG_SZ, (const BYTE*)value.c_str(), (DWORD)value.size() + 1);
		RegCloseKey(hKey);
	}
}

static void WriteRegBool(const char* key, bool value)
{
	HKEY hKey;
	DWORD disp;
	if (RegCreateKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &disp) == ERROR_SUCCESS)
	{
		DWORD v = value ? 1 : 0;
		RegSetValueExA(hKey, key, 0, REG_DWORD, (const BYTE*)&v, sizeof(DWORD));
		RegCloseKey(hKey);
	}
}

static void WriteRegInt(const char* key, int value)
{
	HKEY hKey;
	DWORD disp;
	if (RegCreateKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &disp) == ERROR_SUCCESS)
	{
		DWORD v = value;
		RegSetValueExA(hKey, key, 0, REG_DWORD, (const BYTE*)&v, sizeof(DWORD));
		RegCloseKey(hKey);
	}
}

// ========================================================
// CREAR DEFAULTS (ANTES DE INIT)
// ========================================================

static void CreateDefaultConfig()
{
	HKEY hKey;
	DWORD disp;
	if (RegCreateKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &hKey, &disp) != ERROR_SUCCESS)
		return;

	// Si no hay valores en la clave, la llenamos
	DWORD count = 0;
	RegQueryInfoKeyA(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &count, NULL, NULL, NULL, NULL);

	if (count == 0)
	{
		WriteRegInt("width", 1024);
		WriteRegInt("height", 768);
		WriteRegBool("fullscreen", true);
		WriteRegBool("windowed", false);
		WriteRegBool("maintas", false);
		WriteRegString("aspect_ratio", "");
		WriteRegBool("boxing", false);
		WriteRegInt("maxfps", -1);
		WriteRegBool("vsync", false);
		WriteRegBool("adjmouse", true);
		WriteRegString("shader", "Shaders\\interpolation\\catmull-rom-bilinear.glsl");
		WriteRegInt("posX", 303);
		WriteRegInt("posY", 171);
		WriteRegString("renderer", "auto");
		WriteRegBool("devmode", false);
		WriteRegBool("border", true);
		WriteRegInt("savesettings", 1);
		WriteRegBool("resizable", true);
		WriteRegInt("d3d9_filter", 2);
		WriteRegInt("anti_aliased_fonts_min_size", 13);
		WriteRegInt("min_font_size", 0);
		WriteRegInt("center_window", 1);
		WriteRegString("inject_resolution", "");
		WriteRegBool("vhack", false);
		WriteRegString("screenshotdir", ".\\Screenshots\\");
		WriteRegBool("toggle_borderless", false);
		WriteRegBool("toggle_upscaled", true);
		WriteRegBool("noactivateapp", false);
		WriteRegInt("maxgameticks", 0);
		WriteRegInt("limiter_type", 0);
		WriteRegInt("minfps", 0);
		WriteRegBool("nonexclusive", true);
		WriteRegBool("singlecpu", true);
		WriteRegInt("resolutions", 0);
		WriteRegInt("fixchilds", 2);
		WriteRegBool("hook_peekmessage", false);
		WriteRegBool("fix_alt_key_stuck", false);
		WriteRegBool("game_handles_close", false);
		WriteRegBool("fix_not_responding", false);
		WriteRegBool("no_compat_warning", false);
		WriteRegInt("guard_lines", 200);
		WriteRegInt("max_resolutions", 0);
		WriteRegBool("lock_surfaces", false);
		WriteRegBool("flipclear", false);
		WriteRegBool("rgb555", false);
		WriteRegBool("no_dinput_hook", false);
		WriteRegBool("center_cursor_fix", false);
		WriteRegBool("lock_mouse_top_left", false);
		WriteRegInt("hook", 4);
		WriteRegBool("limit_gdi_handles", false);
		WriteRegBool("remove_menu", false);
		WriteRegInt("refresh_rate", 0);
		WriteRegBool("terminate_process", false);
		WriteRegInt("keytogglefullscreen", 0x0D);
		WriteRegInt("keytogglefullscreen2", 0x00);
		WriteRegInt("keytogglemaximize", 0x22);
		WriteRegInt("keytogglemaximize2", 0x00);
		WriteRegInt("keyunlockcursor1", 0x09);
		WriteRegInt("keyunlockcursor2", 0xA3);
		WriteRegInt("keyscreenshot", 0x2C);
		WriteRegString("configlang", "auto");
		WriteRegString("configtheme", "Windows10");
		WriteRegBool("hide_compat_tab", false);
		WriteRegBool("allow_reset", true);
	}

	RegCloseKey(hKey);
}

// ========================================================
// INIT (lee argumentos y actualiza registro)
// ========================================================

bool Init()
{



	// === Intentar eliminar ddraw.ini (silencioso) ===
	try
	{
		char iniPath[MAX_PATH];
		GetModuleFileNameA(NULL, iniPath, sizeof(iniPath) - 1);

		char* p = strrchr(iniPath, '\\');
		if (p) *(p + 1) = '\0';
		strcat_s(iniPath, sizeof(iniPath), "ddraw.ini");

		// Intentar borrar el archivo
		if (!DeleteFileA(iniPath))
		{
			// Si falla, ignoramos cualquier error
			DWORD err = GetLastError();
			(void)err; // evitar warning, no se usa
		}
	}
	catch (...)
	{
		// Silenciar cualquier excepción (por seguridad)
	}


	// === Obtener argumentos ===
	LPWSTR* szArglist;
	int nArgs = 0;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	auto ToAnsi = [](LPCWSTR wstr) -> std::string {
		int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
		std::string str(len - 1, '\0');
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, &str[0], len, nullptr, nullptr);
		return str;
	};

	std::string api = "auto";
	std::string mode = "fullscreen";
	std::string vsync = "true";

	for (int i = 0; i < nArgs; i++)
	{
		std::string a = ToAnsi(szArglist[i]);
		if (i == 1)
		{
			if (a == "1") { b640x480 = true; b800x600 = false; b1024x768 = false; }
			else if (a == "2") { b640x480 = false; b800x600 = true; b1024x768 = false; }
			else if (a == "3") { b640x480 = false; b800x600 = false; b1024x768 = true; }
		}
		else if (i == 2)
		{
			if (a == "ddraw") api = "auto";
			else if (a == "dx9") api = "direct3d9";
			else if (a == "ogl") api = "opengl";
			else if (a == "gdi") api = "gdi";
		}
		else if (i == 3) mode = ToAnsi(szArglist[i]);
		else if (i == 4) vsync = ToAnsi(szArglist[i]);
	}

#ifdef _DEBUG
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	printf("Launcher params received:\n");
	for (int i = 0; i < nArgs; i++)
		printf("  Arg %d = %s\n", i, ToAnsi(szArglist[i]).c_str());
#endif

	// === Aplicar overrides ===
	WriteRegString("renderer", api);
	WriteRegBool("fullscreen", (mode == "fullscreen"));
	WriteRegBool("windowed", (mode == "windowed"));
	WriteRegBool("vsync", (vsync == "true" || vsync == "vsync_on"));

	if (b640x480) { WriteRegInt("width", 640); WriteRegInt("height", 480); }
	else if (b800x600) { WriteRegInt("width", 800); WriteRegInt("height", 600); }
	else if (b1024x768) { WriteRegInt("width", 1024); WriteRegInt("height", 768); }

	if (szArglist) LocalFree(szArglist);
	return true;
}

// ========================================================
// WINMAIN (flujo corregido)
// ========================================================

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// === CREAR DEFAULTS PRIMERO ===
	CreateDefaultConfig();

	if (!Init()) return 0;

	
	if (!LoadEmbeddedDDraw()) {
		MessageBoxA(0, "No se pudo cargar ddraw embebida en memoria", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}


	HINSTANCE hDll;
	char cSearchDll[] = "rd`qbg-ckk";
	char cRealName[12];

	srand((unsigned)time(NULL));
	char *pJammer = new char[(rand() % 100) +1];
	G_pGame = new class CGame;
	
	G_pGame->b640x480 = b640x480;
	G_pGame->b800x600 = b800x600;
	G_pGame->b1024x768 = b1024x768;

	ZeroMemory(cRealName, sizeof(cRealName));
	strcpy(cRealName, cSearchDll);
	for (WORD i = 0; i < strlen(cRealName); i++)
	if (cRealName[i] != NULL) cRealName[i]++;

	hDll = LoadLibrary(cRealName);
	if( hDll == NULL ) 
	{	MessageBox(NULL, "don't find search.dll", "ERROR!", MB_OK);
		return 0;
	}
	//Launcher
	if (DEF_USING_LAUNCHER && CheckUpdater() != TRUE)
	{
		MessageBox(NULL, "Only excute Helbreath Cursed Client, in the launcher.", "ERROR!", MB_OK);
		//MessageBox(NULL, "ERROR!", "ERROR!", MB_OK);
		return 0;
	}
	
#ifdef DEF_USING_WIN_IME
	HINSTANCE hRichDll = LoadLibrary( "Riched20.dll" );
#endif

	typedef int (MYPROC)(char *) ;
	MYPROC *pFindHook; 
	pFindHook = (MYPROC *) GetProcAddress(hDll, "__FindHackingDll__") ;

	if (pFindHook== NULL) 
	{	MessageBox(NULL, "can't find search.dll", "ERROR!", MB_OK);
		return 0 ;
	}else if ((*pFindHook)("CRCCHECK") != 1) 
	{	return 0 ;
	}
	FreeLibrary(hDll);

	g_keyboardHook = NULL;
	
	sprintf( szAppClass, "Client-I%d", (int)hInstance);

	HKEY key;
	DWORD dwDisp;
	char myPath[_MAX_PATH+1];
	
	GetModuleFileName(NULL, myPath, _MAX_PATH);

	// Obtener la versión de Windows
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	if (osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion != 1) { // Windows 8 o mayor
	//if (osvi.dwMajorVersion >= 6) { // Windows 8 o mayor
		//lalowin
		RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &dwDisp);
		RegSetValueEx(key, myPath, 0, REG_SZ, (const BYTE*)"$ DWM8And16BitMitigation", sizeof("$ DWM8And16BitMitigation"));
		RegCloseKey(key);
	}
	else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) { // Windows 7
		//lalowin
		RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &dwDisp);
		RegSetValueEx(key, myPath, 0, REG_SZ, (const BYTE*)"16BITCOLOR DWM8And16BitMitigation Layer_ForceDirectDrawEmulation", sizeof("16BITCOLOR DWM8And16BitMitigation Layer_ForceDirectDrawEmulation"));
		RegCloseKey(key);
	}

	if (!InitApplication( hInstance))		return (FALSE);
    if (!InitInstance(hInstance, nCmdShow)) return (FALSE);

	Initialize((char *)lpCmdLine);
	EventLoop();
	delete[] pJammer;
	delete G_pGame;
#ifdef DEF_USING_WIN_IME
	FreeLibrary(hRichDll);
#endif
	

	return 0;
}

void OnDestroy()
{	G_pGame->m_bIsProgramActive = FALSE;		
	_StopTimer(G_mmTimer);
	G_pGame->Quit();
	WSACleanup();
	SetKeyboardHook(false);
	PostQuitMessage(0);
}

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}


MMRESULT _StartTimer(DWORD dwTime)
{TIMECAPS caps;
	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	return timeSetEvent(dwTime,0,_TimerFunc,0, (UINT)TIME_PERIODIC);
}


void _StopTimer(MMRESULT timerid)
{TIMECAPS caps;
	if (timerid != 0) 
	{	timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}



void Initialize(char * pCmdLine)
{int iX, iY, iSum;
 int     iErrCode;
 WORD	 wVersionRequested;
 WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 ); 
	iErrCode = WSAStartup( wVersionRequested, &wsaData );
	if ( iErrCode ) 
	{	MessageBox(G_hWnd, "Winsock-V1.1 not found! Cannot execute program.","ERROR",MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}
	if (G_pGame->bInit(G_hWnd, G_hInstance, pCmdLine) == FALSE) 
	{	PostQuitMessage(0);
		return;
	}	


	//G_mmTimer = _StartTimer(1000);
	//velocidad
	G_mmTimer = _StartTimer(100);
	for (iX = 0; iX < 64; iX++)
	for (iY = 0; iY < 510; iY++) 
	{	iSum = iX + (iY - 255);
		if (iSum <= 0)  iSum = 1;
		if (iSum >= 31) iSum = 31;
		G_iAddTable31[iX][iY] = iSum; 
		iSum = iX + (iY - 255);
		if (iSum <= 0)  iSum = 1;
		if (iSum >= 63) iSum = 63;
		G_iAddTable63[iX][iY] = iSum; 
		if ((iY - 255) < iX) G_iAddTransTable31[iY][iX] = iX;
		else if ((iY - 255) > 31) G_iAddTransTable31[iY][iX] = 31;
		else G_iAddTransTable31[iY][iX] = iY-255;
		if ((iY - 255) < iX) G_iAddTransTable63[iY][iX] = iX;
		else if ((iY - 255) > 63) G_iAddTransTable63[iY][iX] = 63;
		else G_iAddTransTable63[iY][iX] = iY-255;
	}
	if(!G_pGame->m_windowsKey) SetKeyboardHook(true);
	G_pGame->InitDialogs();
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || nCode != HC_ACTION)
		return CallNextHookEx( g_keyboardHook, nCode, wParam, lParam);

	bool bPreventKeypress = false;
	KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
	switch(wParam)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			bPreventKeypress = (G_pGame->m_bIsProgramActive && ((p->vkCode == VK_LWIN) || (p->vkCode == VK_RWIN)) && !G_pGame->m_windowsKey);
			break;
		}
	}

	if (bPreventKeypress) return 1;
	else return CallNextHookEx( g_keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam, LPARAM lParam)
{ 

	// --- Captura global por polling (funciona incluso si cnc-ddraw intercepta mensajes) ---
	if (GetAsyncKeyState(VK_SNAPSHOT) & 0x1)
		G_pGame->CreateScreenShot();

	
	if(G_pGame->GetText( hWnd, message, wParam, lParam)) return 0;

	switch (message) {
	case WM_USER_CALCSOCKETEVENT:
		G_pGame->_CalcSocketClosed();
		break;
	
	case WM_CLOSE:
		if ( (G_pGame->m_cGameMode == DEF_GAMEMODE_ONMAINGAME) && ( G_pGame->m_bForceDisconn == FALSE ) )
		{

#ifdef _DEBUGS
			if (G_pGame->m_cLogOutCount == -1 || G_pGame->m_cLogOutCount > 2) G_pGame->m_cLogOutCount = 1;
#else
			if (G_pGame->IsShopMap())
			{
				if (G_pGame->m_cLogOutCount == -1 || G_pGame->m_cLogOutCount > 6)  G_pGame->m_cLogOutCount = 6;
			}
			else
			{
				if (G_pGame->m_cLogOutCount == -1 || G_pGame->m_cLogOutCount > 6) G_pGame->m_cLogOutCount = 6;
			}
			
#endif

		}
			else if (G_pGame->m_cGameMode == DEF_GAMEMODE_ONLOADING) return (DefWindowProc(hWnd, message, wParam, lParam));
			else if (G_pGame->m_cGameMode == DEF_GAMEMODE_ONMAINMENU) G_pGame->ChangeGameMode(DEF_GAMEMODE_ONQUIT);
		break;
	
	case WM_SYSCOMMAND:
		if((wParam&0xFFF0)==SC_SCREENSAVE || (wParam&0xFFF0)==SC_MONITORPOWER) 
			return 0; 
		return DefWindowProc(hWnd, message, wParam, lParam);
			
	case WM_USER_TIMERSIGNAL:
		G_pGame->OnTimer();
		break;

	case WM_KEYDOWN:
		G_pGame->OnKeyDown(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		
	case WM_KEYUP:
		G_pGame->OnKeyUp(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));

	case WM_SYSKEYDOWN:
		G_pGame->OnSysKeyDown(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_SYSKEYUP:
		G_pGame->OnSysKeyUp(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_ACTIVATEAPP:
		if( wParam == 0 ) 
		{	G_pGame->m_bIsProgramActive = FALSE;
			G_pGame->m_DInput.SetAcquire(FALSE);
		}else 
		{	G_pGame->m_bIsProgramActive = TRUE;
			G_pGame->m_DInput.SetAcquire(TRUE);
			G_pGame->m_bCtrlPressed = FALSE;
			if (IsWin8())
			{
				G_pGame->m_bIsRedrawPDBGS = TRUE;
				G_pGame->m_DDraw.ChangeDisplayMode(G_hWnd);

			}
		if (G_pGame->bCheckImportantFile() == FALSE) 
			{	MessageBox(G_pGame->m_hWnd, "File checksum error! Get Update again please!", "ERROR1", MB_ICONEXCLAMATION | MB_OK);
				PostQuitMessage(0);
				return 0;
			}

			if (__FindHackingDll__("CRCCHECK") != 1) 
			{	G_pGame->ChangeGameMode(DEF_GAMEMODE_ONQUIT);
				return NULL;
			}
			
		}
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_SETCURSOR:
		SetCursor(NULL);
		return TRUE;

	case WM_DESTROY:
		OnDestroy();
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
		
	case WM_USER_GAMESOCKETEVENT:
		G_pGame->OnGameSocketEvent(wParam, lParam);
		break;

	case WM_USER_LOGSOCKETEVENT:
		G_pGame->OnLogSocketEvent(wParam, lParam);
		break;

	default: 
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}	
	return NULL;
}

BOOL InitApplication( HINSTANCE hInstance)
{WNDCLASS  wc;
	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	wc.lpfnWndProc   = (WNDPROC)WndProc;             
	wc.cbClsExtra    = 0;                            
	wc.cbWndExtra    = sizeof (int);
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = szAppClass;        
	return (RegisterClass(&wc));
	
}


BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{	
	int cx = GetSystemMetrics(SM_CXFULLSCREEN)/2;
	int cy = GetSystemMetrics(SM_CYFULLSCREEN)/2;
	//if (cy>280) cy -= 40;

	if (c_reso->IsResolution() == C800x600)
	{
	if (cy>340) cy -= 40;
	G_hWnd = CreateWindowEx(WS_EX_TOPMOST, szAppClass, "HBCursed", WS_VISIBLE | WS_POPUP, cx - 400, cy - 300,
	800, 600, NULL, NULL, hInstance, NULL);
	}
	else if (c_reso->IsResolution() == C640x480)
	{
	if (cy>280) cy -= 40;
	G_hWnd = CreateWindowEx(WS_EX_TOPMOST, szAppClass, "HBCursed", WS_VISIBLE | WS_POPUP, cx - 320, cy - 240,
	640, 480, NULL, NULL, hInstance, NULL);
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
	if (cy>424) cy -= 40;
	G_hWnd = CreateWindowEx(WS_EX_TOPMOST, szAppClass, "HBCursed", WS_VISIBLE | WS_POPUP, cx - 512, cy - 384,
	1024, 768, NULL, NULL, hInstance, NULL);
	}

    if (!G_hWnd) return FALSE;
    G_hInstance	= hInstance;
	ShowWindow(G_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(G_hWnd);
	return TRUE;
}

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Logging simple
static void LogCrashOnce(const char* ctx)
{
	if (g_bCrashLogged) return; // ya se registró este crash

	FILE* f = fopen("crashlog.txt", "a");
	if (!f) return;

	SYSTEMTIME t; GetLocalTime(&t);
	fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d] Crash evitado en %s\n",
		t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond,
		ctx ? ctx : "(desconocido)");

	fclose(f);
	g_bCrashLogged = true;
}

// Resetea el flag cuando pasa un frame sano
static inline void ResetCrashFlag() { g_bCrashLogged = false; }

// -----------------------------------------------------------------------------
// Filtro para __except: log una sola vez
static int LogAndHandle(EXCEPTION_POINTERS* info, const char* ctx)
{
	LogCrashOnce(ctx);
	return EXCEPTION_EXECUTE_HANDLER;
}

// -----------------------------------------------------------------------------
// “Sandbox” por frame
template<typename Fn>
static bool TryFrame(Fn&& fn, const char* ctx)
{
	__try
	{
		fn();
		ResetCrashFlag(); // si este frame fue limpio, se puede loguear otro futuro crash
		return true;
	}
	__except (LogAndHandle(GetExceptionInformation(), ctx))
	{
		// Crash manejado: seguimos ejecutando
		return false;
	}
}

// -----------------------------------------------------------------------------
// Wrappers concretos
static void Call_UpdateScreen()            { G_pGame->UpdateScreen(); }
static void Call_UpdateScreen_OnLoading()  { G_pGame->UpdateScreen_OnLoading(FALSE); }

// -----------------------------------------------------------------------------
// EventLoop endurecido (versión ligera)
void EventLoop()
{
	MSG msg{};
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) return;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (G_pGame && G_pGame->m_bIsProgramActive)
		{
			if (!TryFrame(Call_UpdateScreen, "UpdateScreen"))
			{
				// Frame saltado tras crash; nada más, sigue al próximo
			}
		}
		else if (G_pGame && G_pGame->m_cGameMode == DEF_GAMEMODE_ONLOADING)
		{
			TryFrame(Call_UpdateScreen_OnLoading, "UpdateScreen_OnLoading");
		}
		else
		{
			WaitMessage();
			//Sleep(1);  // En vez de WaitMessage()
		}
	}
}
