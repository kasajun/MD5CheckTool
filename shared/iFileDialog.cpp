#include "iFileDialog.h"
#include <ShlObj.h>
#include "charlib.h"
#include "charlib_sysdll.h"
#include "os.h"
#include "c20.h"


#if _MSC_VER <= 1920

#if _MSC_VER <= 1600
extern "C" 
#endif
HRESULT(WINAPI * SHCreateItemFromParsingName_Func)(PCWSTR, IBindCtx*, REFIID, void**) = NULL;
#endif
size_t iFileOpenFolder_Arg = 0;

BOOL iFileOpenFolder_Func0(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle);
BOOL iFileOpenFolder_Func1(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle);
BOOL iFileOpenFolder_Func2(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle);
BOOL iFileOpenFolder_Func3(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle);
int CALLBACK iFileOpenFolder2_CallbackProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#if __cplusplus && _MSC_VER <= 1600
extern "C" {
#endif

HRESULT WINAPI iFileOpenDialog_CoCreateInstance(IFILEDIALOG** pFileDlg);
ULONG WINAPI iFileOpenDialog_Release(IFILEDIALOG* pFileDlg);
HRESULT WINAPI iFileOpenDialog_SetDefaultFolder(IFILEDIALOG* pFileDlg, SHELLITEM* psi);
HRESULT WINAPI iFileOpenDialog_GetFileName(IFILEDIALOG* pFileDlg, LPWSTR** pszName);
HRESULT WINAPI iFileOpenDialog_SetFileName(IFILEDIALOG* pFileDlg, LPCWSTR pszName);
HRESULT WINAPI iFileOpenDialog_GetOptions(IFILEDIALOG* pFileDlg, DWORD* pfos);
HRESULT WINAPI iFileOpenDialog_SetOptions(IFILEDIALOG* pFileDlg, DWORD fos);
HRESULT WINAPI iFileOpenDialog_Show(IFILEDIALOG* pFileDlg, HWND hwndOwner);
HRESULT WINAPI iFileOpenDialog_GetResult(IFILEDIALOG* pFileDlg, SHELLITEM** ppsi);
HRESULT WINAPI ShellItem_GetDisplayName(SHELLITEM* pShellItem, SIGDN sigdnName, LPWSTR* ppszName);
ULONG WINAPI ShellItem_Release(SHELLITEM* pShellItem);
HRESULT WINAPI ShellItem_SHCreateItemFromParsingName(LPWSTR* pszPath, SHELLITEM** ppv);

#if __cplusplus && _MSC_VER <= 1600
}
#endif


#ifndef __IFileDialog_INTERFACE_DEFINED__
#define __IFileDialog_INTERFACE_DEFINED__

/* interface IFileDialog */
/* [unique][object][uuid] */

/* [v1_enum] */
enum _FILEOPENDIALOGOPTIONS
{
	FOS_OVERWRITEPROMPT = 0x2,
	FOS_STRICTFILETYPES = 0x4,
	FOS_NOCHANGEDIR = 0x8,
	FOS_PICKFOLDERS = 0x20,
	FOS_FORCEFILESYSTEM = 0x40,
	FOS_ALLNONSTORAGEITEMS = 0x80,
	FOS_NOVALIDATE = 0x100,
	FOS_ALLOWMULTISELECT = 0x200,
	FOS_PATHMUSTEXIST = 0x800,
	FOS_FILEMUSTEXIST = 0x1000,
	FOS_CREATEPROMPT = 0x2000,
	FOS_SHAREAWARE = 0x4000,
	FOS_NOREADONLYRETURN = 0x8000,
	FOS_NOTESTFILECREATE = 0x10000,
	FOS_HIDEMRUPLACES = 0x20000,
	FOS_HIDEPINNEDPLACES = 0x40000,
	FOS_NODEREFERENCELINKS = 0x100000,
	FOS_OKBUTTONNEEDSINTERACTION = 0x200000,
	FOS_DONTADDTORECENT = 0x2000000,
	FOS_FORCESHOWHIDDEN = 0x10000000,
	FOS_DEFAULTNOMINIMODE = 0x20000000,
	FOS_FORCEPREVIEWPANEON = 0x40000000,
	FOS_SUPPORTSTREAMABLEITEMS = 0x80000000
};
typedef DWORD FILEOPENDIALOGOPTIONS;
#endif


BOOL(*iFileOpenFolder_Func[])(HWND, TCHAR*, const TCHAR*) = {
	iFileOpenFolder_Func0,
	iFileOpenFolder_Func1,
	iFileOpenFolder_Func2,
	iFileOpenFolder_Func3
};

#if _MSC_VER > 1600
//#pragma auto_inline(off)
HRESULT WINAPI iFileOpenDialog_CoCreateInstance(IFILEDIALOG** pFileDlg)
{
	IF_LIKELY(*pFileDlg == NULL) {
		return CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pFileDlg));
	}
	return E_ABORT;
}

ULONG WINAPI iFileOpenDialog_Release(IFILEDIALOG* pFileDlg)
{
	IF_LIKELY(pFileDlg) {
		return pFileDlg->Release();
	}
	return 0;
}

HRESULT WINAPI iFileOpenDialog_SetDefaultFolder(IFILEDIALOG* pFileDlg, SHELLITEM* psi)
{
	IF_LIKELY(pFileDlg) {
		return pFileDlg->SetDefaultFolder(psi);
	}
	return E_POINTER;
}

HRESULT WINAPI iFileOpenDialog_GetFileName(IFILEDIALOG* pFileDlg, LPWSTR **pszName)
{
	IF_LIKELY(pFileDlg) {
		return pFileDlg->GetFileName(*pszName);
	}
	return E_POINTER;
}

HRESULT WINAPI iFileOpenDialog_SetFileName(IFILEDIALOG* pFileDlg, LPCWSTR pszName)
{
	IF_LIKELY(pFileDlg) {
		return pFileDlg->SetFileName(pszName);
	}
	return E_POINTER;
}

HRESULT WINAPI iFileOpenDialog_GetOptions(IFILEDIALOG* pFileDlg, DWORD* pfos)
{
	IF_LIKELY(pFileDlg) {
		return pFileDlg->GetOptions(pfos);
	}
	return E_POINTER;
}

HRESULT WINAPI iFileOpenDialog_SetOptions(IFILEDIALOG* pFileDlg, DWORD fos)
{
	IF_LIKELY(pFileDlg) {
		return pFileDlg->SetOptions(fos);
	}
	return E_POINTER;
}

HRESULT WINAPI iFileOpenDialog_Show(IFILEDIALOG* pFileDlg, HWND hwndOwner)
{
	IF_LIKELY(pFileDlg) {
		return pFileDlg->Show(hwndOwner);
	}
	return E_POINTER;
}

HRESULT WINAPI iFileOpenDialog_GetResult(IFILEDIALOG* pFileDlg, SHELLITEM** ppsi)
{
	IF_LIKELY(pFileDlg) {
		return pFileDlg->GetResult(ppsi);
	}
	return E_POINTER;
}

HRESULT WINAPI ShellItem_GetDisplayName(SHELLITEM* pShellItem, SIGDN sigdnName, LPWSTR* ppszName)
{
	IF_LIKELY(pShellItem) {
		return pShellItem->GetDisplayName(sigdnName, ppszName);
	}
	return E_POINTER;
}

ULONG WINAPI ShellItem_Release(SHELLITEM* pShellItem)
{
	IF_LIKELY(pShellItem) {
		return pShellItem->Release();
	}
	return 0;
}

#if _MSC_VER <= 1920
HRESULT WINAPI ShellItem_SHCreateItemFromParsingName(LPWSTR* pszPath, SHELLITEM** ppv)
{
	return SHCreateItemFromParsingName_Func((PCWSTR)*pszPath, NULL, IID_PPV_ARGS(ppv));
}
#endif
#endif

BOOL iFileOpenFolder_Func0(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle)
{
#if _MSC_VER > 1920
	return iFileOpenFolder_Func1(hWnd, lpFolder, lpTitle);
#else
	HMODULE hDll = NULL;
	const char* szFunc = "SHCreateItemFromParsingName";
	IFILEDIALOG* pFileOpenDialog = NULL;

	iFileOpenFolder_Arg = 3;
	if (!IsWinVistaOrGreater()) {
		goto DLL_FAIL;
	}

	hDll = LOADSYSLIB(_T("SHELL32.DLL"));
	if (hDll == NULL) {
		goto DLL_FAIL;
	}

	SHCreateItemFromParsingName_Func = (HRESULT(WINAPI*)(PCWSTR, IBindCtx*, REFIID, void**))(GetProcAddress(hDll, szFunc));
	if (SHCreateItemFromParsingName_Func == NULL) {
		goto DLL_FAIL;
	}

	iFileOpenFolder_Arg = 1;
	return iFileOpenFolder_Func1(hWnd, lpFolder, lpTitle);

DLL_FAIL:
	iFileOpenDialog_Release(pFileOpenDialog);
	iFileOpenFolder_Arg = 2;
	return iFileOpenFolder_Func2(hWnd, lpFolder, lpTitle);
#endif
}

BOOL iFileOpenFolder_Func1(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle)
{
	LPWSTR lpszItem = NULL;
	BOOL nRet = FALSE;
	IFILEDIALOG* pFileOpenDialog = NULL;
	SHELLITEM* pFileFolder = NULL;
	DWORD dwOptions = 0;
	HRESULT hr = NULL;
#ifndef _UNICODE
	WCHAR szFolder[_MAX_PATH * 2] = L"";
#endif

	hr = iFileOpenDialog_CoCreateInstance(&pFileOpenDialog);
	IF_UNLIKELY(!SUCCEEDED(hr)) {
		return iFileOpenFolder2(hWnd, lpFolder, lpTitle);
	}

#ifdef _UNICODE
	lpszItem = lpFolder;
#else
	lpszItem = CharToWCharConv2(lpFolder, szFolder);
#endif

#if _MSC_VER > 1920
	hr = SHCreateItemFromParsingName((PCWSTR)*lpszItem, NULL, IID_PPV_ARGS(&pFileFolder));
#else
	hr = ShellItem_SHCreateItemFromParsingName(&lpszItem, &pFileFolder);
#endif
	IF_LIKELY(SUCCEEDED(hr)) {
		iFileOpenDialog_SetDefaultFolder(pFileOpenDialog, pFileFolder);
	}

	hr = iFileOpenDialog_GetOptions(pFileOpenDialog, &dwOptions);
	IF_LIKELY(SUCCEEDED(hr) && dwOptions != 0)
	{
		dwOptions |= FOS_PICKFOLDERS;
		iFileOpenDialog_SetOptions(pFileOpenDialog, dwOptions);
	}

	hr = iFileOpenDialog_Show(pFileOpenDialog, hWnd);
	IF_LIKELY(SUCCEEDED(hr))
	{
		SHELLITEM* psi = NULL;

		hr = iFileOpenDialog_GetResult(pFileOpenDialog, &psi);
		IF_LIKELY(SUCCEEDED(hr) && psi != NULL)
		{
			LPWSTR lpszPath = NULL;

			ShellItem_GetDisplayName(psi, SIGDN_FILESYSPATH, &lpszPath);

#ifdef _UNICODE
			qwcscpy(lpFolder, lpszPath);
#else
			WCharToCharConv2(lpszPath, lpFolder);
#endif

			CoTaskMemFree(lpszPath);
			ShellItem_Release(psi);
			nRet = TRUE;
		}
	}
	ShellItem_Release(pFileFolder);
	iFileOpenDialog_Release(pFileOpenDialog);
	return nRet;
}

BOOL iFileOpenFolder_Func2(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle)
{
	BROWSEINFO binfo = {
		hWnd,
		NULL,
		lpFolder,
		lpTitle,
		BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_VALIDATE,
		(BFFCALLBACK)&iFileOpenFolder2_CallbackProc,
		(LPARAM)lpFolder,
		NULL };
	LPITEMIDLIST idlist;
	BOOL nRet = FALSE;

	idlist = SHBrowseForFolder(&binfo);
	IF_LIKELY(idlist != NULL)
	{
		SHGetPathFromIDList(idlist, lpFolder);
		nRet = TRUE;
	}
	CoTaskMemFree(idlist);
	return nRet;
}

BOOL iFileOpenFolder_Func3(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle)
{
	while (iFileOpenFolder_Arg == 3) {
		Sleep(1000);
	}
	return iFileOpenFolder_Func[iFileOpenFolder_Arg](hWnd, lpFolder, lpTitle);
}

BOOL iFileOpenFolder(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle)
{
#if _MSC_VER > 1920
	return iFileOpenFolder_Func1(hWnd, lpFolder, lpTitle);
#else
	return iFileOpenFolder_Func[iFileOpenFolder_Arg](hWnd, lpFolder, lpTitle);
#endif
}

BOOL iFileOpenFolder2(HWND hWnd, TCHAR* lpFolder, const TCHAR* lpTitle)
{
	return iFileOpenFolder_Func2(hWnd, lpFolder, lpTitle);
}

int CALLBACK iFileOpenFolder2_CallbackProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case BFFM_INITIALIZED:
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lParam);
		break;
	case BFFM_SELCHANGED:
		if (1)
		{
			TCHAR szFolder[MAX_PATH_SIZE];
			int nRet;

			nRet = SHGetPathFromIDList((LPITEMIDLIST)wParam, szFolder);
			if (nRet != 0)
			{
				SendMessage(hWnd, BFFM_ENABLEOK, 0, TRUE);
				SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szFolder);
			}
			else
			{
				SendMessage(hWnd, BFFM_ENABLEOK, 0, FALSE);
				SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, NULL);
			}
		}
		break;
	case BFFM_VALIDATEFAILED:
		SendMessage(hWnd, BFFM_ENABLEOK, 0, FALSE);
		SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)_T("フォルダー名が無効です。"));
		return 1;
	}
	return 0;
}
