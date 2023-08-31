#include "charlib_sysdll.h"
#include "c20.h"
#include <Shlwapi.h>

char szByteChar[16] = { 0 };
WCHAR szByteWChar[16] = { 0 };

#ifndef _UNICODE

HMODULE hShlwapi = NULL;
size_t StrFormatKBSizeA_Arg = 0;
LPSTR WINAPI StrFormatKBSizeA_Func0(__int64 qdw, LPSTR szBuf, UINT uiBufSize);
LPSTR WINAPI StrFormatKBSizeA_Func1(__int64 qdw, LPSTR szBuf, UINT uiBufSize);
LPSTR WINAPI StrFormatKBSizeA_Func2(__int64 qdw, LPSTR szBuf, UINT uiBufSize);


LPSTR(WINAPI * StrFormatKBSizeA_Func[])(__int64, LPSTR, UINT) = {
	StrFormatKBSizeA_Func0,
	NULL,
	StrFormatKBSizeA_Func2
};

LPSTR WINAPI StrFormatKBSizeA_Func0(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	const char* szFunc = "StrFormatKBSizeA";

	StrFormatKBSizeA_Arg = 2;
	hShlwapi = LOADSYSLIBA("SHLWAPI.DLL");
	if (hShlwapi == NULL) {
		goto DLL_FAIL;
	}

	StrFormatKBSizeA_Func[1] = (LPSTR(WINAPI*)(__int64, LPSTR, UINT))(GetProcAddress(hShlwapi, szFunc));
	if (StrFormatKBSizeA_Func[1] == NULL) {
		goto DLL_FAIL;
	}

	StrFormatKBSizeA_Arg = 1;
	return StrFormatKBSizeA_Func[1](qdw, szBuf, uiBufSize);

DLL_FAIL:
	StrFormatKBSizeA_Func[1] = StrFormatKBSizeA_Func1;
	StrFormatKBSizeA_Arg = 1;
	return StrFormatKBSizeA_Func[1](qdw, szBuf, uiBufSize);
}

LPSTR WINAPI StrFormatKBSizeA_Func1(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	char szNumBuf[32];
	char szKBBuf[36];
	int i = 0;
	int y = 0;
	int nBufSize;

	qdw = INT_CEIL(qdw, 1024);
#ifndef _NODLL
	_i64toa_s(qdw, szNumBuf, sizeof(szNumBuf), 10);
#else
	_i64toa(qdw, szNumBuf, 10);
#endif
	nBufSize = (int)strlen(szNumBuf);

	while (i < nBufSize)
	{
		szKBBuf[y++] = szNumBuf[i++];
		if ((nBufSize - i) % 3 == 0 && i < nBufSize - 1) {
			szKBBuf[y++] = ',';
		}
	}
	szKBBuf[y++] = ' ';
	szKBBuf[y++] = 'K';
	szKBBuf[y++] = 'B';
	szKBBuf[y] = '\0';
	STRCPY_FUNC(szBuf, uiBufSize, szKBBuf);
	return szBuf;
}

LPSTR WINAPI StrFormatKBSizeA_Func2(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	while (StrFormatKBSizeA_Arg == 2)
	{
		Sleep(1000);
	}
	return StrFormatKBSizeA_Func[StrFormatKBSizeA_Arg](qdw, szBuf, uiBufSize);
}

LPSTR StrFormatKBSize_Func(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	return StrFormatKBSizeA_Func[StrFormatKBSizeA_Arg](qdw, szBuf, uiBufSize);
}


size_t StrFormatByteSizeA_Arg = 0;
LPSTR WINAPI StrFormatByteSizeA_Func0(__int64 qdw, LPSTR szBuf, UINT uiBufSize);
LPSTR WINAPI StrFormatByteSizeA_Func1(__int64 qdw, LPSTR szBuf, UINT uiBufSize);
LPSTR WINAPI StrFormatByteSizeA_Func1v2(__int64 qdw, LPSTR szBuf, UINT uiBufSize);
LPSTR WINAPI StrFormatByteSizeA_Func2(__int64 qdw, LPSTR szBuf, UINT uiBufSize);


LPSTR(WINAPI * StrFormatByteSizeA_Func[])(__int64, LPSTR, UINT) = {
	StrFormatByteSizeA_Func0,
	NULL,
	StrFormatByteSizeA_Func2
};

LPSTR WINAPI StrFormatByteSizeA_Func0(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	const char* szFunc = "StrFormatByteSize64A";

	StrFormatByteSizeA_Arg = 2;
	hShlwapi = LOADSYSLIBA("SHLWAPI.DLL");
	if (hShlwapi == NULL) {
		goto DLL_FAIL;
	}

	StrFormatByteSizeA_Func[1] = (LPSTR(WINAPI*)(__int64, LPSTR, UINT))(GetProcAddress(hShlwapi, szFunc));
	if (StrFormatByteSizeA_Func[1] == NULL) {
		goto DLL_FAIL;
	}

	StrFormatByteSizeA_Arg = 1;
	return StrFormatByteSizeA_Func[1](qdw, szBuf, uiBufSize);

DLL_FAIL:
	if (1)
	{
		char szlowdw[12];

		StrFormatByteSizeA(0x40000000, szlowdw, sizeof(szlowdw));
		// return -> 1.00 GB
		if (szlowdw[4] == ' ') {
			StrFormatByteSizeA_Func[1] = StrFormatByteSizeA_Func1;
		}
		// return -> 1.00GB (Windows 98 + IE4.0)
		else {
			StrFormatByteSizeA_Func[1] = StrFormatByteSizeA_Func1v2;
		}
	}
	StrFormatByteSizeA_Arg = 1;
	return StrFormatByteSizeA_Func[1](qdw, szBuf, uiBufSize);
}

LPSTR WINAPI StrFormatByteSizeA_Func1(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	if ((unsigned __int64)qdw <= UINT_MAX) {
		return StrFormatByteSizeA((DWORD)qdw, szBuf, uiBufSize);
	}
	else
	{
		char szhidw[40];
		char szlowdw[12];
		const unsigned int nlowdw = MAKE_CHAR_TO_INT_4CHAR(' ', 'G', 'B', '\0');
		char* hiptr;
		char* lowptr;
		size_t nhi;

#ifndef _NODLL
		_i64toa_s(qdw / 0x40000000, szhidw, sizeof(szhidw), 10);
#else
		_i64toa(qdw / 0x40000000, szhidw, 10);
#endif
		nhi = strlen(szhidw);
		hiptr = szhidw + nhi;
		StrFormatByteSizeA((((DWORD)qdw) | 0xC0000000) & 0x7FFFFFFF, szlowdw, sizeof(szlowdw));
		// return "x.xx GB"
		lowptr = strchr(szlowdw, '.');
		if (lowptr != NULL)
		{
			// lowptr = ".00 GB"
			*hiptr++ = *lowptr++;
			*hiptr++ = *lowptr++;

			if (nhi < 2)
			{
				// lowptr = "0 GB"
				*hiptr++ = *lowptr++;
			}
			else
			{
				// lowptr = "0 GB"
				lowptr++;
			}
		}
		memcpy(hiptr, &nlowdw, 4);
		STRCPY_FUNC(szBuf, uiBufSize, szhidw);
	}
	return szBuf;
}

LPSTR WINAPI StrFormatByteSizeA_Func1v2(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	if ((unsigned __int64)qdw <= UINT_MAX) {
		return StrFormatByteSizeA((DWORD)qdw, szBuf, uiBufSize);
	}
	else
	{
		char szhidw[40];
		char szlowdw[12];
		const unsigned int nlowdw = MAKE_CHAR_TO_INT_4CHAR('G', 'B', '\0', '\0');
		char* hiptr;
		char* lowptr;
		size_t nhi;

#ifndef _NODLL
		_i64toa_s(qdw / 0x40000000, szhidw, sizeof(szhidw), 10);
#else
		_i64toa(qdw / 0x40000000, szhidw, 10);
#endif
		nhi = strlen(szhidw);
		hiptr = szhidw + nhi;
		StrFormatByteSizeA((((DWORD)qdw) | 0xC0000000) & 0x7FFFFFFF, szlowdw, sizeof(szlowdw));
		// return "x.xxGB"
		lowptr = strchr(szlowdw, '.');
		if (lowptr != NULL)
		{
			// lowptr = ".00GB"
			*hiptr++ = *lowptr++;
			*hiptr++ = *lowptr++;

			if (nhi < 2)
			{
				// lowptr = "0GB"
				*hiptr++ = *lowptr++;
			}
			else
			{
				// lowptr = "0GB"
				lowptr++;
			}
		}
		memcpy(hiptr, &nlowdw, 4);
		STRCPY_FUNC(szBuf, uiBufSize, szhidw);
	}
	return szBuf;
}

LPSTR WINAPI StrFormatByteSizeA_Func2(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	while (StrFormatByteSizeA_Arg == 2)
	{
		Sleep(1000);
	}
	return StrFormatByteSizeA_Func[StrFormatByteSizeA_Arg](qdw, szBuf, uiBufSize);
}

LPSTR StrFormatByteSize_Func(__int64 qdw, LPSTR szBuf, UINT uiBufSize)
{
	return StrFormatByteSizeA_Func[StrFormatByteSizeA_Arg](qdw, szBuf, uiBufSize);
}

#define STRFORMATKBSIZEA_MACRO StrFormatKBSizeA_Func[StrFormatKBSizeA_Arg]
#define STRFORMATBYTESIZEA_MACRO StrFormatByteSizeA_Func[StrFormatByteSizeA_Arg]
#else
#define STRFORMATKBSIZEA_MACRO StrFormatKBSizeA
#define STRFORMATBYTESIZEA_MACRO StrFormatByteSize64A
#endif

char* FileSizeText2ToChar(const __int64 cdwlFileSize, char* pOutChar)
{
	char szDot[] = ".";
	char szConma[] = ",";
	char szNumber[33];
	const NUMBERFMTA nfmt = { 0, 0, 3, szDot, szConma, 1 };
	char* pTemp;
	__int64 llFileSize;
	int nRet;

	IF_UNLIKELY(pOutChar == NULL)
	{
		pOutChar = (char*)malloc((size_t)100 + 1);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}

	llFileSize = cdwlFileSize;
	pTemp = pOutChar;
	IF_UNLIKELY(cdwlFileSize < 0)
	{
		*pTemp++ = '-';
		llFileSize = -cdwlFileSize;
	}
	STRFORMATBYTESIZEA_MACRO(llFileSize, pTemp, sizeof(szNumber));

	pTemp += strlen(pTemp);
	*pTemp++ = ' ';
	*pTemp++ = '(';

#ifndef _NODLL
	_i64toa_s(cdwlFileSize, szNumber, sizeof(szNumber), 10);
#else
	_i64toa(cdwlFileSize, szNumber, 10);
#endif

	nRet = GetNumberFormatA(LOCALE_SYSTEM_DEFAULT, 0, szNumber, &nfmt, pTemp, sizeof(szNumber));
	pTemp += nRet;
	pTemp--;

	IF_UNLIKELY(*szByteChar == '\0')
	{
		char szBuf[sizeof(szByteChar)];
		STRFORMATBYTESIZEA_MACRO(0, szBuf, sizeof(szByteChar));
		qstrcpy(szByteChar, szBuf + 1);
	}

	pTemp = qstrcpy(pTemp, szByteChar);
	*pTemp++ = ')';
	*pTemp = '\0';
	return pOutChar;
}

WCHAR* FileSizeText2ToWChar(const __int64 cdwlFileSize, WCHAR* pOutWChar)
{
	WCHAR szDot[] = L".";
	WCHAR szConma[] = L",";
	WCHAR szNumber[33];
	const NUMBERFMTW nfmt = { 0, 0, 3, szDot, szConma, 1 };
	WCHAR* pTemp;
	__int64 llFileSize;
	int nRet;

	IF_UNLIKELY(pOutWChar == NULL)
	{
		pOutWChar = (WCHAR*)malloc(((size_t)100 + 1) * sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}

	llFileSize = cdwlFileSize;
	pTemp = pOutWChar;
	IF_UNLIKELY(llFileSize < 0)
	{
		*pTemp++ = '-';
		llFileSize = -cdwlFileSize;
	}
	StrFormatByteSizeW(llFileSize, pTemp, SIZEOF_NUM(szNumber));

	pTemp += wcslen(pTemp);
	*pTemp++ = ' ';
	*pTemp++ = '(';

#ifndef _NODLL
	_i64tow_s(cdwlFileSize, szNumber, SIZEOF_NUM(szNumber), 10);
#else
	_i64tow(cdwlFileSize, szNumber, 10);
#endif

	nRet = GetNumberFormatW(LOCALE_SYSTEM_DEFAULT, 0, szNumber, &nfmt, pTemp, 33);
	pTemp += nRet;
	pTemp--;

	IF_UNLIKELY(*szByteWChar == '\0')
	{
		WCHAR szBuf[SIZEOF_NUM(szByteWChar)];
		StrFormatByteSizeW(0, szBuf, SIZEOF_NUM(szBuf));
		qwcscpy(szByteWChar, szBuf + 1);
	}

	pTemp = qwcscpy(pTemp, szByteWChar);
	*pTemp++ = ')';
	*pTemp = '\0';
	return pOutWChar;
}

char* FileSizePerSecTextToChar(const __int64 cdwlFileSize, char* pOutChar)
{
	char* pTemp;
	char szNumber[33];
	__int64 llFileSize;

	IF_UNLIKELY(pOutChar == NULL)
	{
		pOutChar = (char*)malloc((size_t)100 + 1);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}

	llFileSize = cdwlFileSize;
	pTemp = pOutChar;
	IF_UNLIKELY(cdwlFileSize < 0)
	{
		*pTemp++ = '-';
		llFileSize = -cdwlFileSize;
	}
	STRFORMATBYTESIZEA_MACRO(llFileSize, szNumber, sizeof(szNumber));

	pTemp = qstrcpy(pTemp, szNumber);
	*pTemp++ = '/';
	*pTemp++ = -107;//(char)0x95; MBS_HIWORD('•b');
	*pTemp++ = 98;//0x62; MBS_LOWORD('•b');
	*pTemp++ = '\0';
	return pOutChar;
}

WCHAR* FileSizePerSecTextToWChar(const __int64 cdwlFileSize, WCHAR* pOutWChar)
{
	WCHAR* pTemp;
	WCHAR szNumber[33];
	__int64 llFileSize;

	IF_UNLIKELY(pOutWChar == NULL)
	{
		pOutWChar = (WCHAR*)malloc(((size_t)100 + 1) * sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}

	llFileSize = cdwlFileSize;
	pTemp = pOutWChar;
	IF_UNLIKELY(llFileSize < 0)
	{
		*pTemp++ = '-';
		llFileSize = -cdwlFileSize;
	}
	StrFormatByteSizeW(llFileSize, szNumber, SIZEOF_NUM(szNumber));

	pTemp = qwcscpy(pTemp, szNumber);
	*pTemp++ = '/';
	*pTemp++ = 0x79d2;//L'•b';
	*pTemp++ = '\0';
	return pOutWChar;
}

char* FileSizePerSecText2ToChar(const __int64 cdwlFileSize, char* pOutChar)
{
	char szDot[] = ".";
	char szConma[] = ",";
	char szNumber[33];
	const NUMBERFMTA nfmt = { 0, 0, 3, szDot, szConma, 1 };
	char* pTemp;
	__int64 llFileSize;
	int nRet;

	IF_UNLIKELY(pOutChar == NULL)
	{
		pOutChar = (char*)malloc((101));
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}

	llFileSize = cdwlFileSize;
	pTemp = pOutChar;
	IF_UNLIKELY(cdwlFileSize < 0)
	{
		*pTemp++ = '-';
		llFileSize = -cdwlFileSize;
	}
	STRFORMATBYTESIZEA_MACRO(llFileSize, pTemp, sizeof(szNumber));

	pTemp += strlen(pTemp);
	*pTemp++ = '/';
	*pTemp++ = -107;//(char)0x95; MBS_HIWORD('•b');
	*pTemp++ = 98;//0x62; MBS_LOWORD('•b');
	*pTemp++ = ' ';
	*pTemp++ = '(';

#ifndef _NODLL
	_i64toa_s(cdwlFileSize, szNumber, sizeof(szNumber), 10);
#else
	_i64toa(cdwlFileSize, szNumber, 10);
#endif

	nRet = GetNumberFormatA(LOCALE_SYSTEM_DEFAULT, 0, szNumber, &nfmt, pTemp, sizeof(szNumber));
	pTemp += nRet;
	pTemp--;

	IF_UNLIKELY(*szByteChar == '\0')
	{
		char szBuf[sizeof(szByteChar)];
		STRFORMATBYTESIZEA_MACRO(0, szBuf, sizeof(szByteChar));
		qstrcpy(szByteChar, szBuf + 1);
	}

	pTemp = qstrcpy(pTemp, szByteChar);
	*pTemp++ = '/';
	*pTemp++ = -107;//(char)0x95; MBS_HIWORD('•b');
	*pTemp++ = 98;//0x62; MBS_LOWORD('•b');
	*pTemp++ = ')';
	*pTemp++ = '\0';
	return pOutChar;
}

WCHAR* FileSizePerSecText2ToWChar(const __int64 cdwlFileSize, WCHAR* pOutWChar)
{
	WCHAR szDot[] = L".";
	WCHAR szConma[] = L",";
	WCHAR szNumber[33];
	const NUMBERFMTW nfmt = { 0, 0, 3, szDot, szConma, 1 };
	WCHAR* pTemp;
	__int64 llFileSize;
	int nRet;

	IF_UNLIKELY(pOutWChar == NULL)
	{
		pOutWChar = (WCHAR*)malloc((101) * sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}

	llFileSize = cdwlFileSize;
	pTemp = pOutWChar;
	IF_UNLIKELY(llFileSize < 0)
	{
		*pTemp++ = '-';
		llFileSize = -cdwlFileSize;
	}
	StrFormatByteSizeW(llFileSize, pTemp, SIZEOF_NUM(szNumber));

	pTemp += wcslen(pTemp);
	*pTemp++ = '/';
	*pTemp++ = 0x79d2;//L'•b';
	*pTemp++ = ' ';
	*pTemp++ = '(';

#ifndef _NODLL
	_i64tow_s(cdwlFileSize, szNumber, SIZEOF_NUM(szNumber), 10);
#else
	_i64tow(cdwlFileSize, szNumber, 10);
#endif

	nRet = GetNumberFormatW(LOCALE_SYSTEM_DEFAULT, 0, szNumber, &nfmt, pTemp, SIZEOF_NUM(szNumber));
	pTemp += nRet;
	pTemp--;

	IF_UNLIKELY(*szByteWChar == '\0')
	{
		WCHAR szBuf[SIZEOF_NUM(szByteWChar)];
		StrFormatByteSizeW(0, szBuf, SIZEOF_NUM(szBuf));
		qwcscpy(szByteWChar, szBuf + 1);
	}

	pTemp = qwcscpy(pTemp, szByteWChar);
	*pTemp++ = '/';
	*pTemp++ = 0x79d2;//L'•b';
	*pTemp++ = ')';
	*pTemp++ = '\0';
	return pOutWChar;
}

BOOL nSetSystemDll_Ret = -1;

#if _MSC_VER <= 1920
BOOL LoadSystemLibraryToChar_Arg = 0;
HMODULE LoadSystemLibraryToChar_Func1(const char* cpInDllName, size_t nDllNameLen);
HMODULE LoadSystemLibraryToChar_Func2(const char* cpInDllName, size_t nDllNameLen);
HMODULE LoadSystemLibraryToChar_Func3(const char* cpInDllName, size_t nDllNameLen);

HMODULE(*LoadSystemLibraryToChar_Func[])(const char*, size_t) = {
	LoadSystemLibraryToChar_Func1,
	LoadSystemLibraryToChar_Func2,
	LoadSystemLibraryToChar_Func3
};

HMODULE LoadSystemLibraryToChar_Func1(const char* cpInDllName, size_t nDllNameLen)
{
	SetSystemDllToChar();
	return LoadSystemLibraryToChar_Func[LoadSystemLibraryToChar_Arg](cpInDllName, nDllNameLen);
}

HMODULE LoadSystemLibraryToChar_Func2(const char* cpInDllName, size_t nDllNameLen)
{
	static char szSysPath[_MAX_PATH];
	static char* ptr = szSysPath;
	HMODULE hRet;

	if (szSysPath == ptr)
	{
		const UINT nSysDirLen = GetSystemDirectoryA(szSysPath, SIZEOF_NUM(szSysPath));
		ptr += nSysDirLen;
		*ptr++ = '\\';
	}

	if ((ptr - szSysPath + nDllNameLen) > SIZEOF_NUM(szSysPath)) {
		return NULL;
	}

	memcpy(ptr, cpInDllName, nDllNameLen);
	hRet = LoadLibraryA(szSysPath);
	memset(ptr, 0, nDllNameLen);
	return hRet;
}

HMODULE LoadSystemLibraryToChar_Func3(const char* cpInDllName, size_t nDllNameLen)
{
	UNREFERENCED_PARAMETER(nDllNameLen);
	return LoadLibraryA(cpInDllName);
}
#endif

HMODULE LoadSystemLibraryToChar(const char* cpInDllName)
{
#if _MSC_VER > 1920
	return LoadLibraryExA(cpInDllName, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
#else
	const size_t nDllNameLen = strlen(cpInDllName) + 1;
	return LoadSystemLibraryToChar_Func[LoadSystemLibraryToChar_Arg](cpInDllName, nDllNameLen);
#endif
}

HMODULE LoadSystemLibraryToChar(const char* cpInDllName, size_t nDllNameLen)
{
#if _MSC_VER > 1920
	UNREFERENCED_PARAMETER(nDllNameLen);
	return LoadLibraryExA(cpInDllName, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
#else
	return LoadSystemLibraryToChar_Func[LoadSystemLibraryToChar_Arg](cpInDllName, nDllNameLen);
#endif
}

#if _MSC_VER <= 1920
BOOL LoadSystemLibraryToWChar_Arg = 0;
HMODULE LoadSystemLibraryToWChar_Func1(const WCHAR* cpInDllName, size_t nDllNameLen);
HMODULE LoadSystemLibraryToWChar_Func2(const WCHAR* cpInDllName, size_t nDllNameLen);
HMODULE LoadSystemLibraryToWChar_Func3(const WCHAR* cpInDllName, size_t nDllNameLen);

HMODULE(*LoadSystemLibraryToWChar_Func[])(const WCHAR*, size_t) = {
	LoadSystemLibraryToWChar_Func1,
	LoadSystemLibraryToWChar_Func2,
	LoadSystemLibraryToWChar_Func3
};

HMODULE LoadSystemLibraryToWChar_Func1(const WCHAR* cpInDllName, size_t nDllNameLen)
{
	SetSystemDllToWChar();
	return LoadSystemLibraryToWChar_Func[LoadSystemLibraryToWChar_Arg](cpInDllName, nDllNameLen);
}

HMODULE LoadSystemLibraryToWChar_Func2(const WCHAR* cpInDllName, size_t nDllNameLen)
{
	static WCHAR szSysPath[_MAX_PATH];
	static WCHAR* ptr = szSysPath;
	HMODULE hRet;

	if (szSysPath == ptr)
	{
		const UINT nSysDirLen = GetSystemDirectoryW(szSysPath, SIZEOF_NUM(szSysPath));
		ptr += nSysDirLen;
		*ptr++ = '\\';
	}

	if ((ptr - szSysPath + nDllNameLen) > SIZEOF_NUM(szSysPath)) {
		return NULL;
	}

	memcpy(ptr, cpInDllName, nDllNameLen);
	hRet = LoadLibraryW(szSysPath);
	memset(ptr, 0, nDllNameLen);
	return hRet;
}

HMODULE LoadSystemLibraryToWChar_Func3(const WCHAR* cpInDllName, size_t nDllNameLen)
{
	UNREFERENCED_PARAMETER(nDllNameLen);
	return LoadLibraryW(cpInDllName);
}
#endif

HMODULE LoadSystemLibraryToWChar(const WCHAR* cpInDllName)
{
#if _MSC_VER > 1920
	return LoadLibraryExW(cpInDllName, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
#else
	const size_t nDllNameLen = (wcslen(cpInDllName) + 1) * sizeof(WCHAR);
	return LoadSystemLibraryToWChar_Func[LoadSystemLibraryToWChar_Arg](cpInDllName, nDllNameLen);
#endif
}

HMODULE LoadSystemLibraryToWChar(const WCHAR* cpInDllName, size_t nDllNameLen)
{
#if _MSC_VER > 1920
	UNREFERENCED_PARAMETER(nDllNameLen);
	return LoadLibraryExW(cpInDllName, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
#else
	return LoadSystemLibraryToWChar_Func[LoadSystemLibraryToWChar_Arg](cpInDllName, nDllNameLen);
#endif
}

#ifndef LOAD_LIBRARY_SEARCH_SYSTEM32
#define LOAD_LIBRARY_SEARCH_SYSTEM32        0x00000800
#endif

#define LOADSYSLIBFUNC2A(A)	LoadSystemLibraryToChar_Func2(A, sizeof(A))
#define LOADSYSLIBFUNC2W(A)	LoadSystemLibraryToWChar_Func2(A, sizeof(A))

BOOL SetSystemDllToChar()
{
	IF_UNLIKELY(nSetSystemDll_Ret == -1)
	{
#if _MSC_VER <= 1920
		HMODULE hDll;

		nSetSystemDll_Ret = FALSE;
		hDll = LOADSYSLIBFUNC2A("KERNEL32.DLL");
		if (hDll != NULL)
		{
			BOOL(WINAPI * SetDefaultDllDirectories)(DWORD);
			const char* szFunc = "SetDefaultDllDirectories";

			SetDefaultDllDirectories = (BOOL(WINAPI*)(DWORD))(GetProcAddress(hDll, szFunc));
			if (SetDefaultDllDirectories)
			{
#endif

				nSetSystemDll_Ret = SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32);

#if _MSC_VER <= 1920
			}
			else
			{
				BOOL(WINAPI * SetDllDirectoryA)(LPCSTR);
				szFunc = "SetDllDirectoryA";

				SetDllDirectoryA = (BOOL(WINAPI*)(LPCSTR))(GetProcAddress(hDll, szFunc));
				if (SetDllDirectoryA) {
					nSetSystemDll_Ret = (*SetDllDirectoryA)("");
				}
			}
			FreeLibrary(hDll);
		}
		LoadSystemLibraryToChar_Arg = nSetSystemDll_Ret + 1;
		LoadSystemLibraryToWChar_Arg = nSetSystemDll_Ret + 1;
#endif
	}
	return nSetSystemDll_Ret;
}

BOOL SetSystemDllToWChar()
{
	IF_UNLIKELY(nSetSystemDll_Ret == -1)
	{
#if _MSC_VER <= 1920
		HMODULE hDll;

		nSetSystemDll_Ret = FALSE;
		hDll = LOADSYSLIBFUNC2W(L"KERNEL32.DLL");
		if (hDll != NULL)
		{
			BOOL(WINAPI * SetDefaultDllDirectories)(DWORD);
			const char* szFunc = "SetDefaultDllDirectories";

			SetDefaultDllDirectories = (BOOL(WINAPI*)(DWORD))(GetProcAddress(hDll, szFunc));
			if (SetDefaultDllDirectories)
			{
#endif

				nSetSystemDll_Ret = SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32);

#if _MSC_VER <= 1920
			}
			else
			{
				BOOL(WINAPI * SetDllDirectoryW)(LPCWSTR);
				szFunc = "SetDllDirectoryW";

				SetDllDirectoryW = (BOOL(WINAPI*)(LPCWSTR))(GetProcAddress(hDll, szFunc));
				if (SetDllDirectoryW) {
					nSetSystemDll_Ret = (*SetDllDirectoryW)(L"");
				}
			}
			FreeLibrary(hDll);
		}
		LoadSystemLibraryToChar_Arg = nSetSystemDll_Ret + 1;
		LoadSystemLibraryToWChar_Arg = nSetSystemDll_Ret + 1;
#endif
	}
	return nSetSystemDll_Ret;
}

BOOL Charlib_Shutdown()
{
#ifndef _UNICODE
	if (hShlwapi)
	{
		FreeLibrary(hShlwapi);
		hShlwapi = NULL;
	}
#endif
	return TRUE;
}
