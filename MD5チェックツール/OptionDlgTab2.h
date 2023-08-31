#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __OPTIONDLGTAB2_H_BE50A9D5_D6B5_4c8b_AD33_26407F1618C0__
#define __OPTIONDLGTAB2_H_BE50A9D5_D6B5_4c8b_AD33_26407F1618C0__

#include "OptionDlg.h"


// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK OptionDlgTab2_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT OptionDlgTab2_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnButton1(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT OptionDlgTab2_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab2_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif
