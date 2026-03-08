// ============================================================
// RenderBackend_DDraw.cpp
// Implementación de IRenderBackend usando DXC_ddraw existente
// FASE 6: Delega al m_DDraw de CGame por referencia
// ============================================================

#include "RenderBackend_DDraw.h"

// ------------------------------------------------------------
// Constructor / Destructor
// ------------------------------------------------------------

RenderBackend_DDraw::RenderBackend_DDraw(DXC_ddraw& ddraw)
    : m_DDraw(ddraw)
    , m_iWidth(800)
    , m_iHeight(600)
{
}

RenderBackend_DDraw::~RenderBackend_DDraw()
{
    // NO llamamos Shutdown() aquí porque m_DDraw le pertenece a CGame
    // CGame es responsable de su propio ciclo de vida
}

// ------------------------------------------------------------
// Init / Shutdown
// ------------------------------------------------------------

bool RenderBackend_DDraw::Init(HWND hWnd, int iWidth, int iHeight, bool bFullscreen)
{
    // FASE 6: CGame ya llama m_DDraw.bInit() directamente antes
    // de que se use este backend. Este método existe para cumplir
    // la interfaz y preparar el estado interno.
    m_iWidth  = iWidth;
    m_iHeight = iHeight;
    return true;
}

void RenderBackend_DDraw::Shutdown()
{
    // FASE 6: No-op. El shutdown de DDraw lo maneja CGame.
}

// ------------------------------------------------------------
// Control de frame
// ------------------------------------------------------------

void RenderBackend_DDraw::BeginFrame()
{
    // Delega al ClearBackB4 del DDraw existente
    m_DDraw.ClearBackB4();
}

void RenderBackend_DDraw::EndFrame()
{
    // FASE 6: No-op.
    // iFlip() tiene manejo de DDERR_SURFACELOST en Game.cpp:
    //   if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
    // Mover esa lógica es tarea de Fase 7.
    // Por ahora Game.cpp sigue llamando iFlip() directamente.
}

// ------------------------------------------------------------
// Dibujo — stubs (implementación real en Fase 7)
// ------------------------------------------------------------

void RenderBackend_DDraw::DrawSprite(int iDstX, int iDstY,
    int iSrcX, int iSrcY,
    int iSrcW, int iSrcH,
    int iSpriteIndex)
{
    // FASE 7: migrar PutSpriteFast* aquí
}

void RenderBackend_DDraw::DrawTile(int iDstX, int iDstY,
    int iSrcX, int iSrcY,
    int iSrcW, int iSrcH,
    int iMapIndex)
{
    // FASE 7: migrar DrawBackground aquí
}

void RenderBackend_DDraw::DrawText(int iX, int iY,
    const char* szText,
    unsigned long dwColor,
    int iFontId,
    int iEffect)
{
    // Fase 8.F stub: DDraw backend no implementa texto SFML
    // El texto GDI sigue funcionando directamente via DXC_ddraw
    (void)iX; (void)iY; (void)szText; (void)dwColor;
    (void)iFontId; (void)iEffect;
}

void RenderBackend_DDraw::DrawTextCentered(int iX1, int iX2, int iY,
    const char* szText,
    unsigned long dwColor,
    int iFontId)
{
    // Fase 8.F stub: DDraw backend no implementa texto SFML
    (void)iX1; (void)iX2; (void)iY; (void)szText;
    (void)dwColor; (void)iFontId;
}

// ------------------------------------------------------------
// Resolución
// ------------------------------------------------------------

void RenderBackend_DDraw::SetResolution(int iWidth, int iHeight)
{
    m_iWidth  = iWidth;
    m_iHeight = iHeight;
}

int RenderBackend_DDraw::GetWidth()
{
    return m_iWidth;
}

int RenderBackend_DDraw::GetHeight()
{
    return m_iHeight;
}

// ------------------------------------------------------------
// Clear
// ------------------------------------------------------------

void RenderBackend_DDraw::Clear()
{
    // Alias de BeginFrame para compatibilidad con IRenderBackend
    m_DDraw.ClearBackB4();
}
