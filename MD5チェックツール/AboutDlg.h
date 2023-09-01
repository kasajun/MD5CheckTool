#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __ABOUTDLG_H_CD3DE2B1_512E_4fea_B75B_6767B924B0EE__
#define __ABOUTDLG_H_CD3DE2B1_512E_4fea_B75B_6767B924B0EE__

#include "Main.h"
#include "resource.h"

typedef struct tagAboutWindow {
	DWORD dwStructSize;
	HWND hWnd;
	HWND hIcon;
	HWND hStatic[11];
	HWND hGroup[1];
	HWND hIDOK;
	UINT nWindowMessage;
} AboutWindow, *lpAboutWindow;

// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK AboutDlg_WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT AboutDlg_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlg_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlg_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlg_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlg_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlg_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlg_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlg_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT AboutDlg_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT AboutDlg_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam);

VOID AboutDlg(HWND hWnd);
VOID AboutDlg_SetOSText(HWND hWnd, BOOL VerifyVersion);
VOID AboutDlg_GetDpi(VOID);
VOID AboutDlg_SetWindowMove(VOID);
#endif
