#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include "fps_limiter.h"
#include "config.h"
#include "dd.h"
#include "render_d3d9.h"
#include "render_gdi.h"
#include "render_ogl.h"
#include "hook.h"
#include "debug.h"
#include "dllmain.h"
#include "versionhelpers.h"

#define CNC_REG_PATH "Software\\cnc-ddraw"

static void cfg_init();
static BOOL cfg_get_bool(LPCSTR key, BOOL default_value);
static int cfg_get_int(LPCSTR key, int default_value);
static DWORD cfg_get_string(LPCSTR key, LPCSTR default_value, LPSTR out_string, DWORD out_size);

#define GET_INT(a,b,c) a = cfg_get_int(b, c); TRACE("%s=%d\n", b, a)
#define GET_BOOL(a,b,c) a = cfg_get_bool(b, c); TRACE("%s=%s\n", b, a ? "true" : "false")
#define GET_STRING(a,b,c,d) cfg_get_string(a, b, c, d); TRACE("%s=%s\n", a, c)

CNCDDRAWCONFIG g_config =
{ .window_rect = {.left = -32000, .top = -32000, .right = 0, .bottom = 0 }, .window_state = -1, .upscaled_state = -1 };

// ------------------------------------------------------------------
// INIT
// ------------------------------------------------------------------
static void cfg_init()
{
    // Obtener nombre del ejecutable
    if (GetModuleFileNameA(NULL, g_config.game_path, sizeof(g_config.game_path) - 1) > 0)
    {
        _splitpath(g_config.game_path, NULL, NULL, g_config.process_file_name, g_config.process_file_ext);
    }

    // Crear clave base si no existe
    HKEY hKey;
    DWORD disp;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &hKey, &disp) == ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
    }

    TRACE("Config path -> HKEY_CURRENT_USER\\%s\n", CNC_REG_PATH);
}

// ------------------------------------------------------------------
// LECTURA
// ------------------------------------------------------------------
static DWORD cfg_get_string(LPCSTR key, LPCSTR default_value, LPSTR out, DWORD size)
{
    HKEY hKey;
    DWORD type = REG_SZ;
    DWORD len = size;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueExA(hKey, key, NULL, &type, (LPBYTE)out, &len) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return len;
        }
        RegCloseKey(hKey);
    }

    strncpy(out, default_value, size);
    return strlen(out);
}

static BOOL cfg_get_bool(LPCSTR key, BOOL default_value)
{
    HKEY hKey;
    DWORD val = default_value;
    DWORD size = sizeof(DWORD);
    DWORD type = REG_DWORD;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueExA(hKey, key, NULL, &type, (LPBYTE)&val, &size) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return val ? TRUE : FALSE;
        }
        RegCloseKey(hKey);
    }

    return default_value;
}

static int cfg_get_int(LPCSTR key, int default_value)
{
    HKEY hKey;
    DWORD val = default_value;
    DWORD size = sizeof(DWORD);
    DWORD type = REG_DWORD;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueExA(hKey, key, NULL, &type, (LPBYTE)&val, &size) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return (int)val;
        }
        RegCloseKey(hKey);
    }

    return default_value;
}

// ------------------------------------------------------------------
// GUARDAR
// ------------------------------------------------------------------
void cfg_save()
{
    if (!g_config.save_settings)
        return;

    // No guardar mientras está maximizado en macOS
    if (IsMacOS() && !g_config.window_rect.left && !g_config.window_rect.top)
        return;

    HKEY hKey;
    DWORD disp;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, CNC_REG_PATH, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &disp) != ERROR_SUCCESS)
        return;

    if (g_config.window_rect.right)
        RegSetValueExA(hKey, "width", 0, REG_DWORD, (BYTE*)&g_config.window_rect.right, sizeof(DWORD));

    if (g_config.window_rect.bottom)
        RegSetValueExA(hKey, "height", 0, REG_DWORD, (BYTE*)&g_config.window_rect.bottom, sizeof(DWORD));

    if (g_config.window_rect.left != -32000)
        RegSetValueExA(hKey, "posX", 0, REG_DWORD, (BYTE*)&g_config.window_rect.left, sizeof(DWORD));

    if (g_config.window_rect.top != -32000)
        RegSetValueExA(hKey, "posY", 0, REG_DWORD, (BYTE*)&g_config.window_rect.top, sizeof(DWORD));

    if (g_config.window_state != -1)
    {
        DWORD val = g_config.window_state ? 1 : 0;
        RegSetValueExA(hKey, "windowed", 0, REG_DWORD, (BYTE*)&val, sizeof(DWORD));
    }

    if (g_config.upscaled_state != -1)
    {
        DWORD val = g_config.upscaled_state ? 1 : 0;
        RegSetValueExA(hKey, "fullscreen", 0, REG_DWORD, (BYTE*)&val, sizeof(DWORD));
    }

    RegCloseKey(hKey);
}

// ------------------------------------------------------------------
// CARGAR
// ------------------------------------------------------------------
void cfg_load()
{
    cfg_init();

    /* Optional settings */
    GET_INT(g_config.window_rect.right, "width", 0);
    GET_INT(g_config.window_rect.bottom, "height", 0);
    GET_BOOL(g_config.fullscreen, "fullscreen", FALSE);
    GET_BOOL(g_config.windowed, "windowed", FALSE);
    GET_BOOL(g_config.maintas, "maintas", FALSE);
    GET_STRING("aspect_ratio", "", g_config.aspect_ratio, sizeof(g_config.aspect_ratio));
    GET_BOOL(g_config.boxing, "boxing", FALSE);
    GET_INT(g_config.maxfps, "maxfps", -1);
    GET_BOOL(g_config.vsync, "vsync", FALSE);
    GET_BOOL(g_config.adjmouse, "adjmouse", TRUE);
    GET_STRING("shader", "Shaders\\interpolation\\catmull-rom-bilinear.glsl", g_config.shader, sizeof(g_config.shader));
    GET_INT(g_config.window_rect.left, "posX", -32000);
    GET_INT(g_config.window_rect.top, "posY", -32000);
    GET_STRING("renderer", "auto", g_config.renderer, sizeof(g_config.renderer));
    GET_BOOL(g_config.devmode, "devmode", FALSE);
    GET_BOOL(g_config.border, "border", TRUE);
    GET_INT(g_config.save_settings, "savesettings", 1);
    GET_BOOL(g_config.resizable, "resizable", TRUE);
    GET_INT(g_config.d3d9_filter, "d3d9_filter", FILTER_CUBIC);
    GET_INT(g_config.anti_aliased_fonts_min_size, "anti_aliased_fonts_min_size", 13);
    GET_INT(g_config.min_font_size, "min_font_size", 0);
    GET_INT(g_config.center_window, "center_window", CENTER_WINDOW_AUTO);
    GET_STRING("inject_resolution", "", g_config.inject_resolution, sizeof(g_config.inject_resolution));
    GET_BOOL(g_config.vhack, "vhack", FALSE);
    GET_STRING("screenshotdir", ".\\Screenshots\\", g_config.screenshot_dir, sizeof(g_config.screenshot_dir));
    GET_BOOL(g_config.toggle_borderless, "toggle_borderless", FALSE);
    GET_BOOL(g_config.toggle_upscaled, "toggle_upscaled", FALSE);

    /* Compatibility settings */
    GET_BOOL(g_config.noactivateapp, "noactivateapp", FALSE);
    GET_INT(g_config.maxgameticks, "maxgameticks", 0);
    GET_INT(g_config.limiter_type, "limiter_type", LIMIT_AUTO);
    GET_INT(g_config.minfps, "minfps", 0);
    GET_BOOL(g_config.nonexclusive, "nonexclusive", TRUE);
    GET_BOOL(g_config.singlecpu, "singlecpu", TRUE);
    GET_INT(g_config.resolutions, "resolutions", RESLIST_NORMAL);
    GET_INT(g_config.fixchilds, "fixchilds", FIX_CHILDS_DETECT_PAINT);
    GET_BOOL(g_config.hook_peekmessage, "hook_peekmessage", FALSE);

    /* Undocumented settings */
    GET_BOOL(g_config.fix_alt_key_stuck, "fix_alt_key_stuck", FALSE);
    GET_BOOL(GameHandlesClose, "game_handles_close", FALSE);
    GET_BOOL(g_config.fix_not_responding, "fix_not_responding", FALSE);
    GET_BOOL(g_config.no_compat_warning, "no_compat_warning", FALSE);
    GET_INT(g_config.guard_lines, "guard_lines", 200);
    GET_INT(g_config.max_resolutions, "max_resolutions", 0);
    GET_BOOL(g_config.lock_surfaces, "lock_surfaces", FALSE);
    GET_BOOL(g_config.flipclear, "flipclear", FALSE);
    GET_BOOL(g_config.rgb555, "rgb555", FALSE);
    GET_BOOL(g_config.no_dinput_hook, "no_dinput_hook", FALSE);
    GET_BOOL(g_config.center_cursor_fix, "center_cursor_fix", FALSE);
    GET_STRING("fake_mode", "", g_config.fake_mode, sizeof(g_config.fake_mode));
    GET_BOOL(g_config.lock_mouse_top_left, "lock_mouse_top_left", FALSE);
    GET_STRING("win_version", "", g_config.win_version, sizeof(g_config.win_version));
    GET_INT(g_config.hook, "hook", 4);
    GET_BOOL(g_config.limit_gdi_handles, "limit_gdi_handles", FALSE);
    GET_BOOL(g_config.remove_menu, "remove_menu", FALSE);
    GET_INT(g_config.refresh_rate, "refresh_rate", 0);
    GET_BOOL(g_config.terminate_process, "terminate_process", FALSE);

    /* Hotkeys */
    GET_INT(g_config.hotkeys.toggle_fullscreen, "keytogglefullscreen", VK_RETURN);
    GET_INT(g_config.hotkeys.toggle_fullscreen2, "keytogglefullscreen2", 0);
    GET_INT(g_config.hotkeys.toggle_maximize, "keytogglemaximize", VK_NEXT);
    GET_INT(g_config.hotkeys.toggle_maximize2, "keytogglemaximize2", 0);
    GET_INT(g_config.hotkeys.unlock_cursor1, "keyunlockcursor1", VK_TAB);
    GET_INT(g_config.hotkeys.unlock_cursor2, "keyunlockcursor2", VK_RCONTROL);
    GET_INT(g_config.hotkeys.screenshot, "keyscreenshot", VK_SNAPSHOT);

    /* Game specific settings */
    GET_BOOL(g_config.armadahack, "armadahack", FALSE);
    GET_BOOL(g_config.tshack, "tshack", FALSE);
    GET_BOOL(g_config.infantryhack, "infantryhack", FALSE);
    GET_BOOL(g_config.stronghold_hack, "stronghold_hack", FALSE);
    GET_BOOL(g_config.mgs_hack, "mgs_hack", FALSE);
    GET_BOOL(g_config.tlc_hack, "tlc_hack", FALSE);
    GET_BOOL(g_config.carma95_hack, "carma95_hack", FALSE);
    GET_BOOL(g_config.sirtech_hack, "sirtech_hack", FALSE);
    GET_BOOL(g_config.flightsim98_hack, "flightsim98_hack", FALSE);
    GET_BOOL(g_config.darkcolony_hack, "darkcolony_hack", FALSE);

    GameHandlesClose = GameHandlesClose || g_config.infantryhack;

    if (g_config.lock_mouse_top_left)
        g_config.adjmouse = FALSE;

    if (g_config.aspect_ratio[0])
        g_config.maintas = TRUE;
}
