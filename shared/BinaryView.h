#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __BINARYVIEW_H_B7EEA875_1F0D_41e0_B56A_542690D6E4A1__
#define __BINARYVIEW_H_B7EEA875_1F0D_41e0_B56A_542690D6E4A1__

#include "charlib.h"


size_t	VariableView_LengthA(QWORD nInVariable, DWORD dwType, int nIsHex = FALSE, int nIsVariable = TRUE);
size_t	VariableView_LengthW(QWORD nInVariable, DWORD dwType, int nIsHex = FALSE, int nIsVariable = TRUE);

size_t	VariableViewA(char* pOutChar, QWORD nInVariable, DWORD dwType, int nIsHex = FALSE, int nIsVariable = TRUE);
size_t	VariableViewW(WCHAR* pOutWChar, QWORD nInVariable, DWORD dwType, int nIsHex = FALSE, int nIsVariable = TRUE);

size_t	CharToVariableViewA(char* pOutChar, const char* nInChar, int nIsVariable = TRUE);
size_t	WCharToVariableViewA(char* pOutChar, const WCHAR* nInWChar, int nIsVariable = TRUE);

size_t	CharToVariableViewW(WCHAR* pOutWChar, const char* nInChar, int nIsVariable = TRUE);
size_t	WCharToVariableViewW(WCHAR* pOutWChar, const WCHAR* nInWChar, int nIsVariable = TRUE);

size_t	DoubleToVariableViewA(char* pOutChar, double nInDouble, int nIsVariable = TRUE);
size_t	DoubleToVariableViewW(WCHAR* pOutWChar, double nInDouble, int nIsVariable = TRUE);


char*	VariableViewToCharA(QWORD nInVariable, DWORD dwType, int nIsHex = FALSE, int nIsVariable = TRUE);
WCHAR*	VariableViewToWCharW(QWORD nInVariable, DWORD dwType, int nIsHex = FALSE, int nIsVariable = TRUE);

char*	CharToVariableViewAToCharA(const char* nInChar, int nIsVariable = TRUE);
char*	WCharToVariableViewAToCharA(const WCHAR* nInWChar, int nIsVariable = TRUE);

WCHAR*	CharToVariableViewWToWCharW(const char* nInChar, int nIsVariable = TRUE);
WCHAR*	WCharToVariableViewWToWCharW(const WCHAR* nInWChar, int nIsVariable = TRUE);

char*	DoubleToVariableViewToCharA(double nInDouble, int nIsVariable = TRUE);
WCHAR*	DoubleToVariableViewToWCharW(double nInDouble, int nIsVariable = TRUE);


#define ARRAYVIEW_TYPE_BYTE 1
#define ARRAYVIEW_TYPE_WORD 2
#define ARRAYVIEW_TYPE_DWORD 4
#define ARRAYVIEW_TYPE_QWORD 8
#define ARRAYVIEW_TYPE_DOUBLE (0x10000 | 8)
#define ARRAYVIEW_TYPE_PTR (0x1000000)
#define ARRAYVIEW_TYPE_CHAR (ARRAYVIEW_TYPE_PTR | 1)
#define ARRAYVIEW_TYPE_WCHAR (ARRAYVIEW_TYPE_PTR | 2)
#define ARRAYVIEW_TYPE_SIZE_T (sizeof(size_t))


size_t	ArrayView_LengthA(VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex = FALSE);
size_t	ArrayView_LengthW(VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex = FALSE);

size_t	ArrayViewA(char* pOutChar, const VOID* pInArray, DWORD dwType, size_t nArraySize = 1, int nIsHex = FALSE);
size_t	ArrayViewW(WCHAR* pOutWChar, const VOID* pInArray, DWORD dwType, size_t nArraySize = 1, int nIsHex = FALSE);

char*	ArrayViewToCharA(const VOID* pInArray, DWORD dwType, size_t nArraySize = 1, int nIsHex = FALSE);
WCHAR*	ArrayViewToWCharW(const VOID* pInArray, DWORD dwType, size_t nArraySize = 1, int nIsHex = FALSE);


size_t	BitView_Length(size_t nBitNum = sizeof(size_t));

size_t	BitViewA(char* pOutChar, size_t nBitNum, size_t nSize = sizeof(size_t));
size_t	BitViewW(WCHAR* pOutWChar, size_t nBitNum, size_t nSize = sizeof(size_t));

char*	BitViewToCharA(size_t nBitNum, size_t nSize = sizeof(size_t));
WCHAR*	BitViewToWCharW(size_t nBitNum, size_t nSize = sizeof(size_t));


size_t	BinaryView_Length(size_t nLen);

#define BITVIEW_BYTE_SIZE 153
#define BITVIEW_WORD_SIZE 184
#define BITVIEW_DWORD_SIZE 246
#define BITVIEW_QWORD_SIZE 370


size_t	BinaryViewA(char* pOutChar, const BYTE* cpInByte, size_t nLen, size_t nStart = 0);
size_t	BinaryViewW(WCHAR* pOutWChar, const BYTE* cpInByte, size_t nLen, size_t nStart = 0);

size_t	CharToBinaryViewA(char* pOutChar, const char* cpInChar, size_t nLen = -1, size_t nStart = 0);
size_t	WCharToBinaryViewA(char* pOutChar, const WCHAR* cpInWChar, size_t nLen = -1, size_t nStart = 0);

size_t	CharToBinaryViewW(WCHAR* pOutWChar, const char* cpInChar, size_t nLen = -1, size_t nStart = 0);
size_t	WCharToBinaryViewW(WCHAR* pOutWChar, const WCHAR* cpInWChar, size_t nLen = -1, size_t nStart = 0);

char*	CharToBinaryViewToCharA(const char* cpInChar, size_t nLen = -1, size_t nStart = 0);
char*	WCharToBinaryViewToCharA(const WCHAR* cpInWChar, size_t nLen = -1, size_t nStart = 0);

WCHAR*	CharToBinaryViewToWCharW(const char* cpInChar, size_t nLen = -1, size_t nStart = 0);
WCHAR*	WCharToBinaryViewToWCharW(const WCHAR* cpInWChar, size_t nLen = -1, size_t nStart = 0);

char*	BinaryViewToCharA(const BYTE* cpInByte, size_t nLen, int nIsWChar = FALSE, size_t nStart = 0);
WCHAR*	BinaryViewToWCharW(const BYTE* cpInByte, size_t nLen, int nIsWChar = FALSE, size_t nStart = 0);

VOID	BinaryViewFree(VOID* pFree);


#ifdef _UNICODE

#define VariableView VariableViewW
#define TCharToVariableView WCharToVariableViewW
#define CharToVariableView CharToVariableViewW
#define WCharToVariableView WCharToVariableViewW
#define DoubleToVariableView DoubleToVariableViewW

#define VariableViewToTChar VariableViewToWCharW
#define VariableViewToChar VariableViewToCharA
#define VariableViewToWChar VariableViewToWCharW

#define TCharToVariableViewToTChar WCharToVariableViewToWCharW
#define CharToVariableViewToTChar CharToVariableViewToWCharW
#define WCharToVariableViewToTChar WCharToVariableViewToWCharW
#define DoubleToVariableViewToTChar DoubleToVariableViewToWCharW

#define TCharToVariableViewToChar WCharToVariableViewToCharA
#define CharToVariableViewToChar CharToVariableViewToCharA
#define WCharToVariableViewToChar WCharToVariableViewToCharA
#define DoubleToVariableViewToChar DoubleToVariableViewToCharA

#define TCharToVariableViewToWChar WCharToVariableViewToWCharW
#define CharToVariableViewToWChar CharToVariableViewToWCharW
#define WCharToVariableViewToWChar WCharToVariableViewToWCharW
#define DoubleToVariableViewToWChar DoubleToVariableViewToWCharW

#define VariableView_Length VariableView_LengthW


#define ARRAYVIEW_TYPE_TCHAR ARRAYVIEW_TYPE_WCHAR
#define ArrayView ArrayViewW
#define ArrayViewToTChar ArrayViewToWCharW
#define ArrayViewToChar ArrayViewToCharA
#define ArrayViewToWChar ArrayViewToWCharW
#define ArrayView_Length ArrayView_LengthW

#define BitView BitViewW
#define BitViewToTChar BitViewToWCharW
#define BitViewToChar BitViewToCharA
#define BitViewToWChar BitViewToWCharW


#define BinaryView BinaryViewW
#define BinaryViewToTChar BinaryViewToWCharW

#define TCharToBinaryViewA WCharToBinaryViewA
#define TCharToBinaryViewW WCharToBinaryViewW

#define TCharToBinaryView WCharToBinaryViewW
#define TCharToBinaryViewToTChar WCharToBinaryViewToWCharW
#define TCharToBinaryViewToChar WCharToBinaryViewToCharA
#define TCharToBinaryViewToWChar WCharToBinaryViewToWCharW

#define CharToBinaryView CharToBinaryViewW
#define CharToBinaryViewToTChar CharToBinaryViewToWCharW
#define CharToBinaryViewToChar CharToBinaryViewToCharA
#define CharToBinaryViewToWChar CharToBinaryViewToWCharW

#define WCharToBinaryView WCharToBinaryViewW
#define WCharToBinaryViewToTChar WCharToBinaryViewToWCharW
#define WCharToBinaryViewToChar WCharToBinaryViewToCharA
#define WCharToBinaryViewToWChar WCharToBinaryViewToWCharW

#define BinaryViewToTChar BinaryViewToWCharW

#else

#define VariableView VariableViewA
#define TCharToVariableView CharToVariableViewA
#define CharToVariableView CharToVariableViewA
#define WCharToVariableView WCharToVariableViewA
#define DoubleToVariableView DoubleToVariableViewA

#define VariableViewToTChar VariableViewToCharA
#define VariableViewToChar VariableViewToCharA
#define VariableViewToWChar VariableViewToWCharW

#define TCharToVariableViewToTChar CharToVariableViewToCharA
#define CharToVariableViewToTChar CharToVariableViewToCharA
#define WCharToVariableViewToTChar WCharToVariableViewToCharA
#define DoubleToVariableViewToTChar DoubleToVariableViewToCharA

#define TCharToVariableViewToChar CharToVariableViewToCharA
#define CharToVariableViewToChar CharToVariableViewToCharA
#define WCharToVariableViewToChar WCharToVariableViewToCharA
#define DoubleToVariableViewToChar DoubleToVariableViewToCharA

#define TCharToVariableViewToWChar CharToVariableViewToWCharW
#define CharToVariableViewToWChar CharToVariableViewToWCharW
#define WCharToVariableViewToWChar WCharToVariableViewToWCharW
#define DoubleToVariableViewToWChar DoubleToVariableViewToWCharW

#define VariableView_Length VariableView_LengthA


#define ARRAYVIEW_TYPE_TCHAR ARRAYVIEW_TYPE_CHAR
#define ArrayView ArrayViewA
#define ArrayViewToTChar ArrayViewToCharA
#define ArrayViewToChar ArrayViewToCharA
#define ArrayViewToWChar ArrayViewToWCharW
#define ArrayView_Length ArrayView_LengthA

#define BitView BitViewA
#define BitViewToTChar BitViewToCharA
#define BitViewToChar BitViewToCharA
#define BitViewToWChar BitViewToWCharW

#define BinaryView BinaryViewA
#define BinaryViewToTChar BinaryViewToCharA

#define TCharToBinaryView CharToBinaryViewA
#define TCharToBinaryViewA CharToBinaryViewA
#define TCharToBinaryViewW CharToBinaryViewW

#define TCharToBinaryViewToTChar CharToBinaryViewToCharA
#define TCharToBinaryViewToChar CharToBinaryViewToCharA
#define TCharToBinaryViewToWChar CharToBinaryViewToWCharW

#define CharToBinaryView CharToBinaryViewA
#define CharToBinaryViewToTChar CharToBinaryViewToCharA
#define CharToBinaryViewToChar CharToBinaryViewToCharA
#define CharToBinaryViewToWChar CharToBinaryViewToWCharW

#define WCharToBinaryView WCharToBinaryViewA
#define WCharToBinaryViewToTChar WCharToBinaryViewToCharA
#define WCharToBinaryViewToChar WCharToBinaryViewToCharA
#define WCharToBinaryViewToWChar WCharToBinaryViewToWCharW

#endif
#endif
