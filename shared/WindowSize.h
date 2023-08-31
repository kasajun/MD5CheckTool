
#if _MSC_VER > 1000
#pragma once
#endif

// WindowSize.h
//
//
// 以下のサイトのコードを参考にしました。
//
// □ウィンドウサイズの復元
//
//   http://oldworldgarage.web.fc2.com/programing/tip0006_RestoreWindow.html
//

#ifndef __WINDOWSIZE_H_22FD446D_ED33_4579_A9D0_56E5B30E4FAC__
#define __WINDOWSIZE_H_22FD446D_ED33_4579_A9D0_56E5B30E4FAC__

#include "charlib.h"
#include "charlib_sysdll.h"
#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL WindowSize_SaveState(HWND hWnd, LPCTSTR cpInWindowName, LPCTSTR cpInIniFile);
BOOL WindowSize_LoadState(HWND hWnd, LPCTSTR cpInWindowName, LPCTSTR cpInIniFile);

#ifdef __cplusplus
}
#endif
#endif
