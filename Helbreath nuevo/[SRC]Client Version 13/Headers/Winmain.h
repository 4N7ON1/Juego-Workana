// winmain.h

BOOL InitApplication( HINSTANCE hInstance);
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK WndProc( HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam );
void EventLoop();
void OnDestroy();
void Initialize(char * pCmdLine);
void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2);
MMRESULT _StartTimer(DWORD dwTime);
//BOOL CALLBACK NotepadFunc(HWND hwnd,LPARAM lParam );
//void AntiSpeed();
void _StopTimer(MMRESULT timerid);
//void CPSocketConnet() ;
LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
void GoHomepage();
BOOL CheckUpdater();
void SetKeyboardHook(bool enable);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

bool IsWin8();