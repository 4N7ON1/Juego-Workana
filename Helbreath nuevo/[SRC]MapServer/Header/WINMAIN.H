#include "XSocket.h"

// winmain.h
void Borrado();
void _StopTimer(MMRESULT timerid);
bool Initialize();
void PutLogList(char* cMsg);
static std::string ReadCfgValue(const char* filename, const char* key);
void Cmd_CorruptGlobalLoginSock();
void Cmd_DeleteGlobalLoginSock();
bool IsSocketValid(XSocket* sock);
BOOL SafeDeleteSocket(XSocket*& ptr, const char* name);
bool RestartSockets();
void OnTimer();
void PutGMLogData(char* cStr);
void CharacterLogList(char* cMsg);
void ConfigList(char* cMsg);
void UpdateConfigList(char* cMsg);
void ErrorList(char* cMsg);
void ChatLogs(char* cMsg);
void PutLogTradeFileList(char* cStr);
void PutLogCoinsFileList(char* cStr);
void PutLogHacksFileList(char* cStr);
void PutLogDrops(char* cStr);
void PutLogOnlinesFileList(char* cStr);
void PutLogItemsList(char* cMsg);
void PutEkAndPkLogFileList(char* cMsg);
void Deinitialize();
void OnDestroy();
void OnAccept();
void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2);
void OnAcceptLogin();
LRESULT CALLBACK BackgroundWindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
MMRESULT StartTimer(DWORD dwTime);
HWND CreateBackgroundWindow();
void InitializeSockets();
void StartCoreserver();
void RestartFailed();