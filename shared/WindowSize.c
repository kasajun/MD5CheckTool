// WindowSize.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "WindowSize.h"
#include "c20.h"

TCHAR szWindowSizeBuf[32];
TCHAR szOldWindowSizeBuf[32];
int nDpi;
int nDeskTopX;
int nDeskTopY;

static int gLoad_nState = -1;
static RECT gLoad_rc = { (LONG)-1 };


int GetDpi(HWND hWnd)
{
	HDC hDC = NULL;
	int nRet = 96;

	hDC = GetDC(hWnd);
	IF_LIKELY(hDC)
	{
		nRet = GetDeviceCaps(hDC, LOGPIXELSX);
		ReleaseDC(hWnd, hDC);
	}
	return nRet;
}

BOOL WindowSize_GetScreen(HWND hWnd)
{
	nDpi = GetDpi(hWnd);
	nDeskTopX = GetSystemMetrics(SM_CXSCREEN);
	nDeskTopY = GetSystemMetrics(SM_CYSCREEN);

	STPRINTF_FUNC(szWindowSizeBuf,
#if _MSC_VER > 1300
		SIZEOF_NUM(szWindowSizeBuf),
#endif
		_T("WindowSize%dx%dx%d"), nDeskTopX, nDeskTopY, nDpi);
	return TRUE;
}

BOOL WindowSize_SaveState(HWND hWnd, LPCTSTR cpInWindowName, LPCTSTR cpInIniFile)
{
	WINDOWPLACEMENT wndPlace = { 0 };
	RECT rc = { 0 };
	int nState = 0;
	TCHAR szNum[64] = { 0 };
	BOOL nRet = 0;

	WindowSize_GetScreen(hWnd);
	wndPlace.length = sizeof(WINDOWPLACEMENT);
	nRet = GetWindowPlacement(hWnd, &wndPlace);
	IF_UNLIKELY(nRet == FALSE) {
		return FALSE;
	}

	//ウィンドウ状態
	switch (wndPlace.showCmd)
	{
	case SW_SHOWMAXIMIZED:
		//最大化
		rc = wndPlace.rcNormalPosition;
		nState = 1;
		break;
	case SW_SHOWMINIMIZED:
		//最小化
		rc = wndPlace.rcNormalPosition;
		nState = 2;
		break;
	default:
		nRet = GetWindowRect(hWnd, &rc);
		IF_UNLIKELY(nRet == FALSE) {
			return FALSE;
		}
		else
		{
			MONITORINFO mInfo = { 0 };
			HMONITOR hMonitor1 = NULL;
			HMONITOR hMonitor2 = NULL;
			APPBARDATA ad = { 0 };
			UINT_PTR uRet = 0;

#if _MSC_VER < 1400
			HMODULE hDll = NULL;

			hDll = LOADSYSLIB(_T("USER32.DLL"));
			if (hDll != NULL)
			{
				BOOL (WINAPI * GetMonitorInfo)(HMONITOR hMonitor, LPMONITORINFO lpmi);
				HMONITOR (WINAPI * MonitorFromRect)(LPCRECT lprc, DWORD dwFlags);
#ifdef _UNICODE
				const char* szFunc1 = "GetMonitorInfoW";
#else
				const char* szFunc1 = "GetMonitorInfoA";
#endif
				const char* szFunc2 = "MonitorFromRect";

				GetMonitorInfo = (BOOL(WINAPI*)(HMONITOR, LPMONITORINFO))(GetProcAddress(hDll, szFunc1));
				MonitorFromRect = (HMONITOR(WINAPI*)(LPCRECT, DWORD))(GetProcAddress(hDll, szFunc2));

				if (GetMonitorInfo != NULL && MonitorFromRect != NULL)
				{
#endif
					//ウィンドウのあるモニタハンドルを取得
					hMonitor1 = MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST);
					mInfo.cbSize = sizeof(MONITORINFO);
					nRet = GetMonitorInfo(hMonitor1, &mInfo);
					IF_UNLIKELY(nRet == FALSE) {
						goto MONITOR_INFO_EXIT;
					}

					//タスクバーのあるモニタハンドルを取得
					ad.cbSize = sizeof(APPBARDATA);
					ad.hWnd = NULL;
					SHAppBarMessage(ABM_GETTASKBARPOS, &ad);
					hMonitor2 = MonitorFromRect(&(ad.rc), MONITOR_DEFAULTTONEAREST);
					IF_UNLIKELY(hMonitor2 == NULL) {
						goto MONITOR_INFO_EXIT;
					}

					//ウィンドウとタスクバーが同じモニタにある
					if (hMonitor1 == hMonitor2 && (HWND)SHAppBarMessage(ABM_GETAUTOHIDEBAR, &ad) == NULL)
					{
						//常に表示
						uRet = SHAppBarMessage(ABM_GETTASKBARPOS, &ad);

						if (uRet == 1)
						{
							//座標補正
							switch (ad.uEdge)
							{
							case ABE_TOP:
								rc.top = rc.top - (ad.rc.bottom - ad.rc.top);
								rc.bottom = rc.bottom - (ad.rc.bottom - ad.rc.top);
								break;
							case ABE_LEFT:
								rc.left = rc.left - (ad.rc.right - ad.rc.left);
								rc.right = rc.right - (ad.rc.right - ad.rc.left);
								break;
							}
						}
					}

					if (rc.bottom == wndPlace.rcNormalPosition.bottom
						&& rc.left == wndPlace.rcNormalPosition.left
						&& rc.right == wndPlace.rcNormalPosition.right
						&& rc.top == wndPlace.rcNormalPosition.top)
					{
						//通常
						rc = wndPlace.rcNormalPosition;
						nState = 0;
					}
					else
					{
						//Aero snap
						nState = 3;
					}
#if _MSC_VER < 1400
				}
			}
#endif

		}
		break;
	}
MONITOR_INFO_EXIT:


	if (rc.left != gLoad_rc.left || rc.top != gLoad_rc.top || rc.right != gLoad_rc.right || rc.bottom != gLoad_rc.bottom)
	{
		STPRINTF_FUNC(szNum,
#if _MSC_VER > 1300
			SIZEOF_NUM(szNum),
#endif			
			_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
		WritePrivateProfileString(cpInWindowName, szWindowSizeBuf, szNum, cpInIniFile);
	}

	if (gLoad_nState != nState)
	{
		ITOT_FUNC(nState, szNum, SIZEOF_NUM(szNum), 10);
		WritePrivateProfileString(cpInWindowName, _T("WindowState"), szNum, cpInIniFile);
	}
	return TRUE;
}

BOOL WindowSize_LoadState(HWND hWnd, LPCTSTR cpInWindowName, LPCTSTR cpInIniFile)
{
	WINDOWPLACEMENT wndPlace = { 0 };
	RECT rc = { 0 };
	RECT rcWnd = { 0 };
	int nState = 0;
	int n = 0;
	TCHAR szNum[64] = { 0 };
	BOOL nRet = 0;

	WindowSize_GetScreen(hWnd);
	wndPlace.length = sizeof(WINDOWPLACEMENT);
	nRet = GetWindowPlacement(hWnd, &wndPlace);
	IF_UNLIKELY(nRet == FALSE) {
		return FALSE;
	}

	//ウィンドウ状態
	switch (wndPlace.showCmd)
	{
	case SW_SHOWMAXIMIZED:
		//最大化
		rcWnd = wndPlace.rcNormalPosition;
		nState = 1;
		break;
	case SW_SHOWMINIMIZED:
		//最小化
		rcWnd = wndPlace.rcNormalPosition;
		nState = 2;
		break;
	default:
		nRet = GetWindowRect(hWnd, &rcWnd);
		IF_UNLIKELY(nRet == FALSE) {
			return FALSE;
		}
		else
		{
			MONITORINFO mInfo = { 0 };
			HMONITOR hMonitor1 = NULL;
			HMONITOR hMonitor2 = NULL;
			APPBARDATA ad = { 0 };

#if _MSC_VER < 1400
			HMODULE hDll = NULL;

			hDll = LOADSYSLIB(_T("USER32.DLL"));
			if (hDll != NULL)
			{
				BOOL (WINAPI * GetMonitorInfo)(HMONITOR hMonitor, LPMONITORINFO lpmi);
				HMONITOR (WINAPI * MonitorFromRect)(LPCRECT lprc, DWORD dwFlags);
#ifdef _UNICODE
				const char* szFunc1 = "GetMonitorInfoW";
#else
				const char* szFunc1 = "GetMonitorInfoA";
#endif
				const char* szFunc2 = "MonitorFromRect";

				GetMonitorInfo = (BOOL(WINAPI*)(HMONITOR, LPMONITORINFO))(GetProcAddress(hDll, szFunc1));
				MonitorFromRect = (HMONITOR(WINAPI*)(LPCRECT, DWORD))(GetProcAddress(hDll, szFunc2));

				if (GetMonitorInfo != NULL && MonitorFromRect != NULL)
				{
#endif

					//ウィンドウのあるモニタハンドルを取得
					hMonitor1 = MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST);
					mInfo.cbSize = sizeof(MONITORINFO);
					nRet = GetMonitorInfo(hMonitor1, &mInfo);
					IF_UNLIKELY(nRet == FALSE) {
						goto MONITOR_INFO_EXIT;
					}

					//タスクバーのあるモニタハンドルを取得
					ad.cbSize = sizeof(APPBARDATA);
					ad.hWnd = NULL;
					SHAppBarMessage(ABM_GETTASKBARPOS, &ad);
					hMonitor2 = MonitorFromRect(&(ad.rc), MONITOR_DEFAULTTONEAREST);
					IF_UNLIKELY(hMonitor2 == NULL) {
						goto MONITOR_INFO_EXIT;
					}

					//ウィンドウとタスクバーが同じモニタにある
					if (hMonitor1 == hMonitor2 && (HWND)SHAppBarMessage(ABM_GETAUTOHIDEBAR, &ad) == NULL)
					{
						//常に表示
						const UINT_PTR unRet = SHAppBarMessage(ABM_GETTASKBARPOS, &ad);

						if (unRet == 1)
						{
							int nDiff;

							//座標補正
							switch (ad.uEdge)
							{
							case ABE_TOP:
								nDiff = (ad.rc.bottom - ad.rc.top);
								rcWnd.top = rcWnd.top - nDiff;
								rcWnd.bottom = rcWnd.bottom - nDiff;
								nDeskTopY -= nDiff;
								break;
							case ABE_LEFT:
								nDiff = (ad.rc.right - ad.rc.left);
								rcWnd.left = rcWnd.left - nDiff;
								rcWnd.right = rcWnd.right - nDiff;
								nDeskTopX -= nDiff;
								break;
							}
						}
					}

					if (rcWnd.bottom == wndPlace.rcNormalPosition.bottom
						&& rcWnd.left == wndPlace.rcNormalPosition.left
						&& rcWnd.right == wndPlace.rcNormalPosition.right
						&& rcWnd.top == wndPlace.rcNormalPosition.top)
					{
						//通常
						rcWnd = wndPlace.rcNormalPosition;
					}

#if _MSC_VER < 1400
				}
			}
#endif

		}
		break;
	}
MONITOR_INFO_EXIT:

	// デフォルトのウィンドウ位置取得
	nRet = GetWindowRect(hWnd, &rc);//GetClientRect
	IF_UNLIKELY(nRet == FALSE) {
		return FALSE;
	}

	// ウィンドウ位置読み込み
	// 無ければデフォルトを返す
	nState = (int)GetPrivateProfileString(cpInWindowName, szWindowSizeBuf, _T(""), szNum, SIZEOF_NUM(szNum), cpInIniFile);
	if (nState == 0) {
		return FALSE;
	}
	else if (nState > 6)
	{
#ifdef _DEBUG
		OutputDebugString(_T("WindowSize: WindowSize_LoadState()\r\nState: 0\r\n"));
#endif
		STSCANF_FUNC(szNum, _T("%ld,%ld,%ld,%ld"), &(rc.left), &(rc.top), &(rc.right), &(rc.bottom));

		gLoad_rc.left = rc.left;
		gLoad_rc.top = rc.top;
		gLoad_rc.right = rc.right;
		gLoad_rc.bottom = rc.bottom;
	}

	nState = GetPrivateProfileInt(cpInWindowName, _T("WindowState"), -1, cpInIniFile);
	gLoad_nState = nState;

	n = rc.left - rcWnd.left;
	rcWnd.left += n;
	rcWnd.right += n;

	n = rc.top - rcWnd.top;
	rcWnd.top += n;
	rcWnd.bottom += n;

	if (rcWnd.right > nDeskTopX)
	{
		const int nDiffX = rcWnd.right - nDeskTopX;
		rc.left -= nDiffX;
		rc.right -= nDiffX;
	}

	if (rcWnd.bottom > nDeskTopY)
	{
		const int nDiffY = rcWnd.bottom - nDeskTopY;
		rc.top -= nDiffY;
		rc.bottom -= nDiffY;
	}

	//ウィンドウ位置コピー
	wndPlace.rcNormalPosition = rc;

	switch (nState)
	{
	case 0:
		//通常
	case 2:
		//最小化
	case 3:
		//Aero snap
		wndPlace.showCmd = SW_SHOW;
		SetWindowPlacement(hWnd, &wndPlace);
		break;
	case 1:
		//最大化
		wndPlace.showCmd = SW_SHOWMAXIMIZED;
		SetWindowPlacement(hWnd, &wndPlace);
		break;
	}
	InvalidateRect(hWnd, NULL, TRUE);
	return TRUE;
}
