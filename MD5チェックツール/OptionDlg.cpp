// OptionDlg.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "..\shared\cpuid.h"
#include "..\shared\ecoqos.h"
#include "..\shared\WindowSize.h"

// グローバル変数の定義します:
OptionWindow tagOptionWindow1;

// 外部ファイルの変数定義します。
extern MainWindow tagMainWindow1;
extern FileListWindow tagFileListWindow1;
extern AboutWindow tagAboutWindow1;
extern DWORD dwAppFrag;
extern DWORD dwHashFileOutToCopyFolderMode;
extern DWORD dwHashCpuMode;
extern unsigned int OPENSSL_ia32cap_P1[4];
extern BOOL(WINAPI* SetLayeredWindowAttributes_Func)(HWND, COLORREF, BYTE, DWORD);
extern int nEditMarginX[4];
extern int nEditMarginY[4];
extern int nStringX[3];
extern int nStringY[3];

static DWORD gLoad_dwAppFrag = (DWORD)-1;


#define OPTIONDLG_X 230
#define OPTIONDLG_Y 237
#define OPTIONCLDDLG_X 215
#define OPTIONCLDDLG_Y 195


int OptionDlg_Free()
{
	free(tagOptionWindow1.pBuf);
	tagOptionWindow1.pBuf = NULL;

	free(tagOptionWindow1.pCheckSumFile);
	tagOptionWindow1.pCheckSumFile = NULL;
	return TRUE;
}

int OptionDlg_Malloc(size_t nSize)
{
	tagOptionWindow1.pBuf = (TCHAR*)malloc(nSize * sizeof(TCHAR));
	IF_UNLIKELY(tagOptionWindow1.pBuf == NULL) {
		goto MALLOC_ERROR;
	}
	*tagOptionWindow1.pBuf = '\0';

	tagOptionWindow1.pCheckSumFile = (TCHAR*)malloc(1024 * sizeof(TCHAR));
	IF_UNLIKELY(tagOptionWindow1.pCheckSumFile == NULL) {
		goto MALLOC_ERROR;
	}
	*tagOptionWindow1.pCheckSumFile = '\0';

	return TRUE;

MALLOC_ERROR:
	OptionDlg_Free();
	return FALSE;
}

// オプション画面のメッセージ ハンドラです。
LRESULT CALLBACK OptionDlg_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagOptionWindow1.nWindowMessage)
	{
		switch (wParam)
		{
		case APP_MESSAGE_WINDOWSET:
			return OptionDlg_OnMessageWindowSet(hWnd, wParam, lParam);
		case APP_MASSAGE_DPICHANGE:
			return OptionDlg_OnMessageDpiChange(hWnd, wParam, lParam);
		}
		return FALSE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		return OptionDlg_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return OptionDlg_OnCommand(hWnd, wParam, lParam);
	case WM_GETMINMAXINFO:
		return OptionDlg_OnGetMinMaxInfo(hWnd, wParam, lParam);
	case WM_SIZE:
		return OptionDlg_OnSize(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return OptionDlg_OnSysColorChange(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return OptionDlg_OnThemeChanged(hWnd, wParam, lParam);
	case WM_NOTIFY:
		return OptionDlg_OnNotify(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlg_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	TCITEM tItem = { 0 };
	RECT rc = { 0 };

	if (~dwAppFrag & APP_ENABLE_MINIMAM_HANDLE) {
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)tagMainWindow1.hIcon[0]);
	}

	tagOptionWindow1.dwStructSize = sizeof(tagOptionWindow1);
	tagOptionWindow1.hWnd = hWnd;
	tagOptionWindow1.dwAppFrag = dwAppFrag;
	tagOptionWindow1.dwHashType = tagMainWindow1.tagHashThread1.dwHashType;
	tagOptionWindow1.dwRadioState[0] = dwHashCpuMode;
	tagOptionWindow1.dwRadioState[1] = dwHashFileOutToCopyFolderMode;

	IF_UNLIKELY(!OptionDlg_Malloc(1024)) {
		return FALSE;
	}

	tagOptionWindow1.nWindowMessage = RegisterWindowMessage(_T("OptionDlg"));
	tagOptionWindow1.hTab = GetDlgItem(hWnd, IDC_OPTION_TAB1);

	tItem.mask = TCIF_TEXT;
	tItem.pszText = (TCHAR*)_T("メインウィンドウの設定");
	TabCtrl_InsertItem(tagOptionWindow1.hTab, 0, &tItem);

	tItem.pszText = (TCHAR*)_T("ハッシュ計算の設定");
	TabCtrl_InsertItem(tagOptionWindow1.hTab, 1, &tItem);

	tItem.pszText = (TCHAR*)_T("拡張設定");
	TabCtrl_InsertItem(tagOptionWindow1.hTab, 2, &tItem);

	tItem.pszText = (TCHAR*)_T("クリップボードの設定");
	TabCtrl_InsertItem(tagOptionWindow1.hTab, 3, &tItem);

	tagOptionWindow1.hIDOK = GetDlgItem(hWnd, IDOK);
	tagOptionWindow1.hIDCANCEL = GetDlgItem(hWnd, IDCANCEL);

	tagOptionWindow1.hWndTab[0] = CreateDialog(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_OPTION_CLD1), hWnd, (DLGPROC)OptionDlgTab1_WndProc);
	tagOptionWindow1.hWndTab[1] = CreateDialog(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_OPTION_CLD2), hWnd, (DLGPROC)OptionDlgTab2_WndProc);
	tagOptionWindow1.hWndTab[2] = CreateDialog(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_OPTION_CLD3), hWnd, (DLGPROC)OptionDlgTab3_WndProc);
	tagOptionWindow1.hWndTab[3] = CreateDialog(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_OPTION_CLD4), hWnd, (DLGPROC)OptionDlgTab4_WndProc);


	OptionDlg_OnMessageDpiChange(hWnd, wParam, lParam);
	TabCtrl_SetCurSel(tagOptionWindow1.hTab, tagOptionWindow1.nActiveTab);
	OptionDlg_TabChange(tagOptionWindow1.nActiveTab);
	GetClientRect(hWnd, &rc);
	SendMessage(hWnd, WM_SIZE, 0, MAKELPARAM(rc.right, rc.bottom));
	PostMessage(hWnd, tagOptionWindow1.nWindowMessage, APP_MESSAGE_WINDOWSET, 0);
	return TRUE;
}

LRESULT OptionDlg_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);

	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDOK:
		return OptionDlg_OnIdOk(hWnd, wParam, lParam);
	case IDCANCEL:
		return OptionDlg_OnIdCancel(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlg_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DWORD dwRet = (DWORD)SendMessage(tagOptionWindow1.hTab1Combo[0], CB_GETCURSEL, 0, 0);
	int nRet = 0;

	IF_LIKELY(dwRet != tagOptionWindow1.dwHashType && dwRet < MAX_HASH_TYPE) {
		tagOptionWindow1.dwHashType = dwRet;
	}

	if (SendMessage(tagOptionWindow1.hTab1Checkbox[0], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_HASHOUT_LOWER;

		if (~dwAppFrag & APP_HASHTHREAD && *tagMainWindow1.pHashString != '\0')
		{
			_tcslwr(tagMainWindow1.pHashString);
			SetWindowText(tagMainWindow1.hEdit[0], tagMainWindow1.pHashString);
		}
	}
	else
	{
		dwAppFrag &= ~APP_HASHOUT_LOWER;

		if (~dwAppFrag & APP_HASHTHREAD && *tagMainWindow1.pHashString != '\0')
		{
			_tcsupr(tagMainWindow1.pHashString);
			SetWindowText(tagMainWindow1.hEdit[0], tagMainWindow1.pHashString);
		}
	}

	if (SendMessage(tagOptionWindow1.hTab1Checkbox[1], BM_GETCHECK, 0, 0) == BST_CHECKED) {
		dwAppFrag |= APP_ENABLE_MULTILINE;
	}
	else {
		dwAppFrag &= ~APP_ENABLE_MULTILINE;
	}
	Hash_ChangeMsg(tagOptionWindow1.dwHashType);

	if (IsWin10OrGreater())
	{
		nRet = (int)SendMessage(tagOptionWindow1.hTab1Checkbox[2], BM_GETCHECK, 0, 0);
		if (SetEcoQos(nRet))
		{
			if (nRet) {
				dwAppFrag |= APP_ECOQOS;
			}
			else {
				dwAppFrag &= ~APP_ECOQOS;
			}
		}
	}

	if (SendMessage(tagOptionWindow1.hTab1Checkbox[3], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_WINDOWALPHA;
		SetLayeredWindow(tagMainWindow1.hWnd);
	}
	else
	{
		dwAppFrag &= ~APP_WINDOWALPHA;
		(*SetLayeredWindowAttributes_Func)(tagMainWindow1.hWnd, 0, ACTIVE_ALPHA, LWA_ALPHA);
	}

	if (SendMessage(tagOptionWindow1.hTab1Checkbox[4], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_ENABLE_TOPMOST;
		SetWindowPos(tagMainWindow1.hWnd, HWND_TOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
	}
	else
	{
		dwAppFrag &= ~APP_ENABLE_TOPMOST;
		SetWindowPos(tagMainWindow1.hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
	}

	if (SendMessage(tagOptionWindow1.hTab1Checkbox[5], BM_GETCHECK, 0, 0) == BST_CHECKED) {
		dwAppFrag |= APP_DISABLE_MULTIFILE_UPDATE_DRAW;
	}
	else {
		dwAppFrag &= ~APP_DISABLE_MULTIFILE_UPDATE_DRAW;
	}

	if (SendMessage(tagOptionWindow1.hTab1Checkbox[6], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_ENABLE_FILELIST;
		FileListDlg();
	}
	else {
		dwAppFrag &= ~APP_ENABLE_FILELIST;
	}

	if (SendMessage(tagOptionWindow1.hTab1Checkbox[7], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_ENABLE_DEBUG;
		EnableMenuItem(tagMainWindow1.hMenu, IDM_DEBUG, MF_ENABLED);
	}
	else
	{
		dwAppFrag &= ~APP_ENABLE_DEBUG;
		EnableMenuItem(tagMainWindow1.hMenu, IDM_DEBUG, MF_GRAYED);
	}

	if (SendMessage(tagOptionWindow1.hTab1Checkbox[8], BM_GETCHECK, 0, 0) == BST_CHECKED) {
		dwAppFrag |= APP_ENABLE_MINIMAM_HANDLE;
	}
	else {
		dwAppFrag &= ~APP_ENABLE_MINIMAM_HANDLE;
	}


	dwHashCpuMode = tagOptionWindow1.dwRadioState[0];
	OptionDlg_HashCpuMode(dwHashCpuMode);

	if (SendMessage(tagOptionWindow1.hTab2Checkbox[0], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_DISABLE_HASH_NOASM;
		HashThread_SetHashFunc(TRUE);
	}
	else
	{
		dwAppFrag &= ~APP_DISABLE_HASH_NOASM;
		HashThread_SetHashFunc(FALSE);
	}

	if (SendMessage(tagOptionWindow1.hTab2Checkbox[1], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_ENABLE_LARGE_BUFFERSIZE;
		tagMainWindow1.tagHashThread1.dwFileBufferSize = tagMainWindow1.tagHashThread1.dwFileBufferAllocateSize;
	}
	else
	{
		dwAppFrag &= ~APP_ENABLE_LARGE_BUFFERSIZE;
		tagMainWindow1.tagHashThread1.dwFileBufferSize = FILE_BUFFER_SIZE;
	}

	if (SendMessage(tagOptionWindow1.hTab2Checkbox[2], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_FILE_NOCACHE;
		tagMainWindow1.tagHashThread1.nFileNoCache = TRUE;
	}
	else
	{
		dwAppFrag &= ~APP_FILE_NOCACHE;
		tagMainWindow1.tagHashThread1.nFileNoCache = FALSE;
	}

	if (SendMessage(tagOptionWindow1.hTab2Checkbox[3], BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		dwAppFrag |= APP_ENABLE_HIDDENFILE;
		tagMainWindow1.tagHashThread1.dwFileAttributeMask = 0xFFFFFFFF;
	}
	else
	{
		dwAppFrag &= ~APP_ENABLE_HIDDENFILE;
		tagMainWindow1.tagHashThread1.dwFileAttributeMask = (DWORD)DEF_FILE_ATTRIBUTE;
	}

	if (SendMessage(tagOptionWindow1.hTab2Checkbox[4], BM_GETCHECK, 0, 0) == BST_CHECKED) {
		dwAppFrag |= APP_ENABLE_HASHFILE_WRITETIME;
	}
	else {
		dwAppFrag &= ~APP_ENABLE_HASHFILE_WRITETIME;
	}

	if (SendMessage(tagOptionWindow1.hTab2Checkbox[5], BM_GETCHECK, 0, 0) == BST_CHECKED) {
		dwAppFrag |= APP_HASHFILE_ENABLE_HASH;
	}
	else {
		dwAppFrag &= ~APP_HASHFILE_ENABLE_HASH;
	}

	GetWindowText(tagOptionWindow1.hTab2Edit[0], tagOptionWindow1.pBuf, MAX_STRINGTABLE);
	if (_tcsicmp(tagOptionWindow1.pBuf, tagOptionWindow1.pCheckSumFile))
	{
		LoadString(tagMainWindow1.hInst, IDS_CHACKSUM_FILENAME, tagMainWindow1.pStBuf, MAX_STRINGTABLE);
		if (_tcsicmp(tagOptionWindow1.pBuf, tagMainWindow1.pStBuf))
		{
			qtcscpy(tagMainWindow1.pCheckSumFile, tagOptionWindow1.pBuf);
			dwAppFrag |= APP_EDIT_CHECKSUMFILE;
		}
		else
		{
			qtcscpy(tagMainWindow1.pCheckSumFile, tagMainWindow1.pStBuf);
			dwAppFrag &= ~APP_EDIT_CHECKSUMFILE;
		}
		SetCheckSumFile();
	}

	dwHashFileOutToCopyFolderMode = tagOptionWindow1.dwRadioState[1];
	SetIniFileSetting(tagMainWindow1.pINIFile);
	WindowSize_SaveState(tagMainWindow1.hWnd, _T("Option"), tagMainWindow1.pINIFile);
	return OptionDlg_OnIdCancel(hWnd, wParam, lParam);
}

LRESULT OptionDlg_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	TCHAR szBuf[NUMBER_LOADSTRING] = { 0 };

	ITOT_FUNC(tagOptionWindow1.nActiveTab, szBuf, 10);
	WritePrivateProfileString(_T("Option"), _T("ActiveTab"), szBuf, tagMainWindow1.pINIFile);
	OptionDlg_Free();
	tagOptionWindow1.hWnd = NULL;
	EndDialog(hWnd, LOWORD(wParam));
	return TRUE;
}

LRESULT OptionDlg_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	const int cx = MDIVX(OPTIONDLG_X) + tagMainWindow1.dwWindowRectX;
	const int cy = MDIVY(OPTIONDLG_Y) + tagMainWindow1.dwWindowRectY;

	((MINMAXINFO*)lParam)->ptMinTrackSize.x = cx;
	((MINMAXINFO*)lParam)->ptMaxTrackSize.x = cx;
	((MINMAXINFO*)lParam)->ptMinTrackSize.y = cy;
	return TRUE;
}

LRESULT OptionDlg_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nArg[4] = { 0 };
	POINT pt1 = { 0 };
	POINT pt2 = { 0 };
	RECT rc = { 0 };
	int dx = 0;
	int dy = 0;
	int i = 0;

	ClientToScreen(hWnd, &pt1);
	ClientToScreen(tagOptionWindow1.hTab, &pt2);
	GetClientRect(tagOptionWindow1.hTab, &rc);
	TabCtrl_AdjustRect(tagOptionWindow1.hTab, FALSE, &rc);

	dx = pt2.x - pt1.x;
	dy = pt2.y - pt1.y;

	nArg[0] = rc.left + dx;
	nArg[1] = rc.top + dy;
	nArg[2] = rc.right - rc.left;
	nArg[3] = rc.bottom - rc.top;

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hWndTab); i++) {
		MoveWindow(tagOptionWindow1.hWndTab[i], nArg[0], nArg[1], nArg[2], nArg[3], TRUE);
	}

	/*
	nArg[0] = MDIVX(5);
	nArg[1] = MDIVY(5);
	nArg[2] = cx - MDIVX(OPTIONDLG_X - 205);
	nArg[3] = MDIVY(235);
	MoveWindow(tagOptionWindow1.hTab, nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	*/

	nArg[0] = MDIVX(120);
	nArg[1] = MDIVY(215);
	nArg[2] = MDIVX(50);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagOptionWindow1.hIDOK, nArg[0], nArg[1], nArg[2], nArg[3], FALSE);//102, 315, 50, 14

	nArg[0] = MDIVX(175);
	nArg[1] = MDIVY(215);
	nArg[2] = MDIVX(50);
	nArg[3] = nStringY[2] + nEditMarginY[0] + 2;
	MoveWindow(tagOptionWindow1.hIDCANCEL, nArg[0], nArg[1], nArg[2], nArg[3], FALSE);//155, 315, 50, 14
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT OptionDlg_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);

	SendMessage(tagOptionWindow1.hIDOK, WM_SYSCOLORCHANGE, wParam, lParam);
	SendMessage(tagOptionWindow1.hIDCANCEL, WM_SYSCOLORCHANGE, wParam, lParam);
	SendMessage(tagOptionWindow1.hTab, WM_SYSCOLORCHANGE, wParam, lParam);
	return TRUE;
}

LRESULT OptionDlg_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);

	return TRUE;
}

LRESULT OptionDlg_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	switch (((LPNMHDR)lParam)->code)
	{
	case TCN_SELCHANGE:
		tagOptionWindow1.nActiveTab = TabCtrl_GetCurSel(tagOptionWindow1.hTab);
		OptionDlg_TabChange(tagOptionWindow1.nActiveTab);
		break;
	}
	return TRUE;
}


LRESULT OptionDlg_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	OptionDlg_SetWindowMove();
	return TRUE;
}

LRESULT OptionDlg_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	int i = 0;

	SendMessage(tagOptionWindow1.hTab, WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	SendMessage(tagOptionWindow1.hIDOK, WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	SendMessage(tagOptionWindow1.hIDCANCEL, WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hWndTab); i++) {
		SendMessage(tagOptionWindow1.hWndTab[i], tagOptionWindow1.nWindowMessage, APP_MASSAGE_DPICHANGE, lParam);
	}

	OptionDlg_SetWindowMove();
	return TRUE;
}


VOID OptionDlg_HashCpuMode(DWORD dwCpuMode)
{
	memcpy(OPENSSL_ia32cap_P, OPENSSL_ia32cap_P1, sizeof(OPENSSL_ia32cap_P1));

	// SSE
	if (dwCpuMode & 0x00000001)
	{
		OPENSSL_ia32cap_P[0] &= ~(0x06000000);
		OPENSSL_ia32cap_P[1] &= ~(0x00190a41);	// 0x00180200
	}

	// AVX, AVX2, AVX512
	if (dwCpuMode & 0x00000002)
	{
		OPENSSL_ia32cap_P[1] &= ~(0x50001800);
		OPENSSL_ia32cap_P[2] &= ~(0xdc230020);	// 0x00000020
		OPENSSL_ia32cap_P[3] &= ~(0x000000c0);
	}

	// SHAEXT
	if (dwCpuMode & 0x00000004) {
		OPENSSL_ia32cap_P[2] &= ~(0x20000000);
	}
	return;
}

VOID OptionDlg_TabChange(int nTab)
{
	switch (nTab)
	{
	case 0:
		ShowWindow(tagOptionWindow1.hWndTab[0], SW_SHOW);
		ShowWindow(tagOptionWindow1.hWndTab[1], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[2], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[3], SW_HIDE);
		break;
	case 1:
		ShowWindow(tagOptionWindow1.hWndTab[0], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[1], SW_SHOW);
		ShowWindow(tagOptionWindow1.hWndTab[2], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[3], SW_HIDE);
		break;
	case 2:
		ShowWindow(tagOptionWindow1.hWndTab[0], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[1], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[2], SW_SHOW);
		ShowWindow(tagOptionWindow1.hWndTab[3], SW_HIDE);
		break;
	default:
		ShowWindow(tagOptionWindow1.hWndTab[0], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[1], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[2], SW_HIDE);
		ShowWindow(tagOptionWindow1.hWndTab[3], SW_SHOW);
		break;
	}
	return;
}


VOID OptionDlg_SetWindowMove(VOID)
{
	RECT rect = { 0 };

	GetWindowRect(tagOptionWindow1.hWnd, &rect);
	MoveWindow(tagOptionWindow1.hWnd, rect.left, rect.top, MDIVX(OPTIONDLG_X) + tagMainWindow1.dwWindowRectX, MDIVY(OPTIONDLG_Y) + tagMainWindow1.dwWindowRectY, TRUE);
	return;
}

VOID OptionDlg(HWND hWnd)
{
	DialogBox(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_OPTION), hWnd, (DLGPROC)OptionDlg_WndProc);
	return;
}

VOID GetCheckSumFile(TCHAR* pInBuf)
{
	TCHAR* p1 = tagMainWindow1.pCheckSumFile;
	TCHAR* p2 = pInBuf;

	while ((*(p1 + 0) + *(p1 + 1)) != '\0')
	{
		if (*p1 == '\0')
		{
			p1++;
			*p2++ = ';';
		}
		else {
			*p2++ = *p1++;
		}
	}
	*p2 = '\0';
	return;
}

VOID SetCheckSumFile(VOID)
{
	TCHAR** ppChackSumFile = tagMainWindow1.ppCheckSumFile;
	TCHAR* pChackSumFile = tagMainWindow1.pCheckSumFile;

	*ppChackSumFile++ = pChackSumFile;
	while (*pChackSumFile != '\0')
	{
		if (*pChackSumFile == ';')
		{
			*pChackSumFile = '\0';
			*ppChackSumFile++ = pChackSumFile + 1;
		}
		pChackSumFile++;
	}
	*ppChackSumFile = (TCHAR*)'\0';
	*(pChackSumFile + 1) = '\0';
	return;
}

BOOL SetRemoveReadOnly(const TCHAR* cpInIniFile)
{
	const DWORD dwFileAttrib = GetFileAttributes(cpInIniFile);

	if (dwFileAttrib == INVALID_FILE_ATTRIBUTES) {
		return FALSE;
	}

	if (dwFileAttrib & FILE_ATTRIBUTE_READONLY)
	{
		const DWORD dwSetFileAttrib = dwFileAttrib & ~(FILE_ATTRIBUTE_READONLY);

		if (!SetFileAttributes(cpInIniFile, dwSetFileAttrib)) {
			return FALSE;
		}
	}
	return TRUE;
}

VOID GetIniFileSetting(const TCHAR* cpInIniFile)
{
	TCHAR szBuf[MAX_STRINGTABLE];
	HWND hRet = NULL;
	int nRet = 0;


	GetPrivateProfileString(_T("Option"), _T("HashTypeString"), _T(""), szBuf, NUMBER_LOADSTRING, cpInIniFile);
	if (*szBuf != '\0') // 初回起動の判定
	{
		int i = 0;

		for (; i < MAX_HASH_TYPE; i++)
		{
			if (_tcsicmp(szBuf, HashThread_GetHashName(i)) == 0) {
				nRet = (int)i;
			}
		}
		tagMainWindow1.tagHashThread1.dwHashType = nRet;
	}
	else
	{
		GetPrivateProfileString(_T("Option"), _T("HashType"), _T(""), szBuf, NUMBER_LOADSTRING, cpInIniFile);
		if (*szBuf != '\0')
		{
			nRet = _ttoi(szBuf);

			if (nRet > MAX_HASH_TYPE) {
				nRet = 0;
			}

			if (nRet >= HASH_CRC32C_TYPE) {
				nRet--;
			}

			tagMainWindow1.tagHashThread1.dwHashType = nRet;
			WritePrivateProfileString(_T("Option"), _T("HashType"), _T(""), cpInIniFile);
			WritePrivateProfileString(_T("Option"), _T("HashTypeString"), HashThread_GetHashName(nRet), cpInIniFile);
		}
		else
		{
			dwAppFrag |= APP_NO_SETTING;
			tagMainWindow1.tagHashThread1.dwHashType = 0;
		}
	}

	GetPrivateProfileString(_T("Option"), _T("EnableHashLower"), _T("1"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag |= APP_HASHOUT_LOWER;
	}
	else {
		dwAppFrag &= ~APP_HASHOUT_LOWER;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableHashMultiLine"), _T("1"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag |= APP_ENABLE_MULTILINE;
	}
	else {
		dwAppFrag &= ~APP_ENABLE_MULTILINE;
	}

	if (IsWin10OrGreater())
	{
		int nArg = FALSE;

		GetPrivateProfileString(_T("Option"), _T("EnableEcoQos"), _T("0"), szBuf, 2, cpInIniFile);
		if (*szBuf != '0')
		{
			nArg = TRUE;
			dwAppFrag |= APP_ECOQOS;
		}
		else {
			dwAppFrag &= ~APP_ECOQOS;
		}

		nRet = SetEcoQos(nArg);
		IF_UNLIKELY(nArg && !nRet) {
			dwAppFrag &= ~APP_ECOQOS;
		}
	}

	GetPrivateProfileString(_T("Option"), _T("EnableWindowAlpha"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag |= APP_WINDOWALPHA;
	}
	else {
		dwAppFrag &= ~APP_WINDOWALPHA;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableTopMost"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0')
	{
		dwAppFrag |= APP_ENABLE_TOPMOST;
		hRet = HWND_TOPMOST;
	}
	else
	{
		dwAppFrag &= ~APP_ENABLE_TOPMOST;
		hRet = HWND_NOTOPMOST;
	}
	if (tagMainWindow1.hWnd) {
		SetWindowPos(tagMainWindow1.hWnd, hRet, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
	}

	GetPrivateProfileString(_T("Option"), _T("DisableMultiFileUpDateDraw"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag &= ~APP_DISABLE_MULTIFILE_UPDATE_DRAW;
	}
	else {
		dwAppFrag |= APP_DISABLE_MULTIFILE_UPDATE_DRAW;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableFileList"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag |= APP_ENABLE_FILELIST;
	}
	else {
		dwAppFrag &= ~APP_ENABLE_FILELIST;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableDebug"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag |= APP_ENABLE_DEBUG;
	}
	else {
		dwAppFrag &= ~APP_ENABLE_DEBUG;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableMinimumHandle"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag |= APP_ENABLE_MINIMAM_HANDLE;
	}
	else {
		dwAppFrag &= ~APP_ENABLE_MINIMAM_HANDLE;
	}

	GetPrivateProfileString(_T("Option"), _T("DisableHashNoAsm"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0')
	{
		dwAppFrag |= APP_DISABLE_HASH_NOASM;
		HashThread_SetHashFunc(TRUE);
	}
	else
	{
		dwAppFrag &= ~APP_DISABLE_HASH_NOASM;
		HashThread_SetHashFunc(FALSE);
	}

	GetPrivateProfileString(_T("Option"), _T("EnableLargeBufferSize"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0')
	{
		dwAppFrag |= APP_ENABLE_LARGE_BUFFERSIZE;
		tagMainWindow1.tagHashThread1.dwFileBufferSize = 0x100000;
	}
	else
	{
		dwAppFrag &= ~APP_ENABLE_LARGE_BUFFERSIZE;
		tagMainWindow1.tagHashThread1.dwFileBufferSize = FILE_BUFFER_SIZE;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableFileNoCache"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0')
	{
		dwAppFrag |= APP_FILE_NOCACHE;
		tagMainWindow1.tagHashThread1.nFileNoCache = TRUE;
	}
	else
	{
		dwAppFrag &= ~APP_FILE_NOCACHE;
		tagMainWindow1.tagHashThread1.nFileNoCache = FALSE;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableHiddenFile"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0')
	{
		dwAppFrag |= APP_ENABLE_HIDDENFILE;
		tagMainWindow1.tagHashThread1.dwFileAttributeMask = 0xFFFFFFFF;
	}
	else
	{
		dwAppFrag &= ~APP_ENABLE_HIDDENFILE;
		tagMainWindow1.tagHashThread1.dwFileAttributeMask = (DWORD)DEF_FILE_ATTRIBUTE;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableHashFileWriteTime"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag |= APP_ENABLE_HASHFILE_WRITETIME;
	}
	else {
		dwAppFrag &= ~APP_ENABLE_HASHFILE_WRITETIME;
	}

	GetPrivateProfileString(_T("Option"), _T("EnableHashFileHash"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0') {
		dwAppFrag |= APP_HASHFILE_ENABLE_HASH;
	}
	else {
		dwAppFrag &= ~APP_HASHFILE_ENABLE_HASH;
	}


	GetPrivateProfileString(_T("Option"), _T("EnableOldSaveFile"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0')
	{
		dwAppFrag |= APP_OLDHASHFILE;
		tagMainWindow1.ofnFileName[1].nFilterIndex = 2;
	}
	else
	{
		dwAppFrag &= ~APP_OLDHASHFILE;
		tagMainWindow1.ofnFileName[1].nFilterIndex = 1;
	}

	GetPrivateProfileString(_T("Option"), _T("ActiveTab"), _T("0"), szBuf, 2, cpInIniFile);
	if (*szBuf != '0')
	{
		tagOptionWindow1.nActiveTab = _ttoi(szBuf);
		if (tagOptionWindow1.nActiveTab > SIZEOF_NUM(tagOptionWindow1.hWndTab)) {
			tagOptionWindow1.nActiveTab = 0;
		}
	}

	GetPrivateProfileString(_T("Option"), _T("SaveFileCharCode"), _T("2"), szBuf, 2, cpInIniFile);
	tagMainWindow1.dwSaveHashFileCharCode = _ttoi(szBuf);
	if (tagMainWindow1.dwSaveHashFileCharCode > 2) {
		tagMainWindow1.dwSaveHashFileCharCode = 2;
	}


	/*
	GetPrivateProfileString(_T("Option"), _T("SaveFileFilterIndex"), _T("0"), szBuf, NUMBER_LOADSTRING, cpInIniFile);
	if (*szBuf != '0')
	{
		tagMainWindow1.ofnFileName[1].nFilterIndex = _ttoi(szBuf);
		if (tagMainWindow1.ofnFileName[1].nFilterIndex > 2) {
			tagMainWindow1.ofnFileName[1].nFilterIndex = 1;
		}
	}
	*/


	GetPrivateProfileString(_T("Option"), _T("CheckSumFile"), _T(""), szBuf, MAX_STRINGTABLE - 1, cpInIniFile);
	if (*szBuf != '\0')
	{
		qtcscpy(tagMainWindow1.pCheckSumFile, szBuf);
		SetCheckSumFile();
		dwAppFrag |= APP_EDIT_CHECKSUMFILE;
	}
	else {
		dwAppFrag &= ~APP_EDIT_CHECKSUMFILE;
	}

	GetPrivateProfileString(_T("Option"), _T("HashFileOutToCopyFolderMode"), _T("0"), szBuf, NUMBER_LOADSTRING, cpInIniFile);
	if (*szBuf != '0')
	{
		dwHashFileOutToCopyFolderMode = _ttoi(szBuf);
		if (dwHashFileOutToCopyFolderMode > 4) {
			dwHashFileOutToCopyFolderMode = 1;
		}
	}

#if !defined(UNICODE) && _MSC_VER < 1400
	if (!IsWin95())
	{
#endif

		GetPrivateProfileString(_T("Option"), _T("HashCpuMode"), _T("0"), szBuf, NUMBER_LOADSTRING, cpInIniFile);
		if (*szBuf != '0')
		{
			dwHashCpuMode = _ttoi(szBuf);
			if (dwHashCpuMode > 3) {
				dwHashCpuMode = 0;
			}
		}
		OptionDlg_HashCpuMode(dwHashCpuMode);

#if !defined(UNICODE) && _MSC_VER < 1400
	}
#endif

	gLoad_dwAppFrag = dwAppFrag;


	GetPrivateProfileString(_T("FileList"), _T("ListViewTextColorHashComp"), _T(""), szBuf, 11, cpInIniFile);
	if (TCHAR_COMP_TO_INT_2CHAR(szBuf, _T('0'), _T('x'))) {
		STSCANF_FUNC(szBuf + 2, _T("%x"), &tagFileListWindow1.clListTextColor[0]);
	}
	else {
		tagFileListWindow1.clListTextColor[0] = 0x00FF0000;
	}

	GetPrivateProfileString(_T("FileList"), _T("ListViewTextColorHashCompFoul"), _T(""), szBuf, 11, cpInIniFile);
	if (TCHAR_COMP_TO_INT_2CHAR(szBuf, _T('0'), _T('x'))) {
		STSCANF_FUNC(szBuf + 2, _T("%x"), &tagFileListWindow1.clListTextColor[1]);
	}
	else {
		tagFileListWindow1.clListTextColor[1] = 0x000000FF;
	}

	GetPrivateProfileString(_T("FileList"), _T("ListViewTextColorFileError"), _T(""), szBuf, 11, cpInIniFile);
	if (TCHAR_COMP_TO_INT_2CHAR(szBuf, _T('0'), _T('x')))	{
		STSCANF_FUNC(szBuf + 2, _T("%x"), &tagFileListWindow1.clListTextColor[2]);
	}
	else {
		tagFileListWindow1.clListTextColor[2] = 0x000040FF;
	}
	return;
}

VOID SetIniFileSetting(const TCHAR* cpInIniFile)
{
	TCHAR szBuf[MAX_STRINGTABLE] = _T("");

	if (gLoad_dwAppFrag == dwAppFrag) {
		return;
	}

	if (!SetRemoveReadOnly(cpInIniFile)) {
		return;
	}

	if (~dwAppFrag & APP_ARG_HASHTYPE) {
		WritePrivateProfileString(_T("Option"), _T("HashTypeString"), HashThread_GetHashName(tagMainWindow1.tagHashThread1.dwHashType), cpInIniFile);
	}

	WritePrivateProfileString(_T("Option"), _T("EnableHashLower"), dwAppFrag & APP_HASHOUT_LOWER ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableHashMultiLine"), dwAppFrag & APP_ENABLE_MULTILINE ? _T("1") : _T("0"), cpInIniFile);
	if (IsWin10OrGreater()) {
		WritePrivateProfileString(_T("Option"), _T("EnableEcoQos"), dwAppFrag & APP_ECOQOS ? _T("1") : _T("0"), cpInIniFile);
	}
	WritePrivateProfileString(_T("Option"), _T("EnableWindowAlpha"), dwAppFrag & APP_WINDOWALPHA ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableTopMost"), dwAppFrag & APP_ENABLE_TOPMOST ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("DisableMultiFileUpDateDraw"), dwAppFrag & APP_DISABLE_MULTIFILE_UPDATE_DRAW ? _T("0") : _T("1"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableFileList"), dwAppFrag & APP_ENABLE_FILELIST ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableDebug"), dwAppFrag & APP_ENABLE_DEBUG ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableMinimumHandle"), dwAppFrag & APP_ENABLE_MINIMAM_HANDLE ? _T("1") : _T("0"), cpInIniFile);

	WritePrivateProfileString(_T("Option"), _T("DisableHashNoAsm"), dwAppFrag & APP_DISABLE_HASH_NOASM ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableLargeBufferSize"), dwAppFrag & APP_ENABLE_LARGE_BUFFERSIZE ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableFileNoCache"), dwAppFrag & APP_FILE_NOCACHE ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableHiddenFile"), dwAppFrag & APP_ENABLE_HIDDENFILE ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableHashFileWriteTime"), dwAppFrag & APP_ENABLE_HASHFILE_WRITETIME ? _T("1") : _T("0"), cpInIniFile);
	WritePrivateProfileString(_T("Option"), _T("EnableHashFileHash"), dwAppFrag & APP_HASHFILE_ENABLE_HASH ? _T("1") : _T("0"), cpInIniFile);

	WritePrivateProfileString(_T("Option"), _T("EnableOldSaveFile"), dwAppFrag & APP_OLDHASHFILE ? _T("1") : _T("0"), cpInIniFile);

	if (tagMainWindow1.dwSaveHashFileCharCode != 2)
	{
		ITOT_FUNC(tagMainWindow1.dwSaveHashFileCharCode, szBuf, 10);
		WritePrivateProfileString(_T("Option"), _T("SaveFileCharCode"), szBuf, cpInIniFile);
	}

	GetCheckSumFile(szBuf);
	WritePrivateProfileString(_T("Option"), _T("CheckSumFile"), szBuf, cpInIniFile);

	ITOT_FUNC(dwHashFileOutToCopyFolderMode, szBuf, 10);
	WritePrivateProfileString(_T("Option"), _T("HashFileOutToCopyFolderMode"), szBuf, cpInIniFile);

#if !defined(UNICODE) && _MSC_VER < 1400
	if (!IsWin95())
	{
#endif

		ITOT_FUNC(dwHashCpuMode, szBuf, 10);
		WritePrivateProfileString(_T("Option"), _T("HashCpuMode"), szBuf, cpInIniFile);

#if !defined(UNICODE) && _MSC_VER < 1400
	}
#endif

	if (tagFileListWindow1.clListTextColor[0] != 0x00FF0000)
	{
		STPRINTF_FUNC(szBuf, _T("0x%08X"), tagFileListWindow1.clListTextColor[0]);
		WritePrivateProfileString(_T("FileList"), _T("ListViewTextColorHashComp"), szBuf, cpInIniFile);
	}

	if (tagFileListWindow1.clListTextColor[1] != 0x000000FF)
	{
		STPRINTF_FUNC(szBuf, _T("0x%08X"), tagFileListWindow1.clListTextColor[1]);
		WritePrivateProfileString(_T("FileList"), _T("ListViewTextColorHashCompFoul"), szBuf, cpInIniFile);
	}

	if (tagFileListWindow1.clListTextColor[2] != 0x000040FF)
	{
		STPRINTF_FUNC(szBuf, _T("0x%08X"), tagFileListWindow1.clListTextColor[2]);
		WritePrivateProfileString(_T("FileList"), _T("ListViewTextColorFileError"), szBuf, cpInIniFile);
	}
	return;
}
