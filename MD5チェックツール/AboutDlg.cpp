// AboutDlg.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "AboutDlg.h"

// グローバル変数の定義します:
AboutWindow tagAboutWindow1;

// 外部ファイルの変数定義します。
extern MainWindow tagMainWindow1;
extern DWORD dwAppFrag;
extern BOOL g_darkModeSupported;
extern BOOL g_darkModeEnabled;
extern int nEditMarginX[3];
extern int nEditMarginY[3];
extern int nStringX[3];
extern int nStringY[3];

#define ABOUTDLG_X 220
#define ABOUTDLG_Y 205

// バージョン情報ボックスのメッセージ ハンドラです。
LRESULT CALLBACK AboutDlg_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagAboutWindow1.nWindowMessage)
	{
		switch (wParam)
		{
		case APP_MESSAGE_WINDOWSET:
			return AboutDlg_OnMessageWindowSet(hWnd, wParam, lParam);
		case APP_MASSAGE_DPICHANGE:
			return AboutDlg_OnMessageDpiChange(hWnd, wParam, lParam);
		}
		return FALSE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		return AboutDlg_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return AboutDlg_OnCommand(hWnd, wParam, lParam);
	case WM_GETMINMAXINFO:
		return AboutDlg_OnGetMinMaxInfo(hWnd, wParam, lParam);
	case WM_SIZE:
		return AboutDlg_OnSize(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return AboutDlg_OnSysColorChange(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return AboutDlg_OnThemeChanged(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT AboutDlg_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	static TCHAR szAppTitle[MAX_LOADSTRING];
	int i = 0;

	dwAppFrag |= APP_WINDOW_NOALPHA;

	tagAboutWindow1.dwStructSize = sizeof(tagAboutWindow1);
	tagAboutWindow1.hWnd = hWnd;

	if (~dwAppFrag & APP_ENABLE_MINIMAM_HANDLE)
	{
		SetEnableThemeDialogTexture(hWnd);
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)tagMainWindow1.hIcon[0]);
		DarkMode_SetWindowTheme(hWnd);
	}

	tagAboutWindow1.nWindowMessage = RegisterWindowMessage(_T("AboutDlg"));
	tagAboutWindow1.hGroup[0] = GetDlgItem(hWnd, IDC_ABOUT_GROUP1);
	for (i = 0; i < SIZEOF_NUM(tagAboutWindow1.hStatic); i++) {
		tagAboutWindow1.hStatic[i] = GetDlgItem(hWnd, IDC_ABOUT_STATIC1 + i);
	}
	tagAboutWindow1.hIDOK = GetDlgItem(hWnd, IDOK);

	AboutDlg_GetDpi();

	if (*szAppTitle == '\0')
	{
		TCHAR* ptr;

		i = LoadString(tagMainWindow1.hInst, IDS_APP_TITLE, szAppTitle, MAX_LOADSTRING - 1);
		ptr = szAppTitle + i;

#ifdef _M_X64
		ptr = qtcscpy(ptr, _T(" (64ビット版)"));
#else
# ifndef UNICODE
		ptr = qtcscpy(ptr, _T(" (マルチバイト版)"));
# endif
#endif

	}
	SetWindowText(tagAboutWindow1.hStatic[0], szAppTitle);

	// バージョンの更新
	SetWindowText(tagAboutWindow1.hStatic[1], tagMainWindow1.pVersion);

	// Copyrightの更新
	SetWindowText(tagAboutWindow1.hStatic[2], tagMainWindow1.pCopyRight);

	// OSバージョンの更新
	AboutDlg_SetOSText(tagAboutWindow1.hStatic[4], FALSE);

	PostMessage(hWnd, tagAboutWindow1.nWindowMessage, APP_MESSAGE_WINDOWSET, 0);
	MessageBeep(MB_ICONASTERISK);
	return TRUE;
}

LRESULT AboutDlg_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);

	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDOK:
		return AboutDlg_OnIdOk(hWnd, wParam, lParam);
	case IDCANCEL:
		return AboutDlg_OnIdCancel(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT AboutDlg_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	dwAppFrag &= ~(APP_WINDOW_NOALPHA);
	EndDialog(hWnd, LOWORD(wParam));
	tagAboutWindow1.hWnd = NULL;
	return TRUE;
}

LRESULT AboutDlg_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return AboutDlg_OnIdOk(hWnd, wParam, lParam);
}

LRESULT AboutDlg_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	((MINMAXINFO*)lParam)->ptMinTrackSize.x = MDIVX(ABOUTDLG_X) + tagMainWindow1.dwWindowRectX;
	((MINMAXINFO*)lParam)->ptMaxTrackSize.x = MDIVX(ABOUTDLG_X) + tagMainWindow1.dwWindowRectX;
	((MINMAXINFO*)lParam)->ptMinTrackSize.y = MDIVY(ABOUTDLG_Y) + tagMainWindow1.dwWindowRectY;
	return TRUE;
}

LRESULT AboutDlg_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int nArg[4] = { 0 };
	int i = 0;

	nArg[0] = MDIVX(32);
	nArg[1] = MDIVY(11);
	nArg[2] = MDIVX(180);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagAboutWindow1.hStatic[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(32);
	nArg[1] = MDIVY(30);
	nArg[2] = MDIVX(178);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagAboutWindow1.hStatic[1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(32);
	nArg[1] = MDIVY(42);
	nArg[2] = MDIVX(178);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagAboutWindow1.hStatic[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(12);
	nArg[1] = MDIVY(54);
	nArg[2] = MDIVX(178);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagAboutWindow1.hStatic[3], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(18);
	nArg[1] = MDIVY(66);
	nArg[2] = MDIVX(178);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagAboutWindow1.hStatic[4], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(18);
	nArg[1] = MDIVY(78);
	nArg[2] = MDIVX(178);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	MoveWindow(tagAboutWindow1.hStatic[5], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(6);
	nArg[1] = MDIVY(90);
	nArg[2] = MDIVX(208);
	nArg[3] = MDIVY(80);
	MoveWindow(tagAboutWindow1.hGroup[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(12);
	nArg[2] = MDIVX(196);
	nArg[3] = nStringY[0] + nEditMarginY[0];
	for (i = 5; i < SIZEOF_NUM(tagAboutWindow1.hStatic); i++)
	{
		nArg[1] = MDIVY(i * 12 + 32);
		MoveWindow(tagAboutWindow1.hStatic[i], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);
	}

	nArg[0] = MDIVX(80);
	nArg[1] = MDIVY(182);
	nArg[2] = MDIVX(60);
	nArg[3] = nStringY[2] + nEditMarginY[0];
	MoveWindow(tagAboutWindow1.hIDOK, nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT AboutDlg_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	int i = 0;

	SendMessage(tagAboutWindow1.hGroup[0], WM_SYSCOLORCHANGE, wParam, lParam);
	for (i = 0; i < SIZEOF_NUM(tagAboutWindow1.hStatic); i++) {
		SendMessage(tagAboutWindow1.hStatic[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}
	SendMessage(tagAboutWindow1.hIDOK, WM_SYSCOLORCHANGE, wParam, lParam);
	return TRUE;
}

LRESULT AboutDlg_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (g_darkModeSupported)
	{
		AllowDarkModeForWindow(hWnd, g_darkModeEnabled);
		RefreshTitleBarThemeColor(hWnd);
		UpdateWindow(hWnd);
	}
	return TRUE;
}


LRESULT AboutDlg_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	AboutDlg_SetWindowMove();
	return TRUE;
}

LRESULT AboutDlg_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	AboutDlg_GetDpi();
	return TRUE;
}


VOID AboutDlg_GetDpi(VOID)
{
	int i = 0;

	SendMessage(tagAboutWindow1.hGroup[0], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	SendMessage(tagAboutWindow1.hStatic[0], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[1], 0);
	for (i = 1; i < SIZEOF_NUM(tagAboutWindow1.hStatic); i++) {
		SendMessage(tagAboutWindow1.hStatic[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}
	SendMessage(tagAboutWindow1.hIDOK, WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	AboutDlg_SetWindowMove();
}

VOID AboutDlg_SetWindowMove(VOID)
{
	RECT rect = { 0 };

	GetWindowRect(tagAboutWindow1.hWnd, &rect);
	MoveWindow(tagAboutWindow1.hWnd, rect.left, rect.top, MDIVX(ABOUTDLG_X) + tagMainWindow1.dwWindowRectX, MDIVY(ABOUTDLG_Y) + tagMainWindow1.dwWindowRectY, TRUE);
}

VOID AboutDlg(HWND hWnd)
{
	SetFocus(tagMainWindow1.hButton[0]);
	if (tagAboutWindow1.hWnd == NULL)
	{
		tagAboutWindow1.hWnd = CreateDialog(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, (DLGPROC)AboutDlg_WndProc);

		if (tagAboutWindow1.hWnd != NULL)
		{
			ShowWindow(tagAboutWindow1.hWnd, SW_SHOW);
			UpdateWindow(tagAboutWindow1.hWnd);
		}
	}
	else {
		SetForegroundWindow(tagAboutWindow1.hWnd);
	}
}

VOID AboutDlg_SetOSText(HWND hWnd, BOOL VerifyVersion)
{
	TCHAR szBuf[100] = _T("取得に失敗しました。");
	const size_t nLen1 = SIZEOF_NUM(szBuf) - 1;

	if (GetOSName(szBuf, nLen1, TRUE, VerifyVersion)) {
		SetWindowText(hWnd, szBuf);
	}
}
