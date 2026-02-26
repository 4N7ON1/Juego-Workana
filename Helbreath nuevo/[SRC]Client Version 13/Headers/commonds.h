
CAntiSpeedHack g_AntiSpeedHack;

extern class cResolution * c_reso;
Event g_ev;
using namespace std;
// CONFIGURACIÓN
static bool g_bCrashLogged = false; // evita spamear logs si se repite el mismo crash

extern "C" __declspec(dllimport) int __FindHackingDll__(char *);

bool b640x480 = false;
bool b800x600 = false;
bool b1024x768 = true;
// --------------------------------------------------------------

#define WM_USER_TIMERSIGNAL		WM_USER + 500
#define WM_USER_CALCSOCKETEVENT WM_USER + 600

int				G_iAddTable31[64][510], G_iAddTable63[64][510];
int				G_iAddTransTable31[510][64], G_iAddTransTable63[510][64];

long    G_lTransG100[64][64], G_lTransRB100[64][64];
long    G_lTransG70[64][64], G_lTransRB70[64][64];
long    G_lTransG50[64][64], G_lTransRB50[64][64];
long    G_lTransG25[64][64], G_lTransRB25[64][64];
long    G_lTransG2[64][64], G_lTransRB2[64][64];

char			szAppClass[32];
HWND			G_hWnd = NULL;
HWND			G_hEditWnd = NULL;
HINSTANCE       G_hInstance = NULL;
MMRESULT		G_mmTimer;
char   G_cSpriteAlphaDegree;
class CGame * G_pGame;
class XSocket * G_pCalcSocket = NULL;
BOOL  G_bIsCalcSocketConnected = TRUE;
DWORD G_dwCalcSocketTime = NULL, G_dwCalcSocketSendTime = NULL;

char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];

HHOOK		g_keyboardHook;


enum
{
	Client,
	Reso,
	Screen,
	Lang
};



LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
	if (retval == ERROR_SUCCESS)
	{
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata, data);
		RegCloseKey(hkey);
	}
	return retval;
}

void GoHomepage()
{
	LPCTSTR	url = MSG_HOMEPAGE;

#ifdef _DEBUG
	return;
#endif

	int		showcmd = SW_SHOW;
	char	key[MAX_PATH + MAX_PATH];
	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, "open", url, NULL, NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ((UINT)result <= HINSTANCE_ERROR)
	{
		if (GetRegKey(HKEY_CLASSES_ROOT, ".htm", key) == ERROR_SUCCESS)
		{
			lstrcat(key, "\\shell\\open\\command");
			if (GetRegKey(HKEY_CLASSES_ROOT, key, key) == ERROR_SUCCESS)
			{
				char *pos;
				pos = strstr(key, "\"%1\"");
				if (pos == NULL) {                     // No quotes found
					pos = strstr(key, "%1");           // Check for %1, without quotes 
					if (pos == NULL)                   // No parameter at all...
						pos = key + lstrlen(key) - 1;
					else *pos = '\0';                   // Remove the parameter
				}
				else    *pos = '\0';                   // Remove the parameter
				lstrcat(pos, " ");
				lstrcat(pos, url);
				result = (HINSTANCE)WinExec(key, showcmd);
			}
		}
	}
}

BOOL CheckUpdater()
{
	HANDLE hSnapShot;
	PROCESSENTRY32 uProcess;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	uProcess.dwSize = (DWORD)sizeof(PROCESSENTRY32);
	Process32First(hSnapShot, &uProcess);

	do
	{
		if (strstr(uProcess.szExeFile, "HB Cursed - Launcher") != NULL)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)uProcess.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
			return TRUE;
		}
	} while (Process32Next(hSnapShot, &uProcess));

	CloseHandle(hSnapShot);
	return FALSE;
}

void SetKeyboardHook(bool enable)
{
	if (enable) g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, G_hInstance, 0);
	else if (g_keyboardHook != NULL)
		UnhookWindowsHookEx(g_keyboardHook);
}


//lalowin
bool IsWin8()//usar windows 10
{
	OSVERSIONINFO osvi;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);
	if ((osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 2) ||
		(osvi.dwMajorVersion == 10 && osvi.dwMinorVersion >= 0)) {

		return true;
	}
	return false;
}