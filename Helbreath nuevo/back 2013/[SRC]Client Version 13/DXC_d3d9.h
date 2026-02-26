#pragma once
#include <windows.h>
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

class DXC_d3d9
{
public:
	DXC_d3d9();
	~DXC_d3d9();

	bool Init(HWND hWnd, int width, int height, bool fullscreen);
	void Shutdown();

	void ClearBackBuffer();
	void Present();
	void PutPixel(short x, short y, WORD r, WORD g, WORD b);
	void DrawShadowBox(short sX, short sY, short dX, short dY);

	inline WORD* GetBuffer() { return m_pFrameBuffer; }
	inline int GetPitch() const { return m_pitch; }

private:
	// --- D3D9 core ---
	IDirect3D9Ex*         m_pD3D;
	IDirect3DDevice9Ex*   m_pDevice;
	IDirect3DTexture9*    m_pTexture;
	IDirect3DSurface9*    m_pSurface;
	D3DPRESENT_PARAMETERS m_d3dpp;

	// --- Framebuffer ---
	WORD*  m_pFrameBuffer;
	int    m_width;
	int    m_height;
	int    m_pitch;
	bool   m_fullscreen;
	bool   m_ready;
};
