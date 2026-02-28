#pragma once
// ============================================================
// RenderBackend_SFML.h
// Implementacion SFML de IRenderBackend
// FASE 7: Primer sprite dibujado nativamente con SFML
//
// ESTRATEGIA HIBRIDA:
//   DDraw sigue manejando tiles, UI y presentacion final.
//   SFML renderiza sprites a un sf::RenderTexture (off-screen).
//   EndFrame() blitea los pixels SFML al backbuffer DDraw.
//   Esto evita conflictos DDraw/SFML en la misma ventana.
// ============================================================

#ifndef RENDERBACKEND_SFML_H
#define RENDERBACKEND_SFML_H

#include "IRenderBackend.h"
#include "DXC_ddraw.h"

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>

class RenderBackend_SFML : public IRenderBackend
{
public:
    // Recibe referencia a DXC_ddraw para poder blit a su backbuffer en EndFrame
    explicit RenderBackend_SFML(DXC_ddraw& ddraw);
    virtual ~RenderBackend_SFML();

    // --------------------------------------------------------
    // IRenderBackend interface
    // --------------------------------------------------------

    // Crea el sf::RenderTexture en la resolucion del juego
    virtual bool Init(HWND hWnd, int iWidth, int iHeight, bool bFullscreen) override;
    virtual void Shutdown() override;

    // BeginFrame: limpia el canvas SFML (transparente)
    virtual void BeginFrame() override;

    // EndFrame: blitea el RenderTexture al backbuffer DDraw
    virtual void EndFrame() override;

    // DrawSprite: dibuja en el RenderTexture usando sf::Sprite + sf::Texture
    virtual void DrawSprite(int iDstX, int iDstY,
        int iSrcX, int iSrcY,
        int iSrcW, int iSrcH,
        int iSpriteIndex) override;

    // DrawTile / DrawText: stubs - implementacion en Fase 8
    virtual void DrawTile(int iDstX, int iDstY,
        int iSrcX, int iSrcY,
        int iSrcW, int iSrcH,
        int iMapIndex) override;

    virtual void DrawText(int iX, int iY,
        const char* szText,
        unsigned long dwColor) override;

    virtual void SetResolution(int iWidth, int iHeight) override;
    virtual int  GetWidth()  override;
    virtual int  GetHeight() override;
    virtual void Clear()     override;

    // --------------------------------------------------------
    // API especifica de SFML (usada desde Game.cpp para cargar sprites)
    // --------------------------------------------------------

    // Carga una textura desde pixels DDraw 16-bit
    // wColorKey: valor de pixel que se trata como transparente (viene de CSprite::m_wColorKey)
    // iSpriteIndex: indice en el array interno (0..MAX_SFML_SPRITES-1)
    // Retorna true si cargo correctamente
    bool LoadSpriteTexture(int iSpriteIndex,
                           const unsigned short* pPixels16,
                           int iWidth, int iHeight,
                           unsigned short wColorKey = 0x0000);

    // Verifica si un indice tiene textura cargada
    bool IsTextureLoaded(int iSpriteIndex) const;

    // Maximo de sprites que puede manejar este backend en Fase 7
    static const int MAX_SFML_SPRITES = 128;

private:
    DXC_ddraw&         m_DDraw;            // referencia al DDraw de CGame - NO es dueno
    sf::RenderTexture* m_pRenderTex;       // canvas virtual (resolucion del juego)
    int                m_iWidth;
    int                m_iHeight;
    bool               m_bInitialized;

    // Texturas de sprites cargadas (indexadas por iSpriteIndex)
    sf::Texture        m_aTextures[MAX_SFML_SPRITES];
    bool               m_bTextureLoaded[MAX_SFML_SPRITES];

    // Convierte un pixel DDraw 16-bit a sf::Color RGBA
    // wColorKey: valor transparente real del sprite (de CSprite::m_wColorKey)
    // Usa m_DDraw.m_cPixelFormat para detectar RGB565/RGB555/BGR565
    sf::Color ConvertPixel16ToRGBA(unsigned short pixel16, unsigned short wColorKey) const;

    // Blitea los pixels del RenderTexture al backbuffer DDraw
    // Se llama desde EndFrame()
    void BlitRenderTextureToDDraw();
};

#endif // RENDERBACKEND_SFML_H
