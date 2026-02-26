// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "../Headers/Sprite.h"
//added resolution
#include "..\Headers\Game.h"
#include "..\Resolution\Resolution.h"
extern class cResolution * c_reso;

extern char G_cSpriteAlphaDegree;

extern int G_iAddTable31[64][510], G_iAddTable63[64][510];
extern int G_iAddTransTable31[510][64], G_iAddTransTable63[510][64]; 

extern long    G_lTransG100[64][64], G_lTransRB100[64][64];
extern long    G_lTransG70[64][64], G_lTransRB70[64][64];
extern long    G_lTransG50[64][64], G_lTransRB50[64][64];
extern long    G_lTransG25[64][64], G_lTransRB25[64][64];
extern long    G_lTransG2[64][64], G_lTransRB2[64][64];


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite(HANDLE hPakFile, DXC_ddraw *pDDraw, char *cPakFileName, short sNthFile, bool bAlphaEffect, bool Pak)
{
	DWORD  nCount;
	int iASDstart, Mult;

	m_stBrush	= NULL;
	m_lpSurface = NULL;
	m_bIsSurfaceEmpty = TRUE;
	ZeroMemory(m_cPakFileName, sizeof(m_cPakFileName));

	m_cAlphaDegree = 1;
	m_bOnCriticalSection = FALSE;
	m_iTotalFrame = 0;
	m_pDDraw = pDDraw;
	
	if (!Pak) Mult = 77;
	else Mult = 8;
	 
	SetFilePointer(hPakFile, 24 + sNthFile*Mult, NULL, FILE_BEGIN);
	ReadFile(hPakFile, &iASDstart,  4, &nCount, NULL); 

	//i+100       Sprite Confirm
	SetFilePointer(hPakFile, iASDstart+100, NULL, FILE_BEGIN); 
	ReadFile(hPakFile, &m_iTotalFrame,  4, &nCount, NULL);
	m_dwBitmapFileStartLoc = iASDstart  + (108 + (12*m_iTotalFrame));
	m_stBrush = new stBrush[m_iTotalFrame];
	ReadFile(hPakFile, m_stBrush, 12*m_iTotalFrame, &nCount, NULL);
	// PAK
	memcpy(m_cPakFileName, cPakFileName, strlen(cPakFileName));
	m_bAlphaEffect = bAlphaEffect;
}

CSprite::~CSprite()
{
	if (m_stBrush != NULL) delete[] m_stBrush;
	if (m_lpSurface != NULL) m_lpSurface->Release();
}

IDirectDrawSurface7 * CSprite::_pMakeSpriteSurface()
{
 IDirectDrawSurface7 * pdds4;
 HDC hDC;
 WORD * wp;

	m_bOnCriticalSection = TRUE;

	if( m_stBrush == NULL ) return NULL;

	CMyDib mydib(m_cPakFileName, m_dwBitmapFileStartLoc);
	m_wBitmapSizeX = mydib.m_wWidthX;
	m_wBitmapSizeY = mydib.m_wWidthY;
	pdds4 = m_pDDraw->pCreateOffScreenSurface(m_wBitmapSizeX, m_wBitmapSizeY);
    if (pdds4 == NULL) return NULL; 
	pdds4->GetDC(&hDC);
	mydib.PaintImage(hDC);
	pdds4->ReleaseDC(hDC);

	DDSURFACEDESC2  ddsd;
	ddsd.dwSize = 124;
	if (pdds4->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return NULL;
	pdds4->Unlock(NULL);

	wp = (WORD *)ddsd.lpSurface;
	m_wColorKey = *wp;

	m_bOnCriticalSection = FALSE;

    return pdds4;
}

void CSprite::PutSpriteFast(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	RECT rcRect;

	if (this == NULL) return; // Comprobación del puntero this
	if (m_stBrush == NULL) return; // Comprobación del puntero m_stBrush
	if (m_pDDraw == NULL) return; // Comprobación del puntero m_pDDraw
	if (m_pDDraw->m_lpBackB4 == NULL) return; // Comprobación del puntero m_lpBackB4
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return; // Validación de sFrame

	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// Validaciones para recortes en X
	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		sx = sx + (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX + szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	// Validaciones para recortes en Y
	if (dY < m_pDDraw->m_rcClipArea.top)
	{
		sy = sy + (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY + szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE)
	{
		if (_iOpenSprite() == FALSE) return; // Comprobación al abrir el sprite
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) return; // Comprobación al reabrir el sprite
			}
		}
	}

	rcRect.left = sx;
	rcRect.top = sy;
	rcRect.right = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	if (m_lpSurface == NULL) return; // Comprobación del puntero m_lpSurface

	HRESULT hr = m_pDDraw->m_lpBackB4->BltFast(dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	if (FAILED(hr)) {
		// Manejo del error si la operación BltFast falla
		m_bOnCriticalSection = FALSE;
		return;
	}

	m_bOnCriticalSection = FALSE;
}
void CSprite::PutSpriteFastClip(int sX, int sY, int srcX, int srcY, int srcW, int srcH, DWORD dwTime)
{
	if (this == NULL) return;
	if (m_pDDraw == NULL) return;
	if (m_pDDraw->m_lpBackB4 == NULL) return;

	m_bOnCriticalSection = TRUE;
	m_rcBound.top = -1;

	// === Asegurar que la superficie está cargada ===
	if (m_bIsSurfaceEmpty == TRUE)
	{
		if (_iOpenSprite() == FALSE) {
			m_bOnCriticalSection = FALSE;
			return;
		}
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2)
				_SetAlphaDegree();
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) {
					m_bOnCriticalSection = FALSE;
					return;
				}
			}
		}
	}

	if (m_lpSurface == NULL) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	// === Frame base ===
	short baseSX = m_stBrush[0].sx;
	short baseSY = m_stBrush[0].sy;

	// === Definir rectángulo fuente manualmente ===
	RECT rcRect;
	rcRect.left = baseSX + srcX;
	rcRect.top = baseSY + srcY;
	rcRect.right = baseSX + srcX + srcW;
	rcRect.bottom = baseSY + srcY + srcH;

	short dX = sX;
	short dY = sY;

	// Validaciones básicas del clip
	if (dX + srcW <= m_pDDraw->m_rcClipArea.left ||
		dY + srcH <= m_pDDraw->m_rcClipArea.top ||
		dX >= m_pDDraw->m_rcClipArea.right ||
		dY >= m_pDDraw->m_rcClipArea.bottom)
	{
		m_bOnCriticalSection = FALSE;
		return;
	}

	m_dwRefTime = dwTime;

	// === Blit ===
	HRESULT hr = m_pDDraw->m_lpBackB4->BltFast(
		dX, dY,
		m_lpSurface,
		&rcRect,
		DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT
		);

	if (FAILED(hr)) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + srcW;
	m_rcBound.bottom = dY + srcH;
	m_bOnCriticalSection = FALSE;
}



void CSprite::PutSpriteFastDst(LPDIRECTDRAWSURFACE7 lpDstS, int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE)
	{	if( _iOpenSprite() == FALSE ) return;
	}else // AlphaDegree
	{	if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) 
			{	_SetAlphaDegree();
			}else 
			{	_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
	}	}	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;
	lpDstS->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	m_bOnCriticalSection = FALSE;
}


void CSprite::PutSpriteFastNoColorKey(int sX, int sY, int sFrame, DWORD dwTime)
{short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;
	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;
  	dX = sX + pvx;
	dY = sY + pvy;
	if (dX < m_pDDraw->m_rcClipArea.left) 	
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
	{	sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) 
		{	m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{	szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) 
		{	m_rcBound.top = -1;
			return;
	}	}

	if (dY < m_pDDraw->m_rcClipArea.top) 
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
	{	sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) 
		{	m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{	szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) 
		{	m_rcBound.top = -1;
			return;
	}	}
	
	m_dwRefTime = dwTime;
	if (m_bIsSurfaceEmpty == TRUE)
	{	if( _iOpenSprite() == FALSE ) return;
	}else 
	{	if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) 
			{	_SetAlphaDegree();
			}else 
			{	_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
	}	}	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpBackB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutSpriteFastNoColorKeyDst(LPDIRECTDRAWSURFACE7 lpDstS, int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) 
	{	if( _iOpenSprite() == FALSE ) return;
	}else 
	{	if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) 
			{	_SetAlphaDegree();
			}else 
			{	_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
	}	}	}

	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	lpDstS->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutSpriteFastFrontBuffer(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpFrontB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutSpriteFastWidth(int sX, int sY, int sFrame, int sWidth, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (sWidth < szx)
		szx = sWidth;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX); //Agregado (unsigned short)
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX); //Agregado (unsigned short)
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpBackB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}


void CSprite::iRestore()
{
 HDC     hDC;
	
	if (m_bIsSurfaceEmpty) return;
	if( m_stBrush == NULL ) return;
	if (m_lpSurface->IsLost() == DD_OK) return;

	m_lpSurface->Restore();
	DDSURFACEDESC2  ddsd;
	ddsd.dwSize = 124;
	if (m_lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return;
	m_pSurfaceAddr = (WORD *)ddsd.lpSurface;
	m_lpSurface->Unlock(NULL);
	CMyDib mydib(m_cPakFileName, m_dwBitmapFileStartLoc);
	m_lpSurface->GetDC(&hDC);
	mydib.PaintImage(hDC);
	m_lpSurface->ReleaseDC(hDC);
}

void CSprite::PutShadowSprite(int sX, int sY, int sFrame, DWORD dwTime)
{
	short sx,sy,szx,szy,pvx,pvy;
	int  ix, iy;
	WORD * pSrc, * pDst;
	//added resolution
	int res_x = 0;
	int res_y = 0;

	if (c_reso->IsResolution() == C800x600)
	{
		res_x = 800;
		res_y = 547;//599
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		res_x = 1024;
		res_y = 715;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		res_x = 640;
		res_y = 427;//479
	}

	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}

	int iSangX, iSangY;
	pSrc = (WORD *)m_pSurfaceAddr + sx + sy*m_sPitch;
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr;// + dX + ((dY+szy-1)*m_pDDraw->m_sBackB4Pitch);

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		for( iy=0 ; iy<szy ; iy+= 3 )
		{
			for( ix=0 ; ix<szx ; ix++ )
			{
				iSangX = sX+pvx+ix+(iy-szy)/3;
				iSangY = sY+pvy+(iy+szy+szy)/3;
				if (pSrc[ix] != m_wColorKey)
				{
					if (iSangX >= 0 && iSangX < res_x && iSangY >= 0 && iSangY < res_y)
					{
						pDst[iSangY*m_pDDraw->m_sBackB4Pitch+iSangX] = ((pDst[iSangY*m_pDDraw->m_sBackB4Pitch+iSangX] & 0xE79C) >> 2);
					}
				}
			}
			pSrc += m_sPitch + m_sPitch + m_sPitch;
		}
		break;
	case 2:
		for( iy=0 ; iy<szy ; iy+= 3 )
		{
			for( ix=0 ; ix<szx ; ix++ )
			{
				iSangX = sX+pvx+ix+(iy-szy)/3;
				iSangY = sY+pvy+(iy+szy+szy)/3;
				if (pSrc[ix] != m_wColorKey)
				{
					if (iSangX >= 0 && iSangX < res_x && iSangY >= 0 && iSangY < res_y)
					{
						pDst[iSangY*m_pDDraw->m_sBackB4Pitch+iSangX] = ((pDst[iSangY*m_pDDraw->m_sBackB4Pitch+iSangX] & 0x739C) >> 2);
					}
				}
			}
			pSrc += m_sPitch + m_sPitch + m_sPitch;
		}
		break;
	}
	m_bOnCriticalSection = FALSE;
}


void CSprite::PutShadowSpriteClip(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;
	
	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;
	
	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy+szy-1)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY+szy-1)*m_pDDraw->m_sBackB4Pitch);

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		for (iy = 0; iy < szy; iy += 3) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey)
					if ( (dX - (iy/3) + ix)	> 0 )
						pDst[ix] = (pDst[ix] & 0xE79C) >> 2; 
			}
			pSrc -= m_sPitch + m_sPitch + m_sPitch;
			pDst -= m_pDDraw->m_sBackB4Pitch + 1;
		}
		break;

	case 2:
		for (iy = 0; iy < szy; iy += 3) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey)
					if ( (dX - (iy/3) + ix)	> 0 )
						pDst[ix] = (pDst[ix] & 0x739C) >> 2;
			}
			pSrc -= m_sPitch + m_sPitch + m_sPitch;
			pDst -= m_pDDraw->m_sBackB4Pitch + 1;
		}
		break;
	}

	m_bOnCriticalSection = FALSE;
}
void CSprite::PutAlphaSprite(int sX, int sY, int sFrame, DWORD dwTime, int alphaLevel)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	int ix, iy;
	WORD *pSrc, *pDst;

	if (this == NULL) return;
	if (m_stBrush == NULL) return;
	m_rcBound.top = -1;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// Clipping (mantener el mismo código de clipping)
	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		sx = sx + (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX + szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top)
	{
		sy = sy + (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY + szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) return;
	}

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)* m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)* m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	// Asegurar que alphaLevel esté en el rango 0-255
	alphaLevel = max(0, min(255, alphaLevel));
	float alpha = alphaLevel / 255.0f;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1: // 565 format
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey) {
					WORD srcColor = pSrc[ix];
					WORD dstColor = pDst[ix];

					// Extraer componentes RGB
					int srcR = (srcColor & 0xF800) >> 11;
					int srcG = (srcColor & 0x7E0) >> 5;
					int srcB = (srcColor & 0x1F);

					int dstR = (dstColor & 0xF800) >> 11;
					int dstG = (dstColor & 0x7E0) >> 5;
					int dstB = (dstColor & 0x1F);

					// Aplicar alpha blending
					int blendedR = (int)(srcR * alpha + dstR * (1.0f - alpha));
					int blendedG = (int)(srcG * alpha + dstG * (1.0f - alpha));
					int blendedB = (int)(srcB * alpha + dstB * (1.0f - alpha));

					// Asegurar que estén en rango
					blendedR = min(31, max(0, blendedR));
					blendedG = min(63, max(0, blendedG));
					blendedB = min(31, max(0, blendedB));

					pDst[ix] = (WORD)((blendedR << 11) | (blendedG << 5) | blendedB);
				}
			}
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
		}
		break;

	case 2: // 555 format
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey) {
					WORD srcColor = pSrc[ix];
					WORD dstColor = pDst[ix];

					// Extraer componentes RGB
					int srcR = (srcColor & 0x7C00) >> 10;
					int srcG = (srcColor & 0x3E0) >> 5;
					int srcB = (srcColor & 0x1F);

					int dstR = (dstColor & 0x7C00) >> 10;
					int dstG = (dstColor & 0x3E0) >> 5;
					int dstB = (dstColor & 0x1F);

					// Aplicar alpha blending
					int blendedR = (int)(srcR * alpha + dstR * (1.0f - alpha));
					int blendedG = (int)(srcG * alpha + dstG * (1.0f - alpha));
					int blendedB = (int)(srcB * alpha + dstB * (1.0f - alpha));

					// Asegurar que estén en rango
					blendedR = min(31, max(0, blendedR));
					blendedG = min(31, max(0, blendedG));
					blendedB = min(31, max(0, blendedB));

					pDst[ix] = (WORD)((blendedR << 10) | (blendedG << 5) | blendedB);
				}
			}
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
		}
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB100[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG100[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB100[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG100[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSpriteScaled(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth) {
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	int ix, iy;
	WORD *pSrc, *pDst;

	if (this == NULL || m_stBrush == NULL) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// Clipping en el eje X
	if (dX < m_pDDraw->m_rcClipArea.left) {
		sx += (m_pDDraw->m_rcClipArea.left - dX);
		szx -= (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
		dX = static_cast<short>(m_pDDraw->m_rcClipArea.left);
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right) {
		szx -= (dX + szx) - static_cast<short>(m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	// Clipping en el eje Y
	if (dY < m_pDDraw->m_rcClipArea.top) {
		sy += (m_pDDraw->m_rcClipArea.top - dY);
		szy -= (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
		dY = static_cast<short>(m_pDDraw->m_rcClipArea.top);
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom) {
		szy -= (dY + szy) - static_cast<short>(m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) {
			m_bOnCriticalSection = FALSE;
			return;
		}
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) {
					m_bOnCriticalSection = FALSE;
					return;
				}
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	// Dimensiones escaladas
	int scaledWidth = static_cast<int>(szx * 0.7f);
	int scaledHeight = static_cast<int>(szy * 0.7f);

	// Verificar límites
	if (scaledWidth <= 0 || scaledHeight <= 0) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	pSrc = (WORD *)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	// Renderizar directamente con escalado
	for (int destY = 0; destY < scaledHeight; ++destY) {
		for (int destX = 0; destX < scaledWidth; ++destX) {
			int srcX = destX * szx / scaledWidth;
			int srcY = destY * szy / scaledHeight;

			// Calcular posición del origen
			WORD *pCurrentSrc = pSrc + srcY * m_sPitch + srcX;
			WORD *pCurrentDst = pDst + destY * m_pDDraw->m_sBackB4Pitch + destX;

			// Aplicar transparencia
			if (*pCurrentSrc != m_wColorKey) {
				*pCurrentDst = *pCurrentSrc;
			}
		}
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSpriteMiniScaled(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth) {
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	int ix, iy;
	WORD *pSrc, *pDst;

	if (this == NULL || m_stBrush == NULL) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// Clipping en el eje X
	if (dX < m_pDDraw->m_rcClipArea.left) {
		sx += (m_pDDraw->m_rcClipArea.left - dX);
		szx -= (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
		dX = static_cast<short>(m_pDDraw->m_rcClipArea.left);
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right) {
		szx -= (dX + szx) - static_cast<short>(m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	// Clipping en el eje Y
	if (dY < m_pDDraw->m_rcClipArea.top) {
		sy += (m_pDDraw->m_rcClipArea.top - dY);
		szy -= (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
		dY = static_cast<short>(m_pDDraw->m_rcClipArea.top);
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom) {
		szy -= (dY + szy) - static_cast<short>(m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) {
			m_bOnCriticalSection = FALSE;
			return;
		}
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) {
					m_bOnCriticalSection = FALSE;
					return;
				}
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	// Dimensiones escaladas
	int scaledWidth = static_cast<int>(szx * 0.25f);
	int scaledHeight = static_cast<int>(szy * 0.25f);

	// Verificar límites
	if (scaledWidth <= 0 || scaledHeight <= 0) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	pSrc = (WORD *)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	// Renderizar directamente con escalado
	for (int destY = 0; destY < scaledHeight; ++destY) {
		for (int destX = 0; destX < scaledWidth; ++destX) {
			int srcX = destX * szx / scaledWidth;
			int srcY = destY * szy / scaledHeight;

			// Calcular posición del origen
			WORD *pCurrentSrc = pSrc + srcY * m_sPitch + srcX;
			WORD *pCurrentDst = pDst + destY * m_pDDraw->m_sBackB4Pitch + destX;

			// Aplicar transparencia
			if (*pCurrentSrc != m_wColorKey) {
				*pCurrentDst = *pCurrentSrc;
			}
		}
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSpriteMoreMiniScaled(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth) {
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	int ix, iy;
	WORD *pSrc, *pDst;

	if (this == NULL || m_stBrush == NULL) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// Clipping en el eje X
	if (dX < m_pDDraw->m_rcClipArea.left) {
		sx += (m_pDDraw->m_rcClipArea.left - dX);
		szx -= (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
		dX = static_cast<short>(m_pDDraw->m_rcClipArea.left);
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right) {
		szx -= (dX + szx) - static_cast<short>(m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	// Clipping en el eje Y
	if (dY < m_pDDraw->m_rcClipArea.top) {
		sy += (m_pDDraw->m_rcClipArea.top - dY);
		szy -= (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
		dY = static_cast<short>(m_pDDraw->m_rcClipArea.top);
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom) {
		szy -= (dY + szy) - static_cast<short>(m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) {
			m_bOnCriticalSection = FALSE;
			return;
		}
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) {
					m_bOnCriticalSection = FALSE;
					return;
				}
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	// Dimensiones escaladas
	int scaledWidth = static_cast<int>(szx * 0.40f);
	int scaledHeight = static_cast<int>(szy * 0.40f);

	// Verificar límites
	if (scaledWidth <= 0 || scaledHeight <= 0) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	pSrc = (WORD *)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	// Renderizar directamente con escalado
	for (int destY = 0; destY < scaledHeight; ++destY) {
		for (int destX = 0; destX < scaledWidth; ++destX) {
			int srcX = destX * szx / scaledWidth;
			int srcY = destY * szy / scaledHeight;

			// Calcular posición del origen
			WORD *pCurrentSrc = pSrc + srcY * m_sPitch + srcX;
			WORD *pCurrentDst = pDst + destY * m_pDDraw->m_sBackB4Pitch + destX;

			// Aplicar transparencia
			if (*pCurrentSrc != m_wColorKey) {
				*pCurrentDst = *pCurrentSrc;
			}
		}
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB100[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG100[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB100[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG100[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite70(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB70[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG70[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB70[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB70[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG70[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB70[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite70_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{		
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB70[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG70[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB70[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB70[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG70[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB70[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite50(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB50[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG50[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB50[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB50[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG50[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB50[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite50_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB50[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG50[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB50[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB50[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG50[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB50[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite25(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB25[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11] <<11) | (G_lTransG25[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5] <<5) | G_lTransRB25[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB25[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG25[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB25[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite25_NoColorKey(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB25[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11] <<11) | (G_lTransG25[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5] <<5) | G_lTransRB25[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_lTransRB25[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG25[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB25[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSprite2(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB2[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG2[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB2[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB2[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG2[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB2[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutShiftTransSprite2(int sX, int sY, int shX, int shY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = 128;//m_stBrush[sFrame].szx;
	szy = 128;//m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	sx += shX;
	sy += shY;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB2[(pDst[ix]&0xF800)>>11][(pSrc[ix]&0xF800)>>11]<<11) | (G_lTransG2[(pDst[ix]&0x7E0)>>5][(pSrc[ix]&0x7E0)>>5]<<5) | G_lTransRB2[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_lTransRB2[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10]<<10) | (G_lTransG2[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5]<<5) | G_lTransRB2[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutFadeSprite(short sX, short sY, short sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 //int dX,dY,sx,sy,szx,szy,pvx,pvy,sTmp;

	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey) 
					pDst[ix] = ((pDst[ix] & 0xE79C) >> 2); 
				
			}
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
		}
		break;

	case 2:
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey)	
					pDst[ix] = ((pDst[ix] & 0x739C) >> 2);
				
			}
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
		}
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutFadeSpriteDst(WORD * pDstAddr, short sPitch, short sX, short sY, short sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy;
 WORD * pSrc, * pDst;
 //int           iRet, dX,dY,sx,sy,szx,szy,pvx,pvy,sTmp;

	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)pDstAddr + dX + ((dY)*sPitch);

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey) 
					pDst[ix] = ((pDst[ix] & 0xE79C) >> 2); 
				
			}
			pSrc += m_sPitch;
			pDst += sPitch;
		}
		break;

	case 2:
		for (iy = 0; iy < szy; iy++) {
			for (ix = 0; ix < szx; ix++) {
				if (pSrc[ix] != m_wColorKey)	
					pDst[ix] = ((pDst[ix] & 0x739C) >> 2);
				
			}
			pSrc += m_sPitch;
			pDst += sPitch;
		}
		break;
	}

	m_bOnCriticalSection = FALSE;
}


bool CSprite::_iOpenSprite()
{
  	if (m_lpSurface != NULL) return FALSE;
	m_lpSurface = _pMakeSpriteSurface(); 
	if (m_lpSurface == NULL) return FALSE;
	m_pDDraw->iSetColorKey(m_lpSurface, m_wColorKey);
	m_bIsSurfaceEmpty  = FALSE;
	DDSURFACEDESC2  ddsd;
	ddsd.dwSize = 124;
	if (m_lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK) return FALSE;	
	m_pSurfaceAddr = (WORD *)ddsd.lpSurface;
	m_sPitch = (short)ddsd.lPitch >> 1;	
	m_lpSurface->Unlock(NULL);
	_SetAlphaDegree();
//	m_dwRefTime = timeGetTime();
	return TRUE;
}

void CSprite::_iCloseSprite()
{
	if( m_stBrush == NULL ) return;
	if (m_lpSurface == NULL) return;
	if (m_lpSurface->IsLost() != DD_OK)	return;
	m_lpSurface->Release();
	m_lpSurface = NULL;
	m_bIsSurfaceEmpty = TRUE;
	m_cAlphaDegree = 1;
}

void CSprite::PutSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 int  ix, iy, iRedPlus255, iGreenPlus255, iBluePlus255;
 WORD * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	if ((szx == 0) || (szy == 0)) return;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	iRedPlus255   = sRed +255;
	iGreenPlus255 = sGreen +255;
	iBluePlus255  = sBlue +255;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)(( G_iAddTable31[(pSrc[ix]&0xF800)>>11][iRedPlus255] <<11) | ( G_iAddTable63[(pSrc[ix]&0x7E0)>>5][iGreenPlus255] <<5) | G_iAddTable31[(pSrc[ix]&0x1F)][iBluePlus255]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_iAddTable31[(pSrc[ix]&0x7C00)>>10][iRedPlus255]<<10) | (G_iAddTable31[(pSrc[ix]&0x3E0)>>5][iGreenPlus255]<<5) | G_iAddTable31[(pSrc[ix]&0x1F)][iBluePlus255]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutSpriteFastNoColorKeyDstGrayscale(LPDIRECTDRAWSURFACE7 lpDstS, int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;

	if (this == NULL || m_stBrush == NULL) return;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;

	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	m_bOnCriticalSection = TRUE;

	// Obtener parámetros del frame
	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// Recortar el sprite si se sale del área visible
	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		sx += (m_pDDraw->m_rcClipArea.left - dX);
		szx -= (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) { m_rcBound.top = -1; return; }
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right)
	{
		szx -= (dX + szx - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) { m_rcBound.top = -1; return; }
	}

	if (dY < m_pDDraw->m_rcClipArea.top)
	{
		sy += (m_pDDraw->m_rcClipArea.top - dY);
		szy -= (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) { m_rcBound.top = -1; return; }
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy -= (dY + szy - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) { m_rcBound.top = -1; return; }
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE)
	{
		if (!_iOpenSprite()) return;
	}

	if ((szx == 0) || (szy == 0)) return;

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	// Bloquear superficies para acceder a los píxeles
	DDSURFACEDESC2 ddsdSrc, ddsdDst;
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);

	if (FAILED(m_lpSurface->Lock(NULL, &ddsdSrc, DDLOCK_READONLY | DDLOCK_WAIT, NULL))) return;

	ZeroMemory(&ddsdDst, sizeof(ddsdDst));
	ddsdDst.dwSize = sizeof(ddsdDst);

	if (FAILED(lpDstS->Lock(NULL, &ddsdDst, DDLOCK_WRITEONLY | DDLOCK_WAIT, NULL)))
	{
		m_lpSurface->Unlock(NULL);
		return;
	}

	BYTE* pSrc = (BYTE*)ddsdSrc.lpSurface + sy * ddsdSrc.lPitch + sx * 2;
	BYTE* pDst = (BYTE*)ddsdDst.lpSurface + dY * ddsdDst.lPitch + dX * 2;

	// Convertir a escala de grises y copiar píxeles
	for (int y = 0; y < szy; ++y)
	{
		WORD* srcLine = (WORD*)pSrc;
		WORD* dstLine = (WORD*)pDst;

		for (int x = 0; x < szx; ++x)
		{
			if (srcLine[x] != m_wColorKey)
			{
				BYTE red, green, blue;

				if (m_pDDraw->m_cPixelFormat == 1) // RGB565
				{
					red = (srcLine[x] & 0xF800) >> 8;
					green = (srcLine[x] & 0x07E0) >> 3;
					blue = (srcLine[x] & 0x001F) << 3;
				}
				else if (m_pDDraw->m_cPixelFormat == 2) // RGB555
				{
					red = (srcLine[x] & 0x7C00) >> 7;
					green = (srcLine[x] & 0x03E0) >> 2;
					blue = (srcLine[x] & 0x001F) << 3;
				}

				BYTE gray = (BYTE)(0.299 * red + 0.587 * green + 0.114 * blue);

				if (m_pDDraw->m_cPixelFormat == 1) // RGB565
				{
					dstLine[x] = ((gray >> 3) << 11) | ((gray >> 2) << 5) | (gray >> 3);
				}
				else if (m_pDDraw->m_cPixelFormat == 2) // RGB555
				{
					dstLine[x] = ((gray >> 3) << 10) | ((gray >> 3) << 5) | (gray >> 3);
				}
			}
			else
			{
				dstLine[x] = srcLine[x]; // Copiar píxel sin modificar si es color clave
			}
		}

		pSrc += ddsdSrc.lPitch;
		pDst += ddsdDst.lPitch;
	}

	// Desbloquear superficies
	lpDstS->Unlock(NULL);
	m_lpSurface->Unlock(NULL);

	m_bOnCriticalSection = FALSE;
}
void CSprite::PutSpriteFastNoColorKeyDstGammaBoost(LPDIRECTDRAWSURFACE7 lpDstS, int sX, int sY, int sFrame, DWORD dwTime, float fGammaBoost)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;

	if (!this || !m_stBrush || sFrame < 0 || sFrame >= m_iTotalFrame) return;

	m_rcBound.top = -1;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// Clipping rápido
	const auto& clip = m_pDDraw->m_rcClipArea;

	if (dX < clip.left) {
		int delta = clip.left - dX;
		sx += delta; szx -= delta; dX = clip.left;
		if (szx <= 0) return;
	}
	else if (dX + szx > clip.right) {
		szx -= (dX + szx - clip.right);
		if (szx <= 0) return;
	}
	if (dY < clip.top) {
		int delta = clip.top - dY;
		sy += delta; szy -= delta; dY = clip.top;
		if (szy <= 0) return;
	}
	else if (dY + szy > clip.bottom) {
		szy -= (dY + szy - clip.bottom);
		if (szy <= 0) return;
	}

	if (m_bIsSurfaceEmpty || szx == 0 || szy == 0)
		if (!_iOpenSprite()) return;

	m_dwRefTime = dwTime;
	m_rcBound = { dX, dY, (short)(dX + szx), (short)(dY + szy) };

	// Bloqueo de superficies
	DDSURFACEDESC2 srcDesc = { sizeof(DDSURFACEDESC2) };
	if (FAILED(m_lpSurface->Lock(NULL, &srcDesc, DDLOCK_READONLY | DDLOCK_WAIT, NULL))) return;

	DDSURFACEDESC2 dstDesc = { sizeof(DDSURFACEDESC2) };
	if (FAILED(lpDstS->Lock(NULL, &dstDesc, DDLOCK_WRITEONLY | DDLOCK_WAIT, NULL))) {
		m_lpSurface->Unlock(NULL);
		return;
	}

	// Ajustes gamma precomputados
	const float gammaR = fGammaBoost * 1.15f;
	const float gammaG = fGammaBoost * 1.05f;
	const float gammaB = fGammaBoost * 0.90f;

	const int bpp = 2; // 16 bits
	BYTE* pSrcLine = (BYTE*)srcDesc.lpSurface + sy * srcDesc.lPitch + sx * bpp;
	BYTE* pDstLine = (BYTE*)dstDesc.lpSurface + dY * dstDesc.lPitch + dX * bpp;

	for (int y = 0; y < szy; ++y)
	{
		WORD* src = (WORD*)pSrcLine;
		WORD* dst = (WORD*)pDstLine;

		for (int x = 0; x < szx; ++x)
		{
			const WORD col = src[x];
			if (col == m_wColorKey) {
				dst[x] = col;
				continue;
			}

			BYTE r, g, b;

			if (m_pDDraw->m_cPixelFormat == 1) { // RGB565
				r = (col & 0xF800) >> 8;
				g = (col & 0x07E0) >> 3;
				b = (col & 0x001F) << 3;
			}
			else { // RGB555
				r = (col & 0x7C00) >> 7;
				g = (col & 0x03E0) >> 2;
				b = (col & 0x001F) << 3;
			}

			r = (BYTE)min(255, (int)(r * gammaR));
			g = (BYTE)min(255, (int)(g * gammaG));
			b = (BYTE)min(255, (int)(b * gammaB));

			if (m_pDDraw->m_cPixelFormat == 1) {
				dst[x] = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
			}
			else {
				dst[x] = ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3);
			}
		}

		pSrcLine += srcDesc.lPitch;
		pDstLine += dstDesc.lPitch;
	}

	lpDstS->Unlock(NULL);
	m_lpSurface->Unlock(NULL);
	m_bOnCriticalSection = FALSE;
}

void CSprite::PutSolidColorTransSprite(int sX, int sY, int sFrame, const ColorRGB& color, int alphaPercent, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	short ix, iy;
	WORD* pSrc, * pDst;

	if (this == NULL) return;
	if (m_stBrush == NULL) return;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;

	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) {
		sx += (m_pDDraw->m_rcClipArea.left - dX);
		szx -= (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) { m_rcBound.top = -1; m_bOnCriticalSection = FALSE; return; }
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right) {
		szx -= ((dX + szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) { m_rcBound.top = -1; m_bOnCriticalSection = FALSE; return; }
	}

	if (dY < m_pDDraw->m_rcClipArea.top) {
		sy += (m_pDDraw->m_rcClipArea.top - dY);
		szy -= (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) { m_rcBound.top = -1; m_bOnCriticalSection = FALSE; return; }
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom) {
		szy -= ((dY + szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) { m_rcBound.top = -1; m_bOnCriticalSection = FALSE; return; }
	}

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) { m_bOnCriticalSection = FALSE; return; }
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) _SetAlphaDegree();
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) { m_bOnCriticalSection = FALSE; return; }
			}
		}
	}

	if ((szx == 0) || (szy == 0)) { m_bOnCriticalSection = FALSE; return; }

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD*)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD*)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	BYTE r = (BYTE)(color.r >> 3);
	BYTE g = (BYTE)(color.g >> 2);
	BYTE b = (BYTE)(color.b >> 3);

	int alpha = alphaPercent; // 0–100

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
	case 2:
		iy = 0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					WORD bg = pDst[ix];

					BYTE bgR = (bg & 0xF800) >> 11;
					BYTE bgG = (bg & 0x07E0) >> 5;
					BYTE bgB = (bg & 0x001F);

					BYTE newR = (BYTE)((bgR * (100 - alpha) + r * alpha) / 100);
					BYTE newG = (BYTE)((bgG * (100 - alpha) + g * alpha) / 100);
					BYTE newB = (BYTE)((bgB * (100 - alpha) + b * alpha) / 100);

					pDst[ix] = (WORD)((newR << 11) | (newG << 5) | newB);
				}
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}
void CSprite::PutFuegoScrollEffect(int sX, int sY, int sFrame, const ColorRGB& color, int alphaPercent, int offset, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	short ix, iy;
	WORD* pSrc, * pDst;

	if (this == NULL) return;
	if (m_stBrush == NULL) return;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;

	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) {
		sx += (m_pDDraw->m_rcClipArea.left - dX);
		szx -= (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) { m_rcBound.top = -1; m_bOnCriticalSection = FALSE; return; }
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right) {
		szx -= ((dX + szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) { m_rcBound.top = -1; m_bOnCriticalSection = FALSE; return; }
	}

	if (dY < m_pDDraw->m_rcClipArea.top) {
		sy += (m_pDDraw->m_rcClipArea.top - dY);
		szy -= (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) { m_rcBound.top = -1; m_bOnCriticalSection = FALSE; return; }
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom) {
		szy -= ((dY + szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) { m_rcBound.top = -1; m_bOnCriticalSection = FALSE; return; }
	}

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) { m_bOnCriticalSection = FALSE; return; }
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) _SetAlphaDegree();
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) { m_bOnCriticalSection = FALSE; return; }
			}
		}
	}

	if ((szx == 0) || (szy == 0)) { m_bOnCriticalSection = FALSE; return; }

	pSrc = (WORD*)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD*)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	BYTE r = (BYTE)(color.r >> 3);
	BYTE g = (BYTE)(color.g >> 2);
	BYTE b = (BYTE)(color.b >> 3);

	int alpha = alphaPercent;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
	case 2:
		iy = 0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					// SOLO aplica el fuego si está dentro de la zona del "scroll"
					// El offset se mueve (lo manejás vos externamente)
					if ((iy + offset) % szy < szy / 2) { // solo una franja
						WORD bg = pDst[ix];

						BYTE bgR = (bg & 0xF800) >> 11;
						BYTE bgG = (bg & 0x07E0) >> 5;
						BYTE bgB = (bg & 0x001F);

						BYTE newR = (BYTE)((bgR * (100 - alpha) + r * alpha) / 100);
						BYTE newG = (BYTE)((bgG * (100 - alpha) + g * alpha) / 100);
						BYTE newB = (BYTE)((bgB * (100 - alpha) + b * alpha) / 100);

						pDst[ix] = (WORD)((newR << 11) | (newG << 5) | newB);
					}
				}
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

/*
void CSprite::PutTintedSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	short ix, iy;
	WORD  * pSrc, *pDst;

	if (this == NULL) return;
	if (m_stBrush == NULL) return;
	m_rcBound.top = -1;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// --- CLIPPING ---
	if (dX < m_pDDraw->m_rcClipArea.left) {
		sx += (m_pDDraw->m_rcClipArea.left - dX);
		szx -= (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) { m_rcBound.top = -1; return; }
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right) {
		szx -= ((dX + szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) { m_rcBound.top = -1; return; }
	}

	if (dY < m_pDDraw->m_rcClipArea.top) {
		sy += (m_pDDraw->m_rcClipArea.top - dY);
		szy -= (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) { m_rcBound.top = -1; return; }
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom) {
		szy -= ((dY + szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) { m_rcBound.top = -1; return; }
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) return;
			}
		}
	}

	if ((szx == 0) || (szy == 0)) return;

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD*)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD*)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	// --- convertir tint a 16 bits ---
	int rTint = (sRed * 31) / 255;   // 5 bits
	int gTint = (sGreen * 63) / 255; // 6 bits
	int bTint = (sBlue * 31) / 255;  // 5 bits

	// --- factor de interpolación oscilante (0 → 1) ---
	float t = (sinf(dwTime * 0.002f) + 1.0f) * 0.5f; // ajusta 0.002f para velocidad

	// --- pintado ---
	for (iy = 0; iy < szy; iy++) {
		for (ix = 0; ix < szx; ix++) {
			WORD src = pSrc[ix];
			if (src == m_wColorKey) continue;

			int R = (src & 0xF800) >> 11;
			int G = (src & 0x07E0) >> 5;
			int B = (src & 0x001F);

			// Mezcla progresiva: interpolación entre original y tinte
			R = (int)(R * (1.0f - t) + rTint * t);
			G = (int)(G * (1.0f - t) + gTint * t);
			B = (int)(B * (1.0f - t) + bTint * t);

			// clamp
			if (R > 31) R = 31;
			if (G > 63) G = 63;
			if (B > 31) B = 31;

			pDst[ix] = (R << 11) | (G << 5) | B;
		}
		pSrc += m_sPitch;
		pDst += m_pDDraw->m_sBackB4Pitch;
	}

	m_bOnCriticalSection = FALSE;
}*/

void CSprite::PutTintedSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	short ix, iy;
	WORD  * pSrc, *pDst;

	if (this == NULL) return;
	if (m_stBrush == NULL) return;
	m_rcBound.top = -1;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// --- CLIPPING ---
	if (dX < m_pDDraw->m_rcClipArea.left) {
		sx += (m_pDDraw->m_rcClipArea.left - dX);
		szx -= (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) { m_rcBound.top = -1; return; }
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right) {
		szx -= ((dX + szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) { m_rcBound.top = -1; return; }
	}

	if (dY < m_pDDraw->m_rcClipArea.top) {
		sy += (m_pDDraw->m_rcClipArea.top - dY);
		szy -= (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) { m_rcBound.top = -1; return; }
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom) {
		szy -= ((dY + szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) { m_rcBound.top = -1; return; }
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if (_iOpenSprite() == FALSE) return;
			}
		}
	}

	if ((szx == 0) || (szy == 0)) return;

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD*)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD*)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	// --- convertir tint a 16 bits ---
	int rTint = (sRed * 31) / 255;   // 5 bits
	int gTint = (sGreen * 63) / 255; // 6 bits
	int bTint = (sBlue * 31) / 255;  // 5 bits

	// --- factor de interpolación oscilante (0 → 1) ---
	float t = (sinf(dwTime * 0.002f) + 1.0f) * 0.5f;

	// --- limitar la fuerza máxima del tinte ---
	float tintStrength = 0.5f; // 0.5 = 50% como máximo
	float blend = t * tintStrength;

	// --- pintado ---
	for (iy = 0; iy < szy; iy++) {
		for (ix = 0; ix < szx; ix++) {
			WORD src = pSrc[ix];
			if (src == m_wColorKey) continue;

			int R = (src & 0xF800) >> 11;
			int G = (src & 0x07E0) >> 5;
			int B = (src & 0x001F);

			// Mezcla progresiva con intensidad limitada
			R = (int)(R * (1.0f - blend) + rTint * blend);
			G = (int)(G * (1.0f - blend) + gTint * blend);
			B = (int)(B * (1.0f - blend) + bTint * blend);

			// clamp
			if (R > 31) R = 31;
			if (G > 63) G = 63;
			if (B > 31) B = 31;

			pDst[ix] = (R << 11) | (G << 5) | B;
		}
		pSrc += m_sPitch;
		pDst += m_pDDraw->m_sBackB4Pitch;
	}

	m_bOnCriticalSection = FALSE;
}


void CSprite::PutTransSpriteRGB(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 short ix, iy, iRedPlus255, iGreenPlus255, iBluePlus255;
 WORD  * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	if ((szx == 0) || (szy == 0)) return;

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	iRedPlus255   = sRed +255;
	iGreenPlus255 = sGreen +255;
	iBluePlus255  = sBlue +255;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0xF800)>>11][((pSrc[ix]&0xF800)>>11)] + iRedPlus255][(pDst[ix]&0xF800)>>11]<<11) | (G_iAddTransTable63[G_lTransG100[(pDst[ix]&0x7E0)>>5][((pSrc[ix]&0x7E0)>>5)] + iGreenPlus255][(pDst[ix]&0x7E0)>>5]<<5) | G_iAddTransTable31[m_pDDraw->m_lTransRB100[(pDst[ix]&0x1F)][((pSrc[ix]&0x1F))] + iBluePlus255][(pDst[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					pDst[ix] = (WORD)((G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10] +iRedPlus255][(pDst[ix]&0x7C00)>>10]<<10) | (G_iAddTransTable31[G_lTransG100[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5] +iGreenPlus255][(pDst[ix]&0x3E0)>>5]<<5) | G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)] +iBluePlus255][(pDst[ix]&0x1F)]);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSpriteRGB_NoColorKey(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 short ix, iy, iRedPlus255, iGreenPlus255, iBluePlus255;
 WORD  * pSrc, * pDst;
 	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);

	if ((szx == 0) || (szy == 0)) return;

	iRedPlus255   = sRed +255;
	iGreenPlus255 = sGreen +255;
	iBluePlus255  = sBlue +255;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0xF800)>>11][((pSrc[ix]&0xF800)>>11)] + iRedPlus255][(pDst[ix]&0xF800)>>11]<<11) | (G_iAddTransTable63[G_lTransG100[(pDst[ix]&0x7E0)>>5][((pSrc[ix]&0x7E0)>>5)] + iGreenPlus255][(pDst[ix]&0x7E0)>>5]<<5) | G_iAddTransTable31[m_pDDraw->m_lTransRB100[(pDst[ix]&0x1F)][((pSrc[ix]&0x1F))] + iBluePlus255][(pDst[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;

	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				pDst[ix] = (WORD)((G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0x7C00)>>10][(pSrc[ix]&0x7C00)>>10] +iRedPlus255][(pDst[ix]&0x7C00)>>10]<<10) | (G_iAddTransTable31[G_lTransG100[(pDst[ix]&0x3E0)>>5][(pSrc[ix]&0x3E0)>>5] +iGreenPlus255][(pDst[ix]&0x3E0)>>5]<<5) | G_iAddTransTable31[G_lTransRB100[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F)] +iBluePlus255][(pDst[ix]&0x1F)]);
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::_GetSpriteRect(int sX, int sY, int sFrame)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	// v1.4
	m_rcBound.top    = -1;
	m_rcBound.bottom = -1;
	m_rcBound.left   = -1;
	m_rcBound.right  = -1;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	m_sPivotX = pvx;
	m_sPivotY = pvy;
}

void CSprite::_SetAlphaDegree()
{
 WORD * pSrc, wR, wG, wB, wTemp, ix, iy;
 int iR, iG, iB, sRed, sGreen, sBlue;
 
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_bOnCriticalSection = TRUE;
	if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
		
		m_cAlphaDegree = G_cSpriteAlphaDegree;
		switch (m_cAlphaDegree) {
		case 1:
			sRed = sGreen = sBlue = 0;
			break;

		case 2:
			sRed   = -3;
			sGreen = -3;
			sBlue  =  2;
			break;
		}

		pSrc = (WORD *)m_pSurfaceAddr;
		
		switch (m_pDDraw->m_cPixelFormat) {
		case 1:
			for (iy = 0; iy < m_wBitmapSizeY; iy++) 
			{	for (ix = 0; ix < m_wBitmapSizeX; ix++) 
				{	if (pSrc == NULL) return; 
					if (pSrc[ix] != m_wColorKey) 
					{	wR = (WORD)(pSrc[ix]&0xF800)>>11;
						wG = (WORD)(pSrc[ix]&0x7E0)>>5;
						wB = (WORD)(pSrc[ix]&0x1F);
						iR = (int)wR + sRed;
						iG = (int)wG + sGreen;
						iB = (int)wB + sBlue;
						
						if (iR < 0) iR = 0;
						else if (iR > 31) iR = 31;
						if (iG < 0) iG = 0;
						else if (iG > 63) iG = 63;
						if (iB < 0) iB = 0;
						else if (iB > 31) iB = 31;
						
						wTemp = (WORD)((iR<<11) | (iG<<5) | iB);
						if (wTemp != m_wColorKey)
							 pSrc[ix] = wTemp;
						else pSrc[ix] = (WORD)((iR<<11) | (iG<<5) | (iB+1));
				}	}
				pSrc += m_sPitch;
			}
			break;
			
		case 2:
			for (iy = 0; iy < m_wBitmapSizeY; iy++) 
			{	for (ix = 0; ix < m_wBitmapSizeX; ix++) 
				{	if (pSrc == NULL) return; 
					if (pSrc[ix] != m_wColorKey)	
					{	wR = (WORD)(pSrc[ix]&0x7C00)>>10;
						wG = (WORD)(pSrc[ix]&0x3E0)>>5;
						wB = (WORD)(pSrc[ix]&0x1F);
						iR = (int)wR + sRed;
						iG = (int)wG + sGreen;
						iB = (int)wB + sBlue;						
						if (iR < 0) iR = 0;
						else if (iR > 31) iR = 31;
						if (iG < 0) iG = 0;
						else if (iG > 31) iG = 31;
						if (iB < 0) iB = 0;
						else if (iB > 31) iB = 31;						
						wTemp = (WORD)((iR<<10) | (iG<<5) | iB);
						if (wTemp != m_wColorKey)
							 pSrc[ix] = wTemp;
						else pSrc[ix] = (WORD)((iR<<10) | (iG<<5) | (iB+1));				
				}	}
				pSrc += m_sPitch;
			}
			break;	
	}	}

	m_bOnCriticalSection = FALSE;
}

BOOL CSprite::_bCheckCollison(int sX, int sY, short sFrame, int msX, int msY)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
	int  ix, iy;
	WORD * pSrc;
	int  tX, tY;
	
	if( this == NULL ) return FALSE;
	if( m_stBrush == NULL ) return FALSE;
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return FALSE;
	if( m_bIsSurfaceEmpty == TRUE ) return FALSE;
	if( msX < m_pDDraw->m_rcClipArea.left+3 ) return FALSE;
	if( msX > m_pDDraw->m_rcClipArea.right-3 ) return FALSE;
	if( msY < m_pDDraw->m_rcClipArea.top+3 ) return FALSE;
	if( msY > m_pDDraw->m_rcClipArea.bottom-3 ) return FALSE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if( msX < dX ) return FALSE;
	if( msX > dX+szx ) return FALSE;
	if( msY < dY ) return FALSE;
	if( msY > dY+szy ) return FALSE;

//	if (dX < m_pDDraw->m_rcClipArea.left+3) return FALSE;
//	if (dX+szx > m_pDDraw->m_rcClipArea.right-3) return FALSE;
//	if (dY < m_pDDraw->m_rcClipArea.top+3) return FALSE;
//	if (dY+szy > m_pDDraw->m_rcClipArea.bottom-3) return FALSE;

	if (dX < m_pDDraw->m_rcClipArea.left+3)
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left+3 - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left+3 - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return FALSE;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left+3;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right-3)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right-3);
		if (szx < 0) {
			m_rcBound.top = -1;
			return FALSE;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top+3)
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top+3 - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top+3 - dY);
		if (szy < 0) {
			m_rcBound.top = -1;
			return FALSE;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top+3;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom-3)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom-3);
		if (szy < 0) {
			m_rcBound.top = -1;
			return FALSE;
		}
	}
	
	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);

	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	tX = dX;
	tY = dY;


//	pSrc += m_sPitch * ( msY - tY );
//	if( pSrc[msX-tX] != m_wColorKey ) return TRUE;
//	else return FALSE;

	if( msY < tY + 3 ) return FALSE;
	if( msX < tX + 3 ) return FALSE;
	pSrc += m_sPitch * ( msY - tY - 3 );
	for( iy=0 ; iy<=6 ; iy++ )
	{
		for( ix=msX-tX-3 ; ix<=msX-tX+3 ; ix++ )
		{
			if( pSrc[ix] != m_wColorKey ) return TRUE;
		}
		pSrc += m_sPitch;
	}
	return FALSE;
}

void CSprite::PutShiftSpriteFast(int sX, int sY, int shX, int shY, int sFrame, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = 128;//m_stBrush[sFrame].szx;
	szy = 128;//m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	sx += shX;
	sy += shY;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpBackB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutRevTransSprite(int sX, int sY, int sFrame, DWORD dwTime, int alphaDepth)
{
	int  ix, iy;
	int  iR, iG, iB;
	WORD * pSrc, * pDst;
	int dX,dY,sx,sy,szx,szy,pvx,pvy;//,sTmp;
	
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;
	
	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;
	
	dX = sX + pvx;
	dY = sY + pvy;
		
	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);							
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
		
	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy < 0) {	
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy < 0) {
			m_rcBound.top = -1;
			return;
		}
	}
		
	m_dwRefTime = dwTime;
		
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}
	
	SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
		
	pSrc = (WORD *)m_pSurfaceAddr + sx + ((sy)*m_sPitch);
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr + dX + ((dY)*m_pDDraw->m_sBackB4Pitch);
	
	if ((szx == 0) || (szy == 0)) return;
		
	switch (m_pDDraw->m_cPixelFormat) {
	case 1:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					iR = (int)m_pDDraw->m_lFadeRB[((pDst[ix]&0xF800)>>11)][((pSrc[ix]&0xF800)>>11) +alphaDepth];
					iG = (int)m_pDDraw->m_lFadeG[(pDst[ix]&0x7E0)>>5][((pSrc[ix]&0x7E0)>>5) +alphaDepth +alphaDepth];
					iB = (int)m_pDDraw->m_lFadeRB[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F) +alphaDepth];
					pDst[ix] = (WORD)((iR<<11) | (iG<<5) | iB);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
		
	case 2:
		iy =0;
		do {
			ix = 0;
			do {
				if (pSrc[ix] != m_wColorKey) {
					iR = (int)m_pDDraw->m_lFadeRB[(pDst[ix]&0x7C00)>>10][((pSrc[ix]&0x7C00)>>10) +alphaDepth];
					iG = (int)m_pDDraw->m_lFadeG[(pDst[ix]&0x3E0)>>5][((pSrc[ix]&0x3E0)>>5) +alphaDepth];
					iB = (int)m_pDDraw->m_lFadeRB[(pDst[ix]&0x1F)][(pSrc[ix]&0x1F) +alphaDepth];
					pDst[ix] = (WORD)((iR<<10) | (iG<<5) | iB);
				}
				
				ix++;
			} while (ix < szx);
			pSrc += m_sPitch;
			pDst += m_pDDraw->m_sBackB4Pitch;
			iy++;
		} while (iy < szy);
		break;
	}
	m_bOnCriticalSection = FALSE;
}
void CSprite::PutShadowSpriteEight(int sX, int sY, int sFrame, int sEight, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	int ix, iy;
	WORD *pSrc, *pDst;

	// Added resolution
	int res_x = 0;
	int res_y = 0;

	if (c_reso->IsResolution() == C800x600)
	{
		res_x = 800;
		res_y = 547; // Adjusted for shadow drawing
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		res_x = 1024;
		res_y = 715;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		res_x = 640;
		res_y = 427; // Adjusted for shadow drawing
	}

	if (this == NULL) return;
	if (m_stBrush == NULL) return;
	m_rcBound.top = -1;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	// Limit height by sEight
	if (sEight < szy)
		szy = sEight;

	if (dX < m_pDDraw->m_rcClipArea.left)
	{
		sx = sx + (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX + szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX + szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top)
	{
		sy = sy + (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY + szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY + szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE)
	{
		if (_iOpenSprite() == FALSE) return;
	}

	pSrc = (WORD *)m_pSurfaceAddr + sx + sy * m_sPitch;
	pDst = (WORD *)m_pDDraw->m_pBackB4Addr;

	switch (m_pDDraw->m_cPixelFormat)
	{
	case 1:
		for (iy = 0; iy < szy; iy++)
		{
			for (ix = 0; ix < szx; ix++)
			{
				int iSangX = dX + ix;
				int iSangY = dY + iy;
				if (pSrc[ix] != m_wColorKey)
				{
					if (iSangX >= 0 && iSangX < res_x && iSangY >= 0 && iSangY < res_y)
					{
						pDst[iSangY * m_pDDraw->m_sBackB4Pitch + iSangX] = (WORD)((G_lTransRB2[(pDst[iSangY * m_pDDraw->m_sBackB4Pitch + iSangX] & 0xF800) >> 11][(pSrc[ix] & 0xF800) >> 11] << 11) |
							(G_lTransG2[(pDst[iSangY * m_pDDraw->m_sBackB4Pitch + iSangX] & 0x7E0) >> 5][(pSrc[ix] & 0x7E0) >> 5] << 5) |
							G_lTransRB2[(pDst[iSangY * m_pDDraw->m_sBackB4Pitch + iSangX] & 0x1F)][(pSrc[ix] & 0x1F)]);
					}
				}
			}
			pSrc += m_sPitch;
		}
		break;
	case 2:
		for (iy = 0; iy < szy; iy++)
		{
			for (ix = 0; ix < szx; ix++)
			{
				int iSangX = dX + ix;
				int iSangY = dY + iy;
				if (pSrc[ix] != m_wColorKey)
				{
					if (iSangX >= 0 && iSangX < res_x && iSangY >= 0 && iSangY < res_y)
					{
						pDst[iSangY * m_pDDraw->m_sBackB4Pitch + iSangX] = (WORD)((G_lTransRB2[(pDst[iSangY * m_pDDraw->m_sBackB4Pitch + iSangX] & 0x7C00) >> 10][(pSrc[ix] & 0x7C00) >> 10] << 10) |
							(G_lTransG2[(pDst[iSangY * m_pDDraw->m_sBackB4Pitch + iSangX] & 0x3E0) >> 5][(pSrc[ix] & 0x3E0) >> 5] << 5) |
							G_lTransRB2[(pDst[iSangY * m_pDDraw->m_sBackB4Pitch + iSangX] & 0x1F)][(pSrc[ix] & 0x1F)]);
					}
				}
			}
			pSrc += m_sPitch;
		}
		break;
	}

	m_rcBound.left = dX;
	m_rcBound.top = dY;
	m_rcBound.right = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_bOnCriticalSection = FALSE;
}



void CSprite::PutSpriteFastEight(int sX, int sY, int sFrame, int sEight, DWORD dwTime)
{
	short dX,dY,sx,sy,szx,szy,pvx,pvy;
 RECT rcRect;
	if( this == NULL ) return;
	if( m_stBrush == NULL ) return;
	m_rcBound.top = -1; // Fix by Snoopy.... (Reco at mine)
	if ((m_iTotalFrame-1 < sFrame) || (sFrame < 0)) return;	
	m_bOnCriticalSection = TRUE;

	sx  = m_stBrush[sFrame].sx;
	sy  = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

  	dX = sX + pvx;
	dY = sY + pvy;

	if (sEight < szy)
		szy = sEight;

	if (dX < m_pDDraw->m_rcClipArea.left) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sx = sx	+ (m_pDDraw->m_rcClipArea.left - dX);
		szx = szx - (m_pDDraw->m_rcClipArea.left - dX);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dX = (short)m_pDDraw->m_rcClipArea.left;
	}
	else if (dX+szx > m_pDDraw->m_rcClipArea.right)
	{
		szx = szx - ((dX+szx) - (short)m_pDDraw->m_rcClipArea.right);
		if (szx <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}

	if (dY < m_pDDraw->m_rcClipArea.top) 								  
	{
		//Reparando warning LaloRamos anulado v11 - Agregado (unsigned short)
		sy = sy	+ (m_pDDraw->m_rcClipArea.top - dY);
		szy = szy - (m_pDDraw->m_rcClipArea.top - dY);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
		dY = (short)m_pDDraw->m_rcClipArea.top;
	}
	else if (dY+szy > m_pDDraw->m_rcClipArea.bottom)
	{
		szy = szy - ((dY+szy) - (short)m_pDDraw->m_rcClipArea.bottom);
		if (szy <= 0) {
			m_rcBound.top = -1;
			return;
		}
	}
	
	m_dwRefTime = dwTime;
	
	if (m_bIsSurfaceEmpty == TRUE) {
		if( _iOpenSprite() == FALSE ) return;
	}
	else {
		if (m_bAlphaEffect && (m_cAlphaDegree != G_cSpriteAlphaDegree)) {
			if (G_cSpriteAlphaDegree == 2) {
				_SetAlphaDegree();
			}
			else {
				_iCloseSprite();
				if( _iOpenSprite() == FALSE ) return;
			}
		}
	}

	//SetRect(&rcRect,  sx, sy, sx + szx, sy + szy); // our fictitious sprite bitmap is 
	//SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
	rcRect.left = sx;
	rcRect.top  = sy;
	rcRect.right  = sx + szx;
	rcRect.bottom = sy + szy;

	m_rcBound.left = dX;
	m_rcBound.top  = dY;
	m_rcBound.right  = dX + szx;
	m_rcBound.bottom = dY + szy;

	m_pDDraw->m_lpBackB4->BltFast( dX, dY, m_lpSurface, &rcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );

	m_bOnCriticalSection = FALSE;
}

void ReadFramePositions(HANDLE hPakFile, std::vector<int> & framePositions, int frames)
{
	DWORD * dwp, count;
	char * fileHeader = new char[frames * 8 + 8];
	SetFilePointer(hPakFile, 24, NULL, FILE_BEGIN);
	ReadFile(hPakFile, fileHeader, frames * 8, &count, NULL);
	dwp = (DWORD *)fileHeader;
	for (int i = 0; i < frames; i++, dwp += 2)
	{
		framePositions.push_back(*dwp);
	}
	delete[] fileHeader;
}
void CSprite::PutSpriteFastMiniScaled(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	WORD* pSrc, *pDst;

	if (this == NULL || m_stBrush == NULL) return;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;

	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) {
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	// Escalado 0.10x
	int scaledWidth = static_cast<int>(szx * 0.10f);
	int scaledHeight = static_cast<int>(szy * 0.10f);

	if (scaledWidth <= 0 || scaledHeight <= 0) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	// Resolución activa
	int res_x = 0, res_y = 0;
	if (c_reso->IsResolution() == C800x600) {
		res_x = 800; res_y = 600;
	}
	else if (c_reso->IsResolution() == C1024x768) {
		res_x = 1024; res_y = 768;
	}
	else if (c_reso->IsResolution() == C640x480) {
		res_x = 640; res_y = 480;
	}

	// Clipping (recorte seguro)
	if (dX >= res_x || dY >= res_y || dX + scaledWidth <= 0 || dY + scaledHeight <= 0) {
		m_bOnCriticalSection = FALSE;
		return; // Fuera de pantalla
	}

	int clipX = dX < 0 ? -dX : 0;
	int clipY = dY < 0 ? -dY : 0;

	int drawWidth = scaledWidth;
	int drawHeight = scaledHeight;

	if (dX + scaledWidth > res_x)
		drawWidth = res_x - dX;

	if (dY + scaledHeight > res_y)
		drawHeight = res_y - dY;

	pSrc = (WORD*)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD*)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	for (int destY = clipY; destY < drawHeight; ++destY) {
		for (int destX = clipX; destX < drawWidth; ++destX) {

			int srcX = destX * szx / scaledWidth;
			int srcY = destY * szy / scaledHeight;

			WORD* pCurrentSrc = pSrc + srcY * m_sPitch + srcX;
			WORD* pCurrentDst = pDst + destY * m_pDDraw->m_sBackB4Pitch + destX;

			if (*pCurrentSrc != m_wColorKey) {
				*pCurrentDst = *pCurrentSrc;
			}
		}
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutSpriteFastNoColorKeyMiniScaled(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	WORD* pSrc, *pDst;

	if (this == NULL || m_stBrush == NULL) return;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;

	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) {
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	// Escalado 0.25x
	int scaledWidth = static_cast<int>(szx * 0.10f);
	int scaledHeight = static_cast<int>(szy * 0.10f);

	if (scaledWidth <= 0 || scaledHeight <= 0) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	pSrc = (WORD*)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD*)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	for (int destY = 0; destY < scaledHeight; ++destY) {
		for (int destX = 0; destX < scaledWidth; ++destX) {
			int srcX = destX * szx / scaledWidth;
			int srcY = destY * szy / scaledHeight;

			WORD* pCurrentSrc = pSrc + srcY * m_sPitch + srcX;
			WORD* pCurrentDst = pDst + destY * m_pDDraw->m_sBackB4Pitch + destX;

			*pCurrentDst = *pCurrentSrc;
		}
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutTransSprite2MiniScaled(int sX, int sY, int sFrame, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	WORD* pSrc, *pDst;

	if (this == NULL || m_stBrush == NULL) return;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;

	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) {
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	// Escalado 0.25x
	int scaledWidth = static_cast<int>(szx * 0.10f);
	int scaledHeight = static_cast<int>(szy * 0.10f);

	if (scaledWidth <= 0 || scaledHeight <= 0) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	pSrc = (WORD*)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD*)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	for (int destY = 0; destY < scaledHeight; ++destY) {
		for (int destX = 0; destX < scaledWidth; ++destX) {
			int srcX = destX * szx / scaledWidth;
			int srcY = destY * szy / scaledHeight;

			WORD* pCurrentSrc = pSrc + srcY * m_sPitch + srcX;
			WORD* pCurrentDst = pDst + destY * m_pDDraw->m_sBackB4Pitch + destX;

			if (*pCurrentSrc != m_wColorKey) {
				*pCurrentDst = (WORD)((G_lTransRB2[((*pCurrentDst) & 0xF800) >> 11][((*pCurrentSrc) & 0xF800) >> 11] << 11) |
					(G_lTransG2[((*pCurrentDst) & 0x7E0) >> 5][((*pCurrentSrc) & 0x7E0) >> 5] << 5) |
					G_lTransRB2[((*pCurrentDst) & 0x1F)][((*pCurrentSrc) & 0x1F)]);
			}
		}
	}

	m_bOnCriticalSection = FALSE;
}

void CSprite::PutSpriteRGBMiniScaled(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, DWORD dwTime)
{
	short dX, dY, sx, sy, szx, szy, pvx, pvy;
	int ix, iy, iRedPlus255, iGreenPlus255, iBluePlus255;
	WORD* pSrc, *pDst;

	if (this == NULL || m_stBrush == NULL) return;
	if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return;
	m_bOnCriticalSection = TRUE;

	sx = m_stBrush[sFrame].sx;
	sy = m_stBrush[sFrame].sy;
	szx = m_stBrush[sFrame].szx;
	szy = m_stBrush[sFrame].szy;
	pvx = m_stBrush[sFrame].pvx;
	pvy = m_stBrush[sFrame].pvy;

	dX = sX + pvx;
	dY = sY + pvy;

	m_dwRefTime = dwTime;

	if (m_bIsSurfaceEmpty == TRUE) {
		if (_iOpenSprite() == FALSE) {
			m_bOnCriticalSection = FALSE;
			return;
		}
	}

	// Escalado 0.20x
	int scaledWidth = static_cast<int>(szx * 0.20f);
	int scaledHeight = static_cast<int>(szy * 0.20f);

	if (scaledWidth <= 0 || scaledHeight <= 0) {
		m_bOnCriticalSection = FALSE;
		return;
	}

	pSrc = (WORD*)m_pSurfaceAddr + sx + (sy * m_sPitch);
	pDst = (WORD*)m_pDDraw->m_pBackB4Addr + dX + (dY * m_pDDraw->m_sBackB4Pitch);

	iRedPlus255 = sRed + 255;
	iGreenPlus255 = sGreen + 255;
	iBluePlus255 = sBlue + 255;

	switch (m_pDDraw->m_cPixelFormat) {
	case 1: // 565
		for (int destY = 0; destY < scaledHeight; ++destY) {
			for (int destX = 0; destX < scaledWidth; ++destX) {

				int srcX = destX * szx / scaledWidth;
				int srcY = destY * szy / scaledHeight;

				WORD* pCurrentSrc = pSrc + srcY * m_sPitch + srcX;
				WORD* pCurrentDst = pDst + destY * m_pDDraw->m_sBackB4Pitch + destX;

				if (*pCurrentSrc != m_wColorKey) {
					*pCurrentDst = (WORD)(
						(G_iAddTable31[(*pCurrentSrc & 0xF800) >> 11][iRedPlus255] << 11) |
						(G_iAddTable63[(*pCurrentSrc & 0x7E0) >> 5][iGreenPlus255] << 5) |
						(G_iAddTable31[(*pCurrentSrc & 0x1F)][iBluePlus255]));
				}
			}
		}
		break;

	case 2: // 555
		for (int destY = 0; destY < scaledHeight; ++destY) {
			for (int destX = 0; destX < scaledWidth; ++destX) {

				int srcX = destX * szx / scaledWidth;
				int srcY = destY * szy / scaledHeight;

				WORD* pCurrentSrc = pSrc + srcY * m_sPitch + srcX;
				WORD* pCurrentDst = pDst + destY * m_pDDraw->m_sBackB4Pitch + destX;

				if (*pCurrentSrc != m_wColorKey) {
					*pCurrentDst = (WORD)(
						(G_iAddTable31[(*pCurrentSrc & 0x7C00) >> 10][iRedPlus255] << 10) |
						(G_iAddTable31[(*pCurrentSrc & 0x3E0) >> 5][iGreenPlus255] << 5) |
						(G_iAddTable31[(*pCurrentSrc & 0x1F)][iBluePlus255]));
				}
			}
		}
		break;
	}

	m_bOnCriticalSection = FALSE;
}
