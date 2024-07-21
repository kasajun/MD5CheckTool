// os.c

#include "os.h"


HRESULT SetEnableThemeDialogTexture(HWND hWnd)
{
	HRESULT nRet = S_OK;

#if _MSC_VER < 1600 || defined(_NODLL)
	HMODULE hDll;

	hDll = LOADSYSLIB(_T("UXTHEME.DLL"));
	if (hDll != NULL)
	{
		BOOL (WINAPI * IsAppThemed)();
		HRESULT (WINAPI * EnableThemeDialogTexture)(HWND hWnd, DWORD dwFlags);
		const char* szFunc1 = "IsAppThemed";
		const char* szFunc2 = "EnableThemeDialogTexture";

		IsAppThemed = (BOOL(WINAPI*)())(GetProcAddress(hDll, szFunc1));
		EnableThemeDialogTexture = (HRESULT(WINAPI*)(HWND, DWORD))(GetProcAddress(hDll, szFunc2));

		if (IsAppThemed != NULL && EnableThemeDialogTexture != NULL)
		{
#endif /* _MSC_VER < 1600 || defined(_NODLL) */
			if (IsAppThemed()) {
				nRet = EnableThemeDialogTexture(hWnd, ETDT_ENABLETAB);
			}
#if _MSC_VER < 1600 || defined(_NODLL)
		}
		FreeLibrary(hDll);
	}
#endif /* _MSC_VER < 1600 || defined(_NODLL) */
	return nRet;
}

BOOL IsWindowsVersionOrGreater(const WORD wMajorVersion, const WORD wMinorVersion, const WORD wServicePackMajor)
{
	BOOL nRet = FALSE;

#if _MSC_VER < 1500
	HMODULE hDll;

	hDll = LOADSYSLIB(_T("KERNEL32.DLL"));
	if (hDll != NULL)
	{
		BOOL (WINAPI * VerifyVersionInfoW)(LPOSVERSIONINFOEXW, DWORD, DWORDLONG);
		unsigned __int64 (WINAPI * VerSetConditionMask)(unsigned __int64, DWORD, BYTE);
		const char* szFunc1 = "VerifyVersionInfoW";
		const char* szFunc2 = "VerSetConditionMask";

		VerifyVersionInfoW = (BOOL(WINAPI*)(LPOSVERSIONINFOEXW, DWORD, DWORDLONG))(GetProcAddress(hDll, szFunc1));
		VerSetConditionMask = (unsigned __int64(WINAPI*)(unsigned __int64, DWORD, BYTE))(GetProcAddress(hDll, szFunc2));

		if (VerifyVersionInfoW != NULL && VerSetConditionMask != NULL)
		{
#endif /* _MSC_VER < 1500 */
			OSVERSIONINFOEXW osVerInfo = { sizeof(osVerInfo), 0, 0, 0, 0, { 0 }, 0, 0 };
			const unsigned __int64 dwlConditionMask = VerSetConditionMask(VerSetConditionMask(VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL), VER_MINORVERSION, VER_GREATER_EQUAL), VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

			osVerInfo.dwMajorVersion = wMajorVersion;
			osVerInfo.dwMinorVersion = wMinorVersion;
			osVerInfo.wServicePackMajor = wServicePackMajor;

			nRet = VerifyVersionInfoW(&osVerInfo, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask);
#if _MSC_VER < 1500
		}
		FreeLibrary(hDll);
	}
#endif /* _MSC_VER < 1500 */
	return nRet;
}

#ifndef _INC_SDKDDKVER
#define _INC_SDKDDKVER
#define _WIN32_WINNT_NT4					0x0400
#define _WIN32_WINNT_WIN2K					0x0500
#define _WIN32_WINNT_WINXP					0x0501
#define _WIN32_WINNT_WS03					0x0502
#endif

#ifndef _WIN32_WINNT_VISTA
#define _WIN32_WINNT_WIN6					0x0600
#define _WIN32_WINNT_VISTA					0x0600
#define _WIN32_WINNT_WS08					0x0600
#define _WIN32_WINNT_LONGHORN				0x0600
#endif

#ifndef _WIN32_WINNT_WIN7
#define _WIN32_WINNT_WIN7					0x0601
#endif

#ifndef _WIN32_WINNT_WIN8
#define _WIN32_WINNT_WIN8					0x0602
#endif

#ifndef _WIN32_WINNT_WINBLUE
#define _WIN32_WINNT_WINBLUE				0x0603
#endif

#ifndef _WIN32_WINNT_WINTHRESHOLD
#define _WIN32_WINNT_WINTHRESHOLD			0x0A00
#endif

#ifndef _WIN32_WINNT_WIN10
#define _WIN32_WINNT_WIN10					0x0A00
#endif

static int nIsFuncFrag = -1;
static int nIsFunc = 0;

#define _IS_WIN95	0x0001	//  0
#define _IS_WIN98	0x0002	//  1
#define _IS_WIN98SE	0x0004	//  2
#define _IS_WINME	0x0008	//  3
#define _IS_WIN9X	0x0010	//  4
#define _IS_WINNT	0x0020	//  5
#define _IS_WIN2K	0x0040	//  6
#define _IS_WINXP	0x0080	//  7
#define _IS_WS03	0x0100	//  8
#define _IS_VISTA	0x0200	//  9
#define _IS_WIN7	0x0400	// 10
#define _IS_WIN8	0x0800	// 11
#define _IS_WIN81	0x1000	// 12
#define _IS_WIN10	0x2000	// 13
#define _IS_WIN11	0x4000	// 14


BOOL IsWin95()
{
#if _MSC_VER > 1400
	return FALSE;
#else
	if (nIsFuncFrag & _IS_WIN95 && IsWin9X())
	{
		OSVERSIONINFOEX osVerInfo = { 0 };

		nIsFuncFrag &= ~(_IS_WIN95);
		if (GetOSVersion(&osVerInfo))
		{
			const DWORD dwWinVer = osVerInfo.dwBuildNumber >> 16;

			if (dwWinVer == 0x0400) {
				nIsFunc |= (_IS_WIN95);
			}
		}
	}
	return (nIsFunc & _IS_WIN95);
#endif
}

BOOL IsWin98()
{
#if _MSC_VER > 1400
	return FALSE;
#else
	if (nIsFuncFrag & _IS_WIN98 && IsWin9X())
	{
		OSVERSIONINFOEX osVerInfo = { 0 };

		if (GetOSVersion(&osVerInfo))
		{
			const DWORD dwWinVer = osVerInfo.dwBuildNumber >> 16;

			if (dwWinVer == 0x040A)
			{
				if ((osVerInfo.dwBuildNumber & 0xFFFF) > 0x07CE) {
					nIsFunc |= (_IS_WIN98SE);
				}
				nIsFunc |= (_IS_WIN98);
			}
		}
		nIsFuncFrag &= ~(_IS_WIN98 | _IS_WIN98SE);
	}
	return (nIsFunc & _IS_WIN98) >> 1;
#endif
}

BOOL IsWin98SE()
{
#if _MSC_VER > 1400
	return FALSE;
#else
	if (nIsFuncFrag & _IS_WIN98SE && IsWin9X()) {
		IsWin98();
	}
	return (nIsFunc & _IS_WIN98SE) >> 2;
#endif
}

BOOL IsWinME()
{
#if _MSC_VER > 1400
	return FALSE;
#else
	if (nIsFuncFrag & _IS_WINME && IsWin9X())
	{
		OSVERSIONINFOEX osVerInfo = { 0 };

		if (GetOSVersion(&osVerInfo))
		{
			const DWORD dwWinVer = osVerInfo.dwBuildNumber >> 16;

			if (dwWinVer == 0x045A) {
				nIsFunc |= (_IS_WINME);
			}
		}
		nIsFuncFrag &= ~(_IS_WINME);
	}
	return (nIsFunc & _IS_WINME) >> 3;
#endif
}

BOOL IsWin9X()
{
#if _MSC_VER > 1400
	return FALSE;
#else
	if (nIsFuncFrag & _IS_WIN9X) {
		IsWin2kOrGreater();
	}
	return (nIsFunc & _IS_WIN9X) >> 4;
#endif
}

BOOL IsWin2kOrGreater()
{
#if _MSC_VER > 1400
	return TRUE;
#else
	if (nIsFuncFrag & _IS_WIN2K)
	{
		const int nRet = IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN2K), LOBYTE(_WIN32_WINNT_WIN2K), 0);
		if (nRet != 0) {
			nIsFunc |= (_IS_WIN2K);
		}
		else {
			nIsFunc |= (_IS_WIN9X | _IS_WINNT);
		}
		nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN2K);
	}
	return (nIsFunc & _IS_WIN2K) >> 6;
#endif
}

BOOL IsWinXPOrGreater()
{
#if _MSC_VER > 1500
	return TRUE;
#else
	if (nIsFuncFrag & _IS_WINXP)
	{
		const int nRet = IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0);
		if (nRet != 0)
		{
			nIsFunc |= (_IS_WIN2K | _IS_WINXP);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN2K | _IS_WINXP);
		}
		else
		{
			nIsFunc |= (_IS_WIN9X | _IS_WINNT);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WINXP);
		}
	}
	return (nIsFunc & _IS_WINXP) >> 7;
#endif
}

BOOL IsWS03OrGreater()
{
#if _MSC_VER > 1500
	return TRUE;
#else
	if (nIsFuncFrag & _IS_WS03)
	{
		const int nRet = IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WS03), LOBYTE(_WIN32_WINNT_WS03), 0);
		if (nRet != 0)
		{
			nIsFunc |= (_IS_WIN2K | _IS_WINXP | _IS_WS03);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN2K | _IS_WINXP | _IS_WS03);
		}
		else
		{
			nIsFunc |= (_IS_WIN9X | _IS_WINNT);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WS03);
		}
	}
	return (nIsFunc & _IS_WS03) >> 8;
#endif
}

BOOL IsWinVistaOrGreater()
{
	if (nIsFuncFrag & _IS_VISTA)
	{
		const int nRet = IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0);
		if (nRet != 0)
		{
			nIsFunc |= (_IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA);
		}
		else
		{
			nIsFunc |= (_IS_WIN9X | _IS_WINNT);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_VISTA);
		}
	}
	return (nIsFunc & _IS_VISTA) >> 9;
}

BOOL IsWin7OrGreater()
{
	if (nIsFuncFrag & _IS_WIN7)
	{
		const int nRet = IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0);
		if (nRet != 0)
		{
			nIsFunc |= (_IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA | _IS_WIN7);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA | _IS_WIN7);
		}
		else
		{
			nIsFunc |= (_IS_WIN9X | _IS_WINNT);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN7);
		}
	}
	return (nIsFunc & _IS_WIN7) >> 10;
}

BOOL IsWin8OrGreater()
{
	if (nIsFuncFrag & _IS_WIN8)
	{
		const int nRet = IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0);
		if (nRet != 0)
		{
			nIsFunc |= (_IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA | _IS_WIN7 | _IS_WIN8);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA | _IS_WIN7 | _IS_WIN8);
		}
		else
		{
			nIsFunc |= (_IS_WIN9X | _IS_WINNT);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN8);
		}
	}
	return (nIsFunc & _IS_WIN8) >> 11;
}

BOOL IsWin8Point1OrGreater()
{
	if (nIsFuncFrag & _IS_WIN81)
	{
		const int nRet = IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINBLUE), LOBYTE(_WIN32_WINNT_WINBLUE), 0);
		if (nRet != 0)
		{
			nIsFunc |= (_IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA | _IS_WIN7 | _IS_WIN8 | _IS_WIN81);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA | _IS_WIN7 | _IS_WIN8 | _IS_WIN81);
		}
		else
		{
			nIsFunc |= (_IS_WIN9X | _IS_WINNT);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN81);
		}
	}
	return (nIsFunc & _IS_WIN81) >> 12;
}

BOOL IsWin10OrGreater()
{
	if (nIsFuncFrag & _IS_WIN10)
	{
		const int nRet = IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN10), LOBYTE(_WIN32_WINNT_WIN10), 0);
		if (nRet != 0)
		{
			nIsFunc |= (_IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA | _IS_WIN7 | _IS_WIN8 | _IS_WIN81 | _IS_WIN10);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN2K | _IS_WINXP | _IS_WS03 | _IS_VISTA | _IS_WIN7 | _IS_WIN8 | _IS_WIN81 | _IS_WIN10);
		}
		else
		{
			nIsFunc |= (_IS_WIN9X | _IS_WINNT);
			nIsFuncFrag &= ~(_IS_WIN9X | _IS_WINNT | _IS_WIN10);
		}
	}
	return (nIsFunc & _IS_WIN10) >> 13;
}

UINT GetOSVersion(OSVERSIONINFOEX* osVerInfo)
{
	HMODULE hDll = NULL;
	UINT nRet = FALSE;


	osVerInfo->dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	hDll = LOADSYSLIB(_T("KERNEL32.DLL"));
	if (hDll != NULL)
	{
		BOOL(WINAPI * GetVersionEx)(LPOSVERSIONINFO);
#ifdef _UNICODE
		const char* szFunc1 = "GetVersionExW";
#else
		const char* szFunc1 = "GetVersionExA";
#endif

		GetVersionEx = (BOOL(WINAPI*)(LPOSVERSIONINFO))(GetProcAddress(hDll, szFunc1));
		if (GetVersionEx != NULL)
		{
			if ((GetVersionEx((LPOSVERSIONINFO)osVerInfo))) {
				nRet = 2;
			}
			else
			{
				osVerInfo->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				nRet = (GetVersionEx((LPOSVERSIONINFO)(osVerInfo))) ? TRUE : FALSE;
			}
		}
		else {
			nRet = FALSE;
		}
		FreeLibrary(hDll);
	}
	return nRet;
}

UINT RtlGetOSVersion(OSVERSIONINFOEX* osVerInfo)
{
	HMODULE hDll = NULL;
	UINT nRet = FALSE;

#ifndef UNICODE
	OSVERSIONINFOEXW o = { 0 };
	OSVERSIONINFOEXW* osVerInfoW = &o;
#endif

	hDll = LOADSYSLIB(_T("NTDLL.DLL"));
	if (hDll != NULL)
	{
		NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFO);
		const char* szFunc = "RtlGetVersion";

		osVerInfo->dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		RtlGetVersion = (NTSTATUS(WINAPI*)(LPOSVERSIONINFO))(GetProcAddress(hDll, szFunc));

#ifdef UNICODE
		if (RtlGetVersion != NULL && RtlGetVersion((LPOSVERSIONINFO)(osVerInfo)) == 0)
		{
#else
		if (RtlGetVersion != NULL && RtlGetVersion((LPOSVERSIONINFO)(osVerInfoW)) == 0)
		{
			WCHAR* src;
			unsigned char* dtc;
			int i;

			src = osVerInfoW->szCSDVersion;
			dtc = (unsigned char*)osVerInfo->szCSDVersion;

			osVerInfo->dwOSVersionInfoSize = sizeof(*osVerInfo);
			osVerInfo->dwBuildNumber = osVerInfoW->dwBuildNumber;
			osVerInfo->dwMajorVersion = osVerInfoW->dwMajorVersion;
			osVerInfo->dwMinorVersion = osVerInfoW->dwMinorVersion;
			osVerInfo->dwPlatformId = osVerInfoW->dwPlatformId;
			osVerInfo->wReserved = osVerInfoW->wReserved;
			osVerInfo->wServicePackMajor = osVerInfoW->wServicePackMajor;
			osVerInfo->wProductType = osVerInfoW->wProductType;
			osVerInfo->wSuiteMask = osVerInfoW->wSuiteMask;

			for (i = 0; i < SIZEOF_NUM(osVerInfo->szCSDVersion); i++) {
				*dtc++ = (unsigned char)*src++;
			}
			*dtc = '\0';
#endif
			nRet = 2;
		}
		else {
			nRet = FALSE;
		}

		FreeLibrary(hDll);
	}
	return nRet;
}

BOOL GetGokanMode(OSVERSIONINFOEX* osVerInfo)
{
	OSVERSIONINFOEX osVerInfo1 = { 0 };
	BOOL nRet = FALSE;

	if (GetOSVersion(&osVerInfo1) && RtlGetOSVersion(osVerInfo) && !(osVerInfo->dwBuildNumber == osVerInfo1.dwBuildNumber && osVerInfo->dwMajorVersion == osVerInfo1.dwMajorVersion && osVerInfo->dwMinorVersion == osVerInfo1.dwMinorVersion)) {
		nRet = TRUE;
	}
	return nRet;
}

TCHAR szUserAgentBuf[_OS_UA_SIZE];
BOOL nUserAgentRet;

BOOL GetUserAgentName(TCHAR* pInString, size_t nSize)
{
	if (*szUserAgentBuf != '\0') {
		goto GETUSERAGENTNAME_COPY;
	}

#if _MSC_VER < 1600
	if (IsWin2kOrGreater())
#endif
	{
		TCHAR szRegBuf[33] = _T("");
		HKEY hKey = NULL;
		LONG lRet = 0;
		DWORD dwType = REG_SZ;
		DWORD dwByte = 33;
		TCHAR* ptr;

		lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE, &hKey);
		if (lRet != ERROR_SUCCESS) {
			return FALSE;
		}

		lRet = RegQueryValueEx(hKey, _T("CurrentVersion"), 0, &dwType, (LPBYTE)szRegBuf, &dwByte);
		RegCloseKey(hKey);

		if (lRet != ERROR_SUCCESS) {
			return FALSE;
		}

		ptr = qtcscpy(szUserAgentBuf, _T("Windows NT"));
		*ptr++ = ' ';
		qtcscpy(ptr, szRegBuf);
	}

#if _MSC_VER < 1600

	else
	{
		TCHAR szRegBuf[33] = _T("");
		HKEY hKey = NULL;
		LONG lRet = 0;
		DWORD dwType = REG_SZ;
		DWORD dwByte = 33;

		lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion"), 0, KEY_QUERY_VALUE, &hKey);
		if (lRet != ERROR_SUCCESS) {
			return FALSE;
		}

		lRet = RegQueryValueEx(hKey, _T("Version"), 0, &dwType, (LPBYTE)szRegBuf, &dwByte);
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS) {
			return FALSE;
		}
		qtcscpy(szUserAgentBuf, szRegBuf);
	}
#endif

	nUserAgentRet = (BOOL)_tcslen(szUserAgentBuf);

GETUSERAGENTNAME_COPY:

	if (pInString != NULL)
	{
		if (nSize > _OS_UA_SIZE) {
			nSize = _OS_UA_SIZE;
		}
		qtcsncpy(pInString, szUserAgentBuf, nSize - 1);
		*(pInString + nSize) = '\0';
	}
	return nUserAgentRet;
}

BOOL IsUxTheme()
{
	BOOL nRet = FALSE;

#if _MSC_VER < 1600 || defined(_NODLL)
	HMODULE hDll;

	hDll = LOADSYSLIB(_T("UXTHEME.DLL"));
	if (hDll != NULL)
	{
		BOOL (WINAPI * IsAppThemed)();
		const char* szFunc = "IsAppThemed";

		IsAppThemed = (BOOL(WINAPI*)())(GetProcAddress(hDll, szFunc));
		if (IsAppThemed != NULL)
		{
#endif /* _MSC_VER < 1600 || defined(_NODLL) */
			nRet = IsAppThemed();
#if _MSC_VER < 1600 || defined(_NODLL)
		}
		FreeLibrary(hDll);
	}
#endif /* _MSC_VER < 1600 || defined(_NODLL) */
	return nRet;
}

BOOL GetAearoWindowRect(HWND hWnd, RECT *lpRect)
{
	BOOL nRet = FALSE;
	HRESULT hRet;

#if _MSC_VER < 1920 || defined(_NODLL)
	HMODULE hDll;

	hDll = LOADSYSLIB(_T("DWMAPI.DLL"));
	if (hDll != NULL)
	{
		HRESULT (WINAPI * DwmGetWindowAttribute)(HWND, DWORD, PVOID, DWORD);
		const char* szFunc = "DwmGetWindowAttribute";

		DwmGetWindowAttribute = (HRESULT(WINAPI*)(HWND, DWORD, PVOID, DWORD))(GetProcAddress(hDll, szFunc));
		if (DwmGetWindowAttribute != NULL)
		{
#endif /* _MSC_VER < 1920 || defined(_NODLL) */
			hRet = DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, lpRect, sizeof(RECT));
			nRet = SUCCEEDED(hRet) ? TRUE : FALSE;
#if _MSC_VER < 1920 || defined(_NODLL)
		}
		FreeLibrary(hDll);
	}
	else {
		nRet = GetWindowRect(hWnd, lpRect);
	}
#endif /* _MSC_VER < 1920 || defined(_NODLL) */
	return nRet;
}
