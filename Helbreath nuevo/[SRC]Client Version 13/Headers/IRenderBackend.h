#pragma once
// ============================================================
// IRenderBackend.h
// Interfaz abstracta del sistema de render de Helbreath Cursed
// ============================================================

#ifndef IRENDERBACKEND_H
#define IRENDERBACKEND_H

#include <windows.h>

// ============================================================
// Enums de fuentes y efectos de texto (Fase 8.F)
// Mapeo de las 6 fuentes GDI del juego a indices SFML
// ============================================================

enum HBFont {
    HB_FONT_COMIC_SANS = 0, // m_hFontInUse  - Comic Sans MS, 16, Normal
    HB_FONT_CAMBRIA,        // m_hFontInUse2 - Cambria, 16, Bold
    HB_FONT_VERDANA,        // m_hFontInUse3 - Verdana, 13, Normal
    HB_FONT_CALIBRI,        // m_hFontInUse4 - Calibri, 18, Bold
    HB_FONT_TAHOMA,         // m_hFontInUse5 - Tahoma, 13, Normal
    HB_FONT_TIMES,          // m_hFontHP     - Times New Roman, 13, Bold
    HB_FONT_COUNT
};

enum HBTextEffect {
    HB_TEXT_PLAIN    = 0,   // Sin efecto
    HB_TEXT_SHADOW   = 1,   // Sombra derecha+abajo (1px)
    HB_TEXT_OUTLINE4 = 2,   // Outline 4-direcciones
};

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

    // Texto (Fase 8.F: expandido con fuente y efecto)
    virtual void DrawText(int iX, int iY,
        const char* szText,
        unsigned long dwColor,
        int iFontId = HB_FONT_COMIC_SANS,
        int iEffect = HB_TEXT_PLAIN) = 0;

    // Texto centrado horizontalmente entre iX1 e iX2
    virtual void DrawTextCentered(int iX1, int iX2, int iY,
        const char* szText,
        unsigned long dwColor,
        int iFontId = HB_FONT_VERDANA) = 0;

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

    // Obtener el crop actual. PutSpriteFast (personajes/objetos) usa esto
    // para compensar el crop: dibuja en (dX+cropX, dY+cropY) en el canvas SFML,
    // asi cuando BlitRenderTextureToDDraw recorta desde (cropX,cropY),
    // los sprites terminan en la posicion correcta de pantalla.
    // PutSpriteFastNoColorKeyDst (tiles) NO necesita esto porque sus posiciones
    // ya incluyen sModX/sModY (coordenadas PDBGS, no de pantalla).
    virtual int GetCropX() const = 0;
    virtual int GetCropY() const = 0;

    // Fase 8.G: Medir ancho de texto en pixels (para centrado manual)
    virtual int MeasureTextWidth(const char* szText, int iFontId = HB_FONT_COMIC_SANS) = 0;

    // Fase 8.H: Dibujo de sprites con color/alpha (efectos de transparencia, tintado)
    // iR/iG/iB = 0-255 (multiplica color del sprite), iA = 0-255 (opacidad)
    // Valores por defecto (255,255,255,255) = sprite sin modificar = DrawSprite normal
    virtual void DrawSpriteColor(int iDstX, int iDstY,
        int iSrcX, int iSrcY, int iSrcW, int iSrcH,
        int iSpriteIndex,
        int iR = 255, int iG = 255, int iB = 255, int iA = 255) = 0;

    // Fase 8.I: Sombra proyectada con transformacion geometrica (paralelogramo)
    // Usa la silueta del sprite (alpha del colorkey) como mascara.
    // Dibuja en negro semitransparente con shear horizontal + compresion vertical.
    // fShearX: factor de cizallamiento horizontal (1/3 = sombra a la derecha)
    // fScaleY: factor de compresion vertical (1/3 = altura comprimida)
    virtual void DrawSpriteShadow(int iSrcX, int iSrcY, int iSrcW, int iSrcH,
        int iSpriteIndex,
        float fDstX, float fDstY,
        float fShearX, float fScaleY) = 0;

};

#endif // IRENDERBACKEND_H