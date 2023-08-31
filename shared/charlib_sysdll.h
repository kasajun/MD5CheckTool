#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CHARLIB_SYSDLL_H_B77250CD_D21C_4011_A185_97F7AE932855__
#define __CHARLIB_SYSDLL_H_B77250CD_D21C_4011_A185_97F7AE932855__

#include "charlib.h"
#pragma comment(lib, "shlwapi.lib")

#define LOADSYSLIB(A)	LoadSystemLibraryToTChar(A, sizeof(A))
#define LOADSYSLIBA(A)	LoadSystemLibraryToChar(A, sizeof(A))
#define LOADSYSLIBW(A)	LoadSystemLibraryToWChar(A, sizeof(A))

BOOL Charlib_Shutdown();

BOOL SetSystemDllToChar();
BOOL SetSystemDllToWChar();

HMODULE LoadSystemLibraryToChar(const char* cpInDllName);
HMODULE LoadSystemLibraryToChar(const char* cpInDllName, size_t nDllNameLen);

HMODULE LoadSystemLibraryToWChar(const WCHAR* cpInDllName);
HMODULE LoadSystemLibraryToWChar(const WCHAR* cpInDllName, size_t nDllNameLen);

char* FileSizeText2ToChar(const __int64 cdwlFileSize, char* pBufChar = NULL);
WCHAR* FileSizeText2ToWChar(const __int64 cdwlFileSize, WCHAR* pBufWChar = NULL);

char* FileSizePerSecTextToChar(const __int64 cdwlFileSize, char* pBufChar = NULL);
WCHAR* FileSizePerSecTextToWChar(const __int64 cdwlFileSize, WCHAR* pBufWChar = NULL);

char* FileSizePerSecText2ToChar(const __int64 cdwlFileSize, char* pBufChar = NULL);
WCHAR* FileSizePerSecText2ToWChar(const __int64 cdwlFileSize, WCHAR* pBufWChar = NULL);

#ifdef _UNICODE
#define FileSizeText2ToTChar FileSizeText2ToWChar
#define FileSizePerSecTextToTChar FileSizePerSecTextToWChar
#define FileSizePerSecText2ToTChar FileSizePerSecText2ToWChar
#define LoadSystemLibraryToTChar LoadSystemLibraryToWChar
#define SetSystemDllToTChar SetSystemDllToWChar
#define STRFORMATKBSIZE_MACRO StrFormatKBSizeW
#define STRFORMATBYTESIZE_MACRO StrFormatByteSizeW
#else
LPSTR StrFormatKBSize_Func(__int64 qdw, LPSTR szBuf, UINT uiBufSize);
LPSTR StrFormatByteSize_Func(__int64 qdw, LPSTR szBuf, UINT uiBufSize);
#define FileSizeText2ToTChar FileSizeText2ToChar
#define FileSizePerSecTextToTChar FileSizePerSecTextToChar
#define FileSizePerSecText2ToTChar FileSizePerSecText2ToChar
#define LoadSystemLibraryToTChar LoadSystemLibraryToChar
#define SetSystemDllToTChar SetSystemDllToChar
#define STRFORMATKBSIZE_MACRO StrFormatKBSize_Func
#define STRFORMATBYTESIZE_MACRO StrFormatByteSize_Func
#endif
#endif
