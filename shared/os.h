#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __OS_H_26E127AA_78A8_4f28_984F_E3C9DA59C653__
#define __OS_H_26E127AA_78A8_4f28_984F_E3C9DA59C653__

#include "targetver.h"
#include <windows.h>

#if _MSC_VER > 1500
#include <dwmapi.h>
#if _MSC_VER > 1600
#include <Uxtheme.h>
#endif
#else
typedef long NTSTATUS;
#ifndef ETDT_ENABLETAB
#define ETDT_DISABLE                    0x00000001
#define ETDT_ENABLE                     0x00000002
#define ETDT_USETABTEXTURE              0x00000004
#define ETDT_ENABLETAB                  (ETDT_ENABLE | ETDT_USETABTEXTURE)
#endif
enum DWMWINDOWATTRIBUTE
{
    DWMWA_NCRENDERING_ENABLED = 1,      // [get] Is non-client rendering enabled/disabled
    DWMWA_NCRENDERING_POLICY,           // [set] Non-client rendering policy
    DWMWA_TRANSITIONS_FORCEDISABLED,    // [set] Potentially enable/forcibly disable transitions
    DWMWA_ALLOW_NCPAINT,                // [set] Allow contents rendered in the non-client area to be visible on the DWM-drawn frame.
    DWMWA_CAPTION_BUTTON_BOUNDS,        // [get] Bounds of the caption button area in window-relative space.
    DWMWA_NONCLIENT_RTL_LAYOUT,         // [set] Is non-client content RTL mirrored
    DWMWA_FORCE_ICONIC_REPRESENTATION,  // [set] Force this window to display iconic thumbnails.
    DWMWA_FLIP3D_POLICY,                // [set] Designates how Flip3D will treat the window.
    DWMWA_EXTENDED_FRAME_BOUNDS,        // [get] Gets the extended frame bounds rectangle in screen space
    DWMWA_HAS_ICONIC_BITMAP,            // [set] Indicates an available bitmap when there is no better thumbnail representation.
    DWMWA_DISALLOW_PEEK,                // [set] Don't invoke Peek on the window.
    DWMWA_EXCLUDED_FROM_PEEK,           // [set] LivePreview exclusion information
    DWMWA_CLOAK,                        // [set] Cloak or uncloak the window
    DWMWA_CLOAKED,                      // [get] Gets the cloaked state of the window
    DWMWA_FREEZE_REPRESENTATION,        // [set] Force this window to freeze the thumbnail without live update
    DWMWA_LAST
};
#endif

#ifndef LOAD_LIBRARY_SEARCH_SYSTEM32
#define LOAD_LIBRARY_SEARCH_SYSTEM32        0x00000800
#endif

#include <stdio.h>
#include <tchar.h>
#include "charlib.h"
#include "charlib_sysdll.h"

#define _OS_UA_SIZE				96
#define GetVerifyVersionInfo	IsWindowsVersionOrGreater
#define IsWin81OrGreater		IsWin8Point1OrGreater

#ifdef  __cplusplus
extern "C" {
#endif

HRESULT SetEnableThemeDialogTexture(HWND hWnd);
BOOL IsWindowsVersionOrGreater(const WORD wMajorVersion, const WORD wMinorVersion, const WORD wServicePackMajor);
UINT RtlGetOSVersion(OSVERSIONINFOEX* osVerInfo);
BOOL GetGokanMode(OSVERSIONINFOEX* osVerInfo);
BOOL GetUserAgentName(TCHAR* pInString, size_t nSize);

BOOL IsWin95();
BOOL IsWin98();
BOOL IsWin98SE();
BOOL IsWinME();
BOOL IsWin9X();
BOOL IsWin2kOrGreater();
BOOL IsWinXPOrGreater();
BOOL IsWS03OrGreater();
BOOL IsWinVistaOrGreater();
BOOL IsWin7OrGreater();
BOOL IsWin8OrGreater();
BOOL IsWin8Point1OrGreater();
BOOL IsWin10OrGreater();
BOOL IsUxTheme();
UINT GetOSVersion(OSVERSIONINFOEX* osVerInfo);
BOOL GetAearoWindowRect(HWND hWnd, RECT* lpRect);

#ifdef  __cplusplus
}
#endif
#endif
