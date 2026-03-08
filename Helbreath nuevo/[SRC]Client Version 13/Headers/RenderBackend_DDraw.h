#pragma once
// ============================================================
// RenderBackend_DDraw.h
// Implementación DirectDraw de IRenderBackend
// FASE 6: Recibe referencia al DXC_ddraw existente en CGame
//         No crea un segundo objeto DDraw — zero riesgo visual
// ============================================================

#ifndef RENDERBACKEND_DDRAW_H
#define RENDERBACKEND_DDRAW_H

#include "IRenderBackend.h"
#include "DXC_ddraw.h"

class RenderBackend_DDraw : public IRenderBackend
{
public:
    // Recibe referencia al m_DDraw que ya vive en CGame
    explicit RenderBackend_DDraw(DXC_ddraw& ddraw);
    virtual ~RenderBackend_DDraw();

    // --- IRenderBackend interface ---

    // Init: en Fase 6 el juego ya llama bInit() directamente,
    // este método existe para cumplir la interfaz (no-op por ahora)
    virtual bool Init(HWND hWnd, int iWidth, int iHeight, bool bFullscreen) override;
    virtual void Shutdown() override;

    // BeginFrame: delega a ClearBackB4()
    virtual void BeginFrame() override;

    // EndFrame: delega a iFlip() — en Fase 6 el juego sigue
    // manejando DDERR_SURFACELOST directamente, así que este
    // método es no-op. iFlip se llama desde Game.cpp como antes.
    virtual void EndFrame() override;

    // Dibujo — stubs por ahora (Fase 7 los implementa)
    virtual void DrawSprite(int iDstX, int iDstY,
        int iSrcX, int iSrcY,
        int iSrcW, int iSrcH,
        int iSpriteIndex) override;

    virtual void DrawTile(int iDstX, int iDstY,
        int iSrcX, int iSrcY,
        int iSrcW, int iSrcH,
        int iMapIndex) override;

    virtual void DrawText(int iX, int iY,
        const char* szText,
        unsigned long dwColor,
        int iFontId = HB_FONT_COMIC_SANS,
        int iEffect = HB_TEXT_PLAIN) override;

    virtual void DrawTextCentered(int iX1, int iX2, int iY,
        const char* szText,
        unsigned long dwColor,
        int iFontId = HB_FONT_VERDANA) override;

    virtual void SetResolution(int iWidth, int iHeight) override;
    virtual int  GetWidth() override;
    virtual int  GetHeight() override;
    virtual void Clear() override;
    virtual bool IsTextureLoaded(int /*iSpriteIndex*/) override { return false; }
    virtual bool LoadSpriteFromPixels16(int, const unsigned short*, int, int, DWORD) override { return false; }
    virtual bool IsFrameActive() const override { return false; }
    virtual void SetViewCrop(int, int) override {} // no-op en DDraw
    virtual int GetCropX() const override { return 0; }
    virtual int GetCropY() const override { return 0; }
    virtual int MeasureTextWidth(const char*, int = HB_FONT_COMIC_SANS) override { return 0; }
    virtual void DrawSpriteColor(int, int, int, int, int, int, int, int=255, int=255, int=255, int=255) override {} // no-op

    // Acceso al DDraw subyacente (necesario para Fase 7+)
    DXC_ddraw& GetDDraw() { return m_DDraw; }

private:
    DXC_ddraw& m_DDraw;     // referencia al objeto en CGame — NO es dueño
    int        m_iWidth;
    int        m_iHeight;
};

#endif // RENDERBACKEND_DDRAW_H
