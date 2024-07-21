// os_full.c

#include "os_full.h"


BOOL GetOSRegRead(TCHAR* pInString)
{
	TCHAR szRegBuf[_OS_FULL_SIZE] = _T("");
	TCHAR* ptr = NULL;
	HKEY hKey = NULL;
	LONG lRet = 0;
	DWORD dwType = REG_SZ;
	DWORD dwByte = _OS_FULL_SIZE;

	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE, &hKey);
	if (lRet != ERROR_SUCCESS) {
		return FALSE;
	}

	lRet = RegQueryValueEx(hKey, _T("ProductName"), 0, &dwType, (LPBYTE)szRegBuf, &dwByte);
	if (lRet != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
#if _MSC_VER < 1500
		lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion"), 0, KEY_QUERY_VALUE, &hKey);
		if (lRet != ERROR_SUCCESS) {
			return FALSE;
		}

		lRet = RegQueryValueEx(hKey, _T("Version"), 0, &dwType, (LPBYTE)szRegBuf, &dwByte);
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS) {
			return FALSE;
		}
		qtcscpy(pInString, szRegBuf);
		return TRUE;
#else
		return FALSE;
#endif
	}

	ptr = qtcscpy(pInString, szRegBuf);

	// サービスパックの取得
	lRet = RegQueryValueEx(hKey, _T("CSDVersion"), 0, &dwType, (LPBYTE)&szRegBuf, &dwByte);
	RegCloseKey(hKey);
	if (lRet == ERROR_SUCCESS)
	{
		*ptr++ = ' ';
		qtcscpy(ptr, szRegBuf);
	}
	return TRUE;
}


TCHAR szOSFullBuf[_OS_FULL_SIZE];
TCHAR szEditionBuf[_OS_FULL_SIZE];
TCHAR szSubBuf[_OS_FULL_SIZE];
UINT nOSFullRet;

UINT GetOSName(TCHAR* pInString, size_t nSize, BOOL nIsFullName, BOOL nIsVerifyVersion)
{
	OSVERSIONINFOEX osVerInfo = { 0 };
	const TCHAR* pOS = _T("");
	const TCHAR* pEdition = _T("");
	TCHAR* pSub = szSubBuf;
	TCHAR* pEditionBuf = szEditionBuf;
	TCHAR* ptr = szOSFullBuf;
	DWORD dwOSFrag = 0;
	UINT nRet = 0;

#define _OS_IS_NT40EX		0x00000001
#define _OS_IS_R2			0x00000002
#define _OS_IS_GOKANMODE	0x00000004


	if (*szOSFullBuf != '\0') {
		goto GETOSNAME_COPY;
	}

	nRet = GetOSVersion(&osVerInfo);
	if (nRet == FALSE) {
		return 0;
	}

	if (nIsVerifyVersion != FALSE) {
		nRet = GetGokanMode(&osVerInfo);
	}

	if (nRet == FALSE) {
		return FALSE;
	}

	switch (osVerInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_WINDOWS:
#if _MSC_VER >= 1500
		// Visual C++ 2008 から Windows 98 / Me のコード省略
		// NT系の互換モードを使わない限りここに来られません（＾＾；

		if (osVerInfo.dwMinorVersion == 0) {
			pOS = _T("Windows 95");
		}
		else {
			pOS = _T("Windows 98 / Me");
		}

		if (nIsFullName != FALSE)
		{
			pEdition = _T("互換モード");
			dwOSFrag |= _OS_IS_GOKANMODE;
		}
#else
		switch (osVerInfo.dwMinorVersion)
		{
		case 0:
			pOS = _T("Windows 95");

#if _MSC_VER >= 1400
			// VC++2005 から Windows 95 のコード省略
			if (nIsFullName != FALSE)
			{
				pEdition = _T("互換モード");
				dwOSFrag |= _OS_IS_GOKANMODE;
			}
#else
			// Windows 95 の互換モードにするとバグなのか「950」が返ります。通常は「67109814」が返ります。
			if (nIsFullName != FALSE)
			{
				if (IsWin2kOrGreater())	// || osVerInfo.dwBuildNumber < 0xFFFF
				{
					pEdition = _T("互換モード");
					dwOSFrag |= _OS_IS_GOKANMODE;
				}
				else
				{
					switch (osVerInfo.szCSDVersion[1])
					{
					case 'A':
						pEdition = _T("OSR 1");
						break;
					case 'B':
						if ((osVerInfo.dwBuildNumber & 0xFFFF) > 0x0457) {
							pEdition = _T("OSR 2.1");
						}
						else {
							pEdition = _T("OSR 2");
						}
						break;
					case 'C':
						pEdition = _T("OSR 2.5");
						break;
					}
				}
			}
#endif
			break;
		case 10:
			pOS = _T("Windows 98");

			if (nIsFullName != FALSE)
			{
				if (IsWin2kOrGreater())
				{
					pEdition = _T("互換モード");
					dwOSFrag |= _OS_IS_GOKANMODE;
				}
				else if ((osVerInfo.dwBuildNumber & 0xFFFF) > 1998) {
					pEdition = _T("Second Edition");
				}
				else
				{
					TCHAR szRegBuf[32] = _T("");
					HKEY hKey = NULL;
					DWORD dwType = REG_SZ;
					DWORD dwByte = 32;

					nRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Active Setup\\Installed Components\\{91d67b80-5482-11d2-bcc5-0000f83002c6}"), 0, KEY_QUERY_VALUE, &hKey);
					if (nRet == ERROR_SUCCESS)
					{
						RegQueryValueEx(hKey, _T(""), 0, &dwType, (LPBYTE)szRegBuf, &dwByte);
						RegCloseKey(hKey);

						if (_tcsicmp(szRegBuf, _T("Windows 98 Service Pack 1")) == 0) {
							osVerInfo.wServicePackMajor = 1;
						}
					}
				}
			}
			break;
		case 90:
			pOS = _T("Windows Me");

			if (nIsFullName != FALSE && IsWin2kOrGreater())
			{
				pEdition = _T("互換モード");
				dwOSFrag |= _OS_IS_GOKANMODE;
			}
			break;
		}
#endif
		break;
	case VER_PLATFORM_WIN32_NT:
		switch (osVerInfo.dwMajorVersion)
		{
		case 3:
		case 4:
#if _MSC_VER >= 1400
			// VC++2005 から Windows NT 4.0 のコード省略
			pOS = _T("Windows NT 4.0");
			dwOSFrag |= _OS_IS_NT40EX;

			if (nIsFullName != FALSE)
			{
				pEdition = _T("互換モード");
				dwOSFrag |= _OS_IS_GOKANMODE;
			}
#else
			pOS = (osVerInfo.dwMajorVersion == 4) ? _T("Windows NT 4.0") : _T("Windows NT 3.51");

			if (nIsFullName != FALSE)
			{
				if (nRet == 1)
				{
					// NT 4.0 SP6 だけのはず
					dwOSFrag |= _OS_IS_NT40EX;

					if (osVerInfo.wProductType == VER_NT_WORKSTATION) {
						pEdition = _T("Workstation");
					}
					else {
						pEdition = _T("Server");
					}
				}
				else
				{
					// NT 4.0 SP5 以前はレジストリから取得
					TCHAR szRegBuf[_OS_FULL_SIZE] = _T("");
					HKEY  hKey = NULL;
					DWORD dwByte = _OS_FULL_SIZE;

					nRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey);
					if (nRet == ERROR_SUCCESS)
					{
						nRet = RegQueryValueEx(hKey, _T("ProductType"), NULL, NULL, (LPBYTE)szRegBuf, &dwByte);
						RegCloseKey(hKey);

						if (_tcsicmp(szRegBuf, _T("LANMANNT")) == 0) {
							pEdition = _T("Server");
						}
						else if (_tcsicmp(szRegBuf, _T("SERVERNT")) == 0) {
							pEdition = _T("Enterprise");
						}
						else if (_tcsicmp(szRegBuf, _T("WINNT")) == 0) {
							pEdition = _T("Workstation");
						}
					}
				}
			}
#endif
			break;
		case 5:
			switch (osVerInfo.dwMinorVersion)
			{
			case 0:
				// Windows 2000
				pOS = _T("Windows 2000");

				if (nIsFullName != FALSE)
				{
#if _MSC_VER < 1700
					if (IsWinXPOrGreater())
					{
						pEdition = _T("互換モード");
						dwOSFrag |= _OS_IS_GOKANMODE;
					}
					else if (osVerInfo.wProductType == VER_NT_WORKSTATION) {
						pEdition = _T("Professional");
					}
					else
					{
						if (osVerInfo.wSuiteMask & VER_SUITE_DATACENTER) {
							pEdition = _T("Datacenter Server");
						}
						else if (osVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE) {
							pEdition = _T("Advanced Server");
						}
						else {
							pEdition = _T("Server");
						}
					}
#else
					pEdition = _T("互換モード");
					dwOSFrag |= _OS_IS_GOKANMODE;
#endif // _MSC_VER < 1700

				}
				break;
			case 1:
				// Windows XP
				pOS = _T("Windows XP");

				if (nIsFullName != FALSE)
				{
#if _MSC_VER < 1920
					if (IsWS03OrGreater())
					{
						pEdition = _T("互換モード");
						dwOSFrag |= _OS_IS_GOKANMODE;
					}
					else
					{
						if (GetSystemMetrics(SM_TABLETPC)) {
							pEdition = _T("Tablet PC Edition");
						}
						else if (GetSystemMetrics(SM_MEDIACENTER)) {
							pEdition = _T("Media Center Edition");
						}
						else if (GetSystemMetrics(SM_STARTER)) {
							pEdition = _T("Starter Edition");
						}
						else if (osVerInfo.wSuiteMask & VER_SUITE_PERSONAL) {
							pEdition = _T("Home Edition");
						}
						else if (osVerInfo.wSuiteMask & (VER_SUITE_EMBEDDEDNT | VER_SUITE_EMBEDDED_RESTRICTED)) {
							pEdition = _T("Embedded");
						}
						else {
							pEdition = _T("Professional");
						}
					}
#else
					pEdition = _T("互換モード");
					dwOSFrag |= _OS_IS_GOKANMODE;
#endif
				}
				break;
			case 2:
				// Windows Server 2003 / XP x64 Edition
				if (osVerInfo.wProductType == VER_NT_WORKSTATION)
				{
					pOS = _T("Windows XP");

					if (nIsFullName != FALSE)
					{
#if _MSC_VER < 1920
						if (IsWinVistaOrGreater())
						{
#endif
							pEdition = _T("x64 互換モード");
							dwOSFrag |= _OS_IS_GOKANMODE;
#if _MSC_VER < 1920
						}
						else {
							pEdition = _T("Professional x64 Edition");
						}
#endif

					}
				}
				else
				{
					// Windows Server 2003 ファミリー
					pOS = _T("Windows Server 2003");

					if (nIsFullName != FALSE)
					{
						if (IsWinVistaOrGreater())
						{
							pEdition = _T("互換モード");
							dwOSFrag |= _OS_IS_GOKANMODE;
						}
						// Compute Cluster Server には R2 も x64 もつかない
						else
						{
							if (osVerInfo.wSuiteMask & VER_SUITE_WH_SERVER) {
								pOS = _T("Windows Home Server");
							}
							else
							{
								// R2 か
								if (GetSystemMetrics(SM_SERVERR2)) {
									dwOSFrag |= _OS_IS_R2;
								}

								// Edition
								if (osVerInfo.wSuiteMask & VER_SUITE_DATACENTER) {
									pEdition = _T("Datacenter Edition");
								}
								else if (osVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE) {
									pEdition = _T("Enterprise Edition");
								}
								else if (osVerInfo.wSuiteMask & VER_SUITE_BLADE) {
									pEdition = _T("Web Edition");
								}
								else {
									pEdition = _T("Standard Edition");
								}
							}
						}
					}
				}
				break;
			}
			break;
		case 6:
			// Vista / 2008 / 7 / 2008 R2 / 8 / 2012 / 8.1 / 10 TP
			if (osVerInfo.wProductType == VER_NT_WORKSTATION)
			{
				switch (osVerInfo.dwMinorVersion)
				{
				case 0:
					pOS = _T("Windows Vista");
					break;
				case 1:
					pOS = _T("Windows 7");
					break;
				case 2:
					pOS = _T("Windows 8");
					break;
				case 3:
					pOS = _T("Windows 8.1");
					break;
				case 4:
					pOS = _T("Windows 10 Technical Preview");
					break;
				}
			}
			// Server 2008 / R2
			else
			{
				switch (osVerInfo.dwMinorVersion)
				{
				case 0:
					pOS = _T("Windows Server 2008");
					break;
				case 1:
					pOS = _T("Windows Server 2008");
					dwOSFrag |= _OS_IS_R2;
					break;
				case 2:
					pOS = _T("Windows Server 2012");
					break;
				case 3:
					pOS = _T("Windows Server 2012");
					dwOSFrag |= _OS_IS_R2;
					break;
				}
			}

			if (nIsFullName != FALSE)
			{
				int nIsRet = 0;

				switch (osVerInfo.dwMinorVersion)
				{
				case 0:
					nIsRet = IsWin7OrGreater();
					break;
				case 1:
					nIsRet = IsWin8OrGreater();
					break;
				case 2:
					nIsRet = IsWin8Point1OrGreater();
					break;
				case 3:
					nIsRet = IsWindowsVersionOrGreater(6, 4, 0);
					break;
				case 4:
					nIsRet = IsWin10OrGreater();
					break;
				}

				if (nIsRet != FALSE)
				{
					pEdition = _T("互換モード");
					dwOSFrag |= _OS_IS_GOKANMODE;
				}
				else
				{
					DWORD dwType = 0;

#if _MSC_VER < 1920 || defined(_NODLL)
					HMODULE hDll;

					hDll = LOADSYSLIB(_T("KERNEL32.DLL"));
					if (hDll != NULL)
					{
						BOOL (WINAPI * GetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
						GetProductInfo = (BOOL(WINAPI*)(DWORD, DWORD, DWORD, DWORD, PDWORD))(GetProcAddress(hDll, "GetProductInfo"));

						if (GetProductInfo != NULL)
						{
#endif // _MSC_VER < 1920 || defined(_NODLL)

							nRet = GetProductInfo(osVerInfo.dwMajorVersion, osVerInfo.dwMinorVersion, 0, 0, &dwType);

#if _MSC_VER < 1920 || defined(_NODLL)
						}
						FreeLibrary(hDll);
					}
#endif // _MSC_VER < 1920 || defined(_NODLL)

					if (nRet != FALSE)
					{
						switch (dwType)
						{
						// Windows Vista / 7 / 8
						case PRODUCT_ULTIMATE:
						case PRODUCT_ULTIMATE_N:
							pEdition = _T("Ultimate");
							break;
						case PRODUCT_HOME_BASIC:
						case PRODUCT_HOME_BASIC_N:
							pEdition = _T("Home Basic");
							break;
						case PRODUCT_HOME_PREMIUM:
						case PRODUCT_HOME_PREMIUM_N:
							pEdition = _T("Home Premium");
							break;
						case PRODUCT_ENTERPRISE:
						case PRODUCT_ENTERPRISE_N:
						case PRODUCT_ENTERPRISE_EVALUATION:		// VC++2010
						case PRODUCT_ENTERPRISE_N_EVALUATION:	// VC++2010
							pEdition = _T("Enterprise");
							break;
						case PRODUCT_BUSINESS:
						case PRODUCT_BUSINESS_N:
							pEdition = _T("Business");
							break;
						case PRODUCT_STARTER:
						case PRODUCT_STARTER_N:
							pEdition = _T("Starter");
							break;
						case PRODUCT_PROFESSIONAL:
						case PRODUCT_PROFESSIONAL_N:
							switch (osVerInfo.dwMinorVersion)
							{
							case 0:
							case 1:
								pEdition = _T("Professional");
								break;
							default:
								pEdition = _T("Pro");
							}
							break;
							// Windows Server 2008 / R2
						case PRODUCT_STANDARD_SERVER:
						case PRODUCT_STANDARD_SERVER_CORE:
						case PRODUCT_STANDARD_SERVER_V:
						case PRODUCT_STANDARD_SERVER_CORE_V:
						case PRODUCT_STANDARD_EVALUATION_SERVER:	// VC++2010
							pEdition = _T("Standard Edition");
							break;
						case PRODUCT_DATACENTER_SERVER:
						case PRODUCT_DATACENTER_SERVER_CORE:
						case PRODUCT_DATACENTER_SERVER_V:
						case PRODUCT_DATACENTER_SERVER_CORE_V:
						case PRODUCT_DATACENTER_EVALUATION_SERVER:	// VC++2010
							pEdition = _T("Datacenter Edition");
							break;
						case PRODUCT_ENTERPRISE_SERVER:
						case PRODUCT_ENTERPRISE_SERVER_CORE:
						case PRODUCT_ENTERPRISE_SERVER_V:
						case PRODUCT_ENTERPRISE_SERVER_CORE_V:
						case PRODUCT_ENTERPRISE_SERVER_IA64:
							pEdition = _T("Enterprise Edition");
							break;
						case PRODUCT_HOME_SERVER:
						case PRODUCT_HOME_PREMIUM_SERVER:			// Premium Edition?
							switch (osVerInfo.dwMinorVersion)
							{
							case 0:
							case 1:
								pOS = _T("Windows Home Server 2008");
								break;
							case 2:
								pOS = _T("Windows Home Server 2012");
								break;
							}

							if (dwType == PRODUCT_HOME_PREMIUM_SERVER) {
								pEdition = _T("Premium Edition");
							}
							break;
						case PRODUCT_PROFESSIONAL_WMC:				// VC++2010
							pEdition = _T("Pro with Media Center");
							break;
#ifdef _DEBUG
						default:
							pEdition = _T("Unknown Edition");
							_stprintf_s(pEditionBuf, _OS_FULL_SIZE, _T("Unknown Edition [0x%2X (%d)]"), dwType, dwType);
#endif
						}
						// without Evaluation
						switch (dwType)
						{
						case PRODUCT_ENTERPRISE_EVALUATION:
						case PRODUCT_ENTERPRISE_N_EVALUATION:
						case PRODUCT_STANDARD_EVALUATION_SERVER:
						case PRODUCT_DATACENTER_EVALUATION_SERVER:
							pSub = (TCHAR*)_T("評価版");
							break;
						}
					}
				}
			}
			break;
		case 10:
			// 10
			if (osVerInfo.wProductType == VER_NT_WORKSTATION)
			{
				HKEY hKey = NULL;
				DWORD dwType = REG_SZ;
				DWORD dwByte = _OS_FULL_SIZE;

				if (osVerInfo.dwBuildNumber >= 22000) {
					pOS = _T("Windows 11");
				}
				else {
					pOS = _T("Windows 10");
				}

				nRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE, &hKey);
				if (nRet == ERROR_SUCCESS)
				{
					TCHAR szRegBuf[_OS_FULL_SIZE] = _T("");
					TCHAR* pSubPtr = pSub;
					const size_t nVersionSize = sizeof(_T(" バージョン ")) - sizeof(TCHAR);

					nRet = RegQueryValueEx(hKey, _T("DisplayVersion"), 0, &dwType, (LPBYTE)szRegBuf, &dwByte);
					if (nRet == ERROR_SUCCESS)
					{
						memcpy(pSubPtr, _T(" バージョン "), nVersionSize);
						pSubPtr += nVersionSize / sizeof(TCHAR);
						qtcscpy(pSubPtr, szRegBuf);
					}
					else
					{
						nRet = RegQueryValueEx(hKey, _T("ReleaseId"), 0, &dwType, (LPBYTE)szRegBuf, &dwByte);
						if (nRet == ERROR_SUCCESS)
						{
							memcpy(pSubPtr, _T(" バージョン "), nVersionSize);
							pSubPtr += nVersionSize / sizeof(TCHAR);
							qtcscpy(pSubPtr, szRegBuf);
						}
					}
					RegCloseKey(hKey);
				}
			}
			// Server 2015
			else
			{
				switch (osVerInfo.dwMinorVersion)
				{
				case 0:
					pOS = _T("Windows Server 2015");
					break;
				case 1:
					pOS = _T("Windows Server 2015");
					dwOSFrag |= _OS_IS_R2;
					break;
				default:
					// 不明もしくは新しいWindowsはレジストリから読み込む
					return GetOSRegRead(pInString);
				}
			}

			if (nIsFullName != FALSE)
			{
				int nIsRet = 0;

				switch (osVerInfo.dwMinorVersion)
				{
				case 0:
					nIsRet = IsWindowsVersionOrGreater(10, 1, 0);
					break;
				case 1:
					nIsRet = IsWindowsVersionOrGreater(10, 2, 0);
					break;
				}

				if (nIsRet != FALSE)
				{
					pEdition = _T("互換モード");
					dwOSFrag |= _OS_IS_GOKANMODE;
				}
				else
				{
					DWORD dwType = 0;

#if _MSC_VER < 1920 || defined(_NODLL)
					HMODULE hDll;

					hDll = LOADSYSLIB(_T("KERNEL32.DLL"));
					if (hDll != NULL)
					{
						BOOL(WINAPI * GetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
						GetProductInfo = (BOOL(WINAPI*)(DWORD, DWORD, DWORD, DWORD, PDWORD))(GetProcAddress(hDll, "GetProductInfo"));

						if (GetProductInfo != NULL)
						{
#endif // _MSC_VER < 1920 || defined(_NODLL)

							nRet = GetProductInfo(osVerInfo.dwMajorVersion, osVerInfo.dwMinorVersion, 0, 0, &dwType);

#if _MSC_VER < 1920 || defined(_NODLL)
						}
						FreeLibrary(hDll);
					}
#endif // _MSC_VER < 1920 || defined(_NODLL)

					if (nRet != FALSE)
					{
						switch (dwType)
						{
						// Windows 10
						case PRODUCT_CORE_ARM:	// VC++2010
						case PRODUCT_CORE_N:	// VC++2010
						case PRODUCT_CORE:	// VC++2010
							pEdition = _T("Home");
							break;
						case PRODUCT_ENTERPRISE:
						case PRODUCT_ENTERPRISE_N:
						case PRODUCT_ENTERPRISE_EVALUATION:	// VC++2010
						case PRODUCT_ENTERPRISE_N_EVALUATION:	// VC++2010
							pEdition = _T("Enterprise");
							break;
						case PRODUCT_BUSINESS:
						case PRODUCT_BUSINESS_N:
							pEdition = _T("Business");
							break;
						case PRODUCT_STARTER:
						case PRODUCT_STARTER_N:
							pEdition = _T("Starter");
							break;
						case PRODUCT_PROFESSIONAL:
						case PRODUCT_PROFESSIONAL_N:
							pEdition = _T("Pro");
							break;
							// Windows Server 2015
						case PRODUCT_STANDARD_SERVER:
						case PRODUCT_STANDARD_SERVER_CORE:
						case PRODUCT_STANDARD_SERVER_V:
						case PRODUCT_STANDARD_SERVER_CORE_V:
						case PRODUCT_STANDARD_EVALUATION_SERVER:	// VC++2010
							pEdition = _T("Standard Edition");
							break;
						case PRODUCT_DATACENTER_SERVER:
						case PRODUCT_DATACENTER_SERVER_CORE:
						case PRODUCT_DATACENTER_SERVER_V:
						case PRODUCT_DATACENTER_SERVER_CORE_V:
						case PRODUCT_DATACENTER_EVALUATION_SERVER:	// VC++2010
							pEdition = _T("Datacenter Edition");
							break;
						case PRODUCT_ENTERPRISE_SERVER:
						case PRODUCT_ENTERPRISE_SERVER_CORE:
						case PRODUCT_ENTERPRISE_SERVER_V:
						case PRODUCT_ENTERPRISE_SERVER_CORE_V:
						case PRODUCT_ENTERPRISE_SERVER_IA64:
							pEdition = _T("Enterprise Edition");
							break;
#ifdef _DEBUG
						default:
							pEdition = _T("Unknown Edition");
							_stprintf_s(pEditionBuf, _OS_FULL_SIZE, _T("Unknown Edition [0x%2X (%d)]"), dwType, dwType);
#endif
						}
						// without Evaluation
						switch (dwType)
						{
						case PRODUCT_ENTERPRISE_EVALUATION:
						case PRODUCT_ENTERPRISE_N_EVALUATION:
						case PRODUCT_STANDARD_EVALUATION_SERVER:
						case PRODUCT_DATACENTER_EVALUATION_SERVER:
							pSub = (TCHAR*)_T("評価版");
							break;
						}
					}
				}
			}
			break;
		default:
			// 不明もしくは新しいWindowsはレジストリから読み込む
			return GetOSRegRead(pInString);
		}
		break;
	}

	if (*pOS != '\0') {
		ptr = qtcscpy(ptr, pOS);
	}
	else
	{
		ptr = qtcscpy(ptr, _T("Windows NT"));

		if (osVerInfo.dwMajorVersion >= 3)
		{
			TCHAR szVal[32];

			*ptr++ = ' ';
			ITOT_FUNC(osVerInfo.dwMajorVersion, szVal, SIZEOF_NUM(szVal), 10);
			ptr = qtcscpy(ptr, szVal);
			*ptr++ = '.';
			ITOT_FUNC(osVerInfo.dwMinorVersion, szVal, SIZEOF_NUM(szVal), 10);
			ptr = qtcscpy(ptr, szVal);
		}
	}

	if (nIsFullName != FALSE)
	{
		if (dwOSFrag & _OS_IS_R2)
		{
			*ptr++ = ' ';
			ptr = qtcscpy(ptr, _T("R2"));
		}

		if (*pEdition != '\0')
		{
			*ptr++ = ' ';

			if (*pEditionBuf == '\0') {
				ptr = qtcscpy(ptr, pEdition);
			}
			else {
				ptr = qtcscpy(ptr, pEditionBuf);
			}
		}

		if (*pSub != '\0')
		{
			*ptr++ = ' ';
			ptr = qtcscpy(ptr, pSub);
		}

		if ((~dwOSFrag & _OS_IS_NT40EX | _OS_IS_GOKANMODE) && osVerInfo.wServicePackMajor > 0)
		{
			TCHAR szVal[32];

			*ptr++ = ' ';
			*ptr++ = 'S';
			*ptr++ = 'P';
			ITOT_FUNC((int)osVerInfo.wServicePackMajor, szVal, SIZEOF_NUM(szVal), 10);
			ptr = qtcscpy(ptr, szVal);
		}
#if _MSC_VER < 1500
		// VC++2008 から Windows NT 4.0 のコード省略
		else if (dwOSFrag & _OS_IS_NT40EX)
		{
			*ptr++ = ' ';
			*ptr++ = 'S';
			*ptr++ = 'P';
			*ptr++ = '6';
			*ptr = '\0';
		}
#endif
	}

	nOSFullRet = (UINT)_tcslen(szOSFullBuf) + 1;

GETOSNAME_COPY:

	if (pInString != NULL)
	{
		if (nSize > _OS_FULL_SIZE) {
			nSize = _OS_FULL_SIZE;
		}
		qtcsncpy(pInString, szOSFullBuf, nSize - 1);
		*(pInString + nSize) = '\0';
	}
	return nOSFullRet;
}
