// DXC_dinput.cpp: implementation of the DXC_dinput class.
//
//////////////////////////////////////////////////////////////////////

#include "../Motor/DXC_dinput.h"
//added resolution
#include "..\Headers\Game.h"
#include "..\Resolution\Resolution.h"
extern class cResolution * c_reso;
extern class CGame * G_pGame;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DXC_dinput::DXC_dinput()
{
	m_pDI    = NULL;
	m_pMouse = NULL;
	m_sX     = 0;
	m_sY     = 0;
	m_sZ     = 0;
	m_bModernMouse = true;
}

DXC_dinput::~DXC_dinput()
{
	if (m_pMouse != NULL) {
		m_pMouse->Unacquire();
        m_pMouse->Release();
        m_pMouse = NULL;
	}
	if (m_pDI != NULL) {
		m_pDI->Release();
        m_pDI = NULL;
	}
}

BOOL DXC_dinput::bInit(HWND hWnd, HINSTANCE hInst)
{
 HRESULT hr;
 DIMOUSESTATE dims;	
 POINT Point;

	GetCursorPos(&Point);
	m_sX     = (short)(Point.x);
	m_sY     = (short)(Point.y); 

	hr = DirectInputCreate( hInst, DIRECTINPUT_VERSION, &m_pDI, NULL );
    if (hr != DI_OK) return FALSE;
	hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL );
	if (hr != DI_OK) return FALSE;
	hr = m_pMouse->SetDataFormat( &c_dfDIMouse );
	if (hr != DI_OK) return FALSE;
	hr = m_pMouse->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (hr != DI_OK) return FALSE;

//	m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );
	if ( m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims ) != DI_OK )
	{
		m_pMouse->Acquire();
		//return TRUE;
	}

	return TRUE;
}


void DXC_dinput::SetAcquire(BOOL bFlag)
{
 DIMOUSESTATE dims;

	if (m_pMouse == NULL) return;
	if (bFlag == TRUE) {
		m_pMouse->Acquire();
		m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );
	}
	else m_pMouse->Unacquire();
}
/*

void DXC_dinput::UpdateMouseState(short * pX, short * pY, short * pZ, char * pLB, char * pRB)
{
	//added resolution
	int res_x = 0;
	int res_y = 0;

	if (c_reso->IsResolution() == C800x600)
	{
		res_x = 799;
		res_y = 599;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		res_x = 639;
		res_y = 479;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		res_x = 1023;
		res_y = 767;
	}

	if (m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims) != DI_OK)
	{
		m_pMouse->Acquire();
		return;
	}
	m_sX += (short)dims.lX;
	m_sY += (short)dims.lY;
	if ((short)dims.lZ != 0)m_sZ = (short)dims.lZ;
	if (m_sX < 0) m_sX = 0;
	if (m_sY < 0) m_sY = 0;
	if (m_sX > res_x) m_sX = res_x;
	if (m_sY > res_y) m_sY = res_y;
	*pX = m_sX;
	*pY = m_sY;
	*pZ = m_sZ;
	*pLB = (char)dims.rgbButtons[0];
	*pRB = (char)dims.rgbButtons[1];
}
*/

extern HWND			G_hWnd;

void DXC_dinput::UpdateMouseState(short* pX, short* pY, short* pZ, char* pLB, char* pRB)
{
	// Resolución
	int res_x = 0;
	int res_y = 0;

	if (c_reso->IsResolution() == C800x600)
	{
		res_x = 799;
		res_y = 599;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		res_x = 639;
		res_y = 479;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		res_x = 1023;
		res_y = 767;
	}
	
	if (m_bModernMouse)
	{
	// Get the current cursor position
	POINT cursorPos;
	if (GetCursorPos(&cursorPos))
	{
	// Convert to client coordinates (if necessary)
		ScreenToClient(G_hWnd, &cursorPos);

	// Update mouse positions
	m_sX = static_cast<short>(cursorPos.x);
	m_sY = static_cast<short>(cursorPos.y);

	// Clamp to screen resolution
	if (m_sX < 0) m_sX = 0;
	if (m_sY < 0) m_sY = 0;
	if (m_sX > res_x) m_sX = res_x;
	if (m_sY > res_y) m_sY = res_y;

	*pX = m_sX;
	*pY = m_sY;
	}

	// Handle the scroll using DirectInput logic (retained from original code)
	if (m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims) == DI_OK)
	{
	if ((short)dims.lZ != 0)
	{
	m_sZ += (short)dims.lZ;
	}
	}
	else
	{
	m_pMouse->Acquire(); // Re-acquire if lost
	}

	// Check left and right button states
	*pLB = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
	*pRB = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1 : 0;

	// Pass the scroll value to pZ
	*pZ = m_sZ;
	}
	else
	{
		if (m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims) != DI_OK)
		{
			m_pMouse->Acquire();
			return;
		}
		m_sX += (short)dims.lX;
		m_sY += (short)dims.lY;
		if ((short)dims.lZ != 0)m_sZ = (short)dims.lZ;
		if (m_sX < 0) m_sX = 0;
		if (m_sY < 0) m_sY = 0;
		if (m_sX > res_x) m_sX = res_x;
		if (m_sY > res_y) m_sY = res_y;
		*pX = m_sX;
		*pY = m_sY;
		*pZ = m_sZ;
		*pLB = (char)dims.rgbButtons[0];
		*pRB = (char)dims.rgbButtons[1];
	}
}
