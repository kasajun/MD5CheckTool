#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __DEBUGDLG_H_B032692C_5D22_4c53_93B0_C0A8B373CA51__
#define __DEBUGDLG_H_B032692C_5D22_4c53_93B0_C0A8B373CA51__

typedef struct tagDebugWindow {
	DWORD dwStructSize;
	HWND hWnd;
	HWND hIcon;
	HWND hEdit[1];
	HWND hButton[4];
	UINT nWindowMessage;
} DebugWindow, *lpDebugWindow;

// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK DebugDlg_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT DebugDlg_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT DebugDlg_OnButton1(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnButton2(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnButton3(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnButton4(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT DebugDlg_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT DebugDlg_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam);


VOID DebugDlg(HWND hWnd);
VOID DebugDlg_GetDpi(VOID);
VOID DebugDlg_SetWindowMove(BOOL nIsDefaultMove);

#endif
