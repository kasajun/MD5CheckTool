#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __ITASKBARLIST3_H_F18A8B02_6BD6_46e6_A78E_712FF474DBD5__
#define __ITASKBARLIST3_H_F18A8B02_6BD6_46e6_A78E_712FF474DBD5__

#include "targetver.h"
#include <windows.h>

#if _MSC_VER > 1500
#include <ShlObj.h>
#define ITASKBARLIST3 ITaskbarList3
#else
typedef /* [v1_enum] */
enum TBPFLAG
{
    TBPF_NOPROGRESS = 0,
    TBPF_INDETERMINATE = 0x1,
    TBPF_NORMAL = 0x2,
    TBPF_ERROR = 0x4,
    TBPF_PAUSED = 0x8
} 	TBPFLAG;
#if defined(_M_X64)
#define ITASKBARLIST3 unsigned __int64
#else
#define ITASKBARLIST3 unsigned int
#endif
#endif

#if __cplusplus && _MSC_VER < 1500
extern "C" {
#endif

HRESULT WINAPI iTaskbarList3_CoCreateInstance(ITASKBARLIST3** pTask3);
ULONG WINAPI iTaskbarList3_Release(ITASKBARLIST3* pTask3);
HRESULT WINAPI iTaskbarList3_SetProgressState(ITASKBARLIST3* pTask3, HWND hWnd, size_t tbpFlags);
HRESULT WINAPI iTaskbarList3_SetProgressValue(ITASKBARLIST3* pTask3, HWND hWnd, size_t nCompleted);
HRESULT WINAPI iTaskbarList3_SetOverlayIcon(ITASKBARLIST3* pTask3, HWND hWnd, HICON hIcon, LPCWSTR pszDescription);
HRESULT WINAPI iTaskbarList3_DeleteOverlayIcon(ITASKBARLIST3* pTask3, HWND hWnd);

#if __cplusplus && _MSC_VER < 1500
}
#endif
#endif
