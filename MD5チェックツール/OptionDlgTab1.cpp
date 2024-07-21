// OptionDlg.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "..\shared\cpuid.h"

// 外部ファイルの変数定義します。
extern OptionWindow tagOptionWindow1;
extern MainWindow tagMainWindow1;
extern DWORD dwAppFrag;
extern int nEditMarginX[4];
extern int nEditMarginY[4];
extern int nStringX[3];
extern int nStringY[3];

#define OPTIONDLG_X 230
#define OPTIONDLG_Y 237
#define OPTIONCLDDLG_X 215
#define OPTIONCLDDLG_Y 195


LRESULT CALLBACK OptionDlgTab1_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagOptionWindow1.nWindowMessage)
	{
		switch (wParam)
		{
		case APP_MASSAGE_DPICHANGE:
			return OptionDlgTab1_OnMessageDpiChange(hWnd, wParam, lParam);
		}
		return FALSE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		return OptionDlgTab1_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return OptionDlgTab1_OnCommand(hWnd, wParam, lParam);
	case WM_SIZE:
		return OptionDlgTab1_OnSize(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return OptionDlgTab1_OnSysColorChange(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return OptionDlgTab1_OnThemeChanged(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlgTab1_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	DWORD dwRet = 0;
	int i = 0;

	SetEnableThemeDialogTexture(hWnd);

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Group); i++) {
		tagOptionWindow1.hTab1Group[i] = GetDlgItem(hWnd, IDC_OPTIONTAB1_GROUP1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Checkbox); i++) {
		tagOptionWindow1.hTab1Checkbox[i] = GetDlgItem(hWnd, IDC_OPTIONTAB1_CHECK1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Combo); i++) {
		tagOptionWindow1.hTab1Combo[i] = GetDlgItem(hWnd, IDC_OPTIONTAB1_COMBO1 + i);
	}

	tagOptionWindow1.dwHashType = tagMainWindow1.tagHashThread1.dwHashType;
	for (dwRet = 0; dwRet < MAX_HASH_TYPE; dwRet++)
	{
		if (dwRet == tagOptionWindow1.dwHashType)
		{
			TCHAR szBuf[64] = _T("");
			TCHAR* ptr = NULL;

			ptr = qtcscpy(szBuf, HashThread_GetHashName(dwRet));
			*ptr++ = ' ';
			qtcscpy(ptr, _T("(選択中)"));
			SendMessage(tagOptionWindow1.hTab1Combo[0], CB_ADDSTRING, 0, (LPARAM)szBuf);
		}
		else {
			SendMessage(tagOptionWindow1.hTab1Combo[0], CB_ADDSTRING, 0, (LPARAM)HashThread_GetHashName(dwRet));
		}
	}

	SendMessage(tagOptionWindow1.hTab1Combo[0], CB_SETCURSEL, (WPARAM)tagOptionWindow1.dwHashType, 0);

	dwRet = dwAppFrag & APP_HASHOUT_LOWER ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[0], BM_SETCHECK, (WPARAM)dwRet, 0);

	dwRet = dwAppFrag & APP_ENABLE_MULTILINE ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[1], BM_SETCHECK, (WPARAM)dwRet, 0);

	dwRet = dwAppFrag & APP_ECOQOS ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[2], BM_SETCHECK, (WPARAM)dwRet, 0);
	i = IsWin10OrGreater() ? TRUE : FALSE;
	EnableWindow(tagOptionWindow1.hTab1Checkbox[2], i);

	dwRet = dwAppFrag & APP_WINDOWALPHA ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[3], BM_SETCHECK, (WPARAM)dwRet, 0);
#if !defined(UNICODE) && _MSC_VER < 1500
	i = IsWin2kOrGreater() ? TRUE : FALSE;
	EnableWindow(tagOptionWindow1.hTab1Checkbox[3], i);
#endif

	dwRet = dwAppFrag & APP_ENABLE_TOPMOST ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[4], BM_SETCHECK, (WPARAM)dwRet, 0);

	dwRet = dwAppFrag & APP_DISABLE_MULTIFILE_UPDATE_DRAW ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[5], BM_SETCHECK, (WPARAM)dwRet, 0);

	dwRet = dwAppFrag & APP_ENABLE_FILELIST ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[6], BM_SETCHECK, (WPARAM)dwRet, 0);

	dwRet = dwAppFrag & APP_ENABLE_DEBUG ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[7], BM_SETCHECK, (WPARAM)dwRet, 0);

	dwRet = dwAppFrag & APP_ENABLE_MINIMAM_HANDLE ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab1Checkbox[8], BM_SETCHECK, (WPARAM)dwRet, 0);
	return TRUE;
}

LRESULT OptionDlgTab1_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab1_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab1_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab1_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab1_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	int nArg[4] = { 0 };
	int nRet = 0;
	const int cx = LOWORD(lParam);
	//const int cy = HIWORD(lParam);
	int i = 0;

	nArg[0] = MDIVX(5);
	nArg[1] = MDIVY(5);
	nArg[2] = cx - MDIVX(OPTIONCLDDLG_X - 205);
	nArg[3] = MDIVY(35);
	MoveWindow(tagOptionWindow1.hTab1Group[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(12);
	nArg[1] = MDIVY(20);
	nArg[2] = MDIVX(100);
	nArg[3] = MDIVY(200);
	MoveWindow(tagOptionWindow1.hTab1Combo[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(6);
	//nArg[1] = MDIVY(33);
	nArg[2] = MDIVX(204);
	nArg[3] = nStringY[0] + nEditMarginY[0];

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Checkbox); i++)
	{
		nRet = i * 15 + 50;
		nArg[1] = MDIVY(nRet);
		MoveWindow(tagOptionWindow1.hTab1Checkbox[i], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	}
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT OptionDlgTab1_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	int i = 0;

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Group); i++) {
		SendMessage(tagOptionWindow1.hTab1Group[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Checkbox); i++) {
		SendMessage(tagOptionWindow1.hTab1Checkbox[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Combo); i++) {
		SendMessage(tagOptionWindow1.hTab1Combo[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}
	return TRUE;
}

LRESULT OptionDlgTab1_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab1_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}


LRESULT OptionDlgTab1_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab1_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Group); i++) {
		SendMessage(tagOptionWindow1.hTab1Group[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Combo); i++) {
		SendMessage(tagOptionWindow1.hTab1Combo[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab1Checkbox); i++) {
		SendMessage(tagOptionWindow1.hTab1Checkbox[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}
	return TRUE;
}
