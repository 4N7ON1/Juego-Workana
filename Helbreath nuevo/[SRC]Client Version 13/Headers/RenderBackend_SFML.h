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
#include <unordered_map>

class RenderBackend_SFML : public IRenderBackend
{
public:
    // Recibe referencia a DXC_ddraw para poder blit a su backbuffer en EndFrame
    explicit RenderBackend_SFML(DXC_ddraw& ddraw);
    virtual ~RenderBackend_SFML() override;

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

    // DrawTile: stub - implementacion en Fase 8.G
    virtual void DrawTile(int iDstX, int iDstY,
        int iSrcX, int iSrcY,
        int iSrcW, int iSrcH,
        int iMapIndex) override;

    // Fase 8.F: Texto SFML con fuentes del sistema
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
                           DWORD dwColorKey = 0x10000); // 0x10000 = sin colorkey

    // Verifica si un indice tiene textura cargada
    virtual bool IsTextureLoaded(int iSpriteIndex) override;
    virtual bool LoadSpriteFromPixels16(int iSpriteIndex, const unsigned short* pPixels, int iW, int iH, DWORD dwColorKey) override;

    // Devuelve true entre BeginFrame() y EndFrame()
    // Sprites con m_iSpriteIndex>=0 solo van a SFML si el frame esta activo.
    // Fuera del frame (login, menus, seleccion) caen a DDraw BltFast.
    virtual bool IsFrameActive() const override { return m_bFrameActive; }

    // Fase 8.D fix: recorte del canvas SFML para compensar sModX/sModY
    virtual void SetViewCrop(int cropX, int cropY) override;
    virtual int GetCropX() const override { return m_iCropX; }
    virtual int GetCropY() const override { return m_iCropY; }



private:
    DXC_ddraw&         m_DDraw;            // referencia al DDraw de CGame - NO es dueno
    sf::RenderTexture* m_pRenderTex;       // canvas virtual (resolucion del juego)
    int                m_iWidth;
    int                m_iHeight;
    bool               m_bInitialized;
    bool               m_bFrameActive;    // true entre BeginFrame() y EndFrame()
    int                m_iCropX;          // offset X del recorte (sModX+offsetX)
    int                m_iCropY;          // offset Y del recorte (sModY+offsetY)

    // Mapa dinamico: indice real del sprite -> textura SFML
    // Sin limite fijo - soporta todos los sprites del juego
    std::unordered_map<int, sf::Texture> m_mapTextures;

    // ---- Fase 8.F: Infraestructura de texto SFML ----
    sf::Font m_fonts[HB_FONT_COUNT];      // 6 fuentes del sistema
    bool     m_bFontsLoaded;              // true si al menos 1 fuente cargo

    // Tamanios y estilos por fuente (indices HBFont)
    static const unsigned int  s_fontSizes[HB_FONT_COUNT];
    static const sf::Uint32    s_fontStyles[HB_FONT_COUNT];

    // Convierte un pixel DDraw 16-bit a sf::Color RGBA
    // wColorKey: valor transparente real del sprite (de CSprite::m_wColorKey)
    // Usa m_DDraw.m_cPixelFormat para detectar RGB565/RGB555/BGR565
    sf::Color ConvertPixel16ToRGBA(unsigned short pixel16, DWORD dwColorKey) const;

    // Blitea los pixels del RenderTexture al backbuffer DDraw
    // Se llama desde EndFrame()
    void BlitRenderTextureToDDraw();
};

#endif // RENDERBACKEND_SFML_H
