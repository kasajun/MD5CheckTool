// main.cpp : DLL アプリケーションのエントリ ポイントを定義します。

#include "stdafx.h"
#include "../shared/c20.h"


#define EXPORT extern "C" __declspec(dllexport)

HINSTANCE hInst;
unsigned char szString[] =
{
	0x20, 0x29, 0x58, 0x8e, 0xee, 0xec, 0x8e, 0xef, 0xca, 0x8e, 0xee, 0xee, 0x8e, 0xef, 0xc2, 0x8e,
	0xee, 0xe9, 0x8e, 0xee, 0xd1, 0x8e, 0xee, 0xc6, 0x24, 0x0e, 0x02, 0x03, 0x3f, 0x08, 0x1e, 0x00
};

const char* cpHashExtA[MAX_HASH_TYPE + 1] = {
	".md5", ".sha1", ".sha256_192", ".sha224", ".sha256",
	".sha384", ".sha512", ".sha512_224", ".sha512_256", ".sha3_224",
	".sha3_256", ".sha3_384", ".sha3_512", ".crc32", ".crc32c",
	".xxh32", ".xxh64", ".xxh3", ".xxh128", ".md2",
	".md4", NULL, NULL
};
const char* cpHashTypeA[MAX_HASH_TYPE + 1] = {
	"md5file", "sha1file", "sha256_192file", "sha224file", "sha256file",
	"sha384file", "sha512file", "sha512_224file", "sha512_256file", "sha3_224file",
	"sha3_256file", "sha3_384file", "sha3_512file", "crc32file", "crc32cfile",
	"xxh32file", "xxh64file", "xxh3file", "xxh128file", "md2file",
	"md4file", NULL, NULL
};
const char* cpHashFileNameA[MAX_HASH_TYPE + 1] = {
	"MD5 ファイル", "SHA-1 ファイル", "SHA-256/192 ファイル", "SHA-224 ファイル", "SHA-256 ファイル",
	"SHA-384 ファイル", "SHA-512 ファイル", "SHA-512/224 ファイル", "SHA-512/256 ファイル", "SHA3-224ファイル",
	"SHA3-256 ファイル", "SHA3-384 ファイル", "SHA3-512 ファイル", "crc32 ファイル", "crc32c ファイル",
	"XXH32 ファイル", "XXH64 ファイル", "XXH3 ファイル", "XXH128 ファイル", "MD2 ファイル",
	"MD4 ファイル", NULL, NULL
};

const WCHAR* cpHashExtW[MAX_HASH_TYPE + 1] = {
	L".md5", L".sha1", L".sha256_192", L".sha224", L".sha256",
	L".sha384", L".sha512", L".sha512_224", L".sha512_256", L".sha3_224",
	L".sha3_256", L".sha3_384", L".sha3_512", L".crc32", L".crc32c",
	L".xxh32", L".xxh64", L".xxh3", L".xxh128", L".md2",
	L".md4", NULL, NULL
};
const WCHAR* cpHashTypeW[MAX_HASH_TYPE + 1] = {
	L"md5file", L"sha1file", L"sha256_192file", L"sha224file", L"sha256file",
	L"sha384file", L"sha512file", L"sha512_224file", L"sha512_256file", L"sha3_224file",
	L"sha3_256file", L"sha3_384file", L"sha3_512file", L"crc32file", L"crc32cfile",
	L"xxh32file", L"xxh64file", L"xxh3file", L"xxh128file", L"md2file",
	L"md4file", NULL, NULL
};
const WCHAR* cpHashFileNameW[MAX_HASH_TYPE + 1] = {
	L"MD5 ファイル", L"SHA-1 ファイル", L"SHA-256/192 ファイル", L"SHA-224 ファイル", L"SHA-256 ファイル",
	L"SHA-384 ファイル", L"SHA-512 ファイル", L"SHA-512/224 ファイル", L"SHA-512/256 ファイル", L"SHA3-224 ファイル",
	L"SHA3-256 ファイル", L"SHA3-384 ファイル", L"SHA3-512 ファイル", L"crc32 ファイル", L"crc32c ファイル",
	L"XXH32 ファイル", L"XXH64 ファイル", L"XXH3 ファイル", L"XXH128 ファイル", L"MD2 ファイル",
	L"MD4 ファイル", NULL, NULL
};

const size_t nHashExtLen[MAX_HASH_TYPE + 1] = {
	sizeof("md5"), sizeof("sha1"), sizeof("sha256_192"), sizeof("sha224"), sizeof("sha256"),
	sizeof("sha384"), sizeof("sha512"), sizeof("sha512_224"), sizeof("sha512_256"), sizeof("sha3_224"),
	sizeof("sha3_256"), sizeof("sha3_384"), sizeof("sha3_512"), sizeof("crc32"), sizeof("crc32c"),
	sizeof("xxh32"), sizeof("xxh64"), sizeof("xxh3"), sizeof("xxh128"), sizeof("md2"),
	sizeof("md4"), sizeof("none"), 0
};


BOOL IconRes_Init()
{
	return TRUE;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInst = hModule;
		IconRes_Init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

EXPORT BOOL WINAPI IconRes_Load(unsigned char* pOutChar)
{
	IF_UNLIKELY(pOutChar == NULL) {
		return FALSE;
	}
	memcpy(pOutChar, szString, sizeof(szString));
	return TRUE;
}

EXPORT BOOL WINAPI IconRes_FileAssocA(const char* cpInAppPath)
{
	char szAppPath[1340] = "";
	char* pAppPathPtr = NULL;
	char szDllPath[1340] = "";
	char* pDllPathPtr = NULL;
	char szNotepad[100] = "notepad.exe \"%1\"";
	char szKeyName[100] = "";
	char* pKeyNamePtr = NULL;
	char* pSubKey = NULL;
	char* pSubKey2 = NULL;
	char szReadBuf[32] = "";
	char* pTmp = NULL;
	HKEY hKey = NULL;
	const HKEY hKeyType[2] = { HKEY_CLASSES_ROOT, HKEY_CURRENT_USER };
	const char* cpKeyName[2] = { "", "Software\\Classes\\" };
	LONG nRet = 0;
	DWORD dwDisposition = 0;
	DWORD dwType = 0;
	DWORD dwRead = 0;
	int i = 0;
	const int nIsWinVista = IsWinVistaOrGreater();

	IF_UNLIKELY(cpInAppPath == NULL) {
		return FALSE;
	}

	nRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Classes\\txtfile\\shell\\open\\command", 0, KEY_QUERY_VALUE, &hKey);
	IF_LIKELY(nRet == ERROR_SUCCESS)
	{
		dwRead = sizeof(szNotepad);
		RegQueryValueExA(hKey, NULL, 0, &dwType, (LPBYTE)szNotepad, &dwRead);
		RegCloseKey(hKey);

		if (_strnicmp(szNotepad, "%SystemRoot%\\system32\\notepad.exe", strlen("%SystemRoot%\\system32\\notepad.exe")) == 0)
		{
			nRet = GetSystemDirectoryA(szNotepad, sizeof(szNotepad));
			pTmp = szNotepad + nRet;
			*pTmp++ = '\\';
			pTmp = qstrcpy(pTmp, "notepad.exe");
			qstrcpy(pTmp, " \"%1\"");
		}
	}

	szAppPath[0] = '\"';
	pAppPathPtr = qstrcpy(szAppPath + 1, cpInAppPath);
	*pAppPathPtr++ = '\"';

	pDllPathPtr = qstrcpy(szDllPath, cpInAppPath);
	pDllPathPtr = qstrcpy(pDllPathPtr - 4, "IconRes.icl");
	*pDllPathPtr++ = ',';
	*pDllPathPtr++ = '-';

	pKeyNamePtr = qstrcpy(szKeyName, cpKeyName[nIsWinVista]);

	for (i = 0; i < MAX_HASH_TYPE; i++)
	{
		IF_UNLIKELY(cpHashExtA[i] == NULL) {
			continue;
		}

		qstrcpy(pKeyNamePtr, cpHashExtA[i]);
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			return FALSE;
		}

		dwRead = sizeof(szReadBuf);
		nRet = RegQueryValueExA(hKey, NULL, 0, &dwType, (LPBYTE)szReadBuf, &dwRead);
		if (nRet == ERROR_SUCCESS && strncmp(cpHashTypeA[i], szReadBuf, dwRead) != 0)
		{
			nRet = RegSetValueExA(hKey, "BackUp", 0, REG_SZ, (LPBYTE)szReadBuf, dwRead);
			IF_UNLIKELY(nRet != ERROR_SUCCESS)
			{
				RegCloseKey(hKey);
				continue;
			}
		}

		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)cpHashTypeA[i], (DWORD)(strlen(cpHashTypeA[i]) + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey = qstrcpy(pKeyNamePtr, cpHashTypeA[i]);
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)cpHashFileNameA[i], (DWORD)(strlen(cpHashFileNameA[i]) + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		nRet = RegSetValueExA(hKey, "AlwaysShowExt", 0, REG_SZ, (LPBYTE)"", (DWORD)(sizeof("")));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		*pSubKey++ = '\\';
		qstrcpy(pSubKey, "DefaultIcon");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		_itoa_s(i + 2, pDllPathPtr, 3, 10);
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)szDllPath, (DWORD)(strlen(szDllPath) + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey = qstrcpy(pSubKey, "shell");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)"open", (DWORD)(sizeof("open")));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		*pSubKey++ = '\\';
		pSubKey2 = qstrcpy(pSubKey, "open");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)"開く(&O)", (DWORD)(sizeof("開く(&O)")));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);
		*pSubKey2++ = '\\';
		qstrcpy(pSubKey2, "command");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		pTmp = qstrcpy(pAppPathPtr, " \"%1\"");
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)(pTmp - szAppPath + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qstrcpy(pSubKey, "edit");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)"編集(&E)", (DWORD)(sizeof("編集(&E)")));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);
		*pSubKey2++ = '\\';
		qstrcpy(pSubKey2, "command");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)szNotepad, (DWORD)(strlen(szNotepad) + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);


		pSubKey2 = qstrcpy(pSubKey, "追記モードで開く");
		*pSubKey2++ = '\\';
		qstrcpy(pSubKey2, "command");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		pTmp = qstrcpy(pAppPathPtr, " -a \"%1\"");
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)(pTmp - szAppPath + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qstrcpy(pKeyNamePtr, "*");
		*pSubKey2++ = '\\';
		pSubKey2 = qstrcpy(pSubKey2, "shell");
		*pSubKey2++ = '\\';
		pSubKey2 = qstrcpy(pSubKey2, "MD5チェックツールで開く");
		*pSubKey2++ = '\\';
		qstrcpy(pSubKey2, "command");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		pTmp = qstrcpy(pAppPathPtr, " \"%1\"");
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)(pTmp - szAppPath + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qstrcpy(pKeyNamePtr, "Directory");
		*pSubKey2++ = '\\';
		pSubKey2 = qstrcpy(pSubKey2, "shell");
		*pSubKey2++ = '\\';
		pSubKey2 = qstrcpy(pSubKey2, "MD5チェックツールで開く");
		*pSubKey2++ = '\\';
		qstrcpy(pSubKey2, "command");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)(pTmp - szAppPath + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qstrcpy(pKeyNamePtr, "Drive");
		*pSubKey2++ = '\\';
		pSubKey2 = qstrcpy(pSubKey2, "shell");
		*pSubKey2++ = '\\';
		pSubKey2 = qstrcpy(pSubKey2, "MD5チェックツールで開く");
		*pSubKey2++ = '\\';
		qstrcpy(pSubKey2, "command");
		nRet = RegCreateKeyExA(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)(pTmp - szAppPath + 1));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);
	}
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
	return TRUE;
}

EXPORT BOOL WINAPI IconRes_FileAssocW(const WCHAR* cpInAppPath)
{
	WCHAR szAppPath[1340] = L"";
	WCHAR* pAppPathPtr = NULL;
	WCHAR szDllPath[1340] = L"";
	WCHAR* pDllPathPtr = NULL;
	WCHAR szNotepad[100] = L"notepad.exe \"%1\"";
	WCHAR szKeyName[100] = L"";
	WCHAR* pKeyNamePtr = NULL;
	WCHAR* pSubKey = NULL;
	WCHAR* pSubKey2 = NULL;
	WCHAR szReadBuf[32] = L"";
	WCHAR* pTmp = NULL;
	HKEY hKey = NULL;
	const HKEY hKeyType[2] = { HKEY_CLASSES_ROOT, HKEY_CURRENT_USER };
	const WCHAR* cpKeyName[2] = { L"", L"Software\\Classes\\" };
	LONG nRet = 0;
	DWORD dwDisposition = 0;
	DWORD dwType = 0;
	DWORD dwRead = 0;
	int i = 0;
	const int nIsWinVista = IsWinVistaOrGreater();

	IF_UNLIKELY(cpInAppPath == NULL) {
		return FALSE;
	}

	nRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Classes\\txtfile\\shell\\open\\command", 0, KEY_QUERY_VALUE, &hKey);
	if (nRet == ERROR_SUCCESS)
	{
		dwRead = sizeof(szNotepad);
		RegQueryValueExW(hKey, NULL, 0, &dwType, (LPBYTE)szNotepad, &dwRead);
		RegCloseKey(hKey);

		if (_wcsnicmp(szNotepad, L"%SystemRoot%\\system32\\notepad.exe", wcslen(L"%SystemRoot%\\system32\\notepad.exe")) == 0)
		{
			nRet = GetSystemDirectoryW(szNotepad, SIZEOF_NUM(szNotepad));
			pTmp = szNotepad + nRet;
			*pTmp++ = '\\';
			pTmp = qwcscpy(pTmp, L"notepad.exe");
			qwcscpy(pTmp, L" \"%1\"");
		}
	}

	szAppPath[0] = '\"';
	pAppPathPtr = qwcscpy(szAppPath + 1, cpInAppPath);
	*pAppPathPtr++ = '\"';

	pDllPathPtr = qwcscpy(szDllPath, cpInAppPath);
	pDllPathPtr = qwcscpy(pDllPathPtr - 4, L"IconRes.icl");
	*pDllPathPtr++ = ',';
	*pDllPathPtr++ = '-';

	pKeyNamePtr = qwcscpy(szKeyName, cpKeyName[nIsWinVista]);

	for (i = 0; i < MAX_HASH_TYPE; i++)
	{
		IF_UNLIKELY(cpHashExtW[i] == NULL) {
			continue;
		}

		qwcscpy(pKeyNamePtr, cpHashExtW[i]);
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}

		dwRead = sizeof(szReadBuf);
		nRet = RegQueryValueExW(hKey, NULL, 0, &dwType, (LPBYTE)szReadBuf, &dwRead);
		if (nRet == ERROR_SUCCESS && wcsncmp(cpHashTypeW[i], szReadBuf, dwRead) != 0)
		{
			nRet = RegSetValueExW(hKey, L"BackUp", 0, REG_SZ, (LPBYTE)szReadBuf, dwRead);
			IF_UNLIKELY(nRet != ERROR_SUCCESS)
			{
				RegCloseKey(hKey);
				continue;
			}
		}

		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)cpHashTypeW[i], (DWORD)((wcslen(cpHashTypeW[i]) + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey = qwcscpy(pKeyNamePtr, cpHashTypeW[i]);
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)cpHashFileNameW[i], (DWORD)((wcslen(cpHashFileNameW[i]) + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		nRet = RegSetValueExW(hKey, L"AlwaysShowExt", 0, REG_SZ, (LPBYTE)L"", (DWORD)(sizeof(L"")));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		*pSubKey++ = '\\';
		qwcscpy(pSubKey, L"DefaultIcon");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		_itow_s(i + 2, pDllPathPtr, 3, 10);
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)szDllPath, (DWORD)((wcslen(szDllPath) + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey = qwcscpy(pSubKey, L"shell");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)L"open", (DWORD)(sizeof(L"open")));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		*pSubKey++ = '\\';
		pSubKey2 = qwcscpy(pSubKey, L"open");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)L"開く(&O)", (DWORD)(sizeof(L"開く(&O)")));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);
		*pSubKey2++ = '\\';
		qwcscpy(pSubKey2, L"command");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		pTmp = qwcscpy(pAppPathPtr, L" \"%1\"");
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)((pTmp - szAppPath + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qwcscpy(pSubKey, L"edit");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)L"編集(&E)", (DWORD)(sizeof(L"編集(&E)")));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);
		*pSubKey2++ = '\\';
		qwcscpy(pSubKey2, L"command");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)szNotepad, (DWORD)((wcslen(szNotepad) + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qwcscpy(pSubKey, L"追記モードで開く");
		*pSubKey2++ = '\\';
		qwcscpy(pSubKey2, L"command");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		pTmp = qwcscpy(pAppPathPtr, L" -a \"%1\"");
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)((pTmp - szAppPath + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qwcscpy(pKeyNamePtr, L"*");
		*pSubKey2++ = '\\';
		pSubKey2 = qwcscpy(pSubKey2, L"shell");
		*pSubKey2++ = '\\';
		pSubKey2 = qwcscpy(pSubKey2, L"MD5チェックツールで開く");
		*pSubKey2++ = '\\';
		qwcscpy(pSubKey2, L"command");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		pTmp = qwcscpy(pAppPathPtr, L" \"%1\"");
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)((pTmp - szAppPath + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qwcscpy(pKeyNamePtr, L"Directory");
		*pSubKey2++ = '\\';
		pSubKey2 = qwcscpy(pSubKey2, L"shell");
		*pSubKey2++ = '\\';
		pSubKey2 = qwcscpy(pSubKey2, L"MD5チェックツールで開く");
		*pSubKey2++ = '\\';
		qwcscpy(pSubKey2, L"command");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)((pTmp - szAppPath + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);

		pSubKey2 = qwcscpy(pKeyNamePtr, L"Drive");
		*pSubKey2++ = '\\';
		pSubKey2 = qwcscpy(pSubKey2, L"shell");
		*pSubKey2++ = '\\';
		pSubKey2 = qwcscpy(pSubKey2, L"MD5チェックツールで開く");
		*pSubKey2++ = '\\';
		qwcscpy(pSubKey2, L"command");
		nRet = RegCreateKeyExW(hKeyType[nIsWinVista], szKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
		IF_UNLIKELY(nRet != ERROR_SUCCESS) {
			continue;
		}
		nRet = RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE)szAppPath, (DWORD)((pTmp - szAppPath + 1) * sizeof(WCHAR)));
		IF_UNLIKELY(nRet != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			continue;
		}
		RegCloseKey(hKey);
	}
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
	return TRUE;
}
