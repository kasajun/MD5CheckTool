#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __OPTIONDLGTAB1_H_BE50A9D5_D6B5_4c8b_AD33_26407F1618C0__
#define __OPTIONDLGTAB1_H_BE50A9D5_D6B5_4c8b_AD33_26407F1618C0__

// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK OptionDlgTab1_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT OptionDlgTab1_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT OptionDlgTab1_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab1_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif
