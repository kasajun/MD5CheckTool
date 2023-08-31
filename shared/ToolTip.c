// ToolTip.c : アプリケーションのエントリ ポイントを定義します。
//

#include "ToolTip.h"
#include <commctrl.h>


LRESULT WINAPI SetToolTip(const HWND hTool, const HWND hWnd, const TCHAR* cpInString)
{
	TOOLINFO ti = { 0 };

	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd   = GetParent(hWnd);
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	ti.uId    = (UINT_PTR)hWnd;
	ti.lpszText = (LPTSTR)cpInString;
	return SendMessage(hTool, TTM_ADDTOOL, 0, (LPARAM)&ti);
}

LRESULT WINAPI SetToolTipText(const HWND hTool, const HWND hWnd, const TCHAR* cpInString)
{
	TOOLINFO ti = { 0 };

	ti.cbSize = sizeof(TOOLINFO);
	ti.hwnd   = GetParent(hWnd);
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	ti.uId    = (UINT_PTR)hWnd;
	ti.lpszText = (LPTSTR)cpInString;
	return SendMessage(hTool, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
}
