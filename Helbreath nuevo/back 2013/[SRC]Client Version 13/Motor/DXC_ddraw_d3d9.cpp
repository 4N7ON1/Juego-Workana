/*
#include <windows.h>
#include <d3d9.h>
#include "../Motor/DXC_ddraw.h"

#pragma comment(lib, "d3d9.lib")

static IDirect3D9Ex* g_pD3D = nullptr;
static IDirect3DDevice9Ex* g_pDevice = nullptr;
static IDirect3DTexture9* g_pTexture = nullptr;
static IDirect3DSurface9* g_pBackBuffer = nullptr;
static D3DPRESENT_PARAMETERS g_d3dpp = {};
static int g_width = 0, g_height = 0;
static D3DFORMAT g_fmt = D3DFMT_R5G6B5;

void DXC_ddraw::D3D9_Init(HWND hWnd, int width, int height)
{
    g_width = width;
    g_height = height;

    HRESULT hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &g_pD3D);
    if (FAILED(hr)) { m_bD3D9Ready = false; return; }

    g_fmt = (m_cPixelFormat == 1) ? D3DFMT_R5G6B5 : D3DFMT_X1R5G5B5;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
    g_d3dpp.BackBufferCount = 1;
    g_d3dpp.hDeviceWindow = hWnd;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    g_d3dpp.BackBufferFormat = g_fmt;
    g_d3dpp.BackBufferWidth = width;
    g_d3dpp.BackBufferHeight = height;

    DWORD createFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_NOWINDOWCHANGES;
    IDirect3D9* pTempD3D = (IDirect3D9*)g_pD3D;
    D3DCAPS9 caps;
    if (SUCCEEDED(pTempD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
    {
        if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE)
            createFlags |= D3DCREATE_PUREDEVICE;
    }

    hr = g_pD3D->CreateDeviceEx(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        createFlags,
        &g_d3dpp,
        NULL,
        &g_pDevice
    );
    if (FAILED(hr)) { m_bD3D9Ready = false; return; }

    hr = g_pDevice->CreateTexture(
        width, height, 1,
        D3DUSAGE_DYNAMIC,
        g_fmt, D3DPOOL_DEFAULT,
        &g_pTexture, NULL
    );
    if (FAILED(hr)) { m_bD3D9Ready = false; return; }

    hr = g_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &g_pBackBuffer);
    if (FAILED(hr)) { m_bD3D9Ready = false; return; }

    g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    g_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    m_bD3D9Ready = true;
}

void DXC_ddraw::D3D9_UploadBuffer()
{
    if (!m_bD3D9Ready || !m_pBackB4Addr) return;

    D3DLOCKED_RECT rect;
    if (FAILED(g_pTexture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD | D3DLOCK_NO_DIRTY_UPDATE)))
        return;

    const int bytesPerPixel = 2;
    BYTE* dst = (BYTE*)rect.pBits;
    BYTE* src = (BYTE*)m_pBackB4Addr;
    const int pitchDst = rect.Pitch;
    const int pitchSrc = m_sBackB4Pitch * bytesPerPixel;
    const int copyBytes = g_width * bytesPerPixel;

    // Solo esta optimización probada: copia directa si pitches iguales
    if (pitchDst == pitchSrc && pitchDst == copyBytes) {
        memcpy(dst, src, copyBytes * g_height);
    }
    else {
        for (int y = 0; y < g_height; ++y)
        {
            memcpy(dst, src, copyBytes);
            dst += pitchDst;
            src += pitchSrc;
        }
    }

    g_pTexture->UnlockRect(0);
}

void DXC_ddraw::D3D9_Flip()
{
    if (!m_bD3D9Ready) return;

    IDirect3DSurface9* texSurf = nullptr;
    g_pTexture->GetSurfaceLevel(0, &texSurf);
    g_pDevice->BeginScene();
    g_pDevice->StretchRect(texSurf, NULL, g_pBackBuffer, NULL, D3DTEXF_NONE);
    g_pDevice->EndScene();
    g_pDevice->Present(NULL, NULL, NULL, NULL);
    texSurf->Release();
}

void DXC_ddraw::D3D9_Shutdown()
{
    if (g_pBackBuffer) { g_pBackBuffer->Release(); g_pBackBuffer = nullptr; }
    if (g_pTexture) { g_pTexture->Release();    g_pTexture = nullptr; }
    if (g_pDevice) { g_pDevice->Release();     g_pDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release();        g_pD3D = nullptr; }
    m_bD3D9Ready = false;
}*/