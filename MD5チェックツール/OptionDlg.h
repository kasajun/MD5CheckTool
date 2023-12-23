#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __OPTIONDLG_H_BE50A9D5_D6B5_4c8b_AD33_26407F1618C0__
#define __OPTIONDLG_H_BE50A9D5_D6B5_4c8b_AD33_26407F1618C0__

typedef struct tagOptionWindow {
	DWORD dwStructSize; // バージョンアップ用に予約
	HWND hWnd;
	HWND hTab;
	HWND hWndTab[4];
	HWND hIDOK;
	HWND hIDCANCEL;
	HWND hTab1Group[2];
	HWND hTab1Checkbox[9];
	HWND hTab1Combo[1];
	HWND hTab2Group[1];
	HWND hTab2Static[1];
	HWND hTab2Checkbox[6];
	HWND hTab2Edit[2];
	HWND hTab2Button[1];
	HWND hTab3Group[1];
	HWND hTab3Checkbox[3];
	HWND hTab3Static[2];
	HWND hTab3Static1[3][5];
	HWND hTab4Group[1];
	HWND hTab4Radio[6];
	TCHAR* pBuf;
	TCHAR* pCheckSumFile;
	DWORD dwAppFrag;
	int nActiveTab;
	DWORD dwHashType;
	DWORD dwRadioState[2];
	UINT nWindowMessage;
} OptionWindow, *lpOptionWindow;

// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK OptionDlg_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT OptionDlg_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT OptionDlg_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlg_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam);

#include "OptionDlgTab1.h"
#include "OptionDlgTab2.h"
#include "OptionDlgTab3.h"
#include "OptionDlgTab4.h"

VOID OptionDlg(HWND hWnd);
VOID GetIniFileSetting(const TCHAR* cpInIniFile);
VOID SetIniFileSetting(const TCHAR* cpInIniFile);
VOID GetCheckSumFile(TCHAR* pInBuf);
VOID SetChackSumFile(VOID);
VOID OptionDlg_SetWindowMove(VOID);
VOID OptionDlg_TabChange(int nTab);
VOID OptionDlg_HashCpuMode(DWORD dwCpuMode);

#endif
