// IniFilePath.cpp
//

#include "targetver.h"
#include "IniFilePath.h"
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include "charlib.h"
#include "charlib_sysdll.h"
#if _MSC_VER < 1700
#include "os.h"
#endif

// このコード モジュールに含まれる関数の宣言を転送します:


int GetIniFilePath(TCHAR* pInPath, const TCHAR* cpInINIFileName, const TCHAR* cpInAppName)
{
	TCHAR szFilePath[MAX_PATH_SIZE] = _T("");
	TCHAR* ptr = NULL;
	UINT nRet = 0;
	HRESULT hRet;

#if _MSC_VER < 1500
	if (IsWin2kOrGreater())
	{
		HMODULE hDll = NULL;
		const TCHAR* szFile = _T("SHELL32.DLL");
#ifdef _UNICODE
		const char* szFunc = "SHGetFolderPathW";
#else
		const char* szFunc = "SHGetFolderPathA";
#endif

		hDll = LoadSystemLibraryToTChar(szFile);
		if (hDll)
		{
			HRESULT(WINAPI* SHGetFolderPath)(HWND, int, HANDLE, DWORD, LPTSTR);

			SHGetFolderPath = (HRESULT(WINAPI*)(HWND, int, HANDLE, DWORD, LPTSTR))(GetProcAddress(hDll, szFunc));
			if (SHGetFolderPath == NULL) {
				goto FUNC_END;
			}
#endif /* _MSC_VER < 1500 */

			if (GetModuleFileName(NULL, pInPath, MAX_PATH_SIZE) == 0) {
				goto FUNC_END;
			}

			hRet = SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, szFilePath);
			if (SUCCEEDED(hRet) && szFilePath != NULL)
			{
				//  Program File から実行されているか確認。
				ptr = _tcsstr(pInPath, szFilePath);

				//  実行されていたら Appdata のパスにする。
				if (ptr == NULL) {
					goto FUNC_END;
				}

				hRet = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szFilePath);
				if (SUCCEEDED(hRet) && szFilePath != NULL)
				{
					// pInPath に Appdata のパスをコピーする。
					ptr = qtcscpy(pInPath, szFilePath);
					*ptr++ = '\\';

					// pInPath に cpInAppName のパスを追記する。
					ptr = qtcscpy(ptr, cpInAppName);
					*ptr++ = '\\';
					*ptr = '\0';

					// cpInAppName のフォルダーがない場合は作成する。
					if (PathIsDirectory(pInPath) == FALSE) {
						CreateDirectory(pInPath, NULL);
					}

					// 最後に cpInINIFileName を追記してINIファイルのパスが完成。
					qtcscpy(ptr, cpInINIFileName);
					nRet = TRUE;
				}
			}
FUNC_END:
			;
#if _MSC_VER < 1500
		}
		FreeLibrary(hDll);
	}
#endif /* _MSC_VER < 1500 */

	if (nRet == FALSE && GetModuleFileName(NULL, szFilePath, MAX_PATH_SIZE) != FALSE)
	{
		PathRemoveFileSpec(szFilePath);
		ptr = qtcscpy(pInPath, szFilePath);
		*ptr++ = '\\';
		qtcscpy(ptr, cpInINIFileName);
	}
	return TRUE;
}
