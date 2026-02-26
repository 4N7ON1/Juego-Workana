#include "DXC_d3d9.h"
#include <cstring>

DXC_d3d9::DXC_d3d9()
{
	m_pD3D = nullptr;
	m_pDevice = nullptr;
	m_pTexture = nullptr;
	m_pSurface = nullptr;
	m_pFrameBuffer = nullptr;
	m_width = 0;
	m_height = 0;
	m_pitch = 0;
	m_fullscreen = false;
	m_ready = false;
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
}

DXC_d3d9::~DXC_d3d9()
{
	Shutdown();
}

bool DXC_d3d9::Init(HWND hWnd, int width, int height, bool fullscreen)
{
	m_width = width;
	m_height = height;
	m_pitch = width;
	m_fullscreen = fullscreen;

	HRESULT hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &m_pD3D);
	if (FAILED(hr) || !m_pD3D)
		return false;

	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.Windowed = !fullscreen;
	m_d3dpp.hDeviceWindow = hWnd;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	m_d3dpp.BackBufferWidth = width;
	m_d3dpp.BackBufferHeight = height;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = m_pD3D->CreateDeviceEx(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		nullptr,
		&m_pDevice
		);
	if (FAILED(hr) || !m_pDevice)
		return false;

	// Crear textura dinámica (framebuffer 16 bits)
	hr = m_pDevice->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_R5G6B5,
		D3DPOOL_DEFAULT,
		&m_pTexture,
		nullptr
		);
	if (FAILED(hr))
		return false;

	m_pTexture->GetSurfaceLevel(0, &m_pSurface);

	// Crear framebuffer en RAM
	m_pFrameBuffer = new WORD[width * height];
	memset(m_pFrameBuffer, 0, width * height * sizeof(WORD));

	m_ready = true;
	return true;
}

void DXC_d3d9::Shutdown()
{
	if (m_pSurface) { m_pSurface->Release(); m_pSurface = nullptr; }
	if (m_pTexture) { m_pTexture->Release(); m_pTexture = nullptr; }
	if (m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr; }
	if (m_pD3D) { m_pD3D->Release(); m_pD3D = nullptr; }
	delete[] m_pFrameBuffer; m_pFrameBuffer = nullptr;
	m_ready = false;
}

void DXC_d3d9::ClearBackBuffer()
{
	if (!m_ready || !m_pFrameBuffer) return;
	memset(m_pFrameBuffer, 0, m_width * m_height * sizeof(WORD));
}

void DXC_d3d9::PutPixel(short x, short y, WORD r, WORD g, WORD b)
{
	if (!m_ready) return;
	if (x < 0 || y < 0 || x >= m_width || y >= m_height) return;

	WORD color = (WORD)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
	m_pFrameBuffer[y * m_pitch + x] = color;
}

void DXC_d3d9::DrawShadowBox(short sX, short sY, short dX, short dY)
{
	if (!m_ready) return;

	if (sX < 0) sX = 0;
	if (sY < 0) sY = 0;
	if (dX >= m_width) dX = m_width - 1;
	if (dY >= m_height) dY = m_height - 1;

	for (int y = sY; y <= dY; y++)
	{
		WORD* row = &m_pFrameBuffer[y * m_pitch];
		for (int x = sX; x <= dX; x++)
			row[x] = (row[x] & 0xf7de) >> 1; // simple darken
	}
}

void DXC_d3d9::Present()
{
	if (!m_ready || !m_pDevice) return;

	D3DLOCKED_RECT rect;
	HRESULT hr = m_pTexture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
	if (SUCCEEDED(hr))
	{
		BYTE* dest = (BYTE*)rect.pBits;
		const BYTE* src = (BYTE*)m_pFrameBuffer;
		for (int y = 0; y < m_height; y++)
		{
			memcpy(dest + y * rect.Pitch, src + y * m_pitch * 2, m_width * 2);
		}
		m_pTexture->UnlockRect(0);
	}

	m_pDevice->BeginScene();
	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	struct VERTEX {
		float x, y, z, rhw;
		float u, v;
	};
	VERTEX quad[4] = {
		{ 0, 0, 0, 1.0f, 0, 0 },
		{ (float)m_width, 0, 0, 1.0f, 1, 0 },
		{ 0, (float)m_height, 0, 1.0f, 0, 1 },
		{ (float)m_width, (float)m_height, 0, 1.0f, 1, 1 }
	};

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(VERTEX));
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
