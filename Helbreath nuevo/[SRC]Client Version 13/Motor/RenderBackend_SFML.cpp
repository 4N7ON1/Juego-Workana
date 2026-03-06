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
    , m_bFrameActive(false)
    , m_iCropX(0)
    , m_iCropY(0)
{

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

    // Canvas virtual SFML: mas grande que la pantalla para acomodar tiles extra.
    // El PDBGS original es (res_x+32, res_y+32). Agregamos 96 de margen
    // para offsetX/offsetY del movimiento suave + sModX/sModY.
    int canvasW = iWidth + 96;
    int canvasH = iHeight + 96;
    m_pRenderTex = new sf::RenderTexture();
    if (!m_pRenderTex->create(static_cast<unsigned int>(canvasW),
                              static_cast<unsigned int>(canvasH)))
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

    // Activar contexto SFML y MANTENERLO activo durante todo el frame.
    // cnc-ddraw usa shadow buffers CPU para BltFast (no usa OpenGL entre frames),
    // OpenGL de cnc-ddraw solo se necesita en iFlip() que ocurre DESPUES de EndFrame().
    // Eliminar el setActive(false) aqui previene 1000+ context-switches por frame
    // (500 tiles x 2) que corrompian el estado del RenderTexture.
    m_pRenderTex->setActive(true);
    m_pRenderTex->clear(sf::Color(0, 1, 0, 255)); // Centinela: imposible en RGB565
    // NO setActive(false): el contexto permanece activo hasta EndFrame()
    m_bFrameActive = true; // Habilita ruta SFML en PutSpriteFast/PutSpriteFastNoColorKeyDst
    m_iCropX = 0; // Reset crop cada frame (DrawBackground lo setea antes de EndFrame)
    m_iCropY = 0;
}


void RenderBackend_SFML::EndFrame()
{
    if (!m_bInitialized) return;

    // Contexto SFML ya activo desde BeginFrame() - no necesitamos setActive(true) aqui.
    // Finalizar el frame: display() sella el RenderTexture para poder leerlo.
    m_pRenderTex->display();

    // Blit del canvas SFML al backbuffer DDraw (copyToImage necesita contexto activo)
    BlitRenderTextureToDDraw();

    // Liberar contexto OpenGL AHORA para que cnc-ddraw pueda usarlo en iFlip()
    m_bFrameActive = false; // Deshabilita ruta SFML: sprites post-EndFrame van a DDraw
    m_pRenderTex->setActive(false);
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
    if (!m_bFrameActive) return;  // Solo dibujar entre BeginFrame/EndFrame (contexto activo)
    if (m_mapTextures.find(iSpriteIndex) == m_mapTextures.end()) return;

    // Sin setActive(true/false): el contexto ya esta activo desde BeginFrame().
    // Eliminar estos context-switches fue la solucion al black screen con 500+ tiles.
    sf::Sprite spr(m_mapTextures.at(iSpriteIndex));
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
                                            DWORD dwColorKey)
{
    
    if (!pPixels16 || iWidth <= 0 || iHeight <= 0)            return false;

    // Convertir de 16-bit DDraw a RGBA 32-bit usando el colorkey real del sprite
    std::vector<sf::Uint8> pixels32(iWidth * iHeight * 4);

    for (int i = 0; i < iWidth * iHeight; i++)
    {
        sf::Color c = ConvertPixel16ToRGBA(pPixels16[i], dwColorKey);
        pixels32[i * 4 + 0] = c.r;
        pixels32[i * 4 + 1] = c.g;
        pixels32[i * 4 + 2] = c.b;
        pixels32[i * 4 + 3] = c.a;
    }
    sf::Texture tex;
    if (!tex.create(static_cast<unsigned int>(iWidth),
        static_cast<unsigned int>(iHeight)))
        return false;

    tex.update(pixels32.data());
    tex.setSmooth(false);

    m_mapTextures[iSpriteIndex] = std::move(tex);
    return true;

}

bool RenderBackend_SFML::IsTextureLoaded(int iSpriteIndex)

{
    return m_mapTextures.count(iSpriteIndex) > 0;

}
bool RenderBackend_SFML::LoadSpriteFromPixels16(int iSpriteIndex,
    const unsigned short* pPixels,
    int iW, int iH,
    DWORD dwColorKey)
{
    return LoadSpriteTexture(iSpriteIndex, pPixels, iW, iH, dwColorKey);
}


// ============================================================
// Conversion de pixel DDraw 16-bit a RGBA 32-bit
// ============================================================

sf::Color RenderBackend_SFML::ConvertPixel16ToRGBA(unsigned short pixel16,
                                                    DWORD dwColorKey) const
{
    // dwColorKey: colorkey real del sprite (valor RGB565 del fondo transparente)
    // dwColorKey > 0xFFFF = "sin colorkey" (tiles opacos - se usa 0x10000 como sentinel)
    // dwColorKey <= 0xFFFF = colorkey valido (incluye 0xFFFF=blanco, comun en arboles)
    if (dwColorKey <= 0xFFFF && pixel16 == static_cast<unsigned short>(dwColorKey))
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
// SetViewCrop: offset de recorte para replicar el BltFast del PDBGS
// ============================================================

void RenderBackend_SFML::SetViewCrop(int cropX, int cropY)
{
    m_iCropX = cropX;
    m_iCropY = cropY;
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

    int imgW = static_cast<int>(img.getSize().x);
    int imgH = static_cast<int>(img.getSize().y);

    // Aplicar el crop: leer desde (m_iCropX, m_iCropY) del canvas SFML
    // Esto replica el BltFast original que copiaba PDBGS desde (sModX+offsetX, sModY+offsetY)
    // Limitar a la resolucion de pantalla (m_DDraw.res_x/y), no al canvas SFML (que es mas grande)
    int screenW = m_DDraw.res_x;
    int screenH = m_DDraw.res_y;
    int iW = min(screenW, imgW - m_iCropX);
    int iH = min(screenH, imgH - m_iCropY);
    if (iW <= 0 || iH <= 0)
    {
        m_DDraw.m_lpBackB4->Unlock(nullptr);
        return;
    }

    for (int y = 0; y < iH; y++)
    {
        for (int x = 0; x < iW; x++)
        {
            // Leer desde (x + cropX, y + cropY) del canvas SFML
            int srcIdx = ((y + m_iCropY) * imgW + (x + m_iCropX)) * 4;

            sf::Uint8 r = pSrc[srcIdx + 0];
            sf::Uint8 g = pSrc[srcIdx + 1];
            sf::Uint8 b = pSrc[srcIdx + 2];

            // Solo sobreescribir si el pixel SFML no es el centinela (0,1,0)
            if (!(r == 0 && g == 1 && b == 0))
            {
                // Convertir RGBA32 -> RGB565 para DDraw
                unsigned short pixel565 =
                    ((r >> 3) << 11) |
                    ((g >> 2) << 5) |
                    (b >> 3);

                pDst[y * iPitch + x] = pixel565;
            }
        }
    }

    m_DDraw.m_lpBackB4->Unlock(nullptr);
}
