#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __OPTIONDLGTAB3_H_BE50A9D5_D6B5_4c8b_AD33_26407F1618C0__
#define __OPTIONDLGTAB3_H_BE50A9D5_D6B5_4c8b_AD33_26407F1618C0__

#include "OptionDlg.h"


// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK OptionDlgTab3_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT OptionDlgTab3_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnCheck1(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnCheck2(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnCheck3(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnIdOk(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnIdCancel(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnSysColorChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnThemeChanged(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT OptionDlgTab3_OnMessageWindowSet(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OptionDlgTab3_OnMessageDpiChange(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif
