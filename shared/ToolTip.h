#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __TOOLTIP_26EF41C4_4166_4a49_98BB_4DC43BDE2C27__
#define __TOOLTIP_26EF41C4_4166_4a49_98BB_4DC43BDE2C27__

#include "targetver.h"
#include <windows.h>
#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

LRESULT WINAPI SetToolTip(const HWND hTool, const HWND hWnd, const TCHAR* cpInString);
LRESULT WINAPI SetToolTipText(const HWND hTool, const HWND hWnd, const TCHAR* cpInString);

#ifdef __cplusplus
}
#endif
#endif
