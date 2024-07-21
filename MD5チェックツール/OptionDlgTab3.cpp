// OptionDlg.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "..\shared\cpuid.h"

// 外部ファイルの変数定義します。
extern OptionWindow tagOptionWindow1;
extern MainWindow tagMainWindow1;
extern DWORD dwAppFrag;
extern DWORD dwHashCpuMode;
extern unsigned int OPENSSL_ia32cap_P1[4];
extern int nEditMarginX[4];
extern int nEditMarginY[4];
extern int nStringX[3];
extern int nStringY[3];

#define OPTIONDLG_X 230
#define OPTIONDLG_Y 237
#define OPTIONCLDDLG_X 215
#define OPTIONCLDDLG_Y 195


LRESULT CALLBACK OptionDlgTab3_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagOptionWindow1.nWindowMessage)
	{
		switch (wParam)
		{
		case APP_MASSAGE_DPICHANGE:
			return OptionDlgTab3_OnMessageDpiChange(hWnd, wParam, lParam);
		}
		return FALSE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		return OptionDlgTab3_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return OptionDlgTab3_OnCommand(hWnd, wParam, lParam);
	case WM_SIZE:
		return OptionDlgTab3_OnSize(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return OptionDlgTab3_OnSysColorChange(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return OptionDlgTab3_OnThemeChanged(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlgTab3_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	unsigned int OPENSSL_ia32cap_P_Edit[4] = { 0 };
	DWORD dwRet = 0;
	int i = 0;
	int y = 0;

	memcpy(OPENSSL_ia32cap_P_Edit, OPENSSL_ia32cap_P1, sizeof(OPENSSL_ia32cap_P1));
	SetEnableThemeDialogTexture(hWnd);

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Group); i++) {
		tagOptionWindow1.hTab3Group[i] = GetDlgItem(hWnd, IDC_OPTIONTAB3_GROUP1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Checkbox); i++) {
		tagOptionWindow1.hTab3Checkbox[i] = GetDlgItem(hWnd, IDC_OPTIONTAB3_CHECK1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Static); i++) {
		tagOptionWindow1.hTab3Static[i] = GetDlgItem(hWnd, IDC_OPTIONTAB3_STATIC1 + i);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Static1); i++)
	{
		tagOptionWindow1.hTab3Static1[i][0] = GetDlgItem(hWnd, i * 10 + IDC_OPTIONTAB3_STATIC1_1);
		tagOptionWindow1.hTab3Static1[i][1] = GetDlgItem(hWnd, i * 10 + IDC_OPTIONTAB3_STATIC1_2);
		tagOptionWindow1.hTab3Static1[i][2] = GetDlgItem(hWnd, i * 10 + IDC_OPTIONTAB3_STATIC1_3);
		tagOptionWindow1.hTab3Static1[i][3] = GetDlgItem(hWnd, i * 10 + IDC_OPTIONTAB3_STATIC1_4);
		tagOptionWindow1.hTab3Static1[i][4] = GetDlgItem(hWnd, i * 10 + IDC_OPTIONTAB3_STATIC1_5);
	}


	// SSE4.2 crc32c
	if (OPENSSL_ia32cap_P_Edit[1] & 0x00100000) {
		i = TRUE;
	}
	else {
		i = FALSE;
	}
	EnableWindow(tagOptionWindow1.hTab3Static1[0][4], i);


	// SSE
	if (OPENSSL_ia32cap_P_Edit[1] & 0x00000200) {
		y = TRUE;
	}
	else {
		y = FALSE;
	}
	EnableWindow(tagOptionWindow1.hTab3Checkbox[0], i || y);


#if !defined(UNICODE) && _MSC_VER < 1400
	if (IsWin95())
	{
		EnableWindow(tagOptionWindow1.hTab3Static1[0][0], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[0][1], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[0][2], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[0][3], FALSE);

		EnableWindow(tagOptionWindow1.hTab3Checkbox[1], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][0], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][1], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][2], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][3], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][4], FALSE);

		EnableWindow(tagOptionWindow1.hTab3Checkbox[2], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][0], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][1], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][2], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][3], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][4], FALSE);
	}
	else
	{
#endif

		EnableWindow(tagOptionWindow1.hTab3Static1[0][0], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[0][1], i);

#if _MSC_VER < 1400
		EnableWindow(tagOptionWindow1.hTab3Static1[0][2], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[0][3], FALSE);
#else
		EnableWindow(tagOptionWindow1.hTab3Static1[0][2], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[0][3], i);
#endif

		if ((OPENSSL_ia32cap_P_Edit[1] & 0x50000000) == 0x50000000) {
			i = TRUE;
		}
		else {
			i = FALSE;
		}
		EnableWindow(tagOptionWindow1.hTab3Checkbox[1], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][0], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][1], i);

#if _MSC_VER < 1600
		EnableWindow(tagOptionWindow1.hTab3Static1[1][2], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][3], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][4], FALSE);
#else
		EnableWindow(tagOptionWindow1.hTab3Static1[1][2], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][3], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[1][4], FALSE);
#endif

		if (OPENSSL_ia32cap_P_Edit[2] & 0x20000000) {
			i = TRUE;
		}
		else {
			i = FALSE;
		}
		EnableWindow(tagOptionWindow1.hTab3Checkbox[2], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][0], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][1], i);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][2], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][3], FALSE);
		EnableWindow(tagOptionWindow1.hTab3Static1[2][4], FALSE);

#if !defined(UNICODE) && _MSC_VER < 1400
	}
#endif


	// SSE
	dwRet = dwHashCpuMode & 0x00000001 ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab3Checkbox[0], BM_SETCHECK, (WPARAM)dwRet, 0);

	// AVX
	dwRet = dwHashCpuMode & 0x00000002 ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab3Checkbox[1], BM_SETCHECK, (WPARAM)dwRet, 0);

	// SHAEXT
	dwRet = dwHashCpuMode & 0x00000004 ? BST_CHECKED : BST_UNCHECKED;
	SendMessage(tagOptionWindow1.hTab3Checkbox[2], BM_SETCHECK, (WPARAM)dwRet, 0);

	tagOptionWindow1.dwRadioState[0] = dwHashCpuMode;

	return TRUE;
}

LRESULT OptionDlgTab3_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);

	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDC_OPTIONTAB3_CHECK1:
		return OptionDlgTab3_OnCheck1(hWnd, wParam, lParam);
	case IDC_OPTIONTAB3_CHECK2:
		return OptionDlgTab3_OnCheck2(hWnd, wParam, lParam);
	case IDC_OPTIONTAB3_CHECK3:
		return OptionDlgTab3_OnCheck3(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT OptionDlgTab3_OnCheck1(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nRet = 0;

	nRet = SendMessage(tagOptionWindow1.hTab3Checkbox[0], BM_GETCHECK, 0, 0) != 0;
	if (nRet) {
		tagOptionWindow1.dwRadioState[0] |= 0x00000001;
	}
	else {
		tagOptionWindow1.dwRadioState[0] &= ~(0x00000001);
	}
	return TRUE;
}

LRESULT OptionDlgTab3_OnCheck2(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nRet = 0;

	nRet = SendMessage(tagOptionWindow1.hTab3Checkbox[1], BM_GETCHECK, 0, 0) != 0;
	if (nRet) {
		tagOptionWindow1.dwRadioState[0] |= 0x00000002;
	}
	else {
		tagOptionWindow1.dwRadioState[0] &= ~(0x00000002);
	}
	return TRUE;
}

LRESULT OptionDlgTab3_OnCheck3(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nRet = 0;

	nRet = SendMessage(tagOptionWindow1.hTab3Checkbox[2], BM_GETCHECK, 0, 0) != 0;
	if (nRet) {
		tagOptionWindow1.dwRadioState[0] |= 0x00000004;
	}
	else {
		tagOptionWindow1.dwRadioState[0] &= ~(0x00000004);
	}
	return TRUE;
}

LRESULT OptionDlgTab3_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab3_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab3_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab3_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	int nArg[4] = { 0 };
	const int cx = LOWORD(lParam);
	//const int cy = HIWORD(lParam);
	int i = 0;

	nArg[0] = MDIVX(5);
	nArg[1] = MDIVY(5);
	nArg[2] = cx - MDIVX(OPTIONCLDDLG_X - 205);
	nArg[3] = MDIVY(100);
	MoveWindow(tagOptionWindow1.hTab3Group[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(12);
	nArg[2] = MDIVX(58);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Checkbox); i++)
	{
		nArg[1] = MDIVY(i * 15 + 20);
		MoveWindow(tagOptionWindow1.hTab3Checkbox[i], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	}

	nArg[2] = MDIVX(30);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Checkbox); i++)
	{
		nArg[1] = MDIVY(i * 15 + 20);
		nArg[0] = MDIVX(25 * 0 + 70);
		MoveWindow(tagOptionWindow1.hTab3Static1[i][0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

		nArg[0] = MDIVX(25 * 1 + 70);
		MoveWindow(tagOptionWindow1.hTab3Static1[i][1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

		nArg[0] = MDIVX(25 * 2 + 70);
		MoveWindow(tagOptionWindow1.hTab3Static1[i][2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

		nArg[0] = MDIVX(25 * 3 + 67);
		MoveWindow(tagOptionWindow1.hTab3Static1[i][3], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

		nArg[0] = MDIVX(25 * 4 + 72);
		MoveWindow(tagOptionWindow1.hTab3Static1[i][4], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	}

	nArg[0] = MDIVX(12);
	nArg[2] = MDIVX(192);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Static); i++)
	{
		nArg[1] = MDIVY(i * 15 + 70);
		MoveWindow(tagOptionWindow1.hTab3Static[i], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	}
	
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT OptionDlgTab3_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	int i = 0;

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Group); i++) {
		SendMessage(tagOptionWindow1.hTab3Group[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Checkbox); i++) {
		SendMessage(tagOptionWindow1.hTab3Checkbox[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Static); i++) {
		SendMessage(tagOptionWindow1.hTab3Static[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Static1); i++)
	{
		SendMessage(tagOptionWindow1.hTab3Static1[i][0], WM_SYSCOLORCHANGE, wParam, lParam);
		SendMessage(tagOptionWindow1.hTab3Static1[i][1], WM_SYSCOLORCHANGE, wParam, lParam);
		SendMessage(tagOptionWindow1.hTab3Static1[i][2], WM_SYSCOLORCHANGE, wParam, lParam);
		SendMessage(tagOptionWindow1.hTab3Static1[i][3], WM_SYSCOLORCHANGE, wParam, lParam);
		SendMessage(tagOptionWindow1.hTab3Static1[i][4], WM_SYSCOLORCHANGE, wParam, lParam);
	}
	return TRUE;
}

LRESULT OptionDlgTab3_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab3_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}


LRESULT OptionDlgTab3_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT OptionDlgTab3_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Group); i++) {
		SendMessage(tagOptionWindow1.hTab3Group[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Checkbox); i++) {
		SendMessage(tagOptionWindow1.hTab3Checkbox[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Static); i++) {
		SendMessage(tagOptionWindow1.hTab3Static[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}

	for (i = 0; i < SIZEOF_NUM(tagOptionWindow1.hTab3Static1); i++)
	{
		SendMessage(tagOptionWindow1.hTab3Static1[i][0], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
		SendMessage(tagOptionWindow1.hTab3Static1[i][1], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
		SendMessage(tagOptionWindow1.hTab3Static1[i][2], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
		SendMessage(tagOptionWindow1.hTab3Static1[i][3], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
		SendMessage(tagOptionWindow1.hTab3Static1[i][4], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}
	return TRUE;
}
