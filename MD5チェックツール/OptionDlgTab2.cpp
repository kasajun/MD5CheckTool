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


LRESULT CALLBACK OptionDlgTab2_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagOptionWindow1.nWindowMessage)
	{
		switch (wParam)
		{
		case APP_MASSAGE_DPICHANGE:
			return OptionDlgTab2_OnMessageDpiChange(hWnd, wParam, lParam);
		}
		return FALSE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		return OptionDlgTab2_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return OptionDlgTab2_OnCommand(hWnd, wParam, lParam);
	case WM_SIZE:
		return OptionDlgTab2_OnSize(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return OptionDlgTab2_OnSysColorChange(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return OptionDlgTab2_OnThemeChanged(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlgTab2_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	SetEnableThemeDialogTexture(hWnd);

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Checkbox); i++) {
		tagOptionWindow1.hTab2Checkbox[i] = GetDlgItem(hWnd, IDC_OPTIONTAB2_CHECK1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Group); i++) {
		tagOptionWindow1.hTab2Group[i] = GetDlgItem(hWnd, IDC_OPTIONTAB2_GROUP1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Edit); i++) {
		tagOptionWindow1.hTab2Edit[i] = GetDlgItem(hWnd, IDC_OPTIONTAB2_EDIT1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Button); i++) {
		tagOptionWindow1.hTab2Button[i] = GetDlgItem(hWnd, IDC_OPTIONTAB2_BUTTON1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Static); i++) {
		tagOptionWindow1.hTab2Static[i] = GetDlgItem(hWnd, IDC_OPTIONTAB2_STATIC1 + i);
	}

	SendMessage(tagOptionWindow1.hTab2Checkbox[0], BM_SETCHECK, dwAppFrag & APP_DISABLE_HASH_NOASM ? BST_CHECKED : BST_UNCHECKED, 0);
	SendMessage(tagOptionWindow1.hTab2Checkbox[1], BM_SETCHECK, dwAppFrag & APP_ENABLE_LARGE_BUFFERSIZE ? BST_CHECKED : BST_UNCHECKED, 0);
	SendMessage(tagOptionWindow1.hTab2Checkbox[2], BM_SETCHECK, dwAppFrag & APP_FILE_NOCACHE ? BST_CHECKED : BST_UNCHECKED, 0);
	SendMessage(tagOptionWindow1.hTab2Checkbox[3], BM_SETCHECK, dwAppFrag & APP_ENABLE_HIDDENFILE ? BST_CHECKED : BST_UNCHECKED, 0);
	SendMessage(tagOptionWindow1.hTab2Checkbox[4], BM_SETCHECK, dwAppFrag & APP_ENABLE_HASHFILE_WRITETIME ? BST_CHECKED : BST_UNCHECKED, 0);
	SendMessage(tagOptionWindow1.hTab2Checkbox[5], BM_SETCHECK, dwAppFrag & APP_HASHFILE_ENABLE_HASH ? BST_CHECKED : BST_UNCHECKED, 0);

	GetCheckSumFile(tagOptionWindow1.pCheckSumFile);
	SetWindowText(tagOptionWindow1.hTab2Edit[0], tagOptionWindow1.pCheckSumFile);
	SendMessage(tagOptionWindow1.hTab2Edit[0], EM_LIMITTEXT, MAX_LOADSTRING - 1, 0);
	return TRUE;
}

LRESULT OptionDlgTab2_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);

	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDC_OPTIONTAB2_BUTTON1:
		return OptionDlgTab2_OnButton1(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlgTab2_OnButton1(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	i = LoadString(tagMainWindow1.hInst, IDS_CHACKSUM_FILENAME, tagOptionWindow1.pBuf, MAX_STRINGTABLE);
	if (i > 0)
	{
		SetWindowText(tagOptionWindow1.hTab2Edit[0], tagOptionWindow1.pBuf);
		SetFocus(tagOptionWindow1.hTab2Edit[0]);
		SendMessage(tagOptionWindow1.hTab2Edit[0], EM_SETSEL, (WPARAM)i, (LPARAM)i);
		dwAppFrag &= ~APP_EDIT_CHECKSUMFILE;
	}
	return TRUE;
}

LRESULT OptionDlgTab2_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab2_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab2_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab2_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	int nArg[4] = { 0 };
	const int cx = LOWORD(lParam);
	//const int cy = HIWORD(lParam);
	int i = 0;

	nArg[0] = MDIVX(6);
	//nArg[1] = MDIVY(33);
	nArg[2] = MDIVX(204);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Checkbox); i++)
	{
		nArg[1] = MDIVY(i * 15 + 15);
		MoveWindow(tagOptionWindow1.hTab2Checkbox[i], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	}

	nArg[0] = MDIVX(5);
	nArg[1] = MDIVY(110);
	nArg[2] = cx - MDIVX(OPTIONCLDDLG_X - 205);
	nArg[3] = MDIVY(50);
	MoveWindow(tagOptionWindow1.hTab3Group[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(10);
	nArg[1] = MDIVY(125);
	nArg[2] = MDIVX(195);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagOptionWindow1.hTab2Edit[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(135);
	nArg[1] = MDIVY(140);
	nArg[2] = MDIVX(70);
	nArg[3] = nStringY[2] + nEditMarginY[0];
	MoveWindow(tagOptionWindow1.hTab2Button[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(6);
	nArg[1] = MDIVY(170);
	nArg[2] = MDIVX(204);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagOptionWindow1.hTab2Static[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT OptionDlgTab2_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	int i = 0;


	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Checkbox); i++) {
		SendMessage(tagOptionWindow1.hTab2Checkbox[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Group); i++) {
		SendMessage(tagOptionWindow1.hTab2Group[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Edit); i++) {
		SendMessage(tagOptionWindow1.hTab2Edit[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Button); i++) {
		SendMessage(tagOptionWindow1.hTab2Button[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Static); i++) {
		SendMessage(tagOptionWindow1.hTab2Static[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}
	return TRUE;
}

LRESULT OptionDlgTab2_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab2_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}


LRESULT OptionDlgTab2_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab2_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Checkbox); i++) {
		SendMessage(tagOptionWindow1.hTab2Checkbox[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Group); i++) {
		SendMessage(tagOptionWindow1.hTab2Group[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Edit); i++) {
		SendMessage(tagOptionWindow1.hTab2Edit[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Button); i++) {
		SendMessage(tagOptionWindow1.hTab2Button[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab2Static); i++) {
		SendMessage(tagOptionWindow1.hTab2Static[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}
	return TRUE;
}
