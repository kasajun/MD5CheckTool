#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __IFILEDIALOG_H_B93CD180_1A88_40fb_8EDB_013B96E5FDB9__
#define __IFILEDIALOG_H_B93CD180_1A88_40fb_8EDB_013B96E5FDB9__

#include "targetver.h"
#include <windows.h>
#include <tchar.h>

#if _MSC_VER > 1500
#define IFILEDIALOG IFileOpenDialog
#define SHELLITEM IShellItem
#else
#define IFILEDIALOG DWORD
#define SHELLITEM DWORD
#endif

#ifndef MAX_PATH_SIZE
#ifdef _UNICODE
#define MAX_PATH_SIZE	((size_t)_MAX_PATH * 4)
#else
#define MAX_PATH_SIZE	((size_t)_MAX_PATH * 2)
#endif
#endif

#if __cplusplus
extern "C" {
#endif

BOOL iFileOpenFolder(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle);
BOOL iFileOpenFolder2(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle);

#if __cplusplus
}
#endif
#endif
