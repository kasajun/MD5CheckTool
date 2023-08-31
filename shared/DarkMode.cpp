#include "darkmode.h"
#include <tchar.h>
#include "charlib.h"
#include "charlib_sysdll.h"
#include "c20.h"


VOID (WINAPI * RtlGetNtVersionNumbers_Func)(LPDWORD, LPDWORD, LPDWORD) = NULL;
BOOL (WINAPI * SetWindowCompositionAttribute_Func)(HWND, WINDOWCOMPOSITIONATTRIBDATA*) = NULL;
// 1809 17763
BOOL (WINAPI * ShouldAppsUseDarkMode_Func)() = NULL; // ordinal 132
BOOL (WINAPI * AllowDarkModeForWindow_Func)(HWND, BOOL) = NULL; // ordinal 133
BOOL (WINAPI * AllowDarkModeForApp_Func)(BOOL) = NULL; // ordinal 135, in 1809
VOID (WINAPI * FlushMenuThemes_Func)() = NULL; // ordinal 136
VOID (WINAPI * RefreshImmersiveColorPolicyState_Func)() = NULL; // ordinal 104
BOOL (WINAPI * IsDarkModeAllowedForWindow_Func)(HWND) = NULL; // ordinal 137
BOOL (WINAPI * GetIsImmersiveColorUsingHighContrast_Func)(IMMERSIVE_HC_CACHE_MODE) = NULL; // ordinal 106
HTHEME (WINAPI * OpenNcThemeData_Func)(HWND, LPCWSTR) = NULL; // ordinal 49
// 1903 18362
BOOL (WINAPI * ShouldSystemUseDarkMode_Func)() = NULL; // ordinal 138
PreferredAppMode (WINAPI * SetPreferredAppMode_Func)(PreferredAppMode) = NULL; // ordinal 135, in 1903
BOOL (WINAPI * IsDarkModeAllowedForApp_Func)() = NULL; // ordinal 139

BOOL g_darkModeSupported = FALSE;
BOOL g_darkModeEnabled = FALSE;
DWORD g_buildNumber = 0;


BOOL AllowDarkModeForWindow(HWND hWnd, BOOL allow)
{
	if (g_darkModeSupported) {
		return AllowDarkModeForWindow_Func(hWnd, allow);
	}
	return FALSE;
}

BOOL IsHighContrast()
{
	HIGHCONTRAST highContrast = { sizeof(highContrast) };
	if (SystemParametersInfo(SPI_GETHIGHCONTRAST, sizeof(highContrast), &highContrast, FALSE)) {
		return highContrast.dwFlags & HCF_HIGHCONTRASTON;
	}
	return FALSE;
}

VOID RefreshTitleBarThemeColor(HWND hWnd)
{
	BOOL dark = FALSE;
	if (IsDarkModeAllowedForWindow_Func(hWnd) &&
		ShouldAppsUseDarkMode_Func() &&
		!IsHighContrast())
	{
		dark = TRUE;
	}

	if (g_buildNumber < 18362) {
		SetProp(hWnd, _T("UseImmersiveDarkModeColors"), (HANDLE)((INT_PTR)(dark)));
	}
	else if (SetWindowCompositionAttribute_Func)
	{
		WINDOWCOMPOSITIONATTRIBDATA data = { WCA_USEDARKMODECOLORS, &dark, sizeof(dark) };
		SetWindowCompositionAttribute_Func(hWnd, &data);
	}
}

BOOL IsColorSchemeChangeMessage(LPARAM lParam)
{
	BOOL is = FALSE;
	int nRet = FALSE;

#if _MSC_VER <= 1920
	HMODULE hDll = NULL;

	hDll = LOADSYSLIB(_T("KERNEL32.DLL"));
	if (hDll != NULL)
	{
		int (WINAPI * CompareStringOrdinal)(LPCWCH, int, LPCWCH, int, BOOL) = NULL;
		const char* szFunc = "CompareStringOrdinal";

		CompareStringOrdinal = (int(WINAPI *)(LPCWCH, int, LPCWCH, int, BOOL))(GetProcAddress(hDll, szFunc));
		if (CompareStringOrdinal != NULL)
		{
#endif

			nRet = CompareStringOrdinal((LPCWCH)lParam, -1, L"ImmersiveColorSet", -1, TRUE);

#if _MSC_VER <= 1920
		}
		FreeLibrary(hDll);
	}
#endif

	if (lParam && nRet == CSTR_EQUAL)
	{
		RefreshImmersiveColorPolicyState_Func();
		is = TRUE;
	}
	GetIsImmersiveColorUsingHighContrast_Func(IHCM_REFRESH);
	return is;
}

BOOL IsColorSchemeChangeMessage(UINT message, LPARAM lParam)
{
	if (message == WM_SETTINGCHANGE) {
		return IsColorSchemeChangeMessage(lParam);
	}
	return FALSE;
}

VOID AllowDarkModeForApp(BOOL allow)
{
	if (AllowDarkModeForApp_Func) {
		AllowDarkModeForApp_Func(allow);
	}
	else if (SetPreferredAppMode_Func) {
		SetPreferredAppMode_Func(allow ? AllowDark : Default);
	}
	return;
}

BOOL CheckBuildNumber(DWORD buildNumber)
{
	return (buildNumber == 17763 || // 1809
		buildNumber == 18362 || // 1903
		buildNumber == 18363 || // 1909
		buildNumber == 19041 || // 2004
		buildNumber == 19044);
}

HMODULE hUxTheme = NULL;
HMODULE hUser32 = NULL;

BOOL InitDarkMode()
{
	HMODULE hDll = NULL;
	DWORD dwMajor = 0;
	DWORD dwMinor = 0;
	FARPROC ord135 = NULL;

	hDll = LOADSYSLIB(_T("NTDLL.DLL"));
	IF_UNLIKELY(hDll == NULL) {
		goto NTDLL_EXIT;
	}

	RtlGetNtVersionNumbers_Func = (VOID(WINAPI *)(LPDWORD major, LPDWORD minor, LPDWORD build))(GetProcAddress(hDll, "RtlGetNtVersionNumbers"));
	IF_UNLIKELY(RtlGetNtVersionNumbers_Func == NULL) {
		goto NTDLL_EXIT;
	}

	RtlGetNtVersionNumbers_Func(&dwMajor, &dwMinor, &g_buildNumber);
	g_buildNumber &= ~0xF0000000;

NTDLL_EXIT:

	if (hDll) {
		FreeLibrary(hDll);
	}

	IF_UNLIKELY(RtlGetNtVersionNumbers_Func == NULL) {
		return FALSE;
	}

	if (!(dwMajor == 10 && dwMinor == 0 && CheckBuildNumber(g_buildNumber))) {
		return FALSE;
	}

	hUxTheme = LOADSYSLIB(_T("UXTHEME.DLL"));
	IF_UNLIKELY(hUxTheme == NULL) {
		return FALSE;
	}

	OpenNcThemeData_Func = (HTHEME(WINAPI*)(HWND, LPCWSTR))(GetProcAddress(hUxTheme, MAKEINTRESOURCEA(49)));
	RefreshImmersiveColorPolicyState_Func = (VOID(WINAPI*)())(GetProcAddress(hUxTheme, MAKEINTRESOURCEA(104)));
	GetIsImmersiveColorUsingHighContrast_Func = (BOOL(WINAPI*)(IMMERSIVE_HC_CACHE_MODE))(GetProcAddress(hUxTheme, MAKEINTRESOURCEA(106)));
	ShouldAppsUseDarkMode_Func = (BOOL(WINAPI*)())(GetProcAddress(hUxTheme, MAKEINTRESOURCEA(132)));
	AllowDarkModeForWindow_Func = (BOOL(WINAPI*)(HWND, BOOL))(GetProcAddress(hUxTheme, MAKEINTRESOURCEA(133)));

	ord135 = GetProcAddress(hUxTheme, MAKEINTRESOURCEA(135));
	if (g_buildNumber < 18362) {
		AllowDarkModeForApp_Func = (BOOL(WINAPI*)(BOOL))(ord135);
	}
	else {
		SetPreferredAppMode_Func = (PreferredAppMode(WINAPI*)(PreferredAppMode))(ord135);
	}

	//FlushMenuThemes_Func = (VOID(WINAPI*)())(GetProcAddress(hUxTheme, MAKEINTRESOURCEA(136)));
	IsDarkModeAllowedForWindow_Func = (BOOL(WINAPI*)(HWND))(GetProcAddress(hUxTheme, MAKEINTRESOURCEA(137)));

	hUser32 = LOADSYSLIB(_T("USER32.DLL"));
	IF_UNLIKELY(hUser32 == NULL) {
		return FALSE;
	}

	SetWindowCompositionAttribute_Func = (BOOL(WINAPI*)(HWND, WINDOWCOMPOSITIONATTRIBDATA*))(GetProcAddress(hUser32, "SetWindowCompositionAttribute"));

	if (OpenNcThemeData_Func &&
		RefreshImmersiveColorPolicyState_Func &&
		ShouldAppsUseDarkMode_Func &&
		AllowDarkModeForWindow_Func &&
		(AllowDarkModeForApp_Func || SetPreferredAppMode_Func) &&
		//_FlushMenuThemes &&
		IsDarkModeAllowedForWindow_Func)
	{
		g_darkModeSupported = TRUE;

		AllowDarkModeForApp(TRUE);
		RefreshImmersiveColorPolicyState_Func();

		g_darkModeEnabled = ShouldAppsUseDarkMode_Func() && !IsHighContrast();
	}
	return TRUE;
}

BOOL ShutdownDarkMode()
{
	if (hUxTheme)
	{
		FreeLibrary(hUxTheme);
		hUxTheme = NULL;
	}

	if (hUser32)
	{
		FreeLibrary(hUser32);
		hUser32 = NULL;
	}
	return TRUE;
}

HRESULT DarkMode_SetWindowTheme(HWND hWnd)
{
	HRESULT hRet = S_OK;

	if (g_darkModeSupported)
	{
#if _MSC_VER <= 1920
		HMODULE hDll;

		hDll = LOADSYSLIB(_T("UXTHEME.DLL"));
		if (hDll)
		{
			HRESULT(WINAPI * SetWindowTheme)(HWND, LPCWSTR, LPCWSTR);
			const char* szFunc = "SetWindowTheme";

			SetWindowTheme = (HRESULT(WINAPI *)(HWND, LPCWSTR, LPCWSTR))(GetProcAddress(hDll, szFunc));
			if (SetWindowTheme)
			{
#endif
				hRet = SetWindowTheme(hWnd, L"Explorer", NULL);

#if _MSC_VER <= 1920
			}
			FreeLibrary(hDll);
		}
#endif
		SendMessage(hWnd, WM_THEMECHANGED, 0, 0);
	}
	return hRet;
}

BOOL DarkMode_ThemeChanged()
{
	if (ShouldAppsUseDarkMode_Func != NULL) {
		g_darkModeEnabled = ShouldAppsUseDarkMode_Func();
	}
	return g_darkModeEnabled;
}
