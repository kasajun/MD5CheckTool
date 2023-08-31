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

BOOL SetClipboardTextA(HWND hWnd, const char* pText, const char* pRtfText = NULL, const char* pHtmlText = NULL);
BOOL SetClipboardTextW(HWND hWnd, const WCHAR* pText, const char* pRtfText = NULL, const char* pHtmlText = NULL);
DWORD GetClipboardTextA(HWND hWnd, char* pText, size_t size);
DWORD GetClipboardTextW(HWND hWnd, WCHAR* pText, size_t size);
char* GetClipboardMallocTextA(HWND hWnd);
WCHAR* GetClipboardMallocTextW(HWND hWnd);

#ifdef  __cplusplus
}
#endif

#ifdef UNICODE
#define GetClipboardText GetClipboardTextW
#define GetClipboardMallocText GetClipboardMallocTextW
#define SetClipboardText SetClipboardTextW
#else
#define GetClipboardText GetClipboardTextA
#define GetClipboardMallocText GetClipboardMallocTextA
#define SetClipboardText SetClipboardTextA
#endif // !UNICODE
#endif
