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
// Fase 8.F: Configuracion de fuentes (tamanios y estilos GDI)
// ============================================================

// Tamanios de fuente (mapeo de GDI CreateFont height)
const unsigned int RenderBackend_SFML::s_fontSizes[HB_FONT_COUNT] = {
    16, // HB_FONT_COMIC_SANS - Comic Sans MS
    16, // HB_FONT_CAMBRIA    - Cambria
    13, // HB_FONT_VERDANA    - Verdana
    18, // HB_FONT_CALIBRI    - Calibri
    13, // HB_FONT_TAHOMA     - Tahoma
    13  // HB_FONT_TIMES      - Times New Roman
};

// Estilos de fuente (mapeo de GDI FW_NORMAL / FW_BOLD)
const sf::Uint32 RenderBackend_SFML::s_fontStyles[HB_FONT_COUNT] = {
    sf::Text::Regular,  // Comic Sans MS  - FW_NORMAL
    sf::Text::Bold,     // Cambria         - FW_BOLD
    sf::Text::Regular,  // Verdana         - FW_NORMAL
    sf::Text::Bold,     // Calibri         - FW_BOLD
    sf::Text::Regular,  // Tahoma          - FW_NORMAL
    sf::Text::Bold      // Times New Roman - FW_BOLD
};

// ============================================================
// HD SCALING: Factor de supersampling SFML (2x SSAA)
// SFML dibuja TODO al doble de resolucion y BlitRenderTextureToDDraw
// hace un box-filter 2:1 → sprites con anti-aliasing sin cambiar
// su tamano aparente en pantalla. Cambiar a 1 para desactivar.
// ============================================================
static const int   SFML_S  = 1;       // 1=sin SSAA (rapido), 2=SSAA 2x (calidad, mas lento)
static const float SFML_SF = 1.0f;    // Factor float

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
    , m_bFontsLoaded(false)
    , m_pGrayscaleShader  (nullptr)
    , m_bGrayscaleShaderLoaded  (false)
    , m_hSFMLChild(NULL)
    , m_pRenderWin(nullptr)
    , m_bDirectPresent(false)
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
    // Con SSAA 2x: el canvas es el doble de grande en cada dimension.
    // El margen de 96px se escala tambien por SFML_S para mantener el espacio extra.
    int canvasW = (iWidth  + 96) * SFML_S;
    int canvasH = (iHeight + 96) * SFML_S;
    m_pRenderTex = new sf::RenderTexture();
    if (!m_pRenderTex->create(static_cast<unsigned int>(canvasW),
                              static_cast<unsigned int>(canvasH)))
    {
        delete m_pRenderTex;
        m_pRenderTex = nullptr;
        return false;
    }

    m_pRenderTex->setSmooth(false); // Sin interpolacion - pixelart fiel

    // ---- Fase 8.F: Cargar fuentes del sistema ----
    // Rutas de fuentes de Windows (C:\Windows\Fonts\)
    static const char* s_fontPaths[HB_FONT_COUNT] = {
        "C:\\Windows\\Fonts\\comic.ttf",      // Comic Sans MS
        "C:\\Windows\\Fonts\\cambria.ttc",     // Cambria
        "C:\\Windows\\Fonts\\verdana.ttf",     // Verdana
        "C:\\Windows\\Fonts\\calibri.ttf",     // Calibri
        "C:\\Windows\\Fonts\\tahoma.ttf",      // Tahoma
        "C:\\Windows\\Fonts\\times.ttf"        // Times New Roman
    };

    m_bFontsLoaded = false;
    for (int i = 0; i < HB_FONT_COUNT; i++)
    {
        if (m_fonts[i].loadFromFile(s_fontPaths[i]))
        {
            m_bFontsLoaded = true; // Al menos una fuente cargo

            // Fase 8.G: Pre-cargar glifos ASCII (32-126) para evitar
            // lag en el primer frame con texto. SFML construye el atlas
            // de glifos on-demand, lo que puede causar un freeze de varios
            // segundos la primera vez (el servidor kickea por timeout).
            for (unsigned int ch = 32; ch < 127; ch++)
                m_fonts[i].getGlyph(ch, s_fontSizes[i],
                    (s_fontStyles[i] & sf::Text::Bold) != 0);
        }
    }

    m_bInitialized = true;

    // Fase 12 REVERTIDA: El enfoque de ventana hijo STATIC es incompatible
    // con cnc-ddraw porque Windows borra la ventana STATIC automaticamente
    // (WM_ERASEBKGND) causando parpadeo, y el OpenGL del padre pinta encima.
    // Se usa BlitRenderTextureToDDraw con mejora de color incorporada


    // ---- Fase 8.L: Cargar shader de escala de grises ----
    static const char* s_grayscaleFragSrc =
        "uniform sampler2D texture;\n"
        "void main() {\n"
        "    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);\n"
        "    float gray = dot(pixel.rgb, vec3(0.299, 0.587, 0.114));\n"
        "    gl_FragColor = vec4(gray, gray, gray, pixel.a) * gl_Color;\n"
        "}\n";

    if (sf::Shader::isAvailable()) {
        m_pGrayscaleShader = new sf::Shader();
        if (m_pGrayscaleShader->loadFromMemory(s_grayscaleFragSrc, sf::Shader::Fragment)) {
            m_bGrayscaleShaderLoaded = true;
        }
        else {
            delete m_pGrayscaleShader;
            m_pGrayscaleShader = nullptr;
        }
    }

    return true;
}

void RenderBackend_SFML::Shutdown()
{
    // ---- Fase 12: Limpiar ventana de presentacion SFML ----

    if (m_pRenderTex)
    {
        delete m_pRenderTex;
        m_pRenderTex = nullptr;
    }
    m_bInitialized = false;
    if (m_pGrayscaleShader) { delete m_pGrayscaleShader; m_pGrayscaleShader = nullptr; }
    m_bGrayscaleShaderLoaded = false;
}

void RenderBackend_SFML::BeginFrame()
{
    if (!m_bInitialized) return;

    // Activar contexto SFML y mantenerlo activo durante todo el frame.
    // cnc-ddraw usa shadow buffers CPU para BltFast (no usa OpenGL entre frames).
    // OpenGL de cnc-ddraw solo se necesita en iFlip() que ocurre DESPUES de EndFrame().
    m_pRenderTex->setActive(true);
    m_pRenderTex->clear(sf::Color(0, 0, 0, 0)); // alpha=0 = transparente (centinela)
    m_bFrameActive = true;
    m_iCropX = 0; // Reset crop cada frame
    m_iCropY = 0;
}

void RenderBackend_SFML::EndFrame()
{
    if (!m_bInitialized) return;

    // Guardia: si BeginFrame() no fue llamado primero, salir sin hacer nada.
    // Esto evita crasheos si EndFrame llega en un estado inesperado.
    if (!m_bFrameActive) return;

    // Asegurar que el contexto esta activo antes de display()
    m_pRenderTex->setActive(true);

    // Sellar el RenderTexture para que su textura sea legible
    m_pRenderTex->display();

    // Blit al backbuffer DDraw con mejora de color incorporada
    BlitRenderTextureToDDraw();

    // Liberar contexto OpenGL para que cnc-ddraw pueda usarlo en iFlip()
    m_bFrameActive = false;
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
    spr.setPosition(static_cast<float>(iDstX * SFML_S), static_cast<float>(iDstY * SFML_S));
    spr.setScale(SFML_SF, SFML_SF);
    m_pRenderTex->draw(spr);
}

// ============================================================
// Fase 8.H: Dibujo de sprites con color/alpha (transparencia, tintado)
// ============================================================

void RenderBackend_SFML::DrawSpriteColor(int iDstX, int iDstY,
    int iSrcX, int iSrcY, int iSrcW, int iSrcH,
    int iSpriteIndex,
    int iR, int iG, int iB, int iA)
{
    if (!m_bInitialized) return;
    if (!m_bFrameActive) return;
    if (m_mapTextures.find(iSpriteIndex) == m_mapTextures.end()) return;

    sf::Sprite spr(m_mapTextures.at(iSpriteIndex));
    spr.setTextureRect(sf::IntRect(iSrcX, iSrcY, iSrcW, iSrcH));
    spr.setPosition(static_cast<float>(iDstX * SFML_S), static_cast<float>(iDstY * SFML_S));
    spr.setScale(SFML_SF, SFML_SF);

    // Clamp RGBA 0-255 y aplicar color/alpha al sprite
    sf::Uint8 r = static_cast<sf::Uint8>((iR < 0) ? 0 : ((iR > 255) ? 255 : iR));
    sf::Uint8 g = static_cast<sf::Uint8>((iG < 0) ? 0 : ((iG > 255) ? 255 : iG));
    sf::Uint8 b = static_cast<sf::Uint8>((iB < 0) ? 0 : ((iB > 255) ? 255 : iB));
    sf::Uint8 a = static_cast<sf::Uint8>((iA < 0) ? 0 : ((iA > 255) ? 255 : iA));
    spr.setColor(sf::Color(r, g, b, a));

    m_pRenderTex->draw(spr); // sf::BlendAlpha por defecto
}

// ============================================================
// Fase 8.I: Sombra proyectada con sf::Transform
// ============================================================

void RenderBackend_SFML::DrawSpriteShadow(int iSrcX, int iSrcY, int iSrcW, int iSrcH,
    int iSpriteIndex,
    float fDstX, float fDstY,
    float fShearX, float fScaleY)
{
    if (!m_bInitialized || !m_bFrameActive || !m_pRenderTex) return;
    if (m_mapTextures.find(iSpriteIndex) == m_mapTextures.end()) return;

    sf::Sprite spr(m_mapTextures.at(iSpriteIndex));
    spr.setTextureRect(sf::IntRect(iSrcX, iSrcY, iSrcW, iSrcH));
    // Negro con 75% opacidad = oscurece destino a ~25% (equivale a >>2 del DDraw)
    spr.setColor(sf::Color(0, 0, 0, 192));

    // Matriz de transformacion para paralelogramo (escalada x SFML_S para 2x SSAA):
    // En espacio 2x: x' = 2*u + 2*fShearX*v + 2*fDstX
    //                y' = 2*fScaleY*v + 2*fDstY
    sf::Transform shadowTransform(
        SFML_SF,  SFML_SF * fShearX,  SFML_SF * fDstX,
        0.0f,     SFML_SF * fScaleY,  SFML_SF * fDstY,
        0.0f,     0.0f,               1.0f
    );

    m_pRenderTex->draw(spr, sf::RenderStates(shadowTransform));
}

// ============================================================
// Fase 8.K: Sprite escalado con color/alpha
// ============================================================

void RenderBackend_SFML::DrawSpriteScaled(int iDstX, int iDstY,
    int iSrcX, int iSrcY, int iSrcW, int iSrcH,
    int iSpriteIndex,
    float fScaleX, float fScaleY,
    int iR, int iG, int iB, int iA)
{
    if (!m_bInitialized) return;
    if (!m_bFrameActive) return;
    if (m_mapTextures.find(iSpriteIndex) == m_mapTextures.end()) return;

    sf::Sprite spr(m_mapTextures.at(iSpriteIndex));
    spr.setTextureRect(sf::IntRect(iSrcX, iSrcY, iSrcW, iSrcH));
    spr.setPosition(static_cast<float>(iDstX * SFML_S), static_cast<float>(iDstY * SFML_S));
    spr.setScale(fScaleX * SFML_SF, fScaleY * SFML_SF); // escala pedida + factor SSAA

    // Clamp RGBA 0-255
    sf::Uint8 r = static_cast<sf::Uint8>((iR < 0) ? 0 : ((iR > 255) ? 255 : iR));
    sf::Uint8 g = static_cast<sf::Uint8>((iG < 0) ? 0 : ((iG > 255) ? 255 : iG));
    sf::Uint8 b = static_cast<sf::Uint8>((iB < 0) ? 0 : ((iB > 255) ? 255 : iB));
    sf::Uint8 a = static_cast<sf::Uint8>((iA < 0) ? 0 : ((iA > 255) ? 255 : iA));
    spr.setColor(sf::Color(r, g, b, a));

    m_pRenderTex->draw(spr);
}

// ============================================================
// Fase 8.L: Sprite en escala de grises (shader GLSL)
// ============================================================

void RenderBackend_SFML::DrawSpriteGrayscale(int iDstX, int iDstY,
    int iSrcX, int iSrcY, int iSrcW, int iSrcH,
    int iSpriteIndex)
{
    if (!m_bInitialized || !m_bFrameActive || !m_pRenderTex) return;
    if (m_mapTextures.find(iSpriteIndex) == m_mapTextures.end()) return;

    sf::Sprite spr(m_mapTextures.at(iSpriteIndex));
    spr.setTextureRect(sf::IntRect(iSrcX, iSrcY, iSrcW, iSrcH));
    spr.setPosition(static_cast<float>(iDstX * SFML_S), static_cast<float>(iDstY * SFML_S));
    spr.setScale(SFML_SF, SFML_SF);

    if (m_bGrayscaleShaderLoaded && m_pGrayscaleShader) {
        m_pGrayscaleShader->setUniform("texture", sf::Shader::CurrentTexture);
        m_pRenderTex->draw(spr, sf::RenderStates(m_pGrayscaleShader));
    }
    else {
        // Fallback sin shader: dibujar normal (mejor que nada)
        m_pRenderTex->draw(spr);
    }
}

// ============================================================
// Fase 9.A: Rectangulo relleno con color RGBA
// ============================================================

void RenderBackend_SFML::DrawFilledRect(int iX, int iY, int iW, int iH,
    int iR, int iG, int iB, int iA)
{
    if (!m_bInitialized || !m_bFrameActive || !m_pRenderTex) return;

    sf::RectangleShape rect(sf::Vector2f(static_cast<float>(iW * SFML_S), static_cast<float>(iH * SFML_S)));
    rect.setPosition(static_cast<float>(iX * SFML_S), static_cast<float>(iY * SFML_S));

    sf::Uint8 r = static_cast<sf::Uint8>((iR < 0) ? 0 : ((iR > 255) ? 255 : iR));
    sf::Uint8 g = static_cast<sf::Uint8>((iG < 0) ? 0 : ((iG > 255) ? 255 : iG));
    sf::Uint8 b = static_cast<sf::Uint8>((iB < 0) ? 0 : ((iB > 255) ? 255 : iB));
    sf::Uint8 a = static_cast<sf::Uint8>((iA < 0) ? 0 : ((iA > 255) ? 255 : iA));
    rect.setFillColor(sf::Color(r, g, b, a));

    m_pRenderTex->draw(rect);
}

// ============================================================
// Fase 10: Rectangulo con borde (outline) sin relleno
// ============================================================
void RenderBackend_SFML::DrawOutlineRect(int iX, int iY, int iW, int iH,
    int iThickness, int iR, int iG, int iB, int iA)
{
    if (!m_bInitialized || !m_bFrameActive || !m_pRenderTex) return;

    sf::RectangleShape rect(sf::Vector2f(static_cast<float>(iW * SFML_S), static_cast<float>(iH * SFML_S)));
    rect.setPosition(static_cast<float>(iX * SFML_S), static_cast<float>(iY * SFML_S));
    rect.setFillColor(sf::Color::Transparent);

    sf::Uint8 r = static_cast<sf::Uint8>((iR < 0) ? 0 : ((iR > 255) ? 255 : iR));
    sf::Uint8 g = static_cast<sf::Uint8>((iG < 0) ? 0 : ((iG > 255) ? 255 : iG));
    sf::Uint8 b = static_cast<sf::Uint8>((iB < 0) ? 0 : ((iB > 255) ? 255 : iB));
    sf::Uint8 a = static_cast<sf::Uint8>((iA < 0) ? 0 : ((iA > 255) ? 255 : iA));
    rect.setOutlineColor(sf::Color(r, g, b, a));
    rect.setOutlineThickness(static_cast<float>(iThickness * SFML_S));

    m_pRenderTex->draw(rect);
}

// ============================================================
// Fase 10: Circulo relleno con color RGBA
// ============================================================
void RenderBackend_SFML::DrawFilledCircle(int iCenterX, int iCenterY, int iRadius,
    int iR, int iG, int iB, int iA)
{
    if (!m_bInitialized || !m_bFrameActive || !m_pRenderTex) return;
    if (iRadius <= 0) return;

    sf::CircleShape circle(static_cast<float>(iRadius * SFML_S));
    circle.setPosition(static_cast<float>((iCenterX - iRadius) * SFML_S),
                       static_cast<float>((iCenterY - iRadius) * SFML_S));

    sf::Uint8 r = static_cast<sf::Uint8>((iR < 0) ? 0 : ((iR > 255) ? 255 : iR));
    sf::Uint8 g = static_cast<sf::Uint8>((iG < 0) ? 0 : ((iG > 255) ? 255 : iG));
    sf::Uint8 b = static_cast<sf::Uint8>((iB < 0) ? 0 : ((iB > 255) ? 255 : iB));
    sf::Uint8 a = static_cast<sf::Uint8>((iA < 0) ? 0 : ((iA > 255) ? 255 : iA));
    circle.setFillColor(sf::Color(r, g, b, a));

    m_pRenderTex->draw(circle);
}

// ============================================================
// Fase 10: Circulo con borde (outline)
// ============================================================
void RenderBackend_SFML::DrawOutlineCircle(int iCenterX, int iCenterY, int iRadius,
    int iThickness, int iR, int iG, int iB, int iA)
{
    if (!m_bInitialized || !m_bFrameActive || !m_pRenderTex) return;
    if (iRadius <= 0) return;

    sf::CircleShape circle(static_cast<float>(iRadius * SFML_S));
    circle.setPosition(static_cast<float>((iCenterX - iRadius) * SFML_S),
                       static_cast<float>((iCenterY - iRadius) * SFML_S));
    circle.setFillColor(sf::Color::Transparent);

    sf::Uint8 r = static_cast<sf::Uint8>((iR < 0) ? 0 : ((iR > 255) ? 255 : iR));
    sf::Uint8 g = static_cast<sf::Uint8>((iG < 0) ? 0 : ((iG > 255) ? 255 : iG));
    sf::Uint8 b = static_cast<sf::Uint8>((iB < 0) ? 0 : ((iB > 255) ? 255 : iB));
    sf::Uint8 a = static_cast<sf::Uint8>((iA < 0) ? 0 : ((iA > 255) ? 255 : iA));
    circle.setOutlineColor(sf::Color(r, g, b, a));
    circle.setOutlineThickness(static_cast<float>(iThickness * SFML_S));

    m_pRenderTex->draw(circle);
}

// ============================================================
// Stub para tiles (Fase 8.G)
// ============================================================

void RenderBackend_SFML::DrawTile(int iDstX, int iDstY,
    int iSrcX, int iSrcY,
    int iSrcW, int iSrcH,
    int iMapIndex)
{
    // FASE 8.G: migrar DrawBackground aqui
    (void)iDstX; (void)iDstY;
    (void)iSrcX; (void)iSrcY;
    (void)iSrcW; (void)iSrcH;
    (void)iMapIndex;
}

// ============================================================
// Fase 8.F: Texto SFML con fuentes del sistema
// ============================================================

void RenderBackend_SFML::DrawText(int iX, int iY,
    const char* szText,
    unsigned long dwColor,
    int iFontId,
    int iEffect)
{
    if (!m_bInitialized || !m_bFontsLoaded) return;
    if (!szText || !szText[0]) return;
    if (iFontId < 0 || iFontId >= HB_FONT_COUNT) return;
    if (!m_bFrameActive || !m_pRenderTex) return;

    // Verificar que la fuente cargo (getInfo().family esta vacio si fallo)
    if (m_fonts[iFontId].getInfo().family.empty()) return;

    // Configuracion de la fuente (charSize * SFML_S para canvas 2x)
    unsigned int charSize = s_fontSizes[iFontId] * static_cast<unsigned int>(SFML_S);
    sf::Uint32   style    = s_fontStyles[iFontId];

    // Color SFML desde COLORREF (Windows BGR -> SFML RGB)
    sf::Color color(GetRValue(dwColor), GetGValue(dwColor), GetBValue(dwColor));

    sf::Text text;
    text.setFont(m_fonts[iFontId]);
    text.setString(szText);
    text.setCharacterSize(charSize);
    text.setStyle(style);
    text.setFillColor(color);
    text.setPosition(static_cast<float>(iX * SFML_S), static_cast<float>(iY * SFML_S));

    // Efectos de texto
    if (iEffect == HB_TEXT_SHADOW)
    {
        // Sombra: desplazada SFML_S px (equivalente a 1px en espacio de pantalla)
        sf::Text shadow(text);
        shadow.setFillColor(sf::Color::Black);
        shadow.setPosition(static_cast<float>(iX * SFML_S + SFML_S), static_cast<float>(iY * SFML_S + SFML_S));
        if (shadow.getOutlineThickness() != 0.f) shadow.setOutlineThickness(0.f);
        m_pRenderTex->draw(shadow);
    }
    else if (iEffect == HB_TEXT_OUTLINE4)
    {
        // Outline via sf::Text::setOutlineThickness (escalado a SSAA 2x)
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(static_cast<float>(SFML_S));
    }

    m_pRenderTex->draw(text);
}

void RenderBackend_SFML::DrawTextCentered(int iX1, int iX2, int iY,
    const char* szText,
    unsigned long dwColor,
    int iFontId)
{
    if (!m_bInitialized || !m_bFontsLoaded) return;
    if (!szText || !szText[0]) return;
    if (iFontId < 0 || iFontId >= HB_FONT_COUNT) return;
    if (!m_bFrameActive || !m_pRenderTex) return;
    if (m_fonts[iFontId].getInfo().family.empty()) return;

    // Crear texto temporal para medir su ancho (en espacio 2x canvas)
    unsigned int charSize = s_fontSizes[iFontId] * static_cast<unsigned int>(SFML_S);
    sf::Uint32   style    = s_fontStyles[iFontId];

    sf::Text text;
    text.setFont(m_fonts[iFontId]);
    text.setString(szText);
    text.setCharacterSize(charSize);
    text.setStyle(style);

    // Calcular posicion centrada en espacio 2x: iX1, iX2 escalados por SFML_S
    float textWidth   = text.getLocalBounds().width;
    float regionWidth = static_cast<float>((iX2 - iX1) * SFML_S);
    float centeredX   = static_cast<float>(iX1 * SFML_S) + (regionWidth - textWidth) / 2.0f;

    sf::Color color(GetRValue(dwColor), GetGValue(dwColor), GetBValue(dwColor));
    text.setFillColor(color);
    text.setPosition(centeredX, static_cast<float>(iY * SFML_S));

    m_pRenderTex->draw(text);
}

// ============================================================
// Fase 8.G: Medir ancho de texto en pixels
// ============================================================

int RenderBackend_SFML::MeasureTextWidth(const char* szText, int iFontId)
{
    if (!m_bFontsLoaded || !szText || !szText[0]) return 0;
    if (iFontId < 0 || iFontId >= HB_FONT_COUNT) return 0;
    if (m_fonts[iFontId].getInfo().family.empty()) return 0;

    sf::Text text;
    text.setFont(m_fonts[iFontId]);
    text.setString(szText);
    text.setCharacterSize(s_fontSizes[iFontId]);
    text.setStyle(s_fontStyles[iFontId]);

    return static_cast<int>(text.getLocalBounds().width + 0.5f);
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
    tex.setSmooth(true); // Bilinear para suavizar bordes al escalar 2x

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
    // Clamp a 0 para evitar lecturas fuera del buffer SFML (offsetX puede ser negativo)
    if (cropX < 0) cropX = 0;
    if (cropY < 0) cropY = 0;
    // Con canvas SFML_S x, el crop tambien debe estar en espacio SFML_S x
    m_iCropX = cropX * SFML_S;
    m_iCropY = cropY * SFML_S;
}

// ============================================================
// Blit del RenderTexture SFML al backbuffer DDraw
// Fase 8.E: Optimizado con uint32 reads, alpha sentinel, row pointers
// ============================================================

void RenderBackend_SFML::BlitRenderTextureToDDraw()
{
    // Garantizar que el contexto OpenGL esta activo ANTES de copyToImage().
    // display() puede desactivar el contexto internamente en SFML 2.6,
    // y copyToImage() necesita el contexto activo para leer la GPU.
    if (m_pRenderTex) m_pRenderTex->setActive(true);

    // GPU -> CPU readback
    sf::Image img = m_pRenderTex->getTexture().copyToImage();

    const sf::Uint8* pSrc = img.getPixelsPtr();
    if (!pSrc) return;

    // Lock del backbuffer DDraw (una vez por frame)
    DDSURFACEDESC2 ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);

    if (!m_DDraw.m_lpBackB4) return;

    HRESULT hr = m_DDraw.m_lpBackB4->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr);
    if (FAILED(hr)) return;

    unsigned short* pDst = reinterpret_cast<unsigned short*>(ddsd.lpSurface);
    int iPitch = ddsd.lPitch / 2; // pitch en shorts (16-bit por pixel)
    int imgW = static_cast<int>(img.getSize().x);

    // ---- HD Scaling: blit con box-filter 2x SSAA ----
    // El canvas SFML es 2x en cada dimension. Para cada pixel destino (x,y)
    // leemos los 4 pixeles fuente correspondientes (2x+cx, 2y+cy) a (2x+1+cx, 2y+1+cy)
    // y los promediamos -> anti-aliasing 2x en sprites SFML.
    int imgH = static_cast<int>(img.getSize().y);
    int iW = min(m_DDraw.res_x, (imgW  - m_iCropX) / SFML_S);
    int iH = min(m_DDraw.res_y, (imgH  - m_iCropY) / SFML_S);
    if (iW <= 0 || iH <= 0)
    {
        m_DDraw.m_lpBackB4->Unlock(nullptr);
        return;
    }

    for (int y = 0; y < iH; y++)
    {
        // Filas fuente: cada fila destino cubre SFML_S filas en el canvas 2x
        int srcY0 = SFML_S * y + m_iCropY;
        int srcY1 = srcY0 + 1;
        if (srcY1 >= imgH) srcY1 = imgH - 1; // Safety: nunca leer fuera del buffer

        const uint32_t* pSrcRow0 = reinterpret_cast<const uint32_t*>(
            pSrc + (srcY0 * imgW + m_iCropX) * 4);
        const uint32_t* pSrcRow1 = reinterpret_cast<const uint32_t*>(
            pSrc + (srcY1 * imgW + m_iCropX) * 4);
        unsigned short* pDstRow = pDst + y * iPitch;

        for (int x = 0; x < iW; x++)
        {
            int sx = SFML_S * x;
            uint32_t p00 = pSrcRow0[sx];

            int ri, gi, bi;
            if (SFML_S >= 2)
            {
                // Box filter 2x2: promediar los 4 pixeles fuente para anti-aliasing
                uint32_t p10 = pSrcRow0[sx + 1];
                uint32_t p01 = pSrcRow1[sx];
                uint32_t p11 = pSrcRow1[sx + 1];
                if (!((p00 | p10 | p01 | p11) & 0xFF000000)) continue;
                ri = static_cast<int>( (p00        & 0xFF) + (p10        & 0xFF)
                                     + (p01        & 0xFF) + (p11        & 0xFF)) >> 2;
                gi = static_cast<int>(((p00 >>  8) & 0xFF) + ((p10 >>  8) & 0xFF)
                                     +((p01 >>  8) & 0xFF) + ((p11 >>  8) & 0xFF)) >> 2;
                bi = static_cast<int>(((p00 >> 16) & 0xFF) + ((p10 >> 16) & 0xFF)
                                     +((p01 >> 16) & 0xFF) + ((p11 >> 16) & 0xFF)) >> 2;
            }
            else
            {
                // SFML_S=1: pixel unico, sin promediar (1:1 mapping)
                if (!(p00 & 0xFF000000)) continue;
                ri = static_cast<int>(p00        & 0xFF);
                gi = static_cast<int>((p00 >>  8) & 0xFF);
                bi = static_cast<int>((p00 >> 16) & 0xFF);
            }

            // Sin modificacion de color: respetar colores originales DDraw
            float r = ri / 255.0f;
            float g = gi / 255.0f;
            float b = bi / 255.0f;

            sf::Uint8 R = static_cast<sf::Uint8>(r * 255.0f);
            sf::Uint8 G = static_cast<sf::Uint8>(g * 255.0f);
            sf::Uint8 B = static_cast<sf::Uint8>(b * 255.0f);
            pDstRow[x] = static_cast<unsigned short>(
                ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3));
        }
    }

    m_DDraw.m_lpBackB4->Unlock(nullptr);
}
