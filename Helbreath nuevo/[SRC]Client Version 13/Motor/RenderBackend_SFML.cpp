// ============================================================
// RenderBackend_SFML.cpp
// Implementacion SFML de IRenderBackend
// FASE 7: Primer sprite dibujado nativamente con SFML
//
// ESTRATEGIA HIBRIDA:
//   DDraw sigue manejando tiles, UI y presentacion final (iFlip).
//   SFML renderiza sprites a un sf::RenderTexture off-screen.
//   BlitRenderTextureToDDraw() copia los pixels SFML al backbuffer
//   DDraw antes del iFlip(), sin conflictos de ventana.
// ============================================================

#include "RenderBackend_SFML.h"

// ============================================================
// Constructor / Destructor
// ============================================================

RenderBackend_SFML::RenderBackend_SFML(DXC_ddraw& ddraw)
    : m_DDraw(ddraw)
    , m_pRenderTex(nullptr)
    , m_iWidth(800)
    , m_iHeight(600)
    , m_bInitialized(false)
{
    memset(m_bTextureLoaded, 0, sizeof(m_bTextureLoaded));
}

RenderBackend_SFML::~RenderBackend_SFML()
{
    Shutdown();
}

// ============================================================
// Init / Shutdown
// ============================================================

bool RenderBackend_SFML::Init(HWND hWnd, int iWidth, int iHeight, bool bFullscreen)
{
    // hWnd y bFullscreen no se usan en la estrategia hibrida:
    // SFML no crea ventana propia, solo usa un RenderTexture off-screen.
    (void)hWnd;
    (void)bFullscreen;

    m_iWidth  = iWidth;
    m_iHeight = iHeight;

    // Canvas virtual SFML en la resolucion logica del juego
    m_pRenderTex = new sf::RenderTexture();
    if (!m_pRenderTex->create(static_cast<unsigned int>(iWidth),
                              static_cast<unsigned int>(iHeight)))
    {
        delete m_pRenderTex;
        m_pRenderTex = nullptr;
        return false;
    }

    m_pRenderTex->setSmooth(false); // Sin interpolacion - pixelart fiel

    m_bInitialized = true;
    return true;
}

void RenderBackend_SFML::Shutdown()
{
    if (m_pRenderTex)
    {
        delete m_pRenderTex;
        m_pRenderTex = nullptr;
    }
    m_bInitialized = false;
}

// ============================================================
// Control de frame
// ============================================================

void RenderBackend_SFML::BeginFrame()
{
    if (!m_bInitialized) return;

    // Limpiar el canvas SFML con transparente total
    // Los pixels que no se dibujen quedan en alpha=0 y no se blit-ean
    m_pRenderTex->clear(sf::Color(0, 0, 0, 0));
}

void RenderBackend_SFML::EndFrame()
{
    if (!m_bInitialized) return;

    // Finalizar el frame SFML
    m_pRenderTex->display();

    // Blit del canvas SFML al backbuffer DDraw
    // Solo los pixels con alpha > 0 sobreescriben el backbuffer
    BlitRenderTextureToDDraw();
}

// ============================================================
// Dibujo de sprites
// ============================================================

void RenderBackend_SFML::DrawSprite(int iDstX, int iDstY,
    int iSrcX, int iSrcY,
    int iSrcW, int iSrcH,
    int iSpriteIndex)
{
    if (!m_bInitialized) return;
    if (iSpriteIndex < 0 || iSpriteIndex >= MAX_SFML_SPRITES) return;
    if (!m_bTextureLoaded[iSpriteIndex]) return;

    sf::Sprite spr(m_aTextures[iSpriteIndex]);
    spr.setTextureRect(sf::IntRect(iSrcX, iSrcY, iSrcW, iSrcH));
    spr.setPosition(static_cast<float>(iDstX), static_cast<float>(iDstY));

    m_pRenderTex->draw(spr);
}

// ============================================================
// Stubs para Fase 8 (tiles y texto)
// ============================================================

void RenderBackend_SFML::DrawTile(int iDstX, int iDstY,
    int iSrcX, int iSrcY,
    int iSrcW, int iSrcH,
    int iMapIndex)
{
    // FASE 8: migrar DrawBackground aqui
    (void)iDstX; (void)iDstY;
    (void)iSrcX; (void)iSrcY;
    (void)iSrcW; (void)iSrcH;
    (void)iMapIndex;
}

void RenderBackend_SFML::DrawText(int iX, int iY,
    const char* szText,
    unsigned long dwColor)
{
    // FASE 8: migrar texto (reemplazar GDI GetDC) aqui
    (void)iX; (void)iY; (void)szText; (void)dwColor;
}

// ============================================================
// Resolucion
// ============================================================

void RenderBackend_SFML::SetResolution(int iWidth, int iHeight)
{
    m_iWidth  = iWidth;
    m_iHeight = iHeight;
}

int RenderBackend_SFML::GetWidth()  { return m_iWidth;  }
int RenderBackend_SFML::GetHeight() { return m_iHeight; }

void RenderBackend_SFML::Clear()
{
    BeginFrame();
}

// ============================================================
// Carga de texturas desde pixels DDraw 16-bit
// ============================================================

bool RenderBackend_SFML::LoadSpriteTexture(int iSpriteIndex,
                                            const unsigned short* pPixels16,
                                            int iWidth, int iHeight,
                                            unsigned short wColorKey)
{
    if (iSpriteIndex < 0 || iSpriteIndex >= MAX_SFML_SPRITES) return false;
    if (!pPixels16 || iWidth <= 0 || iHeight <= 0)            return false;

    // Convertir de 16-bit DDraw a RGBA 32-bit usando el colorkey real del sprite
    std::vector<sf::Uint8> pixels32(iWidth * iHeight * 4);

    for (int i = 0; i < iWidth * iHeight; i++)
    {
        sf::Color c = ConvertPixel16ToRGBA(pPixels16[i], wColorKey);
        pixels32[i * 4 + 0] = c.r;
        pixels32[i * 4 + 1] = c.g;
        pixels32[i * 4 + 2] = c.b;
        pixels32[i * 4 + 3] = c.a;
    }

    if (!m_aTextures[iSpriteIndex].create(
            static_cast<unsigned int>(iWidth),
            static_cast<unsigned int>(iHeight)))
        return false;

    m_aTextures[iSpriteIndex].update(pixels32.data());
    m_aTextures[iSpriteIndex].setSmooth(false); // pixelart: sin blur

    m_bTextureLoaded[iSpriteIndex] = true;
    return true;
}

bool RenderBackend_SFML::IsTextureLoaded(int iSpriteIndex) const
{
    if (iSpriteIndex < 0 || iSpriteIndex >= MAX_SFML_SPRITES) return false;
    return m_bTextureLoaded[iSpriteIndex];
}

// ============================================================
// Conversion de pixel DDraw 16-bit a RGBA 32-bit
// ============================================================

sf::Color RenderBackend_SFML::ConvertPixel16ToRGBA(unsigned short pixel16,
                                                    unsigned short wColorKey) const
{
    // Colorkey real del sprite (primer pixel del sheet = fondo transparente)
    if (pixel16 == wColorKey)
        return sf::Color::Transparent;

    sf::Uint8 r, g, b;

    // Detectar formato de pixel desde DXC_ddraw::m_cPixelFormat:
    //   1 = RGB565  (dwRBitMask = 0xF800)  RRRRRGGGGGGBBBBB
    //   2 = RGB555  (dwRBitMask = 0x7C00)  xRRRRRGGGGGBBBBB
    //   3 = BGR565  (dwRBitMask = 0x001F)  BBBBBGGGGGGRRRRR
    if (m_DDraw.m_cPixelFormat == 2)        // RGB555
    {
        r = static_cast<sf::Uint8>(((pixel16 >> 10) & 0x1F) * 255 / 31);
        g = static_cast<sf::Uint8>(((pixel16 >>  5) & 0x1F) * 255 / 31);
        b = static_cast<sf::Uint8>(( pixel16        & 0x1F) * 255 / 31);
    }
    else if (m_DDraw.m_cPixelFormat == 3)   // BGR565
    {
        b = static_cast<sf::Uint8>(((pixel16 >> 11) & 0x1F) * 255 / 31);
        g = static_cast<sf::Uint8>(((pixel16 >>  5) & 0x3F) * 255 / 63);
        r = static_cast<sf::Uint8>(( pixel16        & 0x1F) * 255 / 31);
    }
    else                                    // RGB565 (formato 1 o desconocido)
    {
        r = static_cast<sf::Uint8>(((pixel16 >> 11) & 0x1F) * 255 / 31);
        g = static_cast<sf::Uint8>(((pixel16 >>  5) & 0x3F) * 255 / 63);
        b = static_cast<sf::Uint8>(( pixel16        & 0x1F) * 255 / 31);
    }

    return sf::Color(r, g, b, 255);
}

// ============================================================
// Blit del RenderTexture SFML al backbuffer DDraw
// ============================================================

void RenderBackend_SFML::BlitRenderTextureToDDraw()
{
    // Obtener los pixels del canvas SFML como imagen RGBA en CPU
    sf::Image img = m_pRenderTex->getTexture().copyToImage();

    const sf::Uint8* pSrc = img.getPixelsPtr();
    if (!pSrc) return;

    // Obtener acceso directo al backbuffer DDraw (Lock)
    DDSURFACEDESC2 ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);

    if (!m_DDraw.m_lpBackB4) return;

    HRESULT hr = m_DDraw.m_lpBackB4->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr);
    if (FAILED(hr)) return;

    unsigned short* pDst = reinterpret_cast<unsigned short*>(ddsd.lpSurface);
    int iPitch = ddsd.lPitch / 2; // pitch en shorts (16-bit por pixel)

    int iW = min(m_iWidth,  static_cast<int>(img.getSize().x));
    int iH = min(m_iHeight, static_cast<int>(img.getSize().y));

    for (int y = 0; y < iH; y++)
    {
        for (int x = 0; x < iW; x++)
        {
            int srcIdx = (y * img.getSize().x + x) * 4;
            sf::Uint8 a = pSrc[srcIdx + 3];

            // Solo sobreescribir si el pixel SFML no es transparente
            if (a > 0)
            {
                sf::Uint8 r = pSrc[srcIdx + 0];
                sf::Uint8 g = pSrc[srcIdx + 1];
                sf::Uint8 b = pSrc[srcIdx + 2];

                // Convertir RGBA32 -> RGB565 para DDraw
                unsigned short pixel565 =
                    ((r >> 3) << 11) |
                    ((g >> 2) <<  5) |
                    ( b >> 3);

                pDst[y * iPitch + x] = pixel565;
            }
        }
    }

    m_DDraw.m_lpBackB4->Unlock(nullptr);
}
