// DXC_ddraw.h: interface for the DXC_ddraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXC_DDRAW_H__C52EBA83_6D9E_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_DXC_DDRAW_H__C52EBA83_6D9E_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <memory.h>
#include <string.h>
#include "ddraw.h"
#include "../Headers/Misc.h"


#define CHANGE16BPP				16
#define CHANGE32BPP				32

using namespace std;
typedef unsigned long long u64;
typedef signed long long i64;
typedef unsigned long u32;
typedef signed long i32;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned char u8;
typedef signed char i8;

class DXC_ddraw  
{
public:	
	void * operator new (size_t size) 
	{
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);	
	};	
	void operator delete(void * mem)
	{
		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem); 		
	};

	DXC_ddraw();
	virtual ~DXC_ddraw();

	void DrawItemShadowBox(short sX, short sY, short dX, short dY, int iType = 0);
	BOOL bInit(HWND hWnd);
	void ChangeBPP(i8 bpp);
	void ColorTransferRGB(COLORREF fcolor, int * iR, int * iG, int * iB);
	HRESULT InitFlipToGDI(HWND hWnd);
	bool ChangeDisplayMode(HWND hWnd);
	void _ReleaseBackBufferDC3();
	void _GetBackBufferDC5();
	void _GetBackBufferDC6();
	void _ReleaseBackBufferDC5();
	void DrawText(LPRECT pRect, char * pString, COLORREF rgb);
	void _ReleaseBackBufferDC();
	void _GetBackBufferDC2();
	void _ReleaseBackBufferDC2();
	void _GetBackBufferDC3();
	void _GetBackBufferDC();
	void TextOut(int x, int y, char * cStr, COLORREF rgb);
	void _TestPixelFormat();
	DWORD _dwColorMatch(IDirectDrawSurface7 * pdds4, WORD wColorKey);
	DWORD _dwColorMatch(IDirectDrawSurface7 * pdds4, COLORREF rgb);
	long _CalcMinValue(int iS, int iD, char cMode);
	long _CalcMaxValue(int iS, int iD, char cMode, char cMethod, double dAlpha);
	HRESULT iSetColorKey(IDirectDrawSurface7 * pdds4, WORD wColorKey);
	void PutPixel(short sX, short sY, WORD wR, WORD wG, WORD wB);
	void _GetBackBufferDC4();
	void _ReleaseBackBufferDC4();
	void DrawShadowBox(short sX, short sY, short dX, short dY, int iType = 0);
	void DrawRedTransparentBox(short sX, short sY, short dX, short dY, int iType = 0);
	void ClearBackB4();
	IDirectDrawSurface7 * pCreateOffScreenSurface(WORD iSzX, WORD iSzY);
	HRESULT iSetColorKey(IDirectDrawSurface7 * pdds4, COLORREF rgb);
	HRESULT iFlip();
	bool Screenshot(LPCTSTR FileName, LPDIRECTDRAWSURFACE7 lpDDS);

	bool ProcessScreenshotFromCopy(LPCTSTR FileName, LPDIRECTDRAWSURFACE7 surfaceCopy, int Width, int Height);
	void DrawDialogShadowBox(short sX, short sY, short dX, short dY, int iType = 0);
	void DrawDialogShadowBoxWithCorners(short sX, short sY, short dX, short dY, int iType = 0);
	void DrawDialogShadowBoxOrange(short sX, short sY, short dX, short dY);
	void DrawDialogCircle(short centerX, short centerY, short radius, int iType);
	//HeatoN 2024

	int res_x;
	int res_y;
	int res_x_mid;
	int res_y_mid;
	int res_y_mid2;

	void PutTransparentPixel(short sX, short sY, WORD wR, WORD wG, WORD wB);
	
	void LaunchMumbleConFocoAsync();
	/*void D3D9_Init(HWND hWnd, int width, int height);
	void D3D9_UploadBuffer();
	void ConvertRGB565toARGB(WORD* src, DWORD* dst, int width);
	void D3D9_Flip();
	void D3D9_Shutdown();
	bool m_bUseD3D9;        // Activar backend acelerado D3D9Ex
	bool m_bD3D9Ready = false;      // Indica si el dispositivo se inicializó correctamente*/
	long    m_lTransG100[64][64], m_lTransRB100[64][64];
	long    m_lTransG70[64][64], m_lTransRB70[64][64];
	long    m_lTransG50[64][64], m_lTransRB50[64][64];
	long    m_lTransG25[64][64], m_lTransRB25[64][64];
	long    m_lTransG2[64][64], m_lTransRB2[64][64];
	long    m_lFadeG[64][64],  m_lFadeRB[64][64];

	//Cambio LaloFullScreen Antihacks
	//BOOL m_bFullMode;
	BOOL m_bSoloGrande;

	LPDIRECTDRAW7		 m_lpDD4;
	LPDIRECTDRAWSURFACE7 m_lpFrontB4, m_lpBackB4, m_lpBackB4flip;
	LPDIRECTDRAWSURFACE7 m_lpPDBGS;// Pre-Draw Background Surface
	WORD * m_pBackB4Addr;
	RECT  m_rcClipArea, m_rcFlipping;
	short  m_sBackB4Pitch;
	char   m_cPixelFormat;
	HDC m_hDC;
	HFONT m_hFontInUse;
	HFONT m_hFontInUse2;
	HFONT m_hFontInUse3;
	HFONT m_hFontInUse4;
	HFONT m_hFontInUse5;

	bool m_init;
};

#endif // !defined(AFX_DXC_DDRAW_H__C52EBA83_6D9E_11D2_A8E6_00001C7030A6__INCLUDED_)
