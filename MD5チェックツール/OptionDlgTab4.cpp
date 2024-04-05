// OptionDlg.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "..\shared\cpuid.h"

// 外部ファイルの変数定義します。
extern OptionWindow tagOptionWindow1;
extern MainWindow tagMainWindow1;
extern DWORD dwAppFrag;
extern DWORD dwHashFileOutToCopyFolderMode;
extern int nEditMarginX[4];
extern int nEditMarginY[4];
extern int nStringX[3];
extern int nStringY[3];

#define OPTIONDLG_X 230
#define OPTIONDLG_Y 237
#define OPTIONCLDDLG_X 215
#define OPTIONCLDDLG_Y 195


LRESULT CALLBACK OptionDlgTab4_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagOptionWindow1.nWindowMessage)
	{
		switch (wParam)
		{
		case APP_MASSAGE_DPICHANGE:
			return OptionDlgTab4_OnMessageDpiChange(hWnd, wParam, lParam);
		}
		return FALSE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		return OptionDlgTab4_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return OptionDlgTab4_OnCommand(hWnd, wParam, lParam);
	case WM_SIZE:
		return OptionDlgTab4_OnSize(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return OptionDlgTab4_OnSysColorChange(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return OptionDlgTab4_OnThemeChanged(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlgTab4_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	SetEnableThemeDialogTexture(hWnd);

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab4Group); i++) {
		tagOptionWindow1.hTab4Group[i] = GetDlgItem(hWnd, IDC_OPTIONTAB4_GROUP1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab4Radio); i++) {
		tagOptionWindow1.hTab4Radio[i] = GetDlgItem(hWnd, IDC_OPTIONTAB4_RADIO1 + i);
	}

	SendMessage(tagOptionWindow1.hTab4Radio[dwHashFileOutToCopyFolderMode], BM_SETCHECK, BST_CHECKED, 0);
	return TRUE;
}

LRESULT OptionDlgTab4_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);

	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDC_OPTIONTAB4_RADIO1:// 絶対パス (初期設定)
		return OptionDlgTab4_OnRadio1(hWnd, wParam, lParam);
	case IDC_OPTIONTAB4_RADIO2:// 同じフォルダー
		return OptionDlgTab4_OnRadio2(hWnd, wParam, lParam);
	case IDC_OPTIONTAB4_RADIO3:// 1つ上のフォルダー
		return OptionDlgTab4_OnRadio3(hWnd, wParam, lParam);
	case IDC_OPTIONTAB4_RADIO4:// 2つ上のフォルダー
		return OptionDlgTab4_OnRadio4(hWnd, wParam, lParam);
	case IDC_OPTIONTAB4_RADIO5:// 3つ上のフォルダー
		return OptionDlgTab4_OnRadio5(hWnd, wParam, lParam);
	case IDC_OPTIONTAB4_RADIO6:// 未実装
		return OptionDlgTab4_OnRadio6(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlgTab4_OnRadio1(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagOptionWindow1.dwRadioState[1] = 0;
	return TRUE;
}

LRESULT OptionDlgTab4_OnRadio2(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagOptionWindow1.dwRadioState[1] = 1;
	return TRUE;
}

LRESULT OptionDlgTab4_OnRadio3(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagOptionWindow1.dwRadioState[1] = 2;
	return TRUE;
}

LRESULT OptionDlgTab4_OnRadio4(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagOptionWindow1.dwRadioState[1] = 3;
	return TRUE;
}

LRESULT OptionDlgTab4_OnRadio5(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagOptionWindow1.dwRadioState[1] = 4;
	return TRUE;
}

LRESULT OptionDlgTab4_OnRadio6(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagOptionWindow1.dwRadioState[1] = 5;
	return TRUE;
}

LRESULT OptionDlgTab4_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab4_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab4_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab4_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	int nArg[4] = { 0 };
	const int cx = LOWORD(lParam);
	//const int cy = HIWORD(lParam);
	int i;

	nArg[0] = MDIVX(5);
	nArg[1] = MDIVY(5);
	nArg[2] = cx - MDIVX(OPTIONCLDDLG_X - 205);
	nArg[3] = MDIVY(110);
	MoveWindow(tagOptionWindow1.hTab4Group[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(12);
	//nArg[1] = MDIVY(80);
	nArg[2] = MDIVX(192);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab4Radio); i++)
	{
		nArg[1] = MDIVY(i * 15 + 20);
		MoveWindow(tagOptionWindow1.hTab4Radio[i], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	}
	
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT OptionDlgTab4_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	int i = 0;

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab4Group); i++) {
		SendMessage(tagOptionWindow1.hTab4Group[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab4Radio); i++) {
		SendMessage(tagOptionWindow1.hTab4Radio[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}
	return TRUE;
}

LRESULT OptionDlgTab4_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab4_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}


LRESULT OptionDlgTab4_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab4_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab4Group); i++) {
		SendMessage(tagOptionWindow1.hTab4Group[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab4Radio); i++) {
		SendMessage(tagOptionWindow1.hTab4Radio[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}
	return TRUE;
}
