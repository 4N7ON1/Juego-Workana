#pragma once
// ============================================================
// IRenderBackend.h
// Interfaz abstracta del sistema de render de Helbreath Cursed
// ============================================================

#ifndef IRENDERBACKEND_H
#define IRENDERBACKEND_H

#include <windows.h>

class IRenderBackend
{
public:
    virtual ~IRenderBackend() {}

    // Inicialización y shutdown
    virtual bool Init(HWND hWnd, int iWidth, int iHeight, bool bFullscreen) = 0;
    virtual void Shutdown() = 0;

    // Control de frame
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    // Dibujo de sprites
    virtual void DrawSprite(int iDstX, int iDstY,
        int iSrcX, int iSrcY,
        int iSrcW, int iSrcH,
        int iSpriteIndex) = 0;

    // Dibujo de tiles
    virtual void DrawTile(int iDstX, int iDstY,
        int iSrcX, int iSrcY,
        int iSrcW, int iSrcH,
        int iMapIndex) = 0;

    // Texto
    virtual void DrawText(int iX, int iY,
        const char* szText,
        unsigned long dwColor) = 0;

    // Resolución
    virtual void SetResolution(int iWidth, int iHeight) = 0;
    virtual int  GetWidth() = 0;
    virtual int  GetHeight() = 0;

    // Limpieza de pantalla
    virtual void Clear() = 0;
};

#endif // IRENDERBACKEND_H