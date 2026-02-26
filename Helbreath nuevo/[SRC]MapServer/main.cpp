#include <stdio.h>
#include <iostream>
#include <thread>
#include <new>
#include <exception>
#include <chrono>
#include <memory>
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <mmsystem.h>
#include <sql.h>
#include "Header/winmain.h"
#include "Header/Game.h"
#include "General/UserMessages.h"
#include "language/resource.h"
#include <shellapi.h>
#include <shlobj.h>
#include "Header/Event.h"

using namespace std;
Event::Handler g_ev;
#define WM_USER_TIMERSIGNAL WM_USER + 500
#pragma warning(disable : 4996)

HWND            G_hWnd = NULL;
char            G_cTxt[512];
char            G_cData50000[50000];
MMRESULT        G_mmTimer = NULL;
class XSocket* G_pListenSock = NULL;
class XSocket* G_pLoginSock = NULL;
class XSocket* G_pLogSock = NULL;
class cCoreServer* c_map = NULL;
BOOL				G_bShutdown = FALSE;
MMRESULT			G_mmTimerCopy = NULL;


void _StopTimer(MMRESULT timerid)
{
	TIMECAPS caps;

	if (timerid != 0) {
		timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}

bool Initialize()
{
	WSADATA data;
	auto version = MAKEWORD(2, 2);
	auto err = WSAStartup(version, &data);
	if (err)
		return false;
	return true;
}

void PutLogList(char* cMsg)
{
	std::cout << cMsg << std::endl;
}

static std::string ReadCfgValue(const char* filename, const char* key)
{
	std::ifstream file(filename);
	std::string line;
	std::string prefix(key);
	prefix += "=";

	while (std::getline(file, line)) {
		if (line.rfind(prefix, 0) == 0) {
			return line.substr(prefix.length());
		}
	}
	return "";
}

void Cmd_CorruptGlobalLoginSock()
{
	if (G_pLoginSock) {
		PutLogList("(!!!) Corrompiendo G_pLoginSock manualmente");
		delete G_pLoginSock;
		G_pLoginSock = (XSocket*)0xDEADBEEF;
	}

	if (G_pListenSock) {
		PutLogList("(!!!) Corrompiendo G_pListenSock manualmente");
		delete G_pListenSock;
		G_pListenSock = (XSocket*)0xDEADBEEF;
	}
}

void Cmd_DeleteGlobalLoginSock()
{
	if (G_pLoginSock) {
		PutLogList("(!!!) Eliminando G_pLoginSock manualmente");
		delete G_pLoginSock;
		G_pLoginSock = nullptr;
	}

	if (G_pListenSock) {
		PutLogList("(!!!) Eliminando G_pListenSock manualmente");
		delete G_pListenSock;
		G_pListenSock = nullptr;
	}
}

bool IsSocketValid(XSocket* sock)
{
	__try {
		if (sock == nullptr) return false;
		volatile int test = sock->m_Sock;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
	return true;
}

BOOL SafeDeleteSocket(XSocket*& ptr, const char* name)
{
	BOOL result = TRUE;
	char szMsg[256];

	__try {
		if (!IsBadReadPtr(ptr, sizeof(XSocket)) && !IsBadWritePtr(ptr, sizeof(XSocket))) {
			delete ptr;
			wsprintf(szMsg, "(!!!) %s eliminado correctamente.", name);
			PutLogList(szMsg);
		}
		else {
			wsprintf(szMsg, "(!!!) %s estaba corrupto (puntero inválido).", name);
			PutLogList(szMsg);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		wsprintf(szMsg, "(!!!) EXCEPCIÓN atrapada al eliminar %s.", name);
		PutLogList(szMsg);
		result = FALSE;
	}

	ptr = nullptr;
	return result;
}
#include <shlwapi.h>
#include <direct.h> // mkdir
#include <ctime>
#include <string>
#include <sstream>
#include <io.h>      // _access

#pragma comment(lib, "Shlwapi.lib") // Para CopyDirectory

bool CopyFolder(const std::string& source, const std::string& destination)
{
	std::stringstream command;
	command << "xcopy /E /I /Y \"" << source << "\" \"" << destination << "\" >nul 2>&1";
	return system(command.str().c_str()) == 0;
}

void RestartFailed()
{
	PutLogList("(!) ERROR CRÍTICO: Reinicio de sockets fallido. Ejecutando recuperación...");

	// 1. Crear copia de respaldo de la carpeta DataBase
	time_t now = time(NULL);
	struct tm t;
	localtime_s(&t, &now);

	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%H_%M_%S_%d_%m_%Y", &t);

	std::string sourceFolder = "DataBase";
	std::string backupFolder = "DataBase_";
	backupFolder += timestamp;

	char txt[500];
	wsprintf(txt, "(!) Creando copia de seguridad: %s", (char*)backupFolder.c_str());
	PutLogList(txt);

	if (!CopyFolder(sourceFolder, backupFolder)) {
		PutLogList("(!) ERROR: Falló la copia de la carpeta DataBase.");
	}
	else {
		PutLogList("(!) Copia de seguridad creada correctamente.");
	}

	// 2. Desconectar jugadores

	PutLogList("(!) Intentando desconectar jugadores.");
	if (c_map->DisconnectAll())
	{
		PutLogList("(!) Jugadores desconectados");
	}
	else
	{

	}

	// 3. Esperar un momento antes de reiniciar
	Sleep(2000);

	// 4. Ejecutar el script externo
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	char commandLine[] = "cmd.exe /c RestartHelper.bat";

	BOOL result = CreateProcessA(
		NULL,
		commandLine,
		NULL, NULL, FALSE,
		CREATE_NO_WINDOW,
		NULL, NULL,
		&si, &pi
		);

	if (result) {
		PutLogList("(!) RestartHelper.bat ejecutado correctamente.");
	}
	else {
		PutLogList("(!) ERROR: No se pudo ejecutar RestartHelper.bat.");
	}

	Sleep(1000);

	// 5. Terminar proceso actual
	ExitProcess(0);
}

bool RestartSockets()
{
	// 1. Detener el timer existente
	if (G_mmTimerCopy != NULL) {
		_StopTimer(G_mmTimerCopy);
		G_mmTimerCopy = NULL;
		PutLogList("(!!!) Timer detenido para reinicio de sockets");
	}

	// 2. Limpiar Winsock
	WSACleanup();

	// 3. Reinicializar Winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		PutLogList("ERROR: No se pudo reinicializar Winsock");
		RestartFailed();
		return false;
	}

	// 4. Eliminar sockets existentes
	PutLogList("(!!!) Reiniciando sockets...");
	if (G_pLoginSock) SafeDeleteSocket(G_pLoginSock, "G_pLoginSock");
	if (G_pListenSock) SafeDeleteSocket(G_pListenSock, "G_pListenSock");

	// 5. Crear nuevos sockets
	try {
		G_pLoginSock = new XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
		G_pListenSock = new XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	}
	catch (...) {
		PutLogList("ERROR: Excepción al crear sockets");
		RestartFailed();
		return false;
	}

	// 6. Configurar escucha
	std::string loginIP = ReadCfgValue("Login.cfg", "LoginServerIP");
	std::string loginPort = ReadCfgValue("Login.cfg", "LoginServerPort");

	if (loginIP.empty() || loginPort.empty()) {
		PutLogList("ERROR: Configuración incompleta");
		RestartFailed();
		return false;
	}

	if (!G_pListenSock->bListen(c_map->m_cGameServerAddr, c_map->m_iGameServerPort, WM_USER_ACCEPT) ||
		!G_pLoginSock->bListen((char*)loginIP.c_str(), atoi(loginPort.c_str()), WM_USER_ACCEPT_LOGIN)) {
		PutLogList("ERROR: Falló bListen");
		RestartFailed();
		return false;
	}

	// 7. Reiniciar el timer
	G_mmTimerCopy = StartTimer(1);
	if (G_mmTimerCopy == NULL) {
		PutLogList("(!!!) Advertencia: Timer no reiniciado");
		// Continuar sin timer es mejor que cerrar el servidor
	}
	else {
		PutLogList("(!!!) Timer reiniciado correctamente");
	}

	return true;
}

void OnTimer()
{
	if (G_mmTimerCopy == NULL) {
		PutLogList("Timer: Timer no está activo, intentando reiniciar...");
		G_mmTimerCopy = StartTimer(1);
		return;
	}

	__try {
		if (c_map) c_map->OnTimer(NULL);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		PutLogList("Timer: Excepción en c_map->OnTimer");
	}

	if (!IsSocketValid(G_pLoginSock)) {
		PutLogList("Timer: Socket inválido, reiniciando...");
		RestartSockets();
	}
}

void PutGMLogData(char* cStr)
{
	
}

void CharacterLogList(char* cMsg)
{
	
}

void ConfigList(char* cMsg)
{
	std::cout << cMsg << std::endl;
}

void UpdateConfigList(char* cMsg)
{
	std::cout << cMsg << std::endl;
}

void ErrorList(char* cMsg)
{
	
}

void ChatLogs(char* cMsg)
{
	std::cout << cMsg << std::endl;
}

void PutLogTradeFileList(char* cStr)
{

}

void PutLogCoinsFileList(char* cStr)
{
	
}

void PutLogHacksFileList(char* cStr)
{
	std::cout << cStr << std::endl;
}

void PutLogDrops(char* cStr)
{
	std::cout << cStr << std::endl;
}

void PutLogOnlinesFileList(char* cStr)
{
	
}

void PutLogItemsList(char* cMsg)
{
	
}

void PutEkAndPkLogFileList(char* cMsg)
{

}

void Deinitialize()
{
	// Limpieza adicional si es necesaria
}

void OnDestroy()
{
	if (G_pListenSock != NULL) delete G_pListenSock;
	if (G_pLogSock != NULL) delete G_pLogSock;

	if (c_map != NULL) {
		c_map->Quit();
		delete c_map;
	}

	if (G_mmTimer != NULL) _StopTimer(G_mmTimer);
	Deinitialize();
	PostQuitMessage(0);
}

void OnAccept()
{
	c_map->bAccept(G_pListenSock);
}

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{
	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}

void OnAcceptLogin()
{
	if (!IsSocketValid(G_pLoginSock)) {
		PutLogList("(!!!) G_pLoginSock inválido en WM_USER_ACCEPT_LOGIN. Reiniciando...");
		if (!RestartSockets()) {
			PutLogList("(!!!) No se pudo reiniciar G_pLoginSock desde WM_USER_ACCEPT_LOGIN");
			return;
		}
		else {
			PutLogList("(!!!) G_pLoginSock reiniciado correctamente desde WM_USER_ACCEPT_LOGIN");
		}
	}

	c_map->AuthenticateLogin(G_pLoginSock);
}

LRESULT CALLBACK BackgroundWindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (hWnd == NULL) {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1012:
			break;
		case 1013:
			break;
		case 1010:
		{
			char commandtxt[100] = { 0 };
		}
		break;
		}
		break;

	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		if (c_map) {
			c_map->OnKeyDown(wParam, lParam);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_KEYUP:
		if (c_map) {
			c_map->OnKeyUp(wParam, lParam);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_USER_STARTGAMESIGNAL:
		if (c_map) {
			c_map->OnStartGameSignal();
		}
		break;

	case WM_USER_TIMERSIGNAL:
		if (!G_bShutdown && c_map) {
			OnTimer();
		}
		break;

	case WM_USER_ACCEPT:
		OnAccept();
		break;

	case WM_USER_ACCEPT_LOGIN:
		OnAcceptLogin();
		break;

	case WM_DESTROY:
		G_bShutdown = TRUE;
		OnDestroy();
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		if (!c_map || c_map->bOnClose()) {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;

	default:
		if (!G_bShutdown) {
			if (c_map) {
				if ((message >= WM_ONCLIENTLOGINSOCKETEVENT + 1) &&
					(message <= WM_ONCLIENTLOGINSOCKETEVENT + MAXCLIENTLOGINSOCK)) {
					c_map->OnSubLogSocketEvent(message, wParam, lParam);
				}

				if ((message >= WM_ONCLIENTSOCKETEVENT) &&
					(message < WM_ONCLIENTSOCKETEVENT + DEF_MAXCLIENTS)) {
					c_map->OnClientSocketEvent(message, wParam, lParam);
				}
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

MMRESULT StartTimer(DWORD dwTime)
{
	TIMECAPS caps;
	MMRESULT timerid;

	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	timerid = timeSetEvent(dwTime, 0, _TimerFunc, 0, (UINT)TIME_PERIODIC);

	return timerid;
}

HWND CreateBackgroundWindow()
{
	WNDCLASS wndclass;
	CHAR* ProviderClass = "AsyncSelect";
	HWND Window;
	char cTitle[100];

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = &BackgroundWindowProcess;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = ProviderClass;

	if (RegisterClass(&wndclass) == 0)
	{
		printf("RegisterClass() failed with error %d\n", GetLastError());
		return NULL;
	}

	Window = CreateWindowEx(
		0,
		ProviderClass,
		cTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL);

	if (Window == NULL)
	{
		printf("CreateWindow() failed with error %d\n", GetLastError());
		return NULL;
	}
	return Window;
}

void InitializeSockets()
{
	if (Initialize() == FALSE) {
		printf("Socket 2.2 not found! Cannot execute program.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}
}
/*

void StartCoreserver()
{
	G_hWnd = CreateBackgroundWindow();

	c_map = new cCoreServer(G_hWnd);
	if (c_map->bInit() == FALSE) {
		printf("(!!!) STOPPED!");
		PutLogList("(!!!) STOPPED!");
		return;
	}
	
	G_pListenSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	G_pListenSock->bListen(c_map->m_cGameServerAddr, c_map->m_iGameServerPort, WM_USER_ACCEPT);

	std::string loginIP = ReadCfgValue("Login.cfg", "LoginServerIP");
	std::string loginPort = ReadCfgValue("Login.cfg", "LoginServerPort");

	if (loginIP.empty() || loginPort.empty()) {
		PutLogList("(!!!) No se pudo leer LoginServerIP o LoginServerPort del archivo coreserver.cfg");
		return;
	}

	G_pLoginSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	G_pLoginSock->bListen((char*)loginIP.c_str(), atoi(loginPort.c_str()), WM_USER_ACCEPT_LOGIN);
}
*/


void StartCoreserver()
{
	PutLogList("(+) Iniciando CoreServer...");

	G_hWnd = CreateBackgroundWindow();
	if (!G_hWnd) {
		PutLogList("(!) Error: No se pudo crear la ventana de background.");
		return;
	}

	c_map = new cCoreServer(G_hWnd);
	if (!c_map || c_map->bInit() == FALSE) {
		PutLogList("(!!!) STOPPED! Falló la inicialización del mapa.");
		return;
	}

	// === Socket principal del GameServer ===
	G_pListenSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	if (!G_pListenSock->bListen(c_map->m_cGameServerAddr, c_map->m_iGameServerPort, WM_USER_ACCEPT))
	{
		PutLogList("(!!!) Error: No se pudo iniciar el socket de escucha principal.");
		delete G_pListenSock;
		G_pListenSock = nullptr;
		return;
	}

	// Activar KeepAlive para el socket principal
	int keepAlive = 1;
	setsockopt(G_pListenSock->m_Sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAlive, sizeof(keepAlive));

	char tmp[256];
	sprintf(tmp, "(+) GameServer escuchando en %s:%d", c_map->m_cGameServerAddr, c_map->m_iGameServerPort);
	PutLogList(tmp);

	// === Socket del LoginServer ===
	std::string loginIP = ReadCfgValue("Login.cfg", "LoginServerIP");
	std::string loginPort = ReadCfgValue("Login.cfg", "LoginServerPort");

	if (loginIP.empty() || loginPort.empty()) {
		PutLogList("(!!!) Error: No se pudo leer LoginServerIP o LoginServerPort del archivo Login.cfg");
		return;
	}

	G_pLoginSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	if (!G_pLoginSock->bListen((char*)loginIP.c_str(), atoi(loginPort.c_str()), WM_USER_ACCEPT_LOGIN))
	{
		PutLogList("(!!!) Error: No se pudo iniciar el socket de LoginServer.");
		delete G_pLoginSock;
		G_pLoginSock = nullptr;
		return;
	}

	// Activar KeepAlive para el socket de login
	setsockopt(G_pLoginSock->m_Sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAlive, sizeof(keepAlive));

	sprintf(tmp, "(+) LoginServer escuchando en %s:%s", loginIP.c_str(), loginPort.c_str());
	PutLogList(tmp);

	PutLogList("(+) CoreServer inicializado correctamente.");
}

int main() {
	// Elevar prioridad del proceso
	if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)) {
		std::cerr << "Error al establecer prioridad del proceso (" << GetLastError() << ")\n";
	}

	// También podés elevar el hilo principal si querés aún más prioridad
	if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)) {
		std::cerr << "Error al establecer prioridad del hilo (" << GetLastError() << ")\n";
	}

	try {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		InitializeSockets();
		StartCoreserver();

		G_mmTimerCopy = StartTimer(1);

		MSG msg;
		/*while (1) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
				if (!GetMessage(&msg, NULL, 0, 0)) {
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				WaitMessage();
			}
		}*/

		while (true) {
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// Pequeño sleep para no sobrecargar CPU ni colgar mensajes de red
			Sleep(1);
		}
	}
	catch (std::bad_alloc& ba) {
		char tmp[1000];
		ZeroMemory(tmp, sizeof(tmp));
		wsprintf(tmp, "bad_alloc caught : %s", ba.what());
		PutLogList(tmp);
	}
	catch (std::exception& e) {
		PutLogList("Exception occurred");
	}
	catch (...) {
		PutLogList("Unknown exception occurred");
	}

	return 0;
}
