#include "../Headers/Game.h"
#include "../Language/lan_eng.h"
//added resolution
#include "..\Resolution\Resolution.h"
#include "..\..\Shared\shared.h"
#include <algorithm>
#include <thread>

extern class cResolution * c_reso;
extern char G_cSpriteAlphaDegree;

extern char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];
extern class XSocket * G_pCalcSocket;
extern BOOL G_bIsCalcSocketConnected;
extern DWORD G_dwCalcSocketTime, G_dwCalcSocketSendTime;
extern HWND	G_hWnd, G_hEditWnd;
extern HINSTANCE G_hInstance;
