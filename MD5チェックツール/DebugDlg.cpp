// DebugDlg.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "..\shared\WindowSize.h"


// グローバル変数の定義します:
DebugWindow tagDebugWindow1;
static TCHAR szDebugBuf[20000];
static TCHAR szFuncBuf[2040];
static OSVERSIONINFOEX osVerInfo[2] = { 0 };


// 外部ファイルの変数定義します。
extern MainWindow tagMainWindow1;
extern FileListWindow tagFileListWindow1;
extern AboutWindow tagAboutWindow1;
extern OptionWindow tagOptionWindow1;
extern DWORD dwAppFrag;
extern unsigned int OPENSSL_ia32cap_P[4];
extern int nEditMarginX[4];
extern int nEditMarginY[4];
extern int nStringX[4];
extern int nStringY[4];

#define DEBUGDLG_X 319
#define DEBUGDLG_Y 258

// バージョン情報ボックスのメッセージ ハンドラです。
LRESULT CALLBACK DebugDlg_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == tagDebugWindow1.nWindowMessage)
	{
		switch (wParam)
		{
		case APP_MESSAGE_WINDOWSET:
			return DebugDlg_OnMessageWindowSet(hWnd, wParam, lParam);
		case APP_MASSAGE_DPICHANGE:
			return DebugDlg_OnMessageDpiChange(hWnd, wParam, lParam);
		}
		return FALSE;
	}

	switch (message)
	{
	case WM_INITDIALOG:
		return DebugDlg_OnInitDialog(hWnd, wParam, lParam);
	case WM_COMMAND:
		return DebugDlg_OnCommand(hWnd, wParam, lParam);
	case WM_GETMINMAXINFO:
		return DebugDlg_OnGetMinMaxInfo(hWnd, wParam, lParam);
	case WM_SIZE:
		return DebugDlg_OnSize(hWnd, wParam, lParam);
	case WM_SYSCOLORCHANGE:
		return DebugDlg_OnSysColorChange(hWnd, wParam, lParam);
	case WM_THEMECHANGED:
		return DebugDlg_OnThemeChanged(hWnd, wParam, lParam);
	case WM_CLOSE:
		return DebugDlg_OnClose(hWnd, wParam, lParam);
	case WM_DESTROY:
		return DebugDlg_OnDestroy(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT DebugDlg_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	int i = 0;

	dwAppFrag |= APP_WINDOW_NOALPHA;

	if (~dwAppFrag & APP_ENABLE_MINIMAM_HANDLE)
	{
		SetEnableThemeDialogTexture(hWnd);
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)tagMainWindow1.hIcon[0]);
	}

	tagDebugWindow1.dwStructSize = sizeof(tagDebugWindow1);
	tagDebugWindow1.hWnd = hWnd;

	tagDebugWindow1.nWindowMessage = RegisterWindowMessage(_T("DebugDlg"));

	tagDebugWindow1.hEdit[0] = GetDlgItem(hWnd, IDC_DEBUGDLG_EDIT1);
	for (i = 0; i < SIZEOF_NUM(tagDebugWindow1.hButton); i++) {
		tagDebugWindow1.hButton[i] = GetDlgItem(hWnd, IDC_DEBUGDLG_BUTTON1 + i);
	}

	if (osVerInfo[0].dwOSVersionInfoSize == 0)
	{
		GetOSVersion(&osVerInfo[0]);
		RtlGetOSVersion(&osVerInfo[1]);
	}

	DebugDlg_GetDpi();
	i = WindowSize_LoadState(hWnd, _T("DebugDlg"), tagMainWindow1.pINIFile);
	if (i == FALSE) {
		DebugDlg_SetWindowMove(TRUE);
	}
	PostMessage(hWnd, tagDebugWindow1.nWindowMessage, APP_MESSAGE_WINDOWSET, 0);
	return TRUE;
}

LRESULT DebugDlg_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);

	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDC_DEBUGDLG_BUTTON1:
		return DebugDlg_OnButton1(hWnd, wParam, lParam);
	case IDC_DEBUGDLG_BUTTON2:
		return DebugDlg_OnButton2(hWnd, wParam, lParam);
	case IDC_DEBUGDLG_BUTTON3:
		return DebugDlg_OnButton3(hWnd, wParam, lParam);
	case IDC_DEBUGDLG_BUTTON4:
		return DebugDlg_OnButton4(hWnd, wParam, lParam);
	}
	return FALSE;
}

LRESULT DebugDlg_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT DebugDlg_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT DebugDlg_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);

	((MINMAXINFO*)lParam)->ptMinTrackSize.x = MDIVX(DEBUGDLG_X) + tagMainWindow1.dwWindowRectX;
	((MINMAXINFO*)lParam)->ptMinTrackSize.y = MDIVY(DEBUGDLG_Y) + tagMainWindow1.dwWindowRectY;
	return TRUE;
}

LRESULT DebugDlg_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	int nArg[4] = { 0 };
	const int cx = LOWORD(lParam);
	const int cy = HIWORD(lParam);

	nArg[0] = 0;
	nArg[1] = 0;
	nArg[2] = cx;
	nArg[3] = cy - MDIVX(DEBUGDLG_Y - 222);
	MoveWindow(tagDebugWindow1.hEdit[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);

	nArg[0] = MDIVX(6);
	nArg[1] = cy - MDIVY(DEBUGDLG_Y - 234);
	nArg[2] = MDIVX(78);
	nArg[3] = nStringY[2] + nEditMarginY[0];
	MoveWindow(tagDebugWindow1.hButton[0], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);//			6,234,78,14

	nArg[0] = MDIVX(84);
	nArg[1] = cy - MDIVY(DEBUGDLG_Y - 234);
	nArg[2] = MDIVX(78);
	nArg[3] = nStringY[2] + nEditMarginY[0];
	MoveWindow(tagDebugWindow1.hButton[1], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);//			84,234,78,14

	nArg[0] = MDIVX(162);
	nArg[1] = cy - MDIVY(DEBUGDLG_Y - 234);
	nArg[2] = MDIVX(78);
	nArg[3] = nStringY[2] + nEditMarginY[0];
	MoveWindow(tagDebugWindow1.hButton[2], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);//			162,234,78,14

	nArg[0] = MDIVX(240);
	nArg[1] = cy - MDIVY(DEBUGDLG_Y - 234);
	nArg[2] = MDIVX(78);
	nArg[3] = nStringY[2] + nEditMarginY[0];
	MoveWindow(tagDebugWindow1.hButton[3], nArg[0], nArg[1], nArg[2], nArg[3], FALSE);//			162,234,78,14

	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}

LRESULT DebugDlg_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	int i = 0;

	SendMessage(tagDebugWindow1.hEdit[0], WM_SYSCOLORCHANGE, wParam, lParam);
	for (i = 0; i < SIZEOF_NUM(tagDebugWindow1.hButton); i++) {
		SendMessage(tagDebugWindow1.hButton[i], WM_SYSCOLORCHANGE, wParam, lParam);
	}
	return TRUE;
}

LRESULT DebugDlg_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT DebugDlg_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

LRESULT DebugDlg_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	DestroyWindow(hWnd);
	return TRUE;
}

LRESULT DebugDlg_OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	tagDebugWindow1.hWnd = NULL;
	WindowSize_SaveState(hWnd, _T("DebugDlg"), tagMainWindow1.pINIFile);
	return TRUE;
}


LRESULT DebugDlg_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	DebugDlg_SetWindowMove(FALSE);
	return TRUE;
}

LRESULT DebugDlg_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	DebugDlg_GetDpi();
	return TRUE;
}

VOID DebugDlg_GetDpi(VOID)
{
	int i = 0;

	SendMessage(tagDebugWindow1.hEdit[0], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[3], 0);
	for (; i < SIZEOF_NUM(tagDebugWindow1.hButton); i++) {
		SendMessage(tagDebugWindow1.hButton[i], WM_SETFONT, (WPARAM)tagMainWindow1.hFont[0], 0);
	}
	DebugDlg_SetWindowMove(FALSE);
	return;
}

VOID DebugDlg_SetWindowMove(BOOL nIsDefaultMove)
{
	RECT rc = { 0 };
	WINDOWPLACEMENT wndPlace = { 0 };

	wndPlace.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(tagDebugWindow1.hWnd, &wndPlace);
	switch (wndPlace.showCmd)
	{
	case SW_SHOWMAXIMIZED:
		rc = wndPlace.rcNormalPosition;
		break;
	default:
		GetWindowRect(tagDebugWindow1.hWnd, &rc);
	}

	if (nIsDefaultMove)
	{
		rc.bottom = rc.bottom - rc.top;
		rc.top = 0;
		rc.right = rc.right - rc.left;
		rc.left = 0;
	}
	MoveWindow(tagDebugWindow1.hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	return;
}

TCHAR* VariableAddArrayCopy(TCHAR* pOutTChar, const TCHAR* cpInTChar, int nSize)
{
	pOutTChar = qtcscpy(pOutTChar, cpInTChar);
	if (nSize > 0)
	{
		TCHAR szNum[16];

		*pOutTChar++ = '[';
		ITOT_FUNC(nSize, szNum, 10);
		pOutTChar = qtcscpy(pOutTChar, szNum);
		*pOutTChar++ = ']';
	}
	*pOutTChar = '\0';
	return pOutTChar;
}

TCHAR* IndentAddCopy(TCHAR* pOutTChar, const TCHAR* cpInTChar, DWORD nIndent)
{
	while (1)
	{
		if (*cpInTChar == '\n')
		{
			DWORD i = nIndent;

			*pOutTChar++ = '\n';
			for (; i > 0; i--) {
				*pOutTChar++ = '\t';
			}
			cpInTChar++;
			continue;
		}
		else if (*cpInTChar == '\0') {
			break;
		}
		*pOutTChar++ = *cpInTChar++;
	}
	*pOutTChar = '\0';
	return pOutTChar;
}

TCHAR* FileTimeToTChar(TCHAR* pOutTChar, const FILETIME* pInFileTime)
{
	TCHAR szNum[12] = { 0 };
	TCHAR* ptr = pOutTChar;
	FILETIME ftLocalFileTime = { 0 };
	SYSTEMTIME stFileTime = { 0 };

	FileTimeToLocalFileTime(pInFileTime , &ftLocalFileTime);
	FileTimeToSystemTime(&ftLocalFileTime , &stFileTime);

	ITOT_FUNC(stFileTime.wYear, szNum, 10);
	ptr = qtcscpy(ptr, szNum);
	memcpy(ptr, _T("年"), sizeof(_T("年")));
	ptr += _tcslen(_T("年"));

	ITOT_FUNC(stFileTime.wMonth, szNum, 10);
	ptr = qtcscpy(ptr, szNum);
	memcpy(ptr, _T("月"), sizeof(_T("月")));
	ptr += _tcslen(_T("月"));

	ITOT_FUNC(stFileTime.wDay, szNum, 10);
	ptr = qtcscpy(ptr, szNum);
	memcpy(ptr, _T("日"), sizeof(_T("日")));
	ptr += _tcslen(_T("日"));

	memcpy(ptr, _T("、"), sizeof(_T("、")));
	ptr += _tcslen(_T("、"));

	ITOT_FUNC(stFileTime.wHour, szNum, 10);
	ptr = qtcscpy(ptr, szNum);
	*ptr++ = ':';

	if (stFileTime.wMinute < 10) {
		*ptr++ = '0';
	}
	ITOT_FUNC(stFileTime.wMinute, szNum, 10);
	ptr = qtcscpy(ptr, szNum);
	*ptr++ = ':';

	if (stFileTime.wSecond < 10) {
		*ptr++ = '0';
	}
	ITOT_FUNC(stFileTime.wSecond, szNum, 10);
	ptr = qtcscpy(ptr, szNum);
	*ptr++ = '\r';
	*ptr++ = '\n';
	*ptr++ = '\0';
	return ptr;
}

VOID DebugDlg(HWND hWnd)
{
	SetFocus(tagMainWindow1.hButton[0]);
	if (tagDebugWindow1.hWnd == NULL)
	{
		tagDebugWindow1.hWnd = CreateDialog(tagMainWindow1.hInst, MAKEINTRESOURCE(IDD_DEBUGDLG), hWnd, (DLGPROC)DebugDlg_WndProc);
		IF_LIKELY(tagDebugWindow1.hWnd != NULL)
		{
			ShowWindow(tagDebugWindow1.hWnd, SW_SHOW);
			UpdateWindow(tagDebugWindow1.hWnd);
		}
	}
	else {
		SetForegroundWindow(tagDebugWindow1.hWnd);
	}
	return;
}

TCHAR* StaticVariableArrayCopy(TCHAR* pInOutChar, const TCHAR* cpInVariableName, size_t nArraySize)
{
	TCHAR szNum[16] = { 0 };

	pInOutChar = qtcscpy(pInOutChar, cpInVariableName);
	*pInOutChar++ = '[';
	_itot((int)nArraySize, szNum, 10);
	pInOutChar = qtcscpy(pInOutChar, szNum);
	*pInOutChar++ = ']';
	return pInOutChar;
}

LRESULT DebugDlg_OnButton1(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	TCHAR* ptr = szDebugBuf;
	size_t n = 0;

#if _MSC_VER > 1900
	int nCPUID[4] = { 0 };
	int nCPUID_MAX = 0;
#endif

	ptr = qtcscpy(ptr, _T("dwAppFrag"));
	BitView(szFuncBuf, dwAppFrag, sizeof(dwAppFrag));
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);
	ptr = qtcscpy(ptr, _T("\r\n"));

#if _MSC_VER > 1900
	__cpuidex(nCPUID, 0, 0);
	nCPUID_MAX = nCPUID[0];
	ptr = qtcscpy(ptr, _T("cpuid(0)"));
	ArrayView(szFuncBuf, nCPUID, sizeof(unsigned int), SIZEOF_NUM(nCPUID), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	if (1 < nCPUID_MAX)
	{
		__cpuid(nCPUID, 1);
		ptr = qtcscpy(ptr, _T("cpuid(1)"));
		ArrayView(szFuncBuf, nCPUID, sizeof(unsigned int), SIZEOF_NUM(nCPUID), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 0);
	}

	if (7 < nCPUID_MAX)
	{
		__cpuidex(nCPUID, 7, 0);
		const int nCPUCount = nCPUID[0];

		ptr = qtcscpy(ptr, _T("cpuid(7)"));
		ArrayView(szFuncBuf, nCPUID, sizeof(unsigned int), SIZEOF_NUM(nCPUID), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 0);

		if (1 <= nCPUCount)
		{
			__cpuidex(nCPUID, 7, 1);
			ptr = qtcscpy(ptr, _T("cpuid(7, 1)"));
			ArrayView(szFuncBuf, nCPUID, sizeof(unsigned int), SIZEOF_NUM(nCPUID), TRUE);
			ptr = IndentAddCopy(ptr, szFuncBuf, 0);
		}

		if (2 <= nCPUCount)
		{
			__cpuidex(nCPUID, 7, 2);
			ptr = qtcscpy(ptr, _T("cpuid(7, 2)"));
			ArrayView(szFuncBuf, nCPUID, sizeof(unsigned int), SIZEOF_NUM(nCPUID), TRUE);
			ptr = IndentAddCopy(ptr, szFuncBuf, 0);
		}
	}
#endif

	ptr = qtcscpy(ptr, _T("OPENSSL_ia32cap_P"));
	ArrayView(szFuncBuf, OPENSSL_ia32cap_P, sizeof(unsigned int), SIZEOF_NUM(OPENSSL_ia32cap_P), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);
	ptr = qtcscpy(ptr, _T("\r\n"));

	ptr = qtcscpy(ptr, _T("tagMainWindow"));
	VariableView(szFuncBuf, (size_t)&tagMainWindow1, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagMainWindow {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));
	ptr = qtcscpy(ptr, _T("dwStructSize"));
	VariableView(szFuncBuf, tagMainWindow1.dwStructSize, sizeof(tagMainWindow1.dwStructSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hWnd"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.hWnd, sizeof(tagMainWindow1.hWnd), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hInst"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.hInst, sizeof(tagMainWindow1.hInst), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hDllInst"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.hDllInst, sizeof(tagMainWindow1.hDllInst), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = VariableAddArrayCopy((TCHAR*)ptr, _T("szIconRes_Load"), ICONRES_SIZE);
	BinaryView(szFuncBuf, tagMainWindow1.szIconRes_Load, ICONRES_SIZE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("tagHashThread1"));
	VariableView(szFuncBuf, (size_t)&tagMainWindow1.tagHashThread1, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nTaskbarButtonMessage"));
	VariableView(szFuncBuf, tagMainWindow1.nTaskbarButtonMessage, sizeof(tagMainWindow1.nTaskbarButtonMessage), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hr3"));
	VariableView(szFuncBuf, tagMainWindow1.hr3, sizeof(tagMainWindow1.hr3), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hBrush"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.hBrush, sizeof(tagMainWindow1.hBrush), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hNextWnd"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.hNextWnd, sizeof(tagMainWindow1.hNextWnd), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hBenchHandle"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.hBenchHandle, sizeof(tagMainWindow1.hBenchHandle), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hSendWindow"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.hSendWindow, sizeof(tagMainWindow1.hSendWindow), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hStatic"));
	ArrayView(szFuncBuf, tagMainWindow1.hStatic, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hStatic), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hBmp"));
	ArrayView(szFuncBuf, tagMainWindow1.hBmp, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hBmp), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hProgress"));
	ArrayView(szFuncBuf, tagMainWindow1.hProgress, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hProgress), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hEdit"));
	ArrayView(szFuncBuf, tagMainWindow1.hEdit, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hEdit), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hButton"));
	ArrayView(szFuncBuf, tagMainWindow1.hButton, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hButton), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hGroup"));
	ArrayView(szFuncBuf, tagMainWindow1.hGroup, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hGroup), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTool"));
	ArrayView(szFuncBuf, tagMainWindow1.hTool, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hTool), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hMenu"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.hMenu, sizeof(tagMainWindow1.hMenu), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hPupMenu"));
	ArrayView(szFuncBuf, tagMainWindow1.hPupMenu, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hPupMenu), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hIcon"));
	ArrayView(szFuncBuf, tagMainWindow1.hIcon, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hIcon), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hStatusIcon"));
	ArrayView(szFuncBuf, tagMainWindow1.hStatusIcon, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hStatusIcon), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hFont"));
	ArrayView(szFuncBuf, tagMainWindow1.hFont, sizeof(size_t), SIZEOF_NUM(tagMainWindow1.hFont), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	for (n = 0; n < SIZEOF_NUM(tagMainWindow1.ofnFileName); n++)
	{
		ptr = StaticVariableArrayCopy(ptr, _T("ofnFileName"), n);
		BinaryView(szFuncBuf, (BYTE*)&tagMainWindow1.ofnFileName[n], sizeof(OPENFILENAME));
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);
	}

	ptr = qtcscpy(ptr, _T("*pTaskbarList3"));
	VariableView(szFuncBuf, (size_t)tagMainWindow1.pTaskbarList3, sizeof(tagMainWindow1.pTaskbarList3), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pBuf"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pBuf);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pStBuf"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pStBuf);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pTempBuf"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pTempBuf);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pTitle"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pTitle);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pWindowClass"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pWindowClass);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pCopyRight"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pCopyRight);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pStatusTitle"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pStatusTitle);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*cpHashName"));
	TCharToVariableView(szFuncBuf, (TCHAR*)tagMainWindow1.cpHashName);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pHashString"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pHashString);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pHashCompString"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pHashCompString);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pBase64String"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pBase64String);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pClipboardString"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pClipboardString);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pInputtext"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pInputtext);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pFile"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pHashFile"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pHashFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pINIFile"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pINIFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pFileExtension"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pFileExtension);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	if (1)
	{
		TCHAR** p = tagMainWindow1.ppCheckSumFile;

		n = 0;
		while (*p != NULL)
		{
			n++;
			p++;
		}
	}

	ptr = qtcscpy(ptr, _T("*ppCheckSumFile"));
	ArrayView(szFuncBuf, tagMainWindow1.ppCheckSumFile, ARRAYVIEW_TYPE_TCHAR, n, TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pCheckSumFile"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pCheckSumFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pNotFoundFile"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pNotFoundFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pNextNotFoundFile"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pNextNotFoundFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pEmptyFolder"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pEmptyFolder);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pNextEmptyFolder"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pNextEmptyFolder);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pHashNotFoundFile"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pHashNotFoundFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pNextHashNotFoundFile"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pHashNotFoundFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pTimeText"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pTimeText);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pTimeView1_pTimeView1_TransPerSec"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pTimeView1_TransPerSec);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pTimeView1_Size"));
	ArrayView(szFuncBuf, tagMainWindow1.pTimeView1_Size, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagMainWindow1.pTimeView1_Size), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pTimeView1_Time"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pTimeView1_Time);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pOfn1Buf"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pOfn1Buf);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pWindowTitle"));
	ArrayView(szFuncBuf, tagMainWindow1.pWindowTitle, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagMainWindow1.pWindowTitle), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pStatusText"));
	ArrayView(szFuncBuf, tagMainWindow1.pStatusText, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagMainWindow1.pStatusText), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pButtonText1"));
	ArrayView(szFuncBuf, tagMainWindow1.pButtonText1, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagMainWindow1.pButtonText1), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pButtonText2"));
	ArrayView(szFuncBuf, tagMainWindow1.pButtonText2, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagMainWindow1.pButtonText2), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pToolTipText"));
	ArrayView(szFuncBuf, tagMainWindow1.pToolTipText, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagMainWindow1.pToolTipText), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pTimeText1"));
	ArrayView(szFuncBuf, tagMainWindow1.pTimeText1, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagMainWindow1.pTimeText1), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pBase64_Msg"));
	ArrayView(szFuncBuf, tagMainWindow1.pBase64_Msg, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagMainWindow1.pBase64_Msg), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pBecthResult"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pBecthResult);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pCmpErrorCount"));
	TCharToVariableView(szFuncBuf, tagMainWindow1.pCmpErrorCount);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("llTimeView1_TransPerSec"));
	VariableView(szFuncBuf, tagMainWindow1.llTimeView1_TransPerSec, sizeof(tagMainWindow1.llTimeView1_TransPerSec), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwTimeView1_Time"));
	VariableView(szFuncBuf, tagMainWindow1.dwTimeView1_Time, sizeof(tagMainWindow1.dwTimeView1_Time), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwOpenItem"));
	VariableView(szFuncBuf, tagMainWindow1.dwOpenItem, sizeof(tagMainWindow1.dwOpenItem), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwStartTime"));
	VariableView(szFuncBuf, tagMainWindow1.dwStartTime, sizeof(tagMainWindow1.dwStartTime), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwEndTime"));
	VariableView(szFuncBuf, tagMainWindow1.dwEndTime, sizeof(tagMainWindow1.dwEndTime), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwBackTime"));
	VariableView(szFuncBuf, tagMainWindow1.dwBackTime, sizeof(tagMainWindow1.dwBackTime), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwPersent"));
	VariableView(szFuncBuf, tagMainWindow1.dwPersent, sizeof(tagMainWindow1.dwPersent), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwTimeViewFileCount"));
	VariableView(szFuncBuf, tagMainWindow1.dwTimeViewFileCount, sizeof(tagMainWindow1.dwTimeViewFileCount), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwLasterror"));
	VariableView(szFuncBuf, tagMainWindow1.dwLasterror, sizeof(tagMainWindow1.dwLasterror), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwSaveHashFileCharCode"));
	VariableView(szFuncBuf, tagMainWindow1.dwSaveHashFileCharCode, sizeof(tagMainWindow1.dwSaveHashFileCharCode), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashTypeBack"));
	VariableView(szFuncBuf, tagMainWindow1.dwHashTypeBack, sizeof(tagMainWindow1.dwHashTypeBack), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nCmpCount"));
	VariableView(szFuncBuf, tagMainWindow1.nCmpCount, sizeof(tagMainWindow1.nCmpCount), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nCmpErrorCount"));
	VariableView(szFuncBuf, tagMainWindow1.nCmpErrorCount, sizeof(tagMainWindow1.nCmpErrorCount), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nCmpErrorCountSize"));
	VariableView(szFuncBuf, tagMainWindow1.nCmpErrorCountSize, sizeof(tagMainWindow1.nCmpErrorCountSize), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nIsSubFolder"));
	VariableView(szFuncBuf, tagMainWindow1.nIsSubFolder, sizeof(tagMainWindow1.nIsSubFolder), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("bIsNotFoundFile"));
	VariableView(szFuncBuf, tagMainWindow1.bIsNotFoundFile, sizeof(tagMainWindow1.bIsNotFoundFile), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("bIsEmptyFolder"));
	VariableView(szFuncBuf, tagMainWindow1.bIsEmptyFolder, sizeof(tagMainWindow1.bIsEmptyFolder), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("bIsHashNotFoundFile"));
	VariableView(szFuncBuf, tagMainWindow1.bIsHashNotFoundFile, sizeof(tagMainWindow1.bIsHashNotFoundFile), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("bIsWindowCancel"));
	VariableView(szFuncBuf, tagMainWindow1.bIsWindowCancel, sizeof(tagMainWindow1.bIsWindowCancel), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwAddFileModeFileCount"));
	VariableView(szFuncBuf, tagMainWindow1.dwAddFileModeFileCount , sizeof(tagMainWindow1.dwAddFileModeFileCount), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("clGrayTextColor"));
	VariableView(szFuncBuf, tagMainWindow1.clGrayTextColor, sizeof(tagMainWindow1.clGrayTextColor), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("sDpiX"));
	VariableView(szFuncBuf, tagMainWindow1.nDpi[0], sizeof(tagMainWindow1.nDpi[0]), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("sDpiY"));
	VariableView(szFuncBuf, tagMainWindow1.nDpi[1], sizeof(tagMainWindow1.nDpi[1]), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwbaseX"));
	VariableView(szFuncBuf, tagMainWindow1.dwbaseX, sizeof(tagMainWindow1.dwbaseX), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwbaseY"));
	VariableView(szFuncBuf, tagMainWindow1.dwbaseY, sizeof(tagMainWindow1.dwbaseY), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwWindowRectX"));
	VariableView(szFuncBuf, tagMainWindow1.dwWindowRectX, sizeof(tagMainWindow1.dwWindowRectX), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwWindowRectY"));
	VariableView(szFuncBuf, tagMainWindow1.dwWindowRectY, sizeof(tagMainWindow1.dwWindowRectY), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dDpiSize"));
	DoubleToVariableView(szFuncBuf, tagMainWindow1.dDpiSize);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nIsWindowSet"));
	VariableView(szFuncBuf, tagMainWindow1.nIsWindowSet, sizeof(tagMainWindow1.nIsWindowSet), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nLeftPos"));
	VariableView(szFuncBuf, tagMainWindow1.nLeftPos, sizeof(tagMainWindow1.nLeftPos), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nTopPos"));
	VariableView(szFuncBuf, tagMainWindow1.nTopPos, sizeof(tagMainWindow1.nTopPos), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("};\r\n\r\n"));

	ptr = qtcscpy(ptr, _T("tagFileListWindow"));
	VariableView(szFuncBuf, (size_t)&tagFileListWindow1, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagFileListWindow {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));

	ptr = qtcscpy(ptr, _T("dwStructSize"));
	VariableView(szFuncBuf, tagFileListWindow1.dwStructSize, sizeof(tagFileListWindow1.dwStructSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hWnd"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.hWnd, sizeof(tagFileListWindow1.hWnd), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nWindowMessage"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.nWindowMessage, sizeof(tagFileListWindow1.nWindowMessage), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hList"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.hList, sizeof(tagFileListWindow1.hList), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hStatus"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.hStatus, sizeof(tagFileListWindow1.hStatus), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hGroup"));
	ArrayView(szFuncBuf, tagFileListWindow1.hGroup, sizeof(size_t), SIZEOF_NUM(tagFileListWindow1.hGroup), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hButton"));
	ArrayView(szFuncBuf, tagFileListWindow1.hButton, sizeof(size_t), SIZEOF_NUM(tagFileListWindow1.hButton), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hRadio"));
	ArrayView(szFuncBuf, tagFileListWindow1.hRadio, sizeof(size_t), SIZEOF_NUM(tagFileListWindow1.hRadio), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hMenu"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.hMenu, sizeof(tagFileListWindow1.hMenu), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hPupMenu"));
	ArrayView(szFuncBuf, tagFileListWindow1.hPupMenu, sizeof(size_t), SIZEOF_NUM(tagFileListWindow1.hPupMenu), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pBuf"));
	TCharToVariableView(szFuncBuf, tagFileListWindow1.pBuf);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pStBuf"));
	TCharToVariableView(szFuncBuf, tagFileListWindow1.pStBuf);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pFileChangeHash"));
	TCharToVariableView(szFuncBuf, tagFileListWindow1.pFileChangeHash);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pButtonText1"));
	ArrayView(szFuncBuf, tagFileListWindow1.pButtonText1, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagFileListWindow1.pButtonText1), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pButtonText2"));
	ArrayView(szFuncBuf, tagFileListWindow1.pButtonText2, ARRAYVIEW_TYPE_TCHAR, SIZEOF_NUM(tagFileListWindow1.pButtonText2), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwNewFile"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.dwNewFile, sizeof(tagFileListWindow1.dwNewFile), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileCount"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.dwFileCount, sizeof(tagFileListWindow1.dwFileCount), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwOpenOption"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.dwFileCount, sizeof(tagFileListWindow1.dwFileCount), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwDragListNo"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.dwDragListNo, sizeof(tagFileListWindow1.dwDragListNo), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwMoveListNo"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.dwMoveListNo, sizeof(tagFileListWindow1.dwMoveListNo), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashCheckCount"));
	ArrayView(szFuncBuf, tagFileListWindow1.dwHashCheckCount, sizeof(DWORD), SIZEOF_NUM(tagFileListWindow1.dwHashCheckCount), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nIsHashFileCompMassege"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.nIsHashFileCompMassege, sizeof(tagFileListWindow1.nIsHashFileCompMassege), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("clListTextColor"));
	ArrayView(szFuncBuf, tagFileListWindow1.clListTextColor, sizeof(DWORD), SIZEOF_NUM(tagFileListWindow1.clListTextColor), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwStatus1_Height"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.dwStatus1_Height, sizeof(tagFileListWindow1.dwStatus1_Height), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nScroll_Width"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.nScroll_Width, sizeof(tagFileListWindow1.nScroll_Width), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nIsSubFolder"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.nIsSubFolder, sizeof(tagFileListWindow1.nIsSubFolder), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nIsListLock"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.nIsListLock, sizeof(tagFileListWindow1.nIsListLock), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("tagFileListItem1"));
	VariableView(szFuncBuf, (size_t)&tagFileListWindow1.tagFileListItem1, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwDialogRect"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.dwDialogRect, sizeof(tagFileListWindow1.dwDialogRect), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hListViewImg"));
	VariableView(szFuncBuf, (size_t)tagFileListWindow1.hListViewImg, sizeof(tagFileListWindow1.hListViewImg), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hListViewIcon"));
	ArrayView(szFuncBuf, tagFileListWindow1.hListViewIcon, sizeof(size_t), SIZEOF_NUM(tagFileListWindow1.hListViewIcon), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);
	ptr--;
	ptr = qtcscpy(ptr, _T("};\r\n\r\n"));

	ptr = qtcscpy(ptr, _T("tagAboutWindow"));
	VariableView(szFuncBuf, (size_t)&tagAboutWindow1, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagAboutWindow {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));

	ptr = qtcscpy(ptr, _T("dwStructSize"));
	VariableView(szFuncBuf, tagAboutWindow1.dwStructSize, sizeof(tagAboutWindow1.dwStructSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hWnd"));
	VariableView(szFuncBuf, (size_t)tagAboutWindow1.hWnd, sizeof(tagAboutWindow1.hWnd), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hIcon"));
	VariableView(szFuncBuf, (size_t)tagAboutWindow1.hIcon, sizeof(tagAboutWindow1.hIcon), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hStatic"));
	ArrayView(szFuncBuf, tagAboutWindow1.hStatic, sizeof(size_t), SIZEOF_NUM(tagAboutWindow1.hStatic), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hIDOK"));
	VariableView(szFuncBuf, (size_t)tagAboutWindow1.hIDOK, sizeof(tagAboutWindow1.hIDOK), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nWindowMessage"));
	VariableView(szFuncBuf, (size_t)tagAboutWindow1.nWindowMessage, sizeof(tagAboutWindow1.nWindowMessage), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("};\r\n\r\n"));

	ptr = qtcscpy(ptr, _T("tagOptionWindow"));
	VariableView(szFuncBuf, (size_t)&tagOptionWindow1, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagOptionWindow {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));

	ptr = qtcscpy(ptr, _T("dwStructSize"));
	VariableView(szFuncBuf, tagOptionWindow1.dwStructSize, sizeof(tagOptionWindow1.dwStructSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hWnd"));
	VariableView(szFuncBuf, (size_t)tagOptionWindow1.hWnd, sizeof(tagOptionWindow1.hWnd), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab"));
	VariableView(szFuncBuf, (size_t)tagOptionWindow1.hTab, sizeof(tagOptionWindow1.hTab), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hIDOK"));
	VariableView(szFuncBuf, (size_t)tagOptionWindow1.hIDOK, sizeof(tagOptionWindow1.hIDOK), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hIDCANCEL"));
	VariableView(szFuncBuf, (size_t)tagOptionWindow1.hIDCANCEL, sizeof(tagOptionWindow1.hIDCANCEL), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTabWnd"));
	ArrayView(szFuncBuf, tagOptionWindow1.hWndTab, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hWndTab), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab1Group"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab1Group, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab1Group), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab1Checkbox"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab1Checkbox, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab1Checkbox), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab1Combo"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab1Combo, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab1Combo), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab2Group"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab2Group, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab2Group), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab2Static"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab2Static, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab2Static), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab2Checkbox"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab2Checkbox, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab2Checkbox), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab2Edit"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab2Edit, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab2Edit), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab2Button"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab2Button, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab2Button), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab3Group"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab3Group, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab3Group), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab3Checkbox"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab3Checkbox, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab3Checkbox), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab4Group"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab4Group, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab4Group), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hTab4Radio"));
	ArrayView(szFuncBuf, tagOptionWindow1.hTab4Radio, sizeof(size_t), SIZEOF_NUM(tagOptionWindow1.hTab4Radio), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pBuf"));
	TCharToVariableView(szFuncBuf, tagOptionWindow1.pBuf);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pChackSumFile"));
	TCharToVariableView(szFuncBuf, tagOptionWindow1.pCheckSumFile);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwAppFrag"));
	VariableView(szFuncBuf, tagOptionWindow1.dwAppFrag, sizeof(tagOptionWindow1.dwAppFrag), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nActiveTab"));
	VariableView(szFuncBuf, (size_t)tagOptionWindow1.nActiveTab, sizeof(tagOptionWindow1.nActiveTab), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashType"));
	VariableView(szFuncBuf, tagOptionWindow1.dwHashType, sizeof(tagOptionWindow1.dwHashType), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwRadioState"));
	ArrayView(szFuncBuf, tagOptionWindow1.dwRadioState, sizeof(DWORD), SIZEOF_NUM(tagOptionWindow1.dwRadioState), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nWindowMessage"));
	VariableView(szFuncBuf, (size_t)tagOptionWindow1.nWindowMessage, sizeof(tagOptionWindow1.nWindowMessage), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("};\r\n\r\n"));

	ptr = qtcscpy(ptr, _T("tagDebugWindow"));
	VariableView(szFuncBuf, (size_t)&tagDebugWindow1, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagDebugWindow {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));

	ptr = qtcscpy(ptr, _T("dwStructSize"));
	VariableView(szFuncBuf, tagDebugWindow1.dwStructSize, sizeof(tagDebugWindow1.dwStructSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hWnd"));
	VariableView(szFuncBuf, (size_t)tagDebugWindow1.hWnd, sizeof(tagDebugWindow1.hWnd), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hIcon"));
	VariableView(szFuncBuf, (size_t)tagDebugWindow1.hIcon, sizeof(tagDebugWindow1.hIcon), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hEdit"));
	ArrayView(szFuncBuf, tagDebugWindow1.hEdit, sizeof(size_t), SIZEOF_NUM(tagDebugWindow1.hEdit), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hButton"));
	ArrayView(szFuncBuf, tagDebugWindow1.hButton, sizeof(size_t), SIZEOF_NUM(tagDebugWindow1.hButton), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nWindowMessage"));
	VariableView(szFuncBuf, (size_t)tagDebugWindow1.nWindowMessage, sizeof(tagDebugWindow1.nWindowMessage), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("};\r\n"));

	SetWindowText(tagDebugWindow1.hEdit[0], szDebugBuf);
	SetFocus(tagDebugWindow1.hEdit[0]);
	return TRUE;
}

LRESULT DebugDlg_OnButton2(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	TCHAR* ptr = szDebugBuf;
	const tagHashThread* cptagHashThread = &tagMainWindow1.tagHashThread1;
	const tagHashThread_MultiFile* cptagHashThread_MultiFile = tagMainWindow1.tagHashThread1.tagMultiFile;

	ptr = qtcscpy(ptr, _T("tagHashThread"));
	VariableView(szFuncBuf, (size_t)cptagHashThread, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagHashThread {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));

	ptr = qtcscpy(ptr, _T("dwStructSize"));
	VariableView(szFuncBuf, cptagHashThread->dwStructSize, sizeof(cptagHashThread->dwStructSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hWnd"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->hWnd, sizeof(cptagHashThread->hWnd), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nMessageID"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->nMessageID, sizeof(cptagHashThread->nMessageID), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nSuspendMessage"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->nSuspendMessage, sizeof(cptagHashThread->nSuspendMessage), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hHashThreadHandle"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->hHashThreadHandle, sizeof(cptagHashThread->hHashThreadHandle), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nThreadID"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->nThreadID, sizeof(cptagHashThread->nThreadID), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashThreadLastError"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->dwHashThreadLastError, sizeof(cptagHashThread->dwHashThreadLastError), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashType"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->dwHashType, sizeof(cptagHashThread->dwHashType), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*cpHashName"));
	TCharToVariableView(szFuncBuf, cptagHashThread->cpHashName);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashLen"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->dwHashLen, sizeof(cptagHashThread->dwHashLen), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = StaticVariableArrayCopy(ptr, _T("bHashByte"), HASH_LEN);
	BinaryView(szFuncBuf, cptagHashThread->bHashByte, HASH_LEN);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*cpHashExt"));
	TCharToVariableView(szFuncBuf, cptagHashThread->cpHashExt);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nHashExtLen"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->nHashExtLen, sizeof(cptagHashThread->nHashExtLen), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nIsFileNoCheck"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->nIsFileNoCheck, sizeof(cptagHashThread->nIsFileNoCheck), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*pFilePath"));
	TCharToBinaryView(szFuncBuf, cptagHashThread->pFilePath);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("llFileSize"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->llFileSize, sizeof(cptagHashThread->llFileSize), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("llFileReadSize"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->llFileReadSize, sizeof(cptagHashThread->llFileReadSize), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileAttributeMask"));
	BitView(szFuncBuf, cptagHashThread->dwFileAttributeMask, sizeof(DWORD));
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nFileNoCache"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->nFileNoCache, sizeof(cptagHashThread->nFileNoCache), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("pInFilePointer"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->pInFilePointer, sizeof(cptagHashThread->pInFilePointer), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("pFileBuffer"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->pFileBuffer, sizeof(cptagHashThread->pFileBuffer), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileBufferAllocateSize"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->dwFileBufferAllocateSize, sizeof(cptagHashThread->dwFileBufferAllocateSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileBufferSize"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->dwFileBufferSize, sizeof(cptagHashThread->dwFileBufferSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwCreateFileAttributes"));
	BitView(szFuncBuf, cptagHashThread->dwCreateFileAttributes, sizeof(DWORD));
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("hFile"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->hFile, sizeof(cptagHashThread->hFile), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*tagMultiFile"));
	VariableView(szFuncBuf, (size_t)cptagHashThread->tagMultiFile, sizeof(cptagHashThread->tagMultiFile), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);
	ptr = qtcscpy(ptr, _T("};\r\n\r\n"));

	ptr = qtcscpy(ptr, _T("tagHashThread1.tagMultiFile"));
	VariableView(szFuncBuf, (size_t)cptagHashThread_MultiFile, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagHashThread_MultiFile {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));

	ptr = qtcscpy(ptr, _T("dwStructSize"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->dwStructSize, sizeof(cptagHashThread_MultiFile->dwStructSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwMaxBufSize"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->dwMaxBufSize, sizeof(cptagHashThread_MultiFile->dwMaxBufSize), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileCount"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->dwFileCount, sizeof(cptagHashThread_MultiFile->dwFileCount), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileCurrentCount"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->dwFileCurrentCount, sizeof(cptagHashThread_MultiFile->dwFileCurrentCount), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = StaticVariableArrayCopy(ptr, _T("szCurrentFolder"), MAX_PATH_SIZE);
	TCharToBinaryView(szFuncBuf, cptagHashThread_MultiFile->szCurrentFolder);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("llFileAllSize"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->llFileAllSize, sizeof(cptagHashThread_MultiFile->llFileAllSize), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("llFileAllCurrentSize"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->llFileAllCurrentSize, sizeof(cptagHashThread_MultiFile->llFileAllCurrentSize), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("llFileAllSizeBackup"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->llFileAllSizeBackup, sizeof(cptagHashThread_MultiFile->llFileAllSizeBackup), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashFileCharCode"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->dwHashFileCharCode, sizeof(cptagHashThread_MultiFile->dwHashFileCharCode), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashFileBom"));
	VariableView(szFuncBuf, cptagHashThread_MultiFile->dwHashFileBom, sizeof(cptagHashThread_MultiFile->dwHashFileBom), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("*ptagFileRecode"));
	VariableView(szFuncBuf, (size_t)cptagHashThread_MultiFile->ptagFileRecode, sizeof(cptagHashThread_MultiFile->ptagFileRecode), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("};\r\n"));

	SetWindowText(tagDebugWindow1.hEdit[0], szDebugBuf);
	SetFocus(tagDebugWindow1.hEdit[0]);
	return TRUE;
}

LRESULT DebugDlg_OnButton3(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	TCHAR* ptr = szDebugBuf;
	const tagHashThread_FileRecode* cptagHashFile_Recode = HashThread_GetFileRecode_Core(&tagMainWindow1.tagHashThread1, tagMainWindow1.dwOpenItem);
	QWORD q;

	ptr = qtcscpy(ptr, _T("dwOpenItem"));
	VariableView(szFuncBuf, tagMainWindow1.dwOpenItem, sizeof(tagMainWindow1.dwOpenItem), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagHashThread"));
	VariableView(szFuncBuf, (size_t)&tagMainWindow1.tagHashThread1, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("tagHashThread.tagMultiFile"));
	VariableView(szFuncBuf, (size_t)&tagMainWindow1.tagHashThread1.tagMultiFile, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = StaticVariableArrayCopy(ptr, _T("tagHashThread.tagMultiFile->tagHashThread_FileRecode"), tagMainWindow1.dwOpenItem);
	VariableView(szFuncBuf, (size_t)cptagHashFile_Recode, sizeof(size_t), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);
	ptr = qtcscpy(ptr, _T("\r\n"));

	ptr = qtcscpy(ptr, _T("tagHashThread_FileRecode {"));
	ptr = qtcscpy(ptr, _T(" \r\n\t"));

	ptr = StaticVariableArrayCopy(ptr, _T("szFileName"), SIZEOF_NUM(cptagHashFile_Recode->szFileName));
	TCharToBinaryView(szFuncBuf, cptagHashFile_Recode->szFileName);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nFileNameLength"));
	VariableView(szFuncBuf, cptagHashFile_Recode->nFileNameLength, sizeof(cptagHashFile_Recode->nFileNameLength), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("llFileSize"));
	VariableView(szFuncBuf, cptagHashFile_Recode->llFileSize, sizeof(cptagHashFile_Recode->llFileSize), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileAttributes"));
	BitView(szFuncBuf, cptagHashFile_Recode->dwFileAttributes, sizeof(DWORD));
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	q = ((QWORD)cptagHashFile_Recode->ftFileLastWriteTime.dwHighDateTime << 32) + cptagHashFile_Recode->ftFileLastWriteTime.dwLowDateTime;
	ptr = qtcscpy(ptr, _T("ftFileLastWriteTime"));
	VariableView(szFuncBuf, q, sizeof(q), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);
	*ptr++ = '\t';
	*ptr++ = '/';
	*ptr++ = '/';
	*ptr++ = ' ';
	FileTimeToTChar(szFuncBuf, &cptagHashFile_Recode->ftFileLastWriteTime);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = StaticVariableArrayCopy(ptr, _T("pFileHashByte"), tagMainWindow1.tagHashThread1.dwHashLen);
	BinaryView(szFuncBuf, cptagHashFile_Recode->pFileHashByte, tagMainWindow1.tagHashThread1.dwHashLen);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileLastError"));
	VariableView(szFuncBuf, cptagHashFile_Recode->dwFileLastError, sizeof(cptagHashFile_Recode->dwFileLastError), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwFileHashType"));
	VariableView(szFuncBuf, cptagHashFile_Recode->dwFileHashType, sizeof(cptagHashFile_Recode->dwFileHashType), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashFileID"));
	VariableView(szFuncBuf, cptagHashFile_Recode->dwHashFileID, sizeof(cptagHashFile_Recode->dwHashFileID), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashFileHashLine"));
	VariableView(szFuncBuf, cptagHashFile_Recode->dwHashFileHashLine, sizeof(cptagHashFile_Recode->dwHashFileHashLine), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nIsHashOldFile"));
	VariableView(szFuncBuf, cptagHashFile_Recode->nIsHashOldFile, sizeof(cptagHashFile_Recode->nIsHashOldFile), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = StaticVariableArrayCopy(ptr, _T("nHashFileHashByte"), tagMainWindow1.tagHashThread1.dwHashLen);
	BinaryView(szFuncBuf, cptagHashFile_Recode->nHashFileHashByte, tagMainWindow1.tagHashThread1.dwHashLen);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nHashFileCmp"));
	VariableView(szFuncBuf, cptagHashFile_Recode->nHashFileCmp, sizeof(cptagHashFile_Recode->nHashFileCmp), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwHashFileHashType"));
	VariableView(szFuncBuf, cptagHashFile_Recode->dwHashFileHashType, sizeof(cptagHashFile_Recode->dwHashFileHashType), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("nNextFile"));
	VariableView(szFuncBuf, cptagHashFile_Recode->nNextFile, sizeof(cptagHashFile_Recode->nNextFile), FALSE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("};\r\n"));

	SetWindowText(tagDebugWindow1.hEdit[0], szDebugBuf);
	SetFocus(tagDebugWindow1.hEdit[0]);
	return TRUE;
}

LRESULT DebugDlg_OnButton4(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	TCHAR* ptr = szDebugBuf;

	ptr = qtcscpy(ptr, _T("OSVERSIONINFOEX osVerInfo[2];"));
	ptr = qtcscpy(ptr, _T("\r\n\r\n"));

	ptr = qtcscpy(ptr, _T("GetVersionEx(&osVerInfo[0]);"));
	ptr = qtcscpy(ptr, _T("\r\n"));

	ptr = qtcscpy(ptr, _T("osVerInfo[0] = {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));

	ptr = qtcscpy(ptr, _T("dwOSVersionInfoSize"));
	VariableView(szFuncBuf, osVerInfo[0].dwOSVersionInfoSize, sizeof(DWORD), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwMajorVersion"));
	VariableView(szFuncBuf, osVerInfo[0].dwMajorVersion, sizeof(DWORD), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwMinorVersion"));
	VariableView(szFuncBuf, osVerInfo[0].dwMinorVersion, sizeof(DWORD), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwBuildNumber"));
	BitView(szFuncBuf, osVerInfo[0].dwBuildNumber, sizeof(DWORD));
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("dwPlatformId"));
	VariableView(szFuncBuf, osVerInfo[0].dwPlatformId, sizeof(DWORD), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = StaticVariableArrayCopy(ptr, _T("szCSDVersion"), SIZEOF_NUM(osVerInfo[0].szCSDVersion));
	BinaryView(szFuncBuf, (BYTE*)osVerInfo[0].szCSDVersion, SIZEOF_NUM(osVerInfo[0].szCSDVersion));
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("wServicePackMajor"));
	VariableView(szFuncBuf, osVerInfo[0].wServicePackMajor, sizeof(WORD), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("wServicePackMinor"));
	VariableView(szFuncBuf, osVerInfo[0].wServicePackMinor, sizeof(WORD), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("wSuiteMask"));
	VariableView(szFuncBuf, osVerInfo[0].wSuiteMask, sizeof(WORD), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("wProductType"));
	VariableView(szFuncBuf, osVerInfo[0].wProductType, sizeof(BYTE), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 1);

	ptr = qtcscpy(ptr, _T("wReserved"));
	VariableView(szFuncBuf, osVerInfo[0].wReserved, sizeof(BYTE), TRUE);
	ptr = IndentAddCopy(ptr, szFuncBuf, 0);

	ptr = qtcscpy(ptr, _T("};\r\n\r\n"));

	ptr = qtcscpy(ptr, _T("RtlGetVersion(&osVerInfo[1]);"));
	ptr = qtcscpy(ptr, _T("\r\n"));

	ptr = qtcscpy(ptr, _T("osVerInfo[1] = {"));
	ptr = qtcscpy(ptr, _T("\r\n\t"));

	if (osVerInfo[1].dwMajorVersion != 0)
	{
		ptr = qtcscpy(ptr, _T("dwOSVersionInfoSize"));
		VariableView(szFuncBuf, osVerInfo[1].dwOSVersionInfoSize, sizeof(DWORD), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("dwMajorVersion"));
		VariableView(szFuncBuf, osVerInfo[1].dwMajorVersion, sizeof(DWORD), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("dwMinorVersion"));
		VariableView(szFuncBuf, osVerInfo[1].dwMinorVersion, sizeof(DWORD), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("dwBuildNumber"));
		BitView(szFuncBuf, osVerInfo[1].dwBuildNumber, sizeof(DWORD));
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("dwPlatformId"));
		VariableView(szFuncBuf, osVerInfo[1].dwPlatformId, sizeof(DWORD), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = StaticVariableArrayCopy(ptr, _T("szCSDVersion"), SIZEOF_NUM(osVerInfo[1].szCSDVersion));
		BinaryView(szFuncBuf, (BYTE*)osVerInfo[1].szCSDVersion, SIZEOF_NUM(osVerInfo[1].szCSDVersion));
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("wServicePackMajor"));
		VariableView(szFuncBuf, osVerInfo[1].wServicePackMajor, sizeof(WORD), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("wServicePackMinor"));
		VariableView(szFuncBuf, osVerInfo[1].wServicePackMinor, sizeof(WORD), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("wSuiteMask"));
		VariableView(szFuncBuf, osVerInfo[1].wSuiteMask, sizeof(WORD), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("wProductType"));
		VariableView(szFuncBuf, osVerInfo[1].wProductType, sizeof(BYTE), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 1);

		ptr = qtcscpy(ptr, _T("wReserved"));
		VariableView(szFuncBuf, osVerInfo[1].wReserved, sizeof(BYTE), TRUE);
		ptr = IndentAddCopy(ptr, szFuncBuf, 0);
	}
	else
	{
		ptr = qtcscpy(ptr, _T("0;// 取得に失敗しました。"));
		ptr = qtcscpy(ptr, _T("\r\n"));
	}

	ptr = qtcscpy(ptr, _T("};\r\n"));

	SetWindowText(tagDebugWindow1.hEdit[0], szDebugBuf);
	SetFocus(tagDebugWindow1.hEdit[0]);
	return TRUE;
}
