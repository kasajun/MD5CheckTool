#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CLIPBOARD_H_F0C0B15E_3934_44a6_AFA4_AE378C117C8D__
#define __CLIPBOARD_H_F0C0B15E_3934_44a6_AFA4_AE378C117C8D__

#include "targetver.h"
#include <windows.h>

#ifdef  __cplusplus
extern "C" {
#endif

BOOL SetClipboardTextA(HWND hWnd, const char* pText);
BOOL SetClipboardText2A(HWND hWnd, const char* pText, const char* pRtfText, const char* pHtmlText);

BOOL SetClipboardTextW(HWND hWnd, const WCHAR* pText);
BOOL SetClipboardText2W(HWND hWnd, const WCHAR* pText, const char* pRtfText, const char* pHtmlText);

DWORD GetClipboardTextA(HWND hWnd, char* pText, size_t size);
DWORD GetClipboardTextW(HWND hWnd, WCHAR* pText, size_t size);

char* GetClipboardMallocTextA(HWND hWnd);
WCHAR* GetClipboardMallocTextW(HWND hWnd);

#ifdef UNICODE
#define GetClipboardText GetClipboardTextW
#define GetClipboardText2 GetClipboardText2W
#define SetClipboardText SetClipboardTextW
#define SetClipboardText2 SetClipboardText2W
#define GetClipboardMallocText GetClipboardMallocTextW
#else
#define GetClipboardText GetClipboardTextA
#define GetClipboardText2 GetClipboardText2A
#define SetClipboardText SetClipboardTextA
#define SetClipboardText2 SetClipboardText2A
#define GetClipboardMallocText GetClipboardMallocTextA
#endif // !UNICODE

#ifdef  __cplusplus
}
#endif
#endif
