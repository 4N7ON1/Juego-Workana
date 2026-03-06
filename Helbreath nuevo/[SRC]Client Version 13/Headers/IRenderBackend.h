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

    // Inicializaci�n y shutdown
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

    // Resoluci�n
    virtual void SetResolution(int iWidth, int iHeight) = 0;
    virtual int  GetWidth() = 0;
    virtual int  GetHeight() = 0;

    // Limpieza de pantalla
    virtual void Clear() = 0;
    // Comprueba si un sprite esta cargado en SFML
    virtual bool IsTextureLoaded(int iSpriteIndex) = 0;
    virtual bool LoadSpriteFromPixels16(int iSpriteIndex, const unsigned short* pPixels, int iW, int iH, DWORD dwColorKey) = 0;

    // Devuelve true si estamos entre BeginFrame() y EndFrame()
    // Usado por PutSpriteFast/PutSpriteFastNoColorKeyDst para decidir si
    // usar SFML (en juego) o caer a DDraw BltFast (login/menus/seleccion)
    virtual bool IsFrameActive() const = 0;

    // Fase 8.D fix: establece el offset de recorte para el blit SFML->DDraw.
    // DrawBackground dibuja tiles con posiciones que incluyen sModX/sModY.
    // El BltFast original recortaba desde (cropX,cropY) del PDBGS.
    // Al saltar el BltFast, SFML necesita aplicar el mismo recorte.
    virtual void SetViewCrop(int cropX, int cropY) = 0;

};

#endif // IRENDERBACKEND_H