# MAPEADO TOTAL DEL PROYECTO HELBREATH CURSED

Documento técnico generado a partir del análisis del código real del repositorio.  
**Ruta raíz:** `e:\Juego Workana\Helbreath nuevo`

---

## A) Resumen ejecutivo

### Qué es este proyecto
- **Tipo:** Cliente/servidor de juego MMORPG 2D (estilo Helbreath clásico).
- **Nombre en solución:** "Helbreath Cursed" (cliente) y "CoreServer" (servidor de mapas/login).
- **Origen:** Cliente basado en Helbreath (1998 Soph), con múltiples extensiones: resolución variable, eventos (Heldenian, CVC, Battle Royale, etc.), sistema de gremios, quests, mercado, voz, etc.

### Qué compila
| Artefacto | Proyecto | Salida |
|----------|----------|--------|
| **Cliente** | Game (`[SRC]Client Version 13\Client.vcxproj`) | `[SRC]Client Version 13\Archive\Helbreath Cursed.exe` (Win32 Release/Desbug) |
| **Cliente x64** | Mismo proyecto, config x64 | `Archive\Helbreath Argentina.exe` |
| **Servidor de mapas** | CoreServer (`[SRC]MapServer\MapServer.vcxproj`) | `[SRC]MapServer\Release\` o `..\Server\` (Debug) — ejecutable del CoreServer |
| **Fuera de la solución** | Launcher 1, UpdateLauncher, cnc-ddraw, Server (datos), zlib | Launcher/UpdateLauncher son VB.NET; cnc-ddraw es C; Server es carpeta de datos/runtime |

### Librerías y APIs usadas
- **Cliente:** Win32 API, **DirectDraw 7** (ddraw, uso de `IDirectDraw7` / `IDirectDrawSurface7`), DirectInput, DirectSound, Winsock2, GDI para texto/fuentes. Opcional: **cnc-ddraw** (wrapper que puede redirigir a OpenGL/D3D9/GDI) vía **ddraw embebida en memoria** (`EmbeddedDDraw`, `MemoryModule`).
- **Resolución:** 640×480, 800×600, 1024×768 (definidas en `Resolution\Resolution.h/cpp` y en `Wmain.cpp` registro cnc-ddraw).
- **Servidor:** Winsock2, Windows API, SQL (headers presentes). Sin DirectX.

### Cómo se ejecuta
1. **Cliente:** Abrir `Helbreath Cursed.sln` → compilar proyecto **Game** (Release Win32). El exe se genera en `[SRC]Client Version 13\Archive\`. Ejecutar desde esa carpeta (necesita `Archive\SPRITES`, `Archive\DATA`, `Archive\MAPDATA`, etc.). Opcionalmente se usa un **Launcher** que pasa resolución y renderer (ddraw/dx9/ogl/gdi).
2. **Servidor:** Compilar **CoreServer**; ejecutar el exe desde la carpeta que contenga configs y bases de datos (por ejemplo `Server\`). Entrada: `main()` en `[SRC]MapServer\main.cpp`.

---

## B) Árbol del proyecto (estructura, hasta 3 niveles)

```
Helbreath nuevo/
├── back 2013/                    # Copia de respaldo del proyecto (misma estructura)
├── cnc-ddraw-master/             # Wrapper DirectDraw → OpenGL/D3D9/GDI (para juegos legacy)
│   ├── .github/
│   ├── bin/
│   ├── config/
│   ├── inc/
│   └── src/
├── Launcher 1/                   # Launcher del juego (VB.NET)
│   └── Launcher/
├── Plans/                        # Documentación/planes (no código compilado)
├── Server/                       # Datos de runtime del servidor (DB, Configs, MapData, Events, Logs)
│   ├── Bans/
│   ├── Configs/
│   ├── Database/
│   ├── Events/
│   ├── General/
│   ├── Guild/
│   ├── Logs/
│   └── MapData/
├── Shared/                       # Código compartido cliente/servidor (solo headers en la sln)
├── UpdateLauncher/               # Actualizador (VB.NET)
│   └── UpdateLauncher/
├── zlib/                         # Librería zlib (compresión)
├── [SRC]Client Version 13/      # ★ CÓDIGO FUENTE DEL CLIENTE
│   ├── Archive/                  # Recursos y salida del exe (SPRITES, DATA, MAPDATA, SOUNDS, etc.)
│   ├── classes/                  # Lógica de juego, ventana, sprites, red, etc.
│   ├── CxImage/                  # Librería de imágenes (JPEG, etc.)
│   ├── Game/                     # Salida intermedia (x64, etc.)
│   ├── General/                  # IDs (ActionID, SpriteID, GlobalDef, NPC_Config)
│   ├── header/                   # (minúscula) posible duplicado/legacy
│   ├── Headers/                  # Headers del cliente (Game, Sprite, XSocket, etc.)
│   ├── jpeg/                     # Código libjpeg
│   ├── Language/                 # Textos e idiomas
│   ├── Library/                  # Sonido (YWSound), headers DDRAW/DINPUT/DSOUND
│   ├── Libs/                     # Librerías estáticas (.lib) para Desbug
│   ├── Motor/                    # ★ Motor gráfico y entrada (DXC_ddraw, DXC_dinput, DXC_dsound)
│   ├── Release/                  # Objetos/out Release
│   ├── Resolution/               # Resolución 640/800/1024
│   ├── Ressources/               # resource.h, recursos RC
│   ├── zlib/                     # zlib dentro del cliente
│   ├── Client.vcxproj
│   ├── EmbeddedDDraw.cpp/h       # Carga ddraw desde recurso embebido
│   ├── MemoryModule.c            # Carga DLL desde memoria
│   └── ...
├── [SRC]MapServer/               # ★ CÓDIGO FUENTE DEL SERVIDOR (CoreServer)
│   ├── Configs/
│   ├── Debug/
│   ├── General/                  # UserMessages, etc.
│   ├── Header/                   # Headers del servidor (Game.h, winmain.h)
│   ├── language/
│   ├── Libs/
│   ├── Source/                   # Fuentes del servidor (Wmain.cpp, etc.)
│   ├── main.cpp                  # ★ Entry point del CoreServer
│   └── MapServer.vcxproj
├── Helbreath Cursed.sln
├── Helbreath Cursed.sdf
├── hb_relay_v2.cpp               # Utilidad relay (fuera de la sln)
└── .gitignore
```

**Descripción por carpeta raíz:**
- **back 2013:** Respaldo; no modificar para desarrollo.
- **cnc-ddraw-master:** Compatibilidad con ventana moderna, OpenGL/D3D9; el cliente puede usar una ddraw embebida (no necesariamente esta carpeta en runtime).
- **Launcher 1 / UpdateLauncher:** Herramientas de arranque y parches (VB.NET).
- **Plans:** Documentación.
- **Server:** Datos en tiempo de ejecución del servidor (cuentas, personajes, mapas, eventos, logs).
- **Shared:** Headers compartidos (`shared.h`, `netmsg.h`, `Random.h`).
- **zlib:** Compresión; enlazada por el cliente (zlibstat.lib).
- **[SRC]Client Version 13:** Todo el código y recursos del cliente (entry WinMain, game loop, render, recursos).
- **[SRC]MapServer:** Código del CoreServer (entry main(), sockets, lógica de mapa/login).

---

## C) Mapeo por carpeta (muy detallado)

### Raíz del repositorio
- **Propósito:** Contiene la solución Visual Studio, el cliente, el MapServer y carpetas de soporte.
- **Archivos clave:**
  - `Helbreath Cursed.sln`: solución VS (proyectos Game y CoreServer; Shared como solución de ítems).
  - `hb_relay_v2.cpp`: utilidad relay independiente (`main()`); no forma parte de la sln.

---

### `[SRC]Client Version 13\`
- **Propósito:** Código fuente completo del cliente del juego.
- **Submódulos:** Archive (recursos), classes, General, Headers, Motor, Resolution, Library, Language, CxImage, jpeg, zlib, Ressources.
- **Archivos clave:**

| Archivo | Rol | Dependencias | Render/Recursos |
|---------|-----|--------------|-----------------|
| `Client.vcxproj` | Proyecto MSBuild del cliente (Game). Define configs Release/Desbug, Win32/x64, libs (zlib, search, ddraw, DINPUT, DSOUND, cximage, jpeg, etc.). | — | — |
| `EmbeddedDDraw.cpp` / `.h` | Carga la DLL ddraw desde un recurso embebido (para usar cnc-ddraw en memoria). | `MemoryModule.c`, recursos | No |
| `MemoryModule.c` | Carga DLL desde memoria (usado para ddraw embebida). | Win32 API | No |
| `ClientUDP.cpp` / `.h` | Cliente UDP (ping, mensajes auxiliares). | Winsock | No |

---

### `[SRC]Client Version 13\classes\`
- **Propósito:** Implementación de la lógica de juego, ventana, red, sprites y dibujo de pantallas.
- **Archivos clave:**

| Archivo | Rol | Dependencias | Render/Recursos |
|---------|-----|--------------|-----------------|
| `Wmain.cpp` | **Entry point del cliente.** `WinMain` → CreateDefaultConfig, Init, LoadEmbeddedDDraw, creación de `CGame`, registro de ventana, `Initialize`, `EventLoop`. Timer 100 ms → `OnTimer`. `EventLoop` llama a `G_pGame->UpdateScreen()` o `UpdateScreen_OnLoading`. | Game.h, Resolution, EmbeddedDDraw, Event, winmain.h | Indirecto (dispara el loop que dibuja) |
| `Game.cpp` | **Núcleo del juego.** `CGame`: init, modos de pantalla, `UpdateScreen` (switch por `m_cGameMode`), `UpdateScreen_OnGame`, `UpdateScreen_OnMainMenu`, `UpdateScreen_OnLoading`, etc. Dibujo de mundo, objetos, UI, diálogos. `MakeSprite`/`MakeTileSpr` cargan sprites/tiles. | Headers/Game.h, Motor/DXC_ddraw, DXC_dinput, Sprite, MapData, XSocket, etc. | **SÍ:** m_DDraw, PutString, DrawBackground, DrawObjects, todos los UpdateScreen_On* |
| `Helbreath.cpp` | Funciones adicionales del cliente (por revisar en detalle en el repo). | Game, Headers | Posible render |
| `Sprite.cpp` | **CSprite:** dibujado de sprites sobre superficies DDraw (`PutSpriteFast`, `PutSpriteFastDst`, `PutSpriteFastNoColorKeyDst`, transparencias, sombras). Carga desde PAK/DIB. | DXC_ddraw.h, Sprite.h, Mydib, colors | **SÍ:** todo el dibujo de sprites |
| `Sprite2.cpp` | Segunda implementación o variante de sprites. | DXC_ddraw2.h | **SÍ** |
| `Tile.cpp` / `TileSpr.cpp` | Tiles de mapa (TileSpr usado en DrawBackground con `m_pTileSpr`). | MapData, DDraw, Sprite | **SÍ** |
| `MapData.cpp` | Datos de mapa (celdas, atributos). | Headers/MapData.h | Datos para render |
| `Mydib.cpp` | Manipulación de DIB para sprites. | Headers/Mydib.h | **SÍ** (origen de pixels) |
| `XSocket.cpp` / `XSocketUDP.cpp` | Sockets TCP y UDP. | Headers/XSocket.h | No |
| `Effect.cpp`, `Msg.cpp`, `Item.cpp`, `Magic.cpp`, `Skill.cpp`, `CharInfo.cpp`, `BuildItem.cpp`, `ItemName.cpp`, `Curse.cpp`, `GameMonitor.cpp`, `MouseInterface.cpp`, `Misc.cpp`, `StrTok.cpp`, `SoundBuffer.cpp`, `AntiSpeedHack.cpp` | Lógica de efectos, mensajes, ítems, magia, habilidades, personajes, construcción, nombres de ítems, maldiciones, monitor, mouse, utilidades, sonido, anti-trampas. | Game.h, Headers varios | Algunos (Effect, Draw*) tocan render |

---

### `[SRC]Client Version 13\Motor\`
- **Propósito:** Motor de gráficos (DirectDraw), entrada (DirectInput) y sonido (DirectSound).
- **Archivos clave:**

| Archivo | Rol | Dependencias | Render/Recursos |
|---------|-----|--------------|-----------------|
| `DXC_ddraw.h` | **Clase DXC_ddraw:** interfaz de render. Superficies: `m_lpFrontB4`, `m_lpBackB4`, `m_lpPDBGS` (pre-draw background). Resolución `res_x`, `res_y`. Métodos: `ClearBackB4`, `iFlip`, `TextOut`, `DrawText`, `PutPixel`, `DrawShadowBox`, `_GetBackBufferDC`/`_ReleaseBackBufferDC`, `Screenshot`, tablas de transparencia (`m_lTransG100`, etc.). | ddraw.h, Misc.h | **SÍ:** núcleo del render |
| `DXC_ddraw.cpp` | Implementación: creación de DDraw, superficies, cambio de resolución, Clear, Flip, DC para GDI, color key, pixel format. Usa `EmbeddedDDraw` para obtener `DirectDrawCreateEx`. | Resolution, Game.h, EmbeddedDDraw.h | **SÍ** |
| `DXC_ddraw_d3d9.cpp` | Backend opcional D3D9 (comentado o condicional en headers). | D3D9 | **SÍ** si se usa |
| `DXC_dinput.cpp` | Entrada teclado/ratón (DirectInput). | DINPUT | No |
| `DXC_dsound.cpp` | Sonido (DirectSound). | DSOUND | No |

---

### `[SRC]Client Version 13\Headers\`
- **Propósito:** Declaraciones del cliente (juego, sprites, red, ítems, UI, etc.).
- **Archivos clave:** `Game.h` (CGame, m_DDraw, m_pSprite, m_pTileSpr, modos de juego, constantes), `Sprite.h` (CSprite, PutSprite*), `MapData.h`, `XSocket.h`, `Winmain.h` (G_hWnd, G_pGame, etc.), `Mydib.h`, `colors.h`, `Effect.h`, `Item.h`, `Magic.h`, `Skill.h`, `CharInfo.h`, `BuildItem.h`, `ItemName.h`, `MouseInterface.h`, `Msg.h`, `Misc.h`, `SoundBuffer.h`, `Tile.h`, `TileSpr.h`, `Event.h`, `commonds.h`, `AntiSpeedHack.h`, `Curse.h`, `GameMonitor.h`.

---

### `[SRC]Client Version 13\General\`
- **Propósito:** IDs y config global (sprites, acciones, objetos dinámicos, NPCs).
- **Archivos clave:** `SpriteID.h`, `ActionID.h`, `DynamicObjectID.h`, `GlobalDef.h`, `NPC_Config.h`, `AFXRES.H`.

---

### `[SRC]Client Version 13\Resolution\`
- **Propósito:** Resolución del cliente (640×480, 800×600, 1024×768).
- **Archivos clave:** `Resolution.h`, `Resolution.cpp` — clase `cResolution`, `IsResolution()`, `GetWidth()`/`GetHeight()`.

---

### `[SRC]Client Version 13\Archive\`
- **Propósito:** Recursos del juego y directorio de salida del exe. Aquí se generan `Helbreath Cursed.exe` y las carpetas de datos.
- **Subcarpetas (recursos):** CONTENTS, DATA (cuentas/personajes por carpeta), FONTS, MAPDATA, MUSIC, SAVE, Screenshots, Shaders (glsl para cnc-ddraw), SOUNDS, SPRITES.
- **Render/Recursos:** SPRITES (archivos .PAK / imágenes), MAPDATA (mapas), DATA (guardados), Shaders (post-proceso si se usa cnc-ddraw), FONTS, SOUNDS, MUSIC.

---

### `[SRC]Client Version 13\Library\`
- **Propósito:** Sonido (YWSound) y headers de DirectX (DDRAW, DINPUT, DSOUND) y utilidades.
- **Archivos clave:** `YWSound.cpp`/`.h`, `DDRAW.H`, `DDRAW4.h`, `DINPUT.H`, `DSOUND.H`, `d3dtypes.h`, `Cint.h`, `CNTREVNT.H`.

---

### `[SRC]Client Version 13\Ressources\`
- **Propósito:** Recurso de recursos Windows (resource.h, iconos, etc.).
- **Archivos clave:** `resource.h` — incluido desde Wmain.cpp y compilación RC.

---

### `[SRC]Client Version 13\CxImage\` y `jpeg\`
- **Propósito:** Carga y guardado de imágenes (JPEG para screenshots, etc.). CxImage usa jpeg.
- **Render/Recursos:** Sí (imágenes; Game usa CxImage para capturas).

---

### `[SRC]MapServer\`
- **Propósito:** Servidor de mapas y login (CoreServer).
- **Submódulos:** Header, General, Source, language, Libs, Configs, Debug.
- **Archivos clave:**

| Archivo | Rol | Dependencias | Render/Recursos |
|---------|-----|--------------|-----------------|
| `main.cpp` | **Entry point del servidor.** `main()` → InitializeSockets, StartCoreserver(), luego loop de mensajes Windows (PeekMessage/Dispatch). | Header/winmain.h, Header/Game.h, General/UserMessages, Event | No |
| `Source\Wmain.cpp` | `WinMain` alternativo (GUI); el proyecto compila también este archivo pero el entry usado es `main()` en main.cpp. | Header/winmain.h, Game.h | No |
| `Header/Game.h`, `Header/winmain.h` | Definiciones del servidor (cCoreServer, sockets globales, etc.). | — | No |
| `General/UserMessages.h` | Mensajes de red del servidor. | — | No |

---

### `Server\`
- **Propósito:** Datos de ejecución del servidor (no es proyecto de compilación). Contiene bases de datos (Account, Character, Drops, EkQuest, Guild, GuildBank, Market, Quest, MobData), Configs (Items, trades), MapData, Events (CaptureTheFlag, CityVsCity, Crusade, etc.), General, Guild, Logs, Bans.
- **No hay entry point ni build aquí;** el ejecutable del CoreServer se copia/ejecuta desde MapServer o se apunta a esta carpeta como working directory.

---

### `Shared\`
- **Propósito:** Código compartido entre cliente y servidor (en la sln solo se referencian headers).
- **Archivos clave:** `shared.h` (constantes, enums, StatConfig, etc.), `netmsg.h`, `Random.h`.

---

### `cnc-ddraw-master\`
- **Propósito:** Wrapper de DirectDraw que permite redirigir a OpenGL, D3D9 o GDI. El cliente puede cargar una versión embebida de ddraw (no necesariamente desde esta carpeta en runtime).
- **Archivos clave:** `config/cnc-ddraw config.cpp` (`_tWinMain` para configurador), `src/render_ogl.c`, `src/render_d3d9.c`, `src/render_gdi.c` — threads de render. `src/IDirectDraw`, `src/IDirect3D` — emulación de interfaces.

---

### `Launcher 1\`, `UpdateLauncher\`
- **Propósito:** Launcher y actualizador del juego (VB.NET). No son parte de la solución C++; se compilan por separado.

---

### `zlib\`
- **Propósito:** Librería de compresión; el cliente la enlaza (zlibstat.lib) para descomprimir datos (p. ej. PAK).

---

## D) Puntos de entrada (entry points)

### Cliente (Game)
- **Archivo:** `[SRC]Client Version 13\classes\Wmain.cpp`
- **Función:** `int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)`
- **Flujo de arranque:**
  1. `CreateDefaultConfig()` — escribe en registro `Software\cnc-ddraw` valores por defecto (width, height, fullscreen, shader, etc.).
  2. `Init()` — lee argumentos de línea de comandos (resolución 1/2/3, api ddraw/dx9/ogl/gdi, mode, vsync) y actualiza registro.
  3. `LoadEmbeddedDDraw()` — carga ddraw desde recurso embebido (MemoryModule). Si falla, muestra mensaje y sale.
  4. Creación de `G_pGame = new CGame`; flags de resolución (`b640x480`, etc.) se asignan a `G_pGame`.
  5. Carga de `search.dll` (ofuscada como `rd`qbg-ckk`), comprobación de hook/CRC (`__FindHackingDll__("CRCCHECK")`).
  6. Registro de ventana (`sprintf(szAppClass, "Client-I%d", ...)`), compatibilidad Windows 7/8 (AppCompatFlags en registro).
  7. `InitApplication(hInstance)` — registra clase de ventana con `WndProc`.
  8. `InitInstance(hInstance, nCmdShow)` — crea ventana según resolución (640×480, 800×600, 1024×768), centrada.
  9. `Initialize(lpCmdLine)` — WSAStartup, `G_pGame->bInit(G_hWnd, G_hInstance, pCmdLine)`, tabla de transparencias, timer 100 ms, `G_pGame->InitDialogs()`.
  10. `EventLoop()` — loop principal: mensajes Windows o, si el programa está activo, `G_pGame->UpdateScreen()` (o `UpdateScreen_OnLoading` en modo loading); si no, `WaitMessage()`.

**Responsables de inicialización y loop:**
- Inicialización: `Wmain.cpp` (CreateDefaultConfig, Init, InitApplication, InitInstance, Initialize).
- Inicialización del juego: `CGame::bInit` en `Game.cpp`.
- Loop principal: `EventLoop()` en `Wmain.cpp`; cada iteración llama a `CGame::UpdateScreen()` o `CGame::UpdateScreen_OnLoading(FALSE)`.

---

### Servidor (CoreServer)
- **Archivo:** `[SRC]MapServer\main.cpp`
- **Función:** `int main()`
- **Flujo de arranque:**
  1. `SetPriorityClass` / `SetThreadPriority` (prioridad alta).
  2. `_CrtSetDbgFlag` (detección de fugas en Debug).
  3. `InitializeSockets()` (WSAStartup).
  4. `StartCoreserver()` — inicia login server y lógica del core (según código en main.cpp: lectura de config, puesta en marcha del servidor).
  5. `StartTimer(1)` (copia del timer).
  6. Loop infinito: `PeekMessage` / `TranslateMessage` / `DispatchMessage`; `Sleep(1)`.

**Responsables:** `main.cpp` (main, InitializeSockets, StartCoreserver); el loop de mensajes está en `main()`.

---

### cnc-ddraw (configurador)
- **Archivo:** `cnc-ddraw-master\config\cnc-ddraw config.cpp`
- **Función:** `int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)` — ventana de configuración del wrapper; no es el entry del juego.

---

### hb_relay_v2
- **Archivo:** `hb_relay_v2.cpp` (raíz del repo, fuera de la sln).
- **Función:** `int main()` — utilidad relay independiente.

---

## E) Loop principal y arquitectura (cliente)

- **Dónde está el game loop:** En `EventLoop()` en `[SRC]Client Version 13\classes\Wmain.cpp`. No hay un “while(running)” separado con sleep de frame; el loop es manejado por mensajes y llamadas a `UpdateScreen` cuando el programa está activo.
- **Orden típico por “frame”:**
  1. `PeekMessage` / `GetMessage` → si hay mensaje, `TranslateMessage`, `DispatchMessage`.
  2. Si no hay mensaje y `G_pGame->m_bIsProgramActive`: `G_pGame->UpdateScreen()` (envuelto en `TryFrame` para capturar excepciones).
  3. Si no hay mensaje y modo loading: `G_pGame->UpdateScreen_OnLoading(FALSE)`.
  4. Si no hay mensaje y no activo: `WaitMessage()`.

Dentro de `UpdateScreen()` (en `Game.cpp`):
- Se actualiza `G_dwGlobalTime = timeGetTime()`.
- Switch según `m_cGameMode`: se llama al `UpdateScreen_On*` correspondiente (OnMainMenu, OnGame, OnLoading, OnConnecting, OnSelectCharacter, etc.).

Para **DEF_GAMEMODE_ONMAINGAME** se llama `UpdateScreen_OnGame()`. Ahí se realiza:
- Lógica de juego (input, comandos, red).
- **Dibujo:** Clear backbuffer, dibujo de fondo (tiles), objetos, efectos, UI, texto; luego `m_DDraw.iFlip()`. Si flip devuelve `DDERR_SURFACELOST`, `RestoreSprites()`.

**Sistemas que se “actualizan” por frame (en UpdateScreen_OnGame y helpers):**
- Input: ya procesado en `WndProc` (WM_KEYDOWN, etc.) y en `OnTimer`; en OnGame se usa estado de teclas/ratón.
- Lógica: movimiento, comandos, diálogos (CommandProcessor, etc.).
- Red: eventos de socket vía `WM_USER_GAMESOCKETEVENT` / `OnGameSocketEvent`.
- Render: todo el dibujo en el `UpdateScreen_On*` activo (m_DDraw, m_pSprite, m_pTileSpr).
- Audio: sonidos disparados desde lógica (PlaySound, etc.).

**Funciones clave del loop:**  
`EventLoop` → `UpdateScreen` → `UpdateScreen_OnGame` (o otro On*) → en OnGame: ClearBackB4, DrawBackground, DrawObjects, dibujo de UI, `m_DDraw.iFlip()`.

---

## I) Build / compilación

- **Solución:** `Helbreath Cursed.sln` (Visual Studio 2013 formato 12.00; puede abrirse en versiones posteriores).
- **Proyectos:**
  - **Game:** `[SRC]Client Version 13\Client.vcxproj` — aplicación Windows.
  - **CoreServer:** `[SRC]MapServer\MapServer.vcxproj` — aplicación Windows (consola en main.cpp).
  - **Shared:** carpeta de solución con ítems (shared.h, netmsg.h, Random.h).
- **Configuraciones:**  
  - Cliente: Debug|Win32, Debug|x64, Release|Win32, Release|x64, Desbug|Win32, Desbug|x64. En la sln, muchas configs de Game apuntan a Release|Win32; Desbug es la variante de depuración.  
  - MapServer: Debug|Win32, Release|Win32.
- **Plataformas:** Win32 (x86), x64 (solo Game; salida “Helbreath Argentina.exe”).
- **Dependencias externas (cliente):**
  - Librerías: `zlibstat.lib`, `search.lib`, `ACHEV.LIB`, `wsock32.lib`, `DINPUT.LIB`, `DSOUND.LIB`, `DXGUID.LIB`, `IMM32.LIB`, `winmm.lib`, `Cint.lib`, `cximage.lib`, `Jpeg.lib`, `kernel32.lib`, etc. En Release Win32 no se enlaza `ddraw.lib` explícitamente en la lista mostrada (se usa ddraw embebida); en Desbug y x64 a veces aparece `ddraw.lib`.
  - Carpetas de libs: `..\zlib\release` o `..\zlib\debug`, y `Libs\` para Desbug.
  - Includes: `..\zlib`, y los del proyecto (General, Headers, Motor, Resolution, etc.).
- **Salidas:**  
  - Game Release Win32: `[SRC]Client Version 13\Archive\Helbreath Cursed.exe`.  
  - MapServer Debug: `..\Server\`; MapServer Release: `.\Release\`.
- **Problemas típicos:**  
  - Dependencias faltantes: `search.lib`, `ACHEV.LIB`, `cximage.lib`, `Jpeg.lib`, `zlibstat.lib` deben estar en Libs o en rutas adicionales.  
  - `/FORCE:UNRESOLVED` en el linker puede ocultar referencias no resueltas; conviene revisar que todas las libs existan.  
  - La ddraw “real” es la cargada por EmbeddedDDraw; si no está el recurso o la DLL embebida, el cliente falla al iniciar.

---

## J) Mapa de dependencias (alto nivel)

```
Wmain.cpp
  → CreateDefaultConfig, Init, LoadEmbeddedDDraw
  → G_pGame = new CGame
  → InitApplication, InitInstance (ventana)
  → Initialize → G_pGame->bInit
  → EventLoop → G_pGame->UpdateScreen / UpdateScreen_OnLoading

Game.cpp (CGame)
  → bInit: carga sprites/tiles (MakeSprite, MakeTileSpr), mapa, sonido, etc.
  → UpdateScreen: switch(m_cGameMode) → UpdateScreen_OnGame, UpdateScreen_OnMainMenu, ...
  → UpdateScreen_OnGame: DrawBackground, DrawObjects, UI, m_DDraw.iFlip
  → m_DDraw (DXC_ddraw), m_pSprite[], m_pTileSpr[], m_pMapData, m_pGSock, m_pLSock, m_DInput, m_DSound, etc.

DXC_ddraw (Motor)
  → IDirectDraw7, IDirectDrawSurface7 (FrontB4, BackB4, PDBGS)
  → ClearBackB4, iFlip, TextOut, DrawText, PutPixel, DrawShadowBox, _GetBackBufferDC*
  → EmbeddedDDraw (DirectDrawCreateEx)

CSprite (Sprite.cpp)
  → DXC_ddraw (para obtener superficie destino)
  → PutSpriteFast*, PutSpriteFastDst, PutSpriteFastNoColorKeyDst, etc.
  → Carga desde PAK/DIB (Mydib, frame positions)

MapData / TileSpr
  → Datos de celdas y tiles; DrawBackground usa m_pTileSpr + mapa
```

- **Core:** Wmain (entry + loop), Game (lógica + orquestación de dibujo), DXC_ddraw (superficies y presentación), CSprite/MapData (contenido dibujado).
- **Helpers:** Misc, StrTok, XSocket, Effect, Msg, Item, Magic, Skill, CharInfo, BuildItem, ItemName, MouseInterface, SoundBuffer, Resolution, EmbeddedDDraw, MemoryModule.
- **Legacy / opcional:** DXC_ddraw_d3d9, Sprite2, posible uso de Wmain.cpp en MapServer (WinMain) frente a main.cpp.

---

---

# PARTE 2: Pipeline de render y hotspots

## F) Pipeline de render/dibujo

**Dueño del render:** Clase `DXC_ddraw` (`Motor/DXC_ddraw.h`, `.cpp`). Instancia: `CGame::m_DDraw`.

**Superficies:** `m_lpFrontB4` (frontal), `m_lpBackB4` (backbuffer), `m_lpPDBGS` (Pre-Draw Background: se dibuja aquí el fondo y luego BltFast a BackB4), `m_lpBackB4flip`, `m_pBackB4Addr` (acceso directo a pixels). Resolución: `res_x`, `res_y` según `c_reso->IsResolution()` (C640x480, C800x600, C1024x768).

**Flujo de un frame:**  
1) EventLoop → UpdateScreen → UpdateScreen_OnGame.  
2) UpdateScreen_OnGame: ClearBackB4 (si primer frame), input (m_DInput), cálculo de vista (sPivotX/Y, sDivX/Y, sModX/Y), **DrawBackground(sDivX, sModX, sDivY, sModY)** (tiles en m_lpPDBGS → BltFast a BackB4), **DrawObjects(...)** (entidades con m_pSprite/m_pTileSpr), **DrawDialogBoxs(msX, msY, msZ, cLB)** (UI), **m_DDraw.iFlip()**; si DDERR_SURFACELOST → RestoreSprites().

**Cámara / smooth:** `m_sViewPointX/Y`, `m_fSmoothViewPointX/Y`, `m_fRenderOffsetX/Y`. CalcViewPoint() actualiza vista; DrawBackground (versión con m_bModernMovement) usa offset para suavizado. Scaling a ventana grande lo hace cnc-ddraw (shaders en Archive\Shaders\).

**Funciones clave:** DXC_ddraw::ClearBackB4, iFlip, TextOut, DrawText, PutPixel, DrawShadowBox*; CGame::DrawBackground, DrawObjects, DrawDialogBoxs; CSprite::PutSpriteFast*, PutSpriteFastNoColorKeyDst.

---

## K) Hotspots para refactor/render

**Top archivos:** Game.cpp, DXC_ddraw.cpp, Sprite.cpp, DXC_ddraw.h, Sprite.h, TileSpr.cpp, Helbreath.cpp, Mydib.cpp, Game.h, MapData, Effect.cpp, DXC_ddraw_d3d9.cpp, DDRAW.H, Mydib.h, colors.h, Wmain.cpp, Resolution.cpp, EmbeddedDDraw.cpp, Sprite2.cpp, TileSpr.h/Tile.h.

**Top funciones:** UpdateScreen, UpdateScreen_OnGame, DrawBackground, DrawObjects, ClearBackB4, iFlip, PutSpriteFastNoColorKeyDst/PutSpriteFastDst, DrawObject*, DrawDialogBoxs, CalcViewPoint.

**Riesgos:** Globales (G_pGame, c_reso, G_dwGlobalTime, tablas transparencia); macros (DEF_MAXSPRITES, res_x/res_y); dos versiones de DrawBackground; estado compartido (m_cGameMode, m_sViewPointX/Y); GetDC/ReleaseDC intensivo para texto.

---

# PARTE 3: Recursos gráficos, data y config

## G) Recursos gráficos (sprites/tiles/UI)

**Ubicación:** `[SRC]Client Version 13\Archive\` — SPRITES\ (PAK, imágenes), DATA\ (por usuario), MAPDATA\, FONTS\, Shaders\ (glsl), SOUNDS\, MUSIC\, CONTENTS\, SAVE\, Screenshots\.

**Formatos:** PAK (sprites/tiles), DIB/BMP vía Mydib; CxImage/jpeg para screenshots; .glsl para cnc-ddraw.

**Carga:** MakeSprite(FileName, sStart, sCount, bAlphaEffect, Pak) y MakeTileSpr(...) en Game.cpp asignan a m_pSprite[] y m_pTileSpr[]. CSprite construye desde PAK o archivo; ReadFramePositions para offsets en PAK. RestoreSprites tras DDERR_SURFACELOST.

**Referencia:** Por ID (m_pSprite[DEF_SPRID_...], m_pTileSpr[sSpr]) y por nombre en MakeSprite/MakeTileSpr ("Ym", "maptiles1", "Trees1", "objects1", ...). MapData da índice de tile por celda; DrawBackground usa m_pTileSpr[sSpr]->PutSpriteFastNoColorKeyDst(...).

**Relación:** SPRITES\*.PAK → CSprite (m_pSprite, m_pTileSpr) → Game.cpp dibujo. MAPDATA → CMapData → DrawBackground. Shaders .glsl → cnc-ddraw al presentar.

---

## H) Configuración y data externos

**Configs:** Registro `Software\cnc-ddraw` (width, height, fullscreen, vsync, shader, renderer, etc.). Cliente borra ddraw.ini en Init(). Server\Configs\ (Items, trades). MapServer lee key=value con ReadCfgValue.

**Tablas/datos:** Server\Database\ (Account, Character, Drops, EkQuest, Guild, GuildBank, Market, Quest, MobData), Server\MapData\. Cliente: Archive\MAPDATA\, Archive\DATA\ por usuario. Items/NPCs en headers y Server\Configs\Items.

**Scripts:** No se detectan; lógica en C++.

**Logs/dumps:** Server\Logs\; cliente crashlog.txt; screenshots en Archive\Screenshots\. PAK en SPRITES; zlib para descompresión si aplica.

---

*Fin del mapeado. Basado en código real del repositorio.*
