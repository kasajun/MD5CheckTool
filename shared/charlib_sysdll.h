#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CHARLIB_SYSDLL_H_B77250CD_D21C_4011_A185_97F7AE932855__
#define __CHARLIB_SYSDLL_H_B77250CD_D21C_4011_A185_97F7AE932855__

#include "charlib.h"
#pragma comment(lib, "shlwapi.lib")

#ifdef  __cplusplus
extern "C" {
#endif

#define LOADSYSLIB(A)	LoadSystemLibraryToTCharSize(A, sizeof(A))
#define LOADSYSLIBA(A)	LoadSystemLibraryToCharSize(A, sizeof(A))
#define LOADSYSLIBW(A)	LoadSystemLibraryToWCharSize(A, sizeof(A))

BOOL Charlib_Shutdown();

BOOL SetSystemDllToChar();
BOOL SetSystemDllToWChar();


HMODULE LoadSystemLibraryToChar(const char* cpInDllName);
HMODULE LoadSystemLibraryToCharSize(const char* cpInDllName, size_t nDllNameLen);

HMODULE LoadSystemLibraryToWChar(const WCHAR* cpInDllName);
HMODULE LoadSystemLibraryToWCharSize(const WCHAR* cpInDllName, size_t nDllNameLen);


char* FileSizeText2ToChar(const __int64 cdwlFileSize);
WCHAR* FileSizeText2ToWChar(const __int64 cdwlFileSize);

char* FileSizeText2ToChar2(const __int64 cdwlFileSize, char* pBufChar);
WCHAR* FileSizeText2ToWChar2(const __int64 cdwlFileSize, WCHAR* pBufWChar);


char* FileSizePerSecTextToChar(const __int64 cdwlFileSize);
WCHAR* FileSizePerSecTextToWChar(const __int64 cdwlFileSize);

char* FileSizePerSecTextToChar2(const __int64 cdwlFileSize, char* pBufChar);
WCHAR* FileSizePerSecTextToWChar2(const __int64 cdwlFileSize, WCHAR* pBufWChar);

char* FileSizePerSecText2ToChar(const __int64 cdwlFileSize);
WCHAR* FileSizePerSecText2ToWChar(const __int64 cdwlFileSize);

char* FileSizePerSecText2ToChar2(const __int64 cdwlFileSize, char* pBufChar);
WCHAR* FileSizePerSecText2ToWChar2(const __int64 cdwlFileSize, WCHAR* pBufWChar);

#ifdef _UNICODE
#define FileSizeText2ToTChar FileSizeText2ToWChar
#define FileSizeText2ToTChar2 FileSizeText2ToWChar2
#define FileSizePerSecTextToTChar FileSizePerSecTextToWChar
#define FileSizePerSecTextToTChar2 FileSizePerSecTextToWChar2
#define FileSizePerSecText2ToTChar FileSizePerSecText2ToWChar
#define FileSizePerSecText2ToTChar2 FileSizePerSecText2ToWChar2
#define LoadSystemLibraryToTChar LoadSystemLibraryToWChar
#define LoadSystemLibraryToTCharSize LoadSystemLibraryToWCharSize
#define SetSystemDllToTChar SetSystemDllToWChar
#define STRFORMATKBSIZE_MACRO StrFormatKBSizeW
#define STRFORMATBYTESIZE_MACRO StrFormatByteSizeW
#else
LPSTR StrFormatKBSize_Func(__int64 qdw, LPSTR szBuf, UINT uiBufSize);
LPSTR StrFormatByteSize_Func(__int64 qdw, LPSTR szBuf, UINT uiBufSize);

#define FileSizeText2ToTChar FileSizeText2ToChar
#define FileSizeText2ToTChar2 FileSizeText2ToChar2
#define FileSizePerSecTextToTChar FileSizePerSecTextToChar
#define FileSizePerSecTextToTChar2 FileSizePerSecTextToChar2
#define FileSizePerSecText2ToTChar FileSizePerSecText2ToChar
#define FileSizePerSecText2ToTChar2 FileSizePerSecText2ToChar2
#define LoadSystemLibraryToTChar LoadSystemLibraryToChar
#define LoadSystemLibraryToTCharSize LoadSystemLibraryToCharSize
#define SetSystemDllToTChar SetSystemDllToChar
#define STRFORMATKBSIZE_MACRO StrFormatKBSize_Func
#define STRFORMATBYTESIZE_MACRO StrFormatByteSize_Func
#endif

#ifdef  __cplusplus
}
#endif
#endif
