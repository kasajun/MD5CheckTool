#if _MSC_VER > 1000
#pragma once
#endif

// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#ifndef __STDAFX_H_54CED900_BDFE_4f3f_AA99_38F2C874CAD7__
#define __STDAFX_H_54CED900_BDFE_4f3f_AA99_38F2C874CAD7__

#if _MSC_VER >= 1400
# define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES			1
# define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT		1
#endif

#ifdef _DEBUG
# include <crtdbg.h>
# define _CRTDBG_MAP_ALLOC
#endif

//#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include "..\shared\targetver.h"
#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>

#if _MSC_VER > 1500
#include <dwmapi.h>
#endif

#include <shobjidl.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <shlobj.h>
#include <commdlg.h>
#include <wincrypt.h>
#include <mmsystem.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>
#include <process.h>
#include <errno.h>
#include <cassert>


// 追加の ヘッダー ファイル
#include "..\shared\c20.h"
#include "..\shared\charlib.h"
#include "..\shared\charlib_base64.h"
#include "..\shared\charlib_url.h"
#include "..\shared\charlib_sysdll.h"
#include "..\shared\ToolTip.h"
#include "..\shared\cpuid.h"
#include "..\shared\getopt.h"
#include "..\shared\os.h"
#include "..\shared\os_full.h"
#include "..\shared\IniFilePath.h"
#include "..\shared\clipboard.h"
#include "..\shared\iTaskbarList3.h"
#include "..\shared\iFileDialog.h"
#include "..\shared\BinaryView.h"
#include "Main.h"
#include "aboutdlg.h"
#include "OptionDlg.h"
#include "FileListDlg.h"
#include "hashthread.h"
#include "DebugDlg.h"


// 追加の LIB ファイル
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "version.lib")
#if _MSC_VER >= 1600
# pragma comment(lib, "uxtheme.lib")
#endif
#if _MSC_VER >= 1800
# pragma comment(lib, "Dwmapi.lib")
#endif


// <CommCtrl.h>
#ifndef PBS_MARQUEE
# define PBS_MARQUEE			0x08
#endif // _WIN32_WINNT >= 0x0501

#ifndef PBM_SETMARQUEE
# define PBM_SETMARQUEE			(WM_USER+10)
#endif // _WIN32_WINNT >= 0x0501

#ifndef WM_THEMECHANGED
# define WM_THEMECHANGED		0x031A
#endif /* _WIN32_WINNT >= 0x0501 */

#ifndef WM_DPICHANGED
# define WM_DPICHANGED			0x02E0
#endif /* WINVER >= 0x0601 */

#endif
