// DXC_ddraw.cpp: implementation of the DXC_ddraw class.
//
//////////////////////////////////////////////////////////////////////

#include <string.h>
#include <objbase.h>
#include "../Motor/DXC_ddraw.h"
//added resolution
#include "..\Headers\Game.h"
#include "..\Resolution\Resolution.h"
class cResolution * c_reso;
extern CGame * G_pGame;

extern HWND G_hEditWnd;
extern HWND G_hWnd;
#include "../EmbeddedDDraw.h"

typedef HRESULT(WINAPI *LPDIRECTDRAWCREATEEX)(
	GUID* lpGuid,
	void** lplpDD,
	REFIID iid,
	IUnknown* pUnkOuter
	);

extern bool m_bDirectDraw;

extern long    G_lTransG100[64][64], G_lTransRB100[64][64];
extern long    G_lTransG70[64][64], G_lTransRB70[64][64];
extern long    G_lTransG50[64][64], G_lTransRB50[64][64];
extern long    G_lTransG25[64][64], G_lTransRB25[64][64];
extern long    G_lTransG2[64][64], G_lTransRB2[64][64];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DXC_ddraw::DXC_ddraw()
{
	m_lpFrontB4		= NULL;
	m_lpDD4			= NULL;
	m_lpPDBGS		= NULL;
	m_lpBackB4flip	= NULL;
	m_cPixelFormat	= 0;

	//added resolution
	c_reso = new cResolution;
	c_reso->sResolution = C640x480;

#ifdef DEF_MIXEDTORNER //DEF_WINDOWED_MODE	
	m_bSoloGrande = FALSE; //FALSE
#else
	m_bSoloGrande = TRUE; //m_bFullMode		= TRUE;
#endif

}

DXC_ddraw::~DXC_ddraw()
{
	if (m_hFontInUse != NULL) DeleteObject(m_hFontInUse);
	if (m_hFontInUse2 != NULL) DeleteObject(m_hFontInUse2);
	if (m_hFontInUse3 != NULL) DeleteObject(m_hFontInUse3);
	if (m_hFontInUse4 != NULL) DeleteObject(m_hFontInUse4);
	if (m_hFontInUse5 != NULL) DeleteObject(m_hFontInUse5);
	if (m_hFontHP != NULL) DeleteObject(m_hFontHP);
	if (m_lpBackB4flip != NULL) m_lpBackB4flip->Release();
	if (m_lpBackB4 != NULL) m_lpBackB4->Release();
	if (m_lpFrontB4 != NULL) m_lpFrontB4->Release();
	if (m_bSoloGrande == TRUE)
	{
		if (m_lpDD4 != NULL) m_lpDD4->RestoreDisplayMode();
	}
	if (m_lpDD4 != NULL) m_lpDD4->Release();
}


void SendAltEnter()
{
	INPUT ip[4] = {};

	// ALT down
	ip[0].type = INPUT_KEYBOARD;
	ip[0].ki.wVk = VK_MENU;

	// ENTER down
	ip[1].type = INPUT_KEYBOARD;
	ip[1].ki.wVk = VK_RETURN;

	// ENTER up
	ip[2].type = INPUT_KEYBOARD;
	ip[2].ki.wVk = VK_RETURN;
	ip[2].ki.dwFlags = KEYEVENTF_KEYUP;

	// ALT up
	ip[3].type = INPUT_KEYBOARD;
	ip[3].ki.wVk = VK_MENU;
	ip[3].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(4, ip, sizeof(INPUT));
}

BOOL DXC_ddraw::bInit(HWND hWnd)
{
 HRESULT        ddVal;
 DDSURFACEDESC2 ddsd;
 int            iS, iD;
	m_init = false;

	//added resolution
	res_x = 0;
	res_y = 0;
	res_x_mid = 0;
	res_y_mid = 0;
	res_y_mid2 = 0;

	if (G_pGame->b800x600) c_reso->sResolution = C800x600;
	else if (G_pGame->b640x480) c_reso->sResolution = C640x480;
	else if (G_pGame->b1024x768) c_reso->sResolution = C1024x768;

	if (c_reso->IsResolution() == C800x600)
	{
		res_x = 800;
		res_y = 600;
		res_x_mid = 400;
		res_y_mid = 300;
		res_y_mid2 = 340;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		res_x = 640;
		res_y = 480;
		res_x_mid = 320;
		res_y_mid = 240;
		res_y_mid2 = 280;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		res_x = 1024;
		res_y = 768;
		res_x_mid = 512;
		res_y_mid = 384;
		res_y_mid2 = 424;
	}

	SetRect(&m_rcClipArea, 0, 0, res_x, res_y);

	//lalowin
	//ddVal = DirectDrawCreateEx((GUID*)DDCREATE_EMULATIONONLY, (VOID**)&m_lpDD4, IID_IDirectDraw7, NULL); //este es el fix de windows 10
	LPDIRECTDRAWCREATEEX pDirectDrawCreateEx =
		(LPDIRECTDRAWCREATEEX)GetEmbeddedDDrawProc("DirectDrawCreateEx");

	if (!pDirectDrawCreateEx) {
		MessageBoxA(0, "No se pudo obtener DirectDrawCreateEx desde la DLL embebida", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ddVal = pDirectDrawCreateEx(NULL, (VOID**)&m_lpDD4, IID_IDirectDraw7, NULL);
	
    if (ddVal != DD_OK) return FALSE;

	if (m_bSoloGrande == TRUE)
	{
		DDSCAPS2       ddscaps;
		ddVal = m_lpDD4->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
		if (ddVal != DD_OK) return FALSE;

		ddVal = m_lpDD4->SetDisplayMode(res_x, res_y, 16, 0, 0);//(640, 480, 16,0,0) cambio bits.
		if (ddVal != DD_OK) return FALSE;
		memset( (VOID *)&ddsd, 0, sizeof(ddsd) );
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.dwBackBufferCount = 1;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		
		ddVal = m_lpDD4->CreateSurface(&ddsd, &m_lpFrontB4, NULL);
		if (ddVal != DD_OK) return FALSE;

		ZeroMemory(&ddscaps, sizeof(ddscaps));
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		ddVal = m_lpFrontB4->GetAttachedSurface(&ddscaps, &m_lpBackB4flip);
		if (ddVal != DD_OK) return FALSE;
		SetRect(&m_rcFlipping, 0, 0, res_x, res_y); // our fictitious sprite bitmap is 
	}
	else
	{
		int cx = GetSystemMetrics(SM_CXFULLSCREEN);
		int cy = GetSystemMetrics(SM_CYFULLSCREEN);

		//ChangeBPP(CHANGE16BPP);

		ddVal = m_lpDD4->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
		if (ddVal != DD_OK) return FALSE;
		
		cx = cx/2;
		cy = cy/2;
		if (cy>res_y_mid2) cy -= 40;
		//if (cy>280) cy -= 40;

		SetWindowPos(hWnd, HWND_TOP, cx - res_x_mid, cy - res_y_mid, res_x, res_y, SWP_SHOWWINDOW);
		memset( (VOID *)&ddsd, 0, sizeof(ddsd) );
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS;
		//ddsd.dwBackBufferCount = 0;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		
		ddVal = m_lpDD4->CreateSurface(&ddsd, &m_lpFrontB4, NULL);
		if (ddVal != DD_OK) return FALSE;

		SetRect(&m_rcFlipping, cx - res_x_mid, cy - res_y_mid, cx + res_x_mid, cy + res_y_mid); // our fictitious sprite bitmap is 
	}

	InitFlipToGDI(hWnd);

	//SendAltEnter();

	m_lpBackB4 = pCreateOffScreenSurface(res_x, res_y);
	if (!m_lpBackB4) {
		MessageBox(NULL, "Error creando m_lpBackB4", "Debug", MB_OK);
		return FALSE;
	}

	// Pre-draw background surface
	m_lpPDBGS = pCreateOffScreenSurface(res_x + 32, res_y + 32);
	if (m_lpPDBGS == NULL) return FALSE;

	ddsd.dwSize = sizeof(ddsd);
	if (m_lpBackB4->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return FALSE;
	m_pBackB4Addr        = (WORD *)ddsd.lpSurface;
	m_sBackB4Pitch       = (short)ddsd.lPitch >> 1;
	m_lpBackB4->Unlock(NULL);

	_TestPixelFormat();
	for (iS = 0; iS < 64; iS++)
	for (iD = 0; iD < 64; iD++) {
		m_lTransRB100[iD][iS] = _CalcMaxValue(iS, iD, 'R', 1, 1.0f);
		m_lTransG100[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 1, 1.0f);

		m_lTransRB70[iD][iS] = _CalcMaxValue(iS, iD, 'R', 1, 0.7f);
		m_lTransG70[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 1, 0.7f);
		
		m_lTransRB50[iD][iS] = _CalcMaxValue(iS, iD, 'R', 1, 0.5f);
		m_lTransG50[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 1, 0.5f);

		m_lTransRB25[iD][iS] = _CalcMaxValue(iS, iD, 'R', 1, 0.25f);
		m_lTransG25[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 1, 0.25f);

		m_lTransRB2[iD][iS] = _CalcMaxValue(iS, iD, 'R', 2, 1.0f);
		m_lTransG2[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 2, 1.0f);

		m_lFadeRB[iD][iS]  = _CalcMinValue(iS, iD, 'R');
		m_lFadeG[iD][iS]   = _CalcMinValue(iS, iD, 'G');

		G_lTransRB100[iD][iS] = _CalcMaxValue(iS, iD, 'R', 1, 1.0f);
		G_lTransG100[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 1, 1.0f);

		G_lTransRB70[iD][iS] = _CalcMaxValue(iS, iD, 'R', 1, 0.7f);
		G_lTransG70[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 1, 0.7f);
		
		G_lTransRB50[iD][iS] = _CalcMaxValue(iS, iD, 'R', 1, 0.5f);
		G_lTransG50[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 1, 0.5f);

		G_lTransRB25[iD][iS] = _CalcMaxValue(iS, iD, 'R', 1, 0.25f);
		G_lTransG25[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 1, 0.25f);

		G_lTransRB2[iD][iS] = _CalcMaxValue(iS, iD, 'R', 2, 1.0f);
		G_lTransG2[iD][iS]  = _CalcMaxValue(iS, iD, 'G', 2, 1.0f);
	}

	m_hFontInUse = NULL;
	m_hFontInUse2 = NULL;
	m_hFontInUse3 = NULL;
	m_hFontInUse4 = NULL;
	m_hFontInUse5 = NULL;
	m_hFontHP = NULL;
	m_hFontInUse = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, "Comic Sans MS");
	m_hFontInUse2 = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, "Cambria");
	m_hFontInUse3 = CreateFont(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, "Verdana");
	m_hFontInUse4 = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, "Calibri");
	m_hFontInUse5 = CreateFont(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, "Tahoma");
	m_hFontHP = CreateFont(13, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, "Times New Roman");
	//m_hFontInUse = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, "Comic Sans MS");
	m_hDC = NULL;
		
	m_init = true;

	/*// === Inicializar backend acelerado D3D9 si está habilitado ===
	m_bUseD3D9 = true;
	if (m_bUseD3D9)
	{
		D3D9_Init(hWnd, res_x, res_y);
	}

	if (m_bD3D9Ready)
	{

		MessageBox(NULL, "dx9 started", "Debug", MB_OK);
	}*/
	return TRUE;
}





void DXC_ddraw::ChangeBPP(i8 bpp) {
	//return;
	DEVMODE d = { 0 };
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &d);

	d.dmBitsPerPel = bpp;
	ChangeDisplaySettings(&d, 0);
}


HRESULT DXC_ddraw::iFlip()
{
	HRESULT ddVal;

	ddVal = m_lpBackB4flip->BltFast(0, 0, m_lpBackB4, &m_rcFlipping, DDBLTFAST_NOCOLORKEY); // | DDBLTFAST_WAIT);
	ddVal = m_lpFrontB4->Flip(m_lpBackB4flip, DDFLIP_NOVSYNC);

	if (ddVal == DDERR_SURFACELOST) {
		OSVERSIONINFO osvi;
		DDSURFACEDESC2 ddsd;

		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		GetVersionEx(&osvi);

		if (osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion >= 2 || osvi.dwMajorVersion >= 10) {
			if (!ChangeDisplayMode(G_hWnd)) return FALSE;
		}
		else {
			m_lpFrontB4->Restore();
			m_lpBackB4->Restore();

			ddsd.dwSize = sizeof(ddsd);
			if (m_lpBackB4->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return FALSE;
			m_pBackB4Addr = (WORD*)ddsd.lpSurface;
			m_lpBackB4->Unlock(NULL);
		}

		return DDERR_SURFACELOST;
	}
	return ddVal;
}

/*
HRESULT DXC_ddraw::iFlip()
{
	// === NUEVO: modo acelerado por D3D9Ex ===
	if (m_bUseD3D9 && m_bD3D9Ready)
	{
		D3D9_UploadBuffer();
		D3D9_Flip();
		return DD_OK; // todo correcto, ya dibujamos con GPU
	}

	// === MODO CLÁSICO DirectDraw7 ===
	HRESULT ddVal;

	ddVal = m_lpBackB4flip->BltFast(0, 0, m_lpBackB4, &m_rcFlipping, DDBLTFAST_NOCOLORKEY);
	ddVal = m_lpFrontB4->Flip(m_lpBackB4flip, DDFLIP_NOVSYNC);

	// --- Manejo de superficies perdidas ---
	if (ddVal == DDERR_SURFACELOST)
	{
		OSVERSIONINFO osvi;
		DDSURFACEDESC2 ddsd;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osvi);

		// Si es Windows 8 o superior, reiniciamos display mode
		if (osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion >= 2 || osvi.dwMajorVersion >= 10)
		{
			if (!ChangeDisplayMode(G_hWnd)) return FALSE;
		}
		else
		{
			m_lpFrontB4->Restore();
			m_lpBackB4->Restore();

			ddsd.dwSize = sizeof(ddsd);
			if (m_lpBackB4->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return FALSE;
			m_pBackB4Addr = (WORD*)ddsd.lpSurface;
			m_lpBackB4->Unlock(NULL);
		}

		return DDERR_SURFACELOST;
	}

	return ddVal;
}
*/
bool DXC_ddraw::ChangeDisplayMode(HWND hWnd)
{
	HRESULT        ddVal;
	DDSURFACEDESC2 ddsd;
	//added resolution
	int res_x = 0;
	int res_y = 0;
	int res_x_mid = 0;
	int res_y_mid = 0;
	int res_y_mid2 = 0;

	if (!m_init)
		return FALSE;

	if (m_lpBackB4flip != NULL)
	{
		m_lpBackB4flip->Release();
		m_lpBackB4flip = NULL;
	}
	if (m_lpBackB4 != NULL) m_lpBackB4->Release();
	if (m_lpFrontB4 != NULL) m_lpFrontB4->Release();
	if (m_lpDD4 != NULL)
	{
		if (m_bSoloGrande == TRUE) m_lpDD4->RestoreDisplayMode();
	}

	if (c_reso->IsResolution() == C800x600)
	{
		res_x = 800;
		res_y = 600;
		res_x_mid = 400;
		res_y_mid = 300;
		res_y_mid2 = 340;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		res_x = 640;
		res_y = 480;
		res_x_mid = 320;
		res_y_mid = 240;
		res_y_mid2 = 280;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		res_x = 1024;
		res_y = 768;
		res_x_mid = 512;
		res_y_mid = 384;
		res_y_mid2 = 424;
	}
	
	SetRect(&m_rcClipArea, 0, 0, res_x, res_y);

	if (m_bSoloGrande == TRUE)
	{
		DDSCAPS2       ddscaps;
		ddVal = m_lpDD4->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
		if (ddVal != DD_OK) return FALSE;

		ddVal = m_lpDD4->SetDisplayMode(res_x, res_y, 16, 0, 0);
		if (ddVal != DD_OK) return FALSE;
		memset((VOID *)&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.dwBackBufferCount = 1;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;

		ddVal = m_lpDD4->CreateSurface(&ddsd, &m_lpFrontB4, NULL);
		if (ddVal != DD_OK) return FALSE;

		ZeroMemory(&ddscaps, sizeof(ddscaps));
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		ddVal = m_lpFrontB4->GetAttachedSurface(&ddscaps, &m_lpBackB4flip);
		if (ddVal != DD_OK) return FALSE;

		SetRect(&m_rcFlipping, 0, 0, res_x, res_y); // our fictitious sprite bitmap is 
	}
	else
	{
		int cx = GetSystemMetrics(SM_CXFULLSCREEN);
		int cy = GetSystemMetrics(SM_CYFULLSCREEN);

		ddVal = m_lpDD4->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
		if (ddVal != DD_OK) return FALSE;

		cx = cx / 2;
		cy = cy / 2;
				
		if (cy>res_y_mid2) cy -= 40;
		//if (cy > 280) cy -= 40;
		SetWindowPos(hWnd, HWND_TOP, cx - res_x_mid, cy - res_y_mid, res_x, res_y, SWP_SHOWWINDOW);

		memset((VOID *)&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		//ddsd.dwBackBufferCount = 0;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		ddVal = m_lpDD4->CreateSurface(&ddsd, &m_lpFrontB4, NULL);
		if (ddVal != DD_OK) return FALSE;
		SetRect(&m_rcFlipping, cx - res_x_mid, cy - res_y_mid, cx + res_x_mid, cy + res_y_mid); // our fictitious sprite bitmap is 
	}

	InitFlipToGDI(hWnd);
	m_lpBackB4 = pCreateOffScreenSurface(res_x, res_y);
	if (m_lpBackB4 == NULL) return FALSE;

	//Pre-draw background surface
	m_lpPDBGS = pCreateOffScreenSurface(res_x + 32, res_y + 32);
	if (m_lpPDBGS == NULL) return FALSE;

	ddsd.dwSize = sizeof(ddsd);
	if (m_lpBackB4->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return FALSE;
	m_pBackB4Addr = (WORD *)ddsd.lpSurface;
	m_sBackB4Pitch = (short)ddsd.lPitch >> 1;
	m_lpBackB4->Unlock(NULL);
	return TRUE;
}


IDirectDrawSurface7 * DXC_ddraw::pCreateOffScreenSurface(WORD wSzX, WORD wSzY)
{
	DDSURFACEDESC2 ddsd;
	IDirectDrawSurface7 * pdds4;

    ZeroMemory(&ddsd, sizeof(ddsd));
	if ((wSzX % 4) != 0) wSzX += 4 - (wSzX % 4);
    ddsd.dwSize  = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth  = (DWORD)wSzX;
    ddsd.dwHeight = (DWORD)wSzY;
    if (m_lpDD4->CreateSurface(&ddsd, &pdds4, NULL) != DD_OK) return NULL;
	return pdds4;
}

HRESULT DXC_ddraw::iSetColorKey(IDirectDrawSurface7 * pdds4, COLORREF rgb)
{
 DDCOLORKEY ddck;

	ddck.dwColorSpaceLowValue  = _dwColorMatch(pdds4, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds4->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

HRESULT DXC_ddraw::iSetColorKey(IDirectDrawSurface7 * pdds4, WORD wColorKey)
{
 DDCOLORKEY ddck;

	ddck.dwColorSpaceLowValue  = _dwColorMatch(pdds4, wColorKey);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds4->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

DWORD DXC_ddraw::_dwColorMatch(IDirectDrawSurface7 * pdds4, COLORREF rgb)
{
 COLORREF rgbT;
 HDC hdc;
 DWORD dw = CLR_INVALID;
 DDSURFACEDESC2 ddsd2;
 HRESULT hres;

    if (rgb != CLR_INVALID && pdds4->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);             
        SetPixel(hdc, 0, 0, rgb);               
        pdds4->ReleaseDC(hdc);
    }

    ddsd2.dwSize = sizeof(ddsd2);
    while ((hres = pdds4->Lock(NULL, &ddsd2, 0, NULL)) == DDERR_WASSTILLDRAWING);

    if (hres == DD_OK)
    {
        dw  = *(DWORD *)ddsd2.lpSurface;                     
        dw &= (1 << ddsd2.ddpfPixelFormat.dwRGBBitCount)-1;  
        pdds4->Unlock(NULL);
    }

    if (rgb != CLR_INVALID && pdds4->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds4->ReleaseDC(hdc);
    }

    return dw;
}

DWORD DXC_ddraw::_dwColorMatch(IDirectDrawSurface7 * pdds4, WORD wColorKey)
{
 DWORD dw = CLR_INVALID, * dwp;
 DDSURFACEDESC2 ddsd2;
 HRESULT hres;
   
    ddsd2.dwSize = sizeof(ddsd2);
    while ((hres = pdds4->Lock(NULL, &ddsd2, 0, NULL)) == DDERR_WASSTILLDRAWING);

    if (hres == DD_OK)
    {
        dwp = (DWORD *)ddsd2.lpSurface;
		*dwp = (DWORD)wColorKey;
		dw  = *(DWORD *)ddsd2.lpSurface;                     
        dw &= (1 << ddsd2.ddpfPixelFormat.dwRGBBitCount)-1;  
        pdds4->Unlock(NULL);
    }

    return dw;
}

void DXC_ddraw::TextOut(int x, int y, char * cStr, COLORREF rgb)
{
	SetTextColor(m_hDC, rgb );
	::TextOut(m_hDC, x, y, cStr, strlen(cStr));
}

void DXC_ddraw::_TestPixelFormat()
{
 DDSURFACEDESC2 ddSurfaceDesc2;
 HRESULT       hResult;

	ZeroMemory(&ddSurfaceDesc2, sizeof(DDSURFACEDESC2));
	ddSurfaceDesc2.dwSize  = sizeof(ddSurfaceDesc2);
	ddSurfaceDesc2.dwFlags = DDSD_PIXELFORMAT;
	hResult                = m_lpBackB4->GetSurfaceDesc(&ddSurfaceDesc2);

	if (hResult == DD_OK)
	{
	   if (ddSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x0000F800) {
		   m_cPixelFormat = 1;
		  // RGB 5:6:5
	   }
	   if (ddSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00007C00) {
		   m_cPixelFormat = 2;
		  // RGB 5:5:5 
	   }
	   if (ddSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x0000001F) {
		   m_cPixelFormat = 3;
		  // BGR 5:6:5 
	   }
	}
}

long DXC_ddraw::_CalcMaxValue(int iS, int iD, char cMode, char cMethod, double dAlpha)
{
 long Sum;
 double dTmp;
 
	switch (cMethod) {
	case 1:
		dTmp = (double)iS;
		dTmp = dTmp * dAlpha;
		iS = (int)dTmp;
		Sum = (iS) + (iD);
		if (Sum < iD) Sum = iD;
		break;

	case 2:
		Sum = (iS + iD) / 2;
		break;
	}

	switch (cMode) {
	case 'G':
		switch (m_cPixelFormat) {
		case 1:
			if (Sum >= 64) Sum = 63; //v1.3
			break;
		case 2:
			if (Sum >= 32) Sum = 31;
			break;
		}
		break;
	
	default:
		if (Sum >= 32) Sum = 31;
		break;
	}

	return Sum;
}

long DXC_ddraw::_CalcMinValue(int iS, int iD, char cMode)
{
 long Sum;

 
	Sum = iD - iS;
	if (Sum < 0) Sum = 0;

	switch (cMode) {
	case 'G':
		switch (m_cPixelFormat) {
		case 1:
			if (Sum >= 64) Sum = 63;
			break;
		case 2:
			if (Sum >= 32) Sum = 31;
			break;
		}
		break;
	
	default:
		if (Sum >= 32) Sum = 31;
		break;
	}

	return Sum;
}

void DXC_ddraw::ClearBackB4()
{
	
	//added resolution
	int res_y = 0;
	if (c_reso->IsResolution() == C800x600)
	{
		res_y = 600;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		res_y = 480;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		res_y = 768;
	}

	DDSURFACEDESC2 ddsd2;
	ddsd2.dwSize = sizeof(ddsd2);
	if (m_lpBackB4->Lock(NULL, &ddsd2, DDLOCK_WAIT, NULL) != DD_OK) return;
	memset((char *)ddsd2.lpSurface, 0, ddsd2.lPitch * res_y);
	m_lpBackB4->Unlock(NULL);
}

void DXC_ddraw::DrawShadowBox(short sX, short sY, short dX, short dY, int iType)
{
	WORD * pDst, wValue;
	int ix, iy;

	//pDst = (WORD *)m_pBackB4Addr + sX + ((sY)*m_sBackB4Pitch);
	
	//fix shadow box (addded resolution)
	if (c_reso->IsResolution() == C800x600)
	{
		if (dX >= 800) dX = 800 - 1;
		if (dX < 0)	return;
		if (sX < 0)	sX = 0;
		if (sX >= 800) return;
		if (dY >= 600) dY = 600 - 1;
		if (dY < 0)	return;
		if (sY < 0)	sY = 0;
		if (sY >= 600) return;

		//if (dX > 800) dX = 800;
		//if (dY > 600) dY = 600;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		if (dX >= 640) dX = 640 - 1;
		if (dX < 0)	return;
		if (sX < 0)	sX = 0;
		if (sX >= 640) return;
		if (dY >= 480) dY = 480 - 1;
		if (dY < 0)	return;
		if (sY < 0)	sY = 0;
		if (sY >= 480) return;

		//if (dX > 640) dX = 640; //original
		//if (dY > 480) dY = 480;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		if (dX >= 1024) dX = 1024 - 1;
		if (dX < 0)	return;
		if (sX < 0)	sX = 0;
		if (sX >= 1024) return;
		if (dY >= 768) dY = 768 - 1;
		if (dY < 0)	return;
		if (sY < 0)	sY = 0;
		if (sY >= 768) return;

		//if (dX > 640) dX = 640; //original
		//if (dY > 480) dY = 480;
	}

	pDst = (WORD *)m_pBackB4Addr + sX + ((sY)*m_sBackB4Pitch);
	//Original
	/*if (dX > 640) dX = 640;
	if (dY > 480) dY = 480;*/

	if (iType == 0) {
		switch (m_cPixelFormat) {
		case 1:
			for (iy = 0; iy <= (dY - sY); iy++) {
				for (ix = 0; ix <= (dX - sX); ix++) 
					pDst[ix] = (pDst[ix] & 0xf7de) >> 1; 	
			
				pDst += m_sBackB4Pitch;
			}
			break;

		case 2:
			for (iy = 0; iy <= (dY - sY); iy++) {
				for (ix = 0; ix <= (dX - sX); ix++) 
					pDst[ix] = (pDst[ix] & 0x7bde) >> 1;
				
				pDst += m_sBackB4Pitch;
			}
			break;
		}
	}
	else 
	{
		switch (iType) {
		case 1:
			if (m_cPixelFormat == 1) 
				 wValue = 0x38e7;
			else wValue = 0x1ce7;
			break;

		case 2:
			if (m_cPixelFormat == 1) 
				 wValue = 0x1863;
			else wValue = 0xc63;
			break;
		}
		
		for (iy = 0; iy <= (dY - sY); iy++) {
			for (ix = 0; ix <= (dX - sX); ix++) 
				pDst[ix] = wValue; 	
				
			pDst += m_sBackB4Pitch;
		}
	}
}

void DXC_ddraw::DrawRedTransparentBox(short sX, short sY, short dX, short dY, int iType)
{
    WORD * pDst, wValue;
    int ix, iy;

    // Ajustes de resolución (igual que el original)
    if (c_reso->IsResolution() == C800x600)
    {
        if (dX >= 800) dX = 800 - 1;
        if (dX < 0)	return;
        if (sX < 0)	sX = 0;
        if (sX >= 800) return;
        if (dY >= 600) dY = 600 - 1;
        if (dY < 0)	return;
        if (sY < 0)	sY = 0;
        if (sY >= 600) return;
    }
    else if (c_reso->IsResolution() == C640x480)
    {
        if (dX >= 640) dX = 640 - 1;
        if (dX < 0)	return;
        if (sX < 0)	sX = 0;
        if (sX >= 640) return;
        if (dY >= 480) dY = 480 - 1;
        if (dY < 0)	return;
        if (sY < 0)	sY = 0;
        if (sY >= 480) return;
    }
    else if (c_reso->IsResolution() == C1024x768)
    {
        if (dX >= 1024) dX = 1024 - 1;
        if (dX < 0)	return;
        if (sX < 0)	sX = 0;
        if (sX >= 1024) return;
        if (dY >= 768) dY = 768 - 1;
        if (dY < 0)	return;
        if (sY < 0)	sY = 0;
        if (sY >= 768) return;
    }

    pDst = (WORD *)m_pBackB4Addr + sX + ((sY)*m_sBackB4Pitch);

    if (iType == 0) {
        // Modo transparente (mezcla con rojo)
        switch (m_cPixelFormat) {
        case 1: // Probablemente 16-bit 565
            for (iy = 0; iy <= (dY - sY); iy++) {
                for (ix = 0; ix <= (dX - sX); ix++) {
                    // Mezcla el color existente con rojo (0xF800) al 50%
                    pDst[ix] = ((pDst[ix] & 0xF7DE) >> 1) | 0x4000;
                }
                pDst += m_sBackB4Pitch;
            }
            break;

        case 2: // Probablemente 16-bit 555
            for (iy = 0; iy <= (dY - sY); iy++) {
                for (ix = 0; ix <= (dX - sX); ix++) {
                    // Mezcla el color existente con rojo (0x7C00) al 50%
                    pDst[ix] = ((pDst[ix] & 0x7BDE) >> 1) | 0x2000;
                }
                pDst += m_sBackB4Pitch;
            }
            break;
        }
    }
    else {
        // Modo sólido (pero rojo)
        switch (iType) {
        case 1: // Rojo semioscuro
            if (m_cPixelFormat == 1) 
                 wValue = 0xA000; // Rojo medio en 565
            else wValue = 0x5000; // Rojo medio en 555
            break;

        case 2: // Rojo oscuro
            if (m_cPixelFormat == 1) 
                 wValue = 0x6000; // Rojo oscuro en 565
            else wValue = 0x3000; // Rojo oscuro en 555
            break;
        }
        
        for (iy = 0; iy <= (dY - sY); iy++) {
            for (ix = 0; ix <= (dX - sX); ix++) 
                pDst[ix] = wValue;
                
            pDst += m_sBackB4Pitch;
        }
    }
}

void DXC_ddraw::PutPixel(short sX, short sY, WORD wR, WORD wG, WORD wB)
{
 WORD * pDst;

	//if ((sX < 0) || (sY < 0) || (sX > 639) || (sY > 479)) return; Original

	//added resolution
	if (c_reso->IsResolution() == C800x600)
	{
		if ((sX < 0) || (sY < 0) || (sX > 799) || (sY > 599)) return;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		if ((sX < 0) || (sY < 0) || (sX > 639) || (sY > 479)) return;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		if ((sX < 0) || (sY < 0) || (sX > 1023) || (sY > 767)) return;
	}


	pDst = (WORD *)m_pBackB4Addr + sX + ((sY)*m_sBackB4Pitch);
	
	switch (m_cPixelFormat) {
	case 1:
		*pDst = (WORD)( ((wR>>3)<<11) | ((wG>>2)<<5) | (wB>>3) );
		//*pDst = (WORD)((wR<<11) | (wG<<5) | wB);
		break;
	case 2:
		*pDst = (WORD)( ((wR>>3)<<10) | ((wG>>3)<<5) | (wB>>3) );
		//*pDst = (WORD)((wR<<10) | (wG<<5) | wB);
		break;
	}
}

void DXC_ddraw::_GetBackBufferDC4()
{
	m_lpBackB4->GetDC(&m_hDC);
	SelectObject(m_hDC, m_hFontInUse);
	SetBkMode(m_hDC, TRANSPARENT);
	SetBkColor(m_hDC, RGB(0, 0, 0));
}
void DXC_ddraw::_GetBackBufferDC()
{
	m_lpBackB4->GetDC(&m_hDC);
	SelectObject(m_hDC, m_hFontInUse);
	SetBkMode(m_hDC, TRANSPARENT);
	SetBkColor(m_hDC, RGB(0,0,0));
}

void DXC_ddraw::_ReleaseBackBufferDC()
{
	m_lpBackB4->ReleaseDC(m_hDC);
}

void DXC_ddraw::_GetBackBufferDC2()
{
	m_lpBackB4->GetDC(&m_hDC);
	SelectObject(m_hDC, m_hFontInUse2);
	SetBkMode(m_hDC, TRANSPARENT);
	SetBkColor(m_hDC, RGB(0, 0, 0));
}

void DXC_ddraw::_ReleaseBackBufferDC2()
{
	m_lpBackB4->ReleaseDC(m_hDC);
}

void DXC_ddraw::_GetBackBufferDC3()
{
	m_lpBackB4->GetDC(&m_hDC);
	SelectObject(m_hDC, m_hFontInUse3);
	SetBkMode(m_hDC, TRANSPARENT);
	SetBkColor(m_hDC, RGB(0, 0, 0));
}

void DXC_ddraw::_ReleaseBackBufferDC3()
{
	m_lpBackB4->ReleaseDC(m_hDC);
}


void DXC_ddraw::_GetBackBufferDC5()
{
	m_lpBackB4->GetDC(&m_hDC);
	SelectObject(m_hDC, m_hFontInUse4);
	SetBkMode(m_hDC, TRANSPARENT);
	SetBkColor(m_hDC, RGB(0, 0, 0));
}


void DXC_ddraw::_GetBackBufferDC6()
{
	m_lpBackB4->GetDC(&m_hDC);
	SelectObject(m_hDC, m_hFontInUse5);
	SetBkMode(m_hDC, TRANSPARENT);
	SetBkColor(m_hDC, RGB(0, 0, 0));
}

void DXC_ddraw::_GetBackBufferDC_HP()
{
	m_lpBackB4->GetDC(&m_hDC);
	SelectObject(m_hDC, m_hFontHP);
	SetBkMode(m_hDC, TRANSPARENT);
	SetBkColor(m_hDC, RGB(0, 0, 0));
}

void DXC_ddraw::_ReleaseBackBufferDC_HP()
{
	m_lpBackB4->ReleaseDC(m_hDC);
}

void DXC_ddraw::_ReleaseBackBufferDC5()
{
	m_lpBackB4->ReleaseDC(m_hDC);
}


void DXC_ddraw::DrawText(LPRECT pRect, char *pString, COLORREF rgb)
{
	SetTextColor(m_hDC, rgb);
	::DrawText(m_hDC, pString, strlen(pString), pRect, DT_CENTER | DT_NOCLIP | DT_WORDBREAK | DT_NOPREFIX);//v2.15
}

HRESULT DXC_ddraw::InitFlipToGDI(HWND hWnd)
{
 LPDIRECTDRAWCLIPPER pClipper;
 HRESULT hr;
 DDCAPS ddcaps;

    ZeroMemory( &ddcaps, sizeof(ddcaps) );
    ddcaps.dwSize = sizeof(ddcaps);
    m_lpDD4->GetCaps( &ddcaps, NULL );

    if( (ddcaps.dwCaps2 & DDCAPS2_CANRENDERWINDOWED) == 0 )
    {
        // This means FlipToGDISurface() is not supported, so to display GDI 
        // on these cards, you you must create a bitmap of the GDI window 
        // and BitBlt the bitmap to the backbuffer then flip as normal. However,
        // this specific sample does not show this. 
        return E_FAIL;
    }

    // Create a clipper when using GDI to draw on the primary surface 
    if( FAILED( hr = m_lpDD4->CreateClipper( 0, &pClipper, NULL ) ) )
        return hr;

    pClipper->SetHWnd( 0, hWnd );

    if( FAILED( hr = m_lpFrontB4->SetClipper( pClipper ) ) ) return hr;

    // We can release the clipper now since g_pDDSPrimary 
    // now maintains a ref count on the clipper
	if( pClipper )
	{
		pClipper->Release();
		pClipper = NULL;
	}
    return S_OK;
}

void DXC_ddraw::ColorTransferRGB(COLORREF fcolor, int * iR, int * iG, int * iB)
{
 WORD wR, wG, wB;

	switch(m_cPixelFormat)
	{
	case 1:
		// R
		wR = (WORD)((fcolor&0x000000f8)>>3);
		// G
		wG = (WORD)((fcolor&0x0000fc00)>>10);
		// B
		wB = (WORD)((fcolor&0x00f80000)>>19);
		*iR = (int)wR;
		*iG = (int)wG;
		*iB = (int)wB;
		break;
	case 2:
		// R
		wR = (WORD)((fcolor&0x000000f8)>>3);
		// G
		wG = (WORD)((fcolor&0x0000f800)>>11);
		// B
		wB = (WORD)((fcolor&0x00f80000)>>19);
		*iR = (int)wR;
		*iG = (int)wG;
		*iB = (int)wB;
		break;
	}
}

//---------------------------------------------------------------------------

bool DXC_ddraw::Screenshot(LPCTSTR FileName, LPDIRECTDRAWSURFACE7 lpDDS)
{
    if (!FileName || !lpDDS) return false;

    bool Success=false;
    HDC SurfDC=NULL;        // GDI-compatible device context for the surface
    HBITMAP OffscrBmp=NULL; // bitmap that is converted to a DIB
    HDC OffscrDC=NULL;      // offscreen DC that we can select OffscrBmp into
    LPBITMAPINFO lpbi=NULL; // bitmap format info; used by GetDIBits
    LPVOID lpvBits=NULL;    // pointer to bitmap bits array
    HANDLE BmpFile=INVALID_HANDLE_VALUE;    // destination .bmp file
    BITMAPFILEHEADER bmfh;  // .bmp file header

try
{
    // Get dimensions of Surface:
    DDSURFACEDESC2 ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    if (FAILED(lpDDS->GetSurfaceDesc(&ddsd))) throw 0;
    int Width = ddsd.dwWidth;
    int Height = ddsd.dwHeight;

    // Create a GDI-compatible device context for the surface:
    if (FAILED(lpDDS->GetDC(&SurfDC))) throw 1;

    // We need an HBITMAP to convert it to a DIB:
    if ((OffscrBmp = CreateCompatibleBitmap(SurfDC, Width, Height)) == NULL)
        throw 2;

    // The bitmap is empty, so let's copy the contents of the surface to it.
    // For that we need to select it into a device context. We create one.
    if ((OffscrDC = CreateCompatibleDC(SurfDC)) == NULL) throw 3;
    // Select OffscrBmp into OffscrDC:
    HBITMAP OldBmp = (HBITMAP)SelectObject(OffscrDC, OffscrBmp);
    // Now we can copy the contents of the surface to the offscreen bitmap:
    BitBlt(OffscrDC, 0, 0, Width, Height, SurfDC, 0, 0, SRCCOPY);

    // We don't need SurfDC anymore. Free it:
    lpDDS->ReleaseDC(SurfDC); SurfDC = NULL;

    // GetDIBits requires format info about the bitmap. We can have GetDIBits
    // fill a structure with that info if we pass a NULL pointer for lpvBits:
    // Reserve memory for bitmap info (BITMAPINFOHEADER + largest possible
    // palette):
    if ((lpbi = (LPBITMAPINFO)(new char[sizeof(BITMAPINFOHEADER) +
        256 * sizeof(RGBQUAD)])) == NULL) throw 4;
    ZeroMemory(&lpbi->bmiHeader, sizeof(BITMAPINFOHEADER));
    lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    // Get info but first de-select OffscrBmp because GetDIBits requires it:
    SelectObject(OffscrDC, OldBmp);
    if (!GetDIBits(OffscrDC, OffscrBmp, 0, Height, NULL, lpbi, DIB_RGB_COLORS))
        throw 5;

    // Reserve memory for bitmap bits:
    if ((lpvBits = new char[lpbi->bmiHeader.biSizeImage]) == NULL)
        throw 6;

    // Have GetDIBits convert OffscrBmp to a DIB (device-independent bitmap):
    if (!GetDIBits(OffscrDC, OffscrBmp, 0, Height, lpvBits, lpbi,
        DIB_RGB_COLORS)) throw 7;

    // Create a file to save the DIB to:
    if ((BmpFile = CreateFile(FileName,
                              GENERIC_WRITE,
                              0, NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL)) == INVALID_HANDLE_VALUE) throw 8;

    DWORD Written;    // number of bytes written by WriteFile
    
    // Write a file header to the file:
    bmfh.bfType = 19778;        // 'BM'
    // bmfh.bfSize = ???        // we'll write that later
    bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
    // bmfh.bfOffBits = ???     // we'll write that later
    if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
        throw 9;
    if (Written < sizeof(bmfh)) throw 9;

    // Write BITMAPINFOHEADER to the file:
    if (!WriteFile(BmpFile, &lpbi->bmiHeader, sizeof(BITMAPINFOHEADER),
        &Written, NULL)) throw 10;
    if (Written < sizeof(BITMAPINFOHEADER)) throw 10;

    // Calculate size of palette:
    int PalEntries;
    // 16-bit or 32-bit bitmaps require bit masks:
    if (lpbi->bmiHeader.biCompression == BI_BITFIELDS) PalEntries = 3;
    else
        // bitmap is palettized?
        PalEntries = (lpbi->bmiHeader.biBitCount <= 8) ?
            // 2^biBitCount palette entries max.:
            (int)(1 << lpbi->bmiHeader.biBitCount)
        // bitmap is TrueColor -> no palette:
        : 0;
    // If biClrUsed use only biClrUsed palette entries:
    if (lpbi->bmiHeader.biClrUsed) PalEntries = lpbi->bmiHeader.biClrUsed;

    // Write palette to the file:
    if (PalEntries)
    {
        if (!WriteFile(BmpFile, &lpbi->bmiColors, PalEntries * sizeof(RGBQUAD),
            &Written, NULL)) throw 11;
        if (Written < PalEntries * sizeof(RGBQUAD)) throw 11;
    }

    // The current position in the file (at the beginning of the bitmap bits)
    // will be saved to the BITMAPFILEHEADER:
    bmfh.bfOffBits = SetFilePointer(BmpFile, 0, 0, FILE_CURRENT);

    // Write bitmap bits to the file:
    if (!WriteFile(BmpFile, lpvBits, lpbi->bmiHeader.biSizeImage,
        &Written, NULL)) throw 12;
    if (Written < lpbi->bmiHeader.biSizeImage) throw 12;

    // The current pos. in the file is the final file size and will be saved:
    bmfh.bfSize = SetFilePointer(BmpFile, 0, 0, FILE_CURRENT);

    // We have all the info for the file header. Save the updated version:
    SetFilePointer(BmpFile, 0, 0, FILE_BEGIN);
    if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
        throw 13;
    if (Written < sizeof(bmfh)) throw 13;
	OutputDebugString("Screenshot Success\r\n");
    Success = true;
}
catch (int &errorcode)
{
    char Buf[100];
    wsprintf(Buf, "Screenshot error #%i\r\n", errorcode);
    OutputDebugString(Buf);
}
catch (...)
{
    OutputDebugString("Screenshot error\r\n");
}

    if (SurfDC) lpDDS->ReleaseDC(SurfDC);
    if (OffscrDC) DeleteDC(OffscrDC);
    if (OffscrBmp) DeleteObject(OffscrBmp);
    if (lpbi) delete[] lpbi;
    if (lpvBits) delete[] lpvBits;
    if (BmpFile != INVALID_HANDLE_VALUE) CloseHandle(BmpFile);
    
    return Success;
}
//---------------------------------------------------------------------------

void DXC_ddraw::DrawDialogShadowBox(short sX, short sY, short dX, short dY, int iType)
{
	WORD* pDst, wValue;
	int ix, iy;

	if (dX >= res_x) dX = res_x - 1;
	if (dX < 0) return;
	if (sX < 0) sX = 0;
	if (sX >= res_x) return;
	if (dY >= res_y) dY = res_y - 1;
	if (dY < 0) return;
	if (sY < 0) sY = 0;
	if (sY >= res_y) return;

	int countx = dX - sX;
	int county = dY - sY;

	// Ajustar el dibujo de los bordes para asegurarse de que todas las esquinas estén cubiertas
	for (int a = 0; a <= countx; a++)
	{
		PutPixel(sX + a, sY, 152, 123, 54);      // Línea superior
		PutPixel(sX + a, sY + 1, 152, 123, 54);  // Segunda línea superior
		PutPixel(sX + a, dY, 152, 123, 54);      // Línea inferior
		PutPixel(sX + a, dY - 1, 152, 123, 54);  // Segunda línea inferior
	}

	for (int b = 0; b <= county; b++)
	{
		PutPixel(sX, sY + b, 152, 123, 54);      // Línea izquierda
		PutPixel(sX + 1, sY + b, 152, 123, 54);  // Segunda línea izquierda
		PutPixel(dX, sY + b, 152, 123, 54);      // Línea derecha
		PutPixel(dX - 1, sY + b, 152, 123, 54);  // Segunda línea derecha
	}

	pDst = (WORD*)m_pBackB4Addr + sX + ((sY)*m_sBackB4Pitch);

	if (iType == 0) {
		switch (m_cPixelFormat) {
		case 1:
			for (iy = 0; iy <= (dY - sY); iy++) {

				for (ix = 0; ix <= (dX - sX); ix++)
					pDst[ix] = (pDst[ix] & 0xf7de) >> 1;

				pDst += m_sBackB4Pitch;
			}
			break;

		case 2:
			for (iy = 0; iy <= (dY - sY); iy++) {

				for (ix = 0; ix <= (dX - sX); ix++)
					pDst[ix] = (pDst[ix] & 0x7bde) >> 1;

				pDst += m_sBackB4Pitch;
			}
			break;
		}
	}
	else
	{
		switch (iType) {
		case 1:
			if (m_cPixelFormat == 1)
				wValue = 0x38e7;
			else wValue = 0x1ce7;
			break;

		case 2:
			if (m_cPixelFormat == 1)
				wValue = 0x1863;
			else wValue = 0xc63;
			break;
		}

		for (iy = 0; iy <= (dY - sY); iy++) {

			for (ix = 0; ix <= (dX - sX); ix++)
				pDst[ix] = wValue;

			pDst += m_sBackB4Pitch;
		}
	}
}

void DXC_ddraw::DrawDialogShadowBoxWithCorners(short sX, short sY, short dX, short dY, int iType)
{
	WORD* pDst, wValue;
	int ix, iy;

	if (dX >= res_x) dX = res_x - 1;
	if (dX < 0) return;
	if (sX < 0) sX = 0;
	if (sX >= res_x) return;
	if (dY >= res_y) dY = res_y - 1;
	if (dY < 0) return;
	if (sY < 0) sY = 0;
	if (sY >= res_y) return;

	int width = dX - sX;
	int height = dY - sY;
	int cornerRadius = 12; // 🔹 tamaño de redondeo (ajustalo a gusto)

	// === Relleno + esquinas redondeadas ===
	pDst = (WORD*)m_pBackB4Addr + sX + (sY * m_sBackB4Pitch);

	for (iy = 0; iy <= height; iy++) {
		for (ix = 0; ix <= width; ix++) {

			// --- Chequear corners ---
			bool skip = false;

			// sup izq
			if (ix < cornerRadius && iy < cornerRadius) {
				int dx = cornerRadius - ix;
				int dy = cornerRadius - iy;
				if (dx*dx + dy*dy > cornerRadius*cornerRadius) skip = true;
			}
			// sup der
			else if (ix > width - cornerRadius && iy < cornerRadius) {
				int dx = ix - (width - cornerRadius);
				int dy = cornerRadius - iy;
				if (dx*dx + dy*dy > cornerRadius*cornerRadius) skip = true;
			}
			// inf izq
			else if (ix < cornerRadius && iy > height - cornerRadius) {
				int dx = cornerRadius - ix;
				int dy = iy - (height - cornerRadius);
				if (dx*dx + dy*dy > cornerRadius*cornerRadius) skip = true;
			}
			// inf der
			else if (ix > width - cornerRadius && iy > height - cornerRadius) {
				int dx = ix - (width - cornerRadius);
				int dy = iy - (height - cornerRadius);
				if (dx*dx + dy*dy > cornerRadius*cornerRadius) skip = true;
			}

			if (skip) continue; // 🔹 no pintamos ese pixel → queda redondeado

			// === mismo relleno de antes ===
			if (iType == 0) {
				if (m_cPixelFormat == 1)
					pDst[ix] = (pDst[ix] & 0xf7de) >> 1;
				else
					pDst[ix] = (pDst[ix] & 0x7bde) >> 1;
			}
			else {
				switch (iType) {
				case 1:
					wValue = (m_cPixelFormat == 1) ? 0x38e7 : 0x1ce7;
					break;
				case 2:
					wValue = (m_cPixelFormat == 1) ? 0x1863 : 0x0c63;
					break;
				}
				pDst[ix] = wValue;
			}
		}
		pDst += m_sBackB4Pitch;
	}

	// 🔹 Ahora dibujar los bordes si querés mantenerlos,
	// pero también respetando el mismo `skip` para que no
	// se vea cuadrado en las esquinas.
}

void DXC_ddraw::DrawDialogShadowBoxOrange(short sX, short sY, short dX, short dY)
{
	WORD* pDst;
	int ix, iy;

	if (dX >= res_x) dX = res_x - 1;
	if (dX < 0) return;
	if (sX < 0) sX = 0;
	if (sX >= res_x) return;
	if (dY >= res_y) dY = res_y - 1;
	if (dY < 0) return;
	if (sY < 0) sY = 0;
	if (sY >= res_y) return;

	int countx = dX - sX;
	int county = dY - sY;

	// Ajustar el dibujo de los bordes para asegurarse de que todas las esquinas estén cubiertas
	for (int a = 0; a <= countx; a++)
	{
		PutPixel(sX + a, sY, 255, 165, 0);      // Línea superior (naranja)
		PutPixel(sX + a, sY + 1, 255, 165, 0);  // Segunda línea superior (naranja)
		PutPixel(sX + a, dY, 255, 165, 0);      // Línea inferior (naranja)
		PutPixel(sX + a, dY - 1, 255, 165, 0);  // Segunda línea inferior (naranja)
	}

	for (int b = 0; b <= county; b++)
	{
		PutPixel(sX, sY + b, 255, 165, 0);      // Línea izquierda (naranja)
		PutPixel(sX + 1, sY + b, 255, 165, 0);  // Segunda línea izquierda (naranja)
		PutPixel(dX, sY + b, 255, 165, 0);      // Línea derecha (naranja)
		PutPixel(dX - 1, sY + b, 255, 165, 0);  // Segunda línea derecha (naranja)
	}

	pDst = (WORD*)m_pBackB4Addr + sX + ((sY)* m_sBackB4Pitch);

	// Color naranja fijo para todo el área interna
	for (iy = 0; iy <= (dY - sY); iy++) {

		for (ix = 0; ix <= (dX - sX); ix++)
			pDst[ix] = 0xFFA500;  // Valor hexadecimal para naranja

		pDst += m_sBackB4Pitch;
	}
}

void DXC_ddraw::DrawDialogCircle(short centerX, short centerY, short radius, int iType)
{
	WORD* pDst, wValue;
	int ix, iy;

	// Calculate bounding box for the circle
	short sX = centerX - radius;
	short sY = centerY - radius;
	short dX = centerX + radius;
	short dY = centerY + radius;

	// Boundary checks
	if (dX >= res_x) dX = res_x - 1;
	if (dX < 0) return;
	if (sX < 0) sX = 0;
	if (sX >= res_x) return;
	if (dY >= res_y) dY = res_y - 1;
	if (dY < 0) return;
	if (sY < 0) sY = 0;
	if (sY >= res_y) return;

	// Draw circle outline
	for (int y = sY; y <= dY; y++) {
		for (int x = sX; x <= dX; x++) {
			int dx = x - centerX;
			int dy = y - centerY;
			int distance = (int)sqrt(dx*dx + dy*dy);

			// Draw pixels that are within 1 pixel of the radius (for anti-aliasing)
			if (abs(distance - radius) <= 1) {
				PutPixel(x, y, 152, 123, 54); // Same color as your boxes
			}
		}
	}

	// Fill the circle if needed (similar to your box filling)
	pDst = (WORD*)m_pBackB4Addr + sX + (sY * m_sBackB4Pitch);

	if (iType == 0) {
		switch (m_cPixelFormat) {
		case 1:
			for (iy = 0; iy <= (dY - sY); iy++) {
				for (ix = 0; ix <= (dX - sX); ix++) {
					int dx = (sX + ix) - centerX;
					int dy = (sY + iy) - centerY;
					if ((dx*dx + dy*dy) <= (radius*radius)) {
						pDst[ix] = (pDst[ix] & 0xf7de) >> 1;
					}
				}
				pDst += m_sBackB4Pitch;
			}
			break;

		case 2:
			for (iy = 0; iy <= (dY - sY); iy++) {
				for (ix = 0; ix <= (dX - sX); ix++) {
					int dx = (sX + ix) - centerX;
					int dy = (sY + iy) - centerY;
					if ((dx*dx + dy*dy) <= (radius*radius)) {
						pDst[ix] = (pDst[ix] & 0x7bde) >> 1;
					}
				}
				pDst += m_sBackB4Pitch;
			}
			break;
		}
	}
	else {
		switch (iType) {
		case 1:
			if (m_cPixelFormat == 1)
				wValue = 0x38e7;
			else wValue = 0x1ce7;
			break;

		case 2:
			if (m_cPixelFormat == 1)
				wValue = 0x1863;
			else wValue = 0xc63;
			break;
		}

		for (iy = 0; iy <= (dY - sY); iy++) {
			for (ix = 0; ix <= (dX - sX); ix++) {
				int dx = (sX + ix) - centerX;
				int dy = (sY + iy) - centerY;
				if ((dx*dx + dy*dy) <= (radius*radius)) {
					pDst[ix] = wValue;
				}
			}
			pDst += m_sBackB4Pitch;
		}
	}
}

void DXC_ddraw::DrawItemShadowBox(short sX, short sY, short dX, short dY, int iType)
{
	WORD * pDst, wValue;
	int ix, iy;
	int res_x = 0, res_y = 0;

	if (c_reso->IsResolution() == C640x480)
	{
		res_x = 640;
		res_y = 480;
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		res_x = 800;
		res_y = 600;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		res_x = 1024;
		res_y = 768;
	}

	if (dX >= res_x) dX = res_x - 1;
	if (dX < 0)	return;
	if (sX < 0)	sX = 0;
	if (sX >= res_x) return;
	if (dY >= res_y) dY = res_y - 1;
	if (dY < 0)	return;
	if (sY < 0)	sY = 0;
	if (sY >= res_y) return;

	pDst = (WORD *)m_pBackB4Addr + sX + ((sY)*m_sBackB4Pitch);

	if (iType == 0) {
		switch (m_cPixelFormat) {
		case 1:
			for (iy = 0; iy <= (dY - sY); iy++) {

				for (ix = 0; ix <= (dX - sX); ix++)
					pDst[ix] = (pDst[ix] & 0xf7de) >> 1;

				pDst += m_sBackB4Pitch;
			}
			break;

		case 2:
			for (iy = 0; iy <= (dY - sY); iy++) {

				for (ix = 0; ix <= (dX - sX); ix++)
					pDst[ix] = (pDst[ix] & 0x7bde) >> 1;

				pDst += m_sBackB4Pitch;
			}
			break;
		}
	}
	else
	{
		switch (iType) {
		case 1:
			if (m_cPixelFormat == 1)
				wValue = 0x38e7;
			else wValue = 0x1ce7;
			break;

		case 2:
			if (m_cPixelFormat == 1)
				wValue = 0x1863;
			else wValue = 0xc63;
			break;
		}

		for (iy = 0; iy <= (dY - sY); iy++) {

			for (ix = 0; ix <= (dX - sX); ix++)
				pDst[ix] = wValue;

			pDst += m_sBackB4Pitch;
		}
	}
}

void DXC_ddraw::PutTransparentPixel(short sX, short sY, WORD wR, WORD wG, WORD wB)
{
	WORD* pDst;

	if ((sX < 0) || (sY < 0) || (sX > res_x - 1) || (sY > res_y - 1)) return;

	pDst = (WORD*)m_pBackB4Addr + sX + ((sY)*m_sBackB4Pitch);

	WORD color;
	switch (m_cPixelFormat) {
	case 1:
		color = (WORD)(((wR >> 3) << 11) | ((wG >> 2) << 5) | (wB >> 3));
		break;
	case 2:
		color = (WORD)(((wR >> 3) << 10) | ((wG >> 3) << 5) | (wB >> 3));
		break;
	}

	// Aplicar transparencia
	WORD existingColor = *pDst;
	WORD transparentColor;

	switch (m_cPixelFormat) {
	case 1:
		transparentColor = ((existingColor & 0xf7de) >> 1) + ((color & 0xf7de) >> 1);
		break;
	case 2:
		transparentColor = ((existingColor & 0x7bde) >> 1) + ((color & 0x7bde) >> 1);
		break;
	}

	*pDst = transparentColor;
}

void DXC_ddraw::LaunchMumbleConFocoAsync() {
	std::thread([=]() {
		if (m_lpDD4 && G_hWnd)
			m_lpDD4->SetCooperativeLevel(G_hWnd, DDSCL_NORMAL);  // Liberar control

		Sleep(150); // Esperar que libere correctamente

		STARTUPINFOW si = { sizeof(si) };  // Unicode struct
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;

		PROCESS_INFORMATION pi;
		if (CreateProcessW(L"mumble\\mumble.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else {
			MessageBoxW(NULL, L"No se pudo lanzar Mumble.exe", L"Error", MB_OK | MB_ICONERROR);
		}

		if (m_lpDD4 && G_hWnd) {
			Sleep(500); // Esperar que Mumble termine de arrancar
			m_lpDD4->SetCooperativeLevel(G_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);  // Restaurar control
		}
	}).detach();
}
