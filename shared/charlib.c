#include "charlib.h"
#include "c20.h"
#include <mbstring.h>


#if _MSC_VER < 1300
#include <stdarg.h>

size_t strcpy_s(char* _Dst, size_t _DstSize, const char* _Src)
{
	size_t nRet;

	nRet = (size_t)strncpy(_Dst, _Src, _DstSize - 1);

	if (*(_Dst + _DstSize - 1) != '\0') {
		*(_Dst + _DstSize - 1) = '\0';
	}
	return nRet;
}

size_t wcscpy_s(WCHAR* _Dst, size_t _DstSize, const WCHAR* _Src)
{
	size_t nRet;

	nRet = (size_t)wcsncpy(_Dst, _Src, _DstSize - 1);

	if (*(_Dst + _DstSize - 1) != '\0') {
		*(_Dst + _DstSize - 1) = '\0';
	}
	return nRet;
}

int sprintf_s(char* _Dst, size_t _SizeInWords, const char* _Format, ...)
{
	char szBuf[256];
	char* ptr = szBuf;
	va_list args;
	int nRet;

	if (_SizeInWords > 255)
	{
		ptr = (char*)malloc(_SizeInWords * sizeof(char));
		if (ptr == NULL) {
			return 0;
		}
	}

	va_start(args, _Format);
	nRet = vsprintf(ptr, _Format, args);
	va_end(args);

	strncpy(_Dst, ptr, _SizeInWords - 1);

	if (*(_Dst + _SizeInWords - 1) != '\0') {
		*(_Dst + _SizeInWords - 1) = '\0';
	}

	if (_SizeInWords > 255) {
		free(ptr);
	}
	return nRet;
}

int _swprintf_s(WCHAR* _Dst, size_t _SizeInWords, const WCHAR* _Format, ...)
{
	WCHAR szBuf[256];
	WCHAR* ptr = szBuf;
	va_list args;
	int nRet;

	if (_SizeInWords > 255)
	{
		ptr = (WCHAR*)malloc(_SizeInWords * sizeof(WCHAR));
		if (ptr == NULL) {
			return 0;
		}
	}

	va_start(args, _Format);
	nRet = vswprintf(ptr, _Format, args);
	va_end(args);

	wcsncpy(_Dst, ptr, _SizeInWords - 1);

	if (*(_Dst + _SizeInWords - 1) != '\0') {
		*(_Dst + _SizeInWords - 1) = '\0';
	}

	if (_SizeInWords > 255) {
		free(ptr);
	}
	return nRet;
}

size_t _itoa_s(int _Val, char* _DstBuf, size_t _SizeInWords, int _Radix)
{
	char _Buf[33];
	size_t nRet;

	_itoa(_Val, _Buf, _Radix);

	nRet = (size_t)strncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _itow_s(int _Val, WCHAR* _DstBuf, size_t _SizeInWords, int _Radix)
{
	WCHAR _Buf[33];
	size_t nRet;

	_itow(_Val, _Buf, _Radix);

	nRet = (size_t)wcsncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _ltoa_s(long _Val, char* _DstBuf, size_t _SizeInWords, int _Radix)
{
	char _Buf[33];
	size_t nRet;

	_ltoa(_Val, _Buf, _Radix);

	nRet = (size_t)strncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _ltow_s(long _Val, WCHAR* _DstBuf, size_t _SizeInWords, int _Radix)
{
	WCHAR _Buf[33];
	size_t nRet;

	_ltow(_Val, _Buf, _Radix);

	nRet = (size_t)wcsncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _ultoa_s(unsigned long _Val, char* _DstBuf, size_t _SizeInWords, int _Radix)
{
	char _Buf[33];
	size_t nRet;

	_ultoa(_Val, _Buf, _Radix);

	nRet = (size_t)strncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _ultow_s(unsigned long _Val, WCHAR* _DstBuf, size_t _SizeInWords, int _Radix)
{
	WCHAR _Buf[33];
	size_t nRet;

	_ultow(_Val, _Buf, _Radix);

	nRet = (size_t)wcsncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _i64toa_s(__int64 _Val, char* _DstBuf, size_t _SizeInWords, int _Radix)
{
	char _Buf[33];
	size_t nRet;

	_i64toa(_Val, _Buf, _Radix);

	nRet = (size_t)strncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _i64tow_s(__int64 _Val, WCHAR* _DstBuf, size_t _SizeInWords, int _Radix)
{
	WCHAR _Buf[33];
	size_t nRet;

	_i64tow(_Val, _Buf, _Radix);

	nRet = (size_t)wcsncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _ui64toa_s(unsigned __int64 _Val, char* _DstBuf, size_t _SizeInWords, int _Radix)
{
	char _Buf[33];
	size_t nRet;

	_ui64toa(_Val, _Buf, _Radix);

	nRet = (size_t)strncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

size_t _ui64tow_s(unsigned __int64 _Val, WCHAR* _DstBuf, size_t _SizeInWords, int _Radix)
{
	WCHAR _Buf[33];
	size_t nRet;

	_ui64tow(_Val, _Buf, _Radix);

	nRet = (size_t)wcsncpy(_DstBuf, _Buf, _SizeInWords - 1);

	if (*(_DstBuf + _SizeInWords - 1) != '\0') {
		*(_DstBuf + _SizeInWords - 1) = '\0';
	}
	return nRet;
}

void __cpuid(int* cpuInfo, int function_id)
{
	__asm {
		mov		esi, dword ptr [cpuInfo]
		mov		eax, function_id
		xor		ecx, ecx
		cpuid
		mov		dword ptr [esi +  0], eax
		mov		dword ptr [esi +  4], ebx
		mov		dword ptr [esi +  8], ecx
		mov		dword ptr [esi + 12], edx
	}
	return;
}

unsigned __int64 __rdtsc()
{
	unsigned __int64 c;
	__asm {
		cpuid
		rdtsc
		mov		dword ptr [c + 0], eax
		mov		dword ptr [c + 4], edx
	}
	return c;
}

/*
unsigned short BSwap16(unsigned short i)
{
	unsigned short j;
	j =  (i << 8) ;
	j += (i >> 8) ;
	return j;
}

unsigned long __fastcall BSwap32(unsigned long i)
{
	__asm {
		mov		eax, ecx
		bswap	eax
	}
}

unsigned __int64 __fastcall BSwap64(unsigned __int64 i)
{
	__asm {
		mov		edx, [ecx + 0]
		mov		eax, [ecx + 4]
		bswap	edx
		bswap	eax
	}
}
*/
#endif

#if _MSC_VER < 1600 && defined(_M_IX86)

void __cpuidex(int* cpuInfo, int function_id, int subfunction_id)
{
	__asm {
		mov		esi, dword ptr [cpuInfo]
		mov		eax, function_id
		mov		ecx, subfunction_id
		cpuid  
		mov		dword ptr [esi +  0], eax
		mov		dword ptr [esi +  4], ebx
		mov		dword ptr [esi +  8], ecx
		mov		dword ptr [esi + 12], edx
	}
	return;
}

#endif

#if _MSC_VER < 1600
void* _AlignedMalloc(size_t s, size_t align)
{
	//assert(align <= 128 && align >= 8); /* range check */
	//assert((align & (align - 1)) == 0);   /* power of 2 */
	//assert(s != 0 && s < (s + align));  /* empty/overflow */

   /* Overallocate to make room for manual realignment and an offset byte */
	unsigned char* base = (unsigned char*)malloc(s + align);
	IF_LIKELY(base != NULL)
	{
		/*
		 * Get the offset needed to align this pointer.
		 *
		 * Even if the returned pointer is aligned, there will always be
		 * at least one byte to store the offset to the original pointer.
		 */
		size_t offset = align - ((size_t)base & (align - 1)); /* base % align */
		/* Add the offset for the now-aligned pointer */
		unsigned char* ptr = base + offset;

		//assert((size_t)ptr % align == 0);

		/* Store the offset immediately before the returned pointer. */
		ptr[-1] = (unsigned char)offset;
		return ptr;
	}
	return NULL;
}

/*
 * Frees an aligned pointer allocated by XXH_alignedMalloc(). Don't pass
 * normal malloc'd pointers, XXH_alignedMalloc has a specific data layout.
 */
void _AlignedFree(void* p)
{
	IF_LIKELY(p != NULL)
	{
		unsigned char* ptr = (unsigned char*)p;
		/* Get the offset byte we added in malloc. */
		const unsigned char offset = ptr[-1];
		/* Free the original malloc'd pointer */
		unsigned char* base = ptr - offset;
		free(base);
	}
}
#endif

char* GetCharToRoot(char* pInFileName)
{
	char* pTmp = pInFileName;

	if (*(pTmp + 1) == ':' && *(pTmp + 2) == '\\') {
		pTmp += 3;
	}
	else
	{
		if (*(pTmp) == '\\' && *(pTmp + 1) == '\\')
		{
			pTmp = (char*)_mbsrchr((unsigned char*)pTmp + 3, '\\');
			if (pTmp == NULL)
			{
				return pInFileName;
			}
			pTmp++;
		}
	}
	*pTmp = '\0';
	return pInFileName;
}

WCHAR* GetWCharToRoot(WCHAR* pInFileName)
{
	WCHAR* pTmp = pInFileName;

	if (*(pTmp + 1) == ':' && *(pTmp + 2) == '\\') {
		pTmp += 3;
	}
	else
	{
		if (*(pTmp) == '\\' && *(pTmp + 1) == '\\')
		{
			pTmp = wcsrchr(pTmp + 3, '\\');
			if (pTmp == NULL)
			{
				return pInFileName;
			}
			pTmp++;
		}
	}
	*pTmp = '\0';
	return pInFileName;
}

char* GetUtf8CharToRoot(char* pInFileName)
{
	char* pTmp = pInFileName;

	if (*(pTmp + 1) == ':' && *(pTmp + 2) == '\\') {
		pTmp += 3;
	}
	else
	{
		if (*(pTmp) == '\\' && *(pTmp + 1) == '\\')
		{
			pTmp = (char*)strrchr(pTmp + 3, '\\');
			if (pTmp == NULL) {
				return pInFileName;
			}
			pTmp++;
		}
	}
	*pTmp = '\0';
	return pInFileName;
}

size_t GetCharToFileNameLen(const char* cpInFileName)
{
	char* pCharFileName;
	size_t nRet;

	pCharFileName = GetCharToFileName(cpInFileName);
	nRet = strlen(pCharFileName);
	return nRet;
}

size_t GetWCharToFileNameLen(const WCHAR* cpInFileName)
{
	WCHAR* pWCharFileName;
	size_t nRet;

	pWCharFileName = GetWCharToFileName(cpInFileName);
	nRet = wcslen(pWCharFileName);
	return nRet;
}

size_t GetUtf8CharToFileNameLen(const char* cpInFileName)
{
	char* pUtf8CharFileName;
	size_t nRet;

	pUtf8CharFileName = GetUtf8CharToFileName(cpInFileName);
	nRet = strlen(pUtf8CharFileName);
	return nRet;
}

size_t GetCharToFileNameNoExtensionLen(const char* cpInFileName)
{
	char* pCharFileName;
	char* pTmp;
	size_t nRet;

	pCharFileName = GetCharToFileName(cpInFileName);
	pTmp = (char*)_mbsrchr((unsigned char*)pCharFileName, '.');
	if (pTmp != NULL)
	{
		*pTmp = '\0';
		nRet = strlen(pCharFileName);
		*pTmp = '.';
	}
	else {
		nRet = strlen(pCharFileName);
	}
	return nRet;
}

size_t GetWCharToFileNameNoExtensionLen(const WCHAR* cpInFileName)
{
	WCHAR* pWCharFileName;
	WCHAR* pTmp;
	size_t nRet;

	pWCharFileName = GetWCharToFileName(cpInFileName);
	pTmp = (WCHAR*)wcsrchr(pWCharFileName, '.');
	if (pTmp != NULL)
	{
		*pTmp = '\0';
		nRet = wcslen(pWCharFileName);
		*pTmp = '.';
	}
	else {
		nRet = wcslen(pWCharFileName);
	}
	return nRet;
}

size_t GetUtf8CharToFileNameNoExtensionLen(const char* cpInFileName)
{
	char* pUtf8CharFileName;
	char* pTmp;
	size_t nRet;

	pUtf8CharFileName = GetUtf8CharToFileName(cpInFileName);
	pTmp = (char*)strrchr(pUtf8CharFileName, '.');
	if (pTmp != NULL)
	{
		*pTmp = '\0';
		nRet = strlen(pUtf8CharFileName);
		*pTmp = '.';
	}
	else {
		nRet = strlen(pUtf8CharFileName);
	}
	return nRet;
}

char* GetCharToFileName(const char* cpInFileName)
{
	char* pTmp;

	pTmp = (char*)_mbsrchr((unsigned char*)cpInFileName, '\\');
	if (pTmp == NULL) {
		return (char*)cpInFileName;
	}
	return pTmp + 1;
}

WCHAR* GetWCharToFileName(const WCHAR* cpInFileName)
{
	WCHAR* pTmp;

	pTmp = (WCHAR*)wcsrchr(cpInFileName, '\\');
	if (pTmp == NULL) {
		return (WCHAR*)cpInFileName;
	}
	return pTmp + 1;
}

char* GetUtf8CharToFileName(const char* cpInFileName)
{
	char* pTmp;

	pTmp = (char*)strrchr(cpInFileName, '\\');
	if (pTmp == NULL) {
		return (char*)cpInFileName;
	}
	return pTmp + 1;
}

char* GetCharToExtension(const char* cpInFileName)
{
	char* pExt;

	pExt = GetCharToFileName(cpInFileName);
	pExt = (char*)_mbsrchr((unsigned char*)pExt, '.');
	if (pExt == NULL) {
		return (char*)cpInFileName + strlen(cpInFileName);
	}
	return pExt;
}

char* GetCharToExtension2(const char* cpInFileName, BOOL bIsNoExtensionReturnNullptr)
{
	char* pExt;

	pExt = GetCharToFileName(cpInFileName);
	pExt = (char*)_mbsrchr((unsigned char*)pExt, '.');
	if (pExt == NULL) {
		return bIsNoExtensionReturnNullptr ? NULL : (char*)cpInFileName + strlen(cpInFileName);
	}
	return pExt;
}

WCHAR* GetWCharToExtension(const WCHAR* cpInFileName)
{
	WCHAR* pExt;

	pExt = GetWCharToFileName(cpInFileName);
	pExt = (WCHAR*)wcsrchr(pExt, '.');
	if (pExt == NULL) {
		return (WCHAR*)cpInFileName + wcslen(cpInFileName);
	}
	return pExt;
}

WCHAR* GetWCharToExtension2(const WCHAR* cpInFileName, BOOL bIsNoExtensionReturnNullptr)
{
	WCHAR* pExt;

	pExt = GetWCharToFileName(cpInFileName);
	pExt = (WCHAR*)wcsrchr(pExt, '.');
	if (pExt == NULL) {
		return bIsNoExtensionReturnNullptr ? NULL : (WCHAR*)cpInFileName + wcslen(cpInFileName);
	}
	return pExt;
}

char* GetUtf8CharToExtension(const char* cpInFileName)
{
	char* pExt;

	pExt = GetUtf8CharToFileName(cpInFileName);
	pExt = (char*)strrchr(pExt, '.');
	if (pExt == NULL) {
		return (char*)cpInFileName + strlen(cpInFileName);
	}
	return pExt;
}

char* GetUtf8CharToExtension2(const char* cpInFileName, BOOL bIsNoExtensionReturnNullptr)
{
	char* pExt;

	pExt = GetUtf8CharToFileName(cpInFileName);
	pExt = (char*)strrchr(pExt, '.');
	if (pExt == NULL) {
		return bIsNoExtensionReturnNullptr ? NULL : (char*)cpInFileName + strlen(cpInFileName);
	}
	return pExt;
}

char* GetCharToFolderName(char* cpInFileName)
{
	char* pTmp;

	pTmp = (char*)_mbsrchr((unsigned char*)cpInFileName, '\\');
	if (pTmp == NULL) {
		return cpInFileName;
	}

	if ((pTmp - cpInFileName) == 2)
	{
		LONG szBuf;
		const LONG nCmp = MAKE_CHAR_TO_INT_4CHAR(0, ':', '\\', 0);

		memcpy(&szBuf, cpInFileName, sizeof(szBuf));
		if ((szBuf & nCmp) == nCmp) {
			pTmp++;
		}
	}
	*pTmp = '\0';
	return cpInFileName;
}

WCHAR* GetWCharToFolderName(WCHAR* cpInFileName)
{
	WCHAR* pTmp;

	pTmp = wcsrchr(cpInFileName, '\\');
	if (pTmp == NULL) {
		return cpInFileName;
	}

	if ((pTmp - cpInFileName) == 2)
	{
		unsigned __int64 szBuf;
		const unsigned __int64 nCmp = MAKE_WCHAR_TO_INT_4CHAR(0, ':', '\\', 0);

		memcpy(&szBuf, cpInFileName, sizeof(szBuf));
		if ((szBuf & nCmp) == nCmp) {
			pTmp++;
		}
	}
	*pTmp = '\0';
	return cpInFileName;
}

char* GetUtf8CharToFolderName(char* cpInFileName)
{
	char* pTmp;

	pTmp = (char*)strrchr(cpInFileName, '\\');
	if (pTmp == NULL) {
		return cpInFileName;
	}
	*pTmp = '\0';
	return cpInFileName;
}

unsigned __int64 FileToFileSizeF(FILE* pInFilePointer)
{
#ifndef _NODLL
	IF_LIKELY(pInFilePointer != NULL && _fseeki64(pInFilePointer, 0, SEEK_END) == 0) {
		return _ftelli64(pInFilePointer);
	}
#else
	IF_LIKELY(pInFilePointer != NULL && fseek(pInFilePointer, 0, SEEK_END) == 0) {
		return ftell(pInFilePointer);
	}
#endif
	return 0;
}

unsigned __int64 FileToFileSizeA(const char* cpInFileName)
{
	WIN32_FIND_DATAA fdFindData = { 0 };
	HANDLE hFind = NULL;
	unsigned __int64 nRet = 0;

	hFind = FindFirstFileA(cpInFileName, &fdFindData);
	IF_UNLIKELY(hFind == INVALID_HANDLE_VALUE) {
		return 0;
	}
	FindClose(hFind);
	nRet = fdFindData.nFileSizeLow;
	nRet += (unsigned __int64)fdFindData.nFileSizeHigh << 32;
	return nRet;
}

unsigned __int64 FileToFileSizeW(const WCHAR* cpInFileName)
{
	WIN32_FIND_DATAW fdFindData = { 0 };
	HANDLE hFind = NULL;
	unsigned __int64 nRet = 0;

	hFind = FindFirstFileW(cpInFileName, &fdFindData);
	IF_UNLIKELY(hFind == INVALID_HANDLE_VALUE) {
		return 0;
	}
	FindClose(hFind);
	nRet = fdFindData.nFileSizeLow;
	nRet += (unsigned __int64)fdFindData.nFileSizeHigh << 32;
	return nRet;
}

char* DecToChar(size_t inDec, char* pOutChar)
{
	IF_UNLIKELY(pOutChar == NULL)
	{
		pOutChar = (char*)malloc(33);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}

#ifndef _NODLL
	_i64toa_s(inDec, pOutChar, 33, 10);
#else
	_i64toa(inDec, pOutChar, 10);
#endif

	return pOutChar;
}

WCHAR* DecToWChar(size_t inDec, WCHAR* pOutWChar)
{
	IF_UNLIKELY(pOutWChar == NULL)
	{
		pOutWChar = (WCHAR*)malloc(33 * sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}

#ifndef _NODLL
	_i64tow_s(inDec, pOutWChar, 33, 10);
#else
	_i64tow(inDec, pOutWChar, 10);
#endif

	return pOutWChar;
}

char* HexToChar(size_t inHex, char* pOutChar)
{
	IF_UNLIKELY(pOutChar == NULL)
	{
		pOutChar = (char*)malloc(35);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}

	CHAR_COPY_TO_INT_2CHAR(pOutChar, '0', 'x');

#ifndef _NODLL
	_i64toa_s(inHex, pOutChar + 2, (size_t)35 - 2, 16);
#else
	_i64toa(inHex, pOutChar + 2, 16);
#endif

	return pOutChar;
}

WCHAR* HexToWChar(size_t inHex, WCHAR* pOutWChar)
{
	IF_UNLIKELY(pOutWChar == NULL)
	{
		pOutWChar = (WCHAR*)malloc(35 * sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}

	WCHAR_COPY_TO_INT_2CHAR(pOutWChar, '0', 'x');

#ifndef _NODLL
	_i64tow_s(inHex, pOutWChar + 2, (size_t)35 - 2, 16);
#else
	_i64tow(inHex, pOutWChar + 2, 16);
#endif

	return pOutWChar;
}

char* CharFileToCharW(const WCHAR* cpInFileName)
{
	FILE* pInFilePointer = NULL;
	BYTE szBuf[4] = { 0 };
	size_t iBufferSize = 0;
	size_t nRet = 0;
	char* pBufChar = 0;
#ifndef _NODLL
	errno_t error = 0;
#endif

	iBufferSize = (size_t)FileToFileSizeW(cpInFileName);
	pBufChar = (char*)malloc(iBufferSize + 4);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	*(pBufChar + iBufferSize) = '\0';

#ifndef _NODLL
	error = _wfopen_s(&pInFilePointer, cpInFileName, L"rb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = _wfopen(cpInFileName, L"rb")) == NULL)
#endif
	{
		return NULL;
	}

	nRet = fread(szBuf, 1, 4, pInFilePointer);
	if (CHAR_COMP_TO_INT_2CHAR(szBuf, 0xFF, 0xFE))
	{
		memcpy(pBufChar, szBuf + 2, 2);
		if (nRet >= 4) {
			nRet = fread(pBufChar + 2, 1, iBufferSize - 4, pInFilePointer);
		}
	}
	else if (CHAR_COMP_TO_INT_3CHAR(szBuf, 0xEF, 0xBB, 0xBF))
	{
		memcpy(pBufChar, szBuf + 3, 1);
		if (nRet >= 4) {
			nRet = fread(pBufChar + 3, 1, iBufferSize - 4, pInFilePointer);
		}
	}
	else
	{
		memcpy(pBufChar, szBuf, 4);
		if (nRet >= 4) {
			nRet = fread(pBufChar + 4, 1, iBufferSize - 4, pInFilePointer);
		}
	}
	fclose(pInFilePointer);
	return pBufChar;
}

char* CharFileToCharA(const char* cpInFileName)
{
	FILE* pInFilePointer = NULL;
	BYTE szBuf[4] = { 0 };
	size_t iBufferSize = 0;
	size_t nRet = 0;
	char* pBufChar = NULL;
#ifndef _NODLL
	errno_t error = 0;
#endif

	iBufferSize = (size_t)FileToFileSizeA(cpInFileName);
	pBufChar = (char*)malloc(iBufferSize + 4);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	*(pBufChar + iBufferSize) = '\0';

#ifndef _NODLL
	error = fopen_s(&pInFilePointer, cpInFileName, "rb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = fopen(cpInFileName, "rb")) == NULL)
#endif
	{
		return NULL;
	}

	nRet = fread(szBuf, 1, 4, pInFilePointer);
	if (CHAR_COMP_TO_INT_2CHAR(szBuf, 0xFF, 0xFE))
	{
		memcpy(pBufChar, szBuf + 2, 2);
		if (nRet >= 4) {
			nRet = fread(pBufChar + 2, 1, iBufferSize - 4, pInFilePointer);
		}
	}
	else if (CHAR_COMP_TO_INT_3CHAR(pBufChar, 0xEF, 0xBB, 0xBF))
	{
		memcpy(pBufChar, szBuf + 3, 1);
		if (nRet >= 4) {
			nRet = fread(pBufChar + 3, 1, iBufferSize - 4, pInFilePointer);
		}
	}
	else
	{
		memcpy(pBufChar, szBuf, 4);
		if (nRet >= 4) {
			nRet = fread(pBufChar + 4, 1, iBufferSize - 4, pInFilePointer);
		}
	}
	fclose(pInFilePointer);
	return pBufChar;
}

char* FileToCharW(const WCHAR* cpInFileName, unsigned __int64* outFilesize)
{
	FILE* pInFilePointer;
	size_t iBufferSize;
	char* pBufChar;
#ifndef _NODLL
	errno_t error;
#endif

	iBufferSize = (size_t)FileToFileSizeW(cpInFileName);
	*outFilesize = 0;
	pBufChar = (char*)malloc(iBufferSize + 1);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	*(pBufChar + iBufferSize) = '\0';

#ifndef _NODLL
	error = _wfopen_s(&pInFilePointer, cpInFileName, L"rb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = _wfopen(cpInFileName, L"rb")) == NULL)
#endif
	{
		return NULL;
	}

	iBufferSize = fread(pBufChar, iBufferSize, 1, pInFilePointer);
	fclose(pInFilePointer);
	*outFilesize = iBufferSize;
	return pBufChar;
}

char* FileToCharA(const char* cpInFileName, unsigned __int64* outFilesize)
{
	FILE* pInFilePointer;
	size_t iBufferSize;
	char* pBufChar;
#ifndef _NODLL
	errno_t error;
#endif

	iBufferSize = (size_t)FileToFileSizeA(cpInFileName);
	*outFilesize = 0;
	pBufChar = (char*)malloc(iBufferSize + 1);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	*(pBufChar + iBufferSize) = '\0';

#ifndef _NODLL
	error = fopen_s(&pInFilePointer, cpInFileName, "rb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = fopen(cpInFileName, "rb")) == NULL)
#endif
	{
		return NULL;
	}

	iBufferSize = fread(pBufChar, iBufferSize, 1, pInFilePointer);
	fclose(pInFilePointer);
	*outFilesize = iBufferSize;
	return pBufChar;
}

char* Utf8CharFileToCharA(const char* cpInFileName)
{
	char* pBufUtf8Char;
	char* pBufChar;

	pBufChar = CharFileToCharA(cpInFileName);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	pBufUtf8Char = Utf8CharToCharConv(pBufChar);
	free(pBufChar);
	return pBufUtf8Char;
}

char* Utf8CharFileToCharW(const WCHAR* cpInFileName)
{
	char* pBufUtf8Char;
	char* pBufChar;

	pBufChar = CharFileToCharW(cpInFileName);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	pBufUtf8Char = Utf8CharToCharConv(pBufChar);
	free(pBufChar);
	return pBufUtf8Char;
}

WCHAR* CharFileToWCharW(const WCHAR* cpInFileName)
{
	WCHAR* pBufWChar;
	char* pBufChar;

	pBufChar = CharFileToCharW(cpInFileName);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	pBufWChar = CharToWCharConv(pBufChar);
	free(pBufChar);
	return pBufWChar;
}

WCHAR* CharFileToWCharA(const char* cpInFileName)
{
	WCHAR* pBufWChar;
	char* pBufChar;

	pBufChar = CharFileToCharA(cpInFileName);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	pBufWChar = CharToWCharConv(pBufChar);
	free(pBufChar);
	return pBufWChar;
}

WCHAR* Utf8CharFileToWCharA(const char* cpInFileName)
{
	WCHAR* pBufWChar;
	char* pBufChar;

	pBufChar = CharFileToCharA(cpInFileName);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	pBufWChar = Utf8CharToWCharConv(pBufChar);
	free(pBufChar);
	return pBufWChar;
}

WCHAR* Utf8CharFileToWCharW(const WCHAR* cpInFileName)
{
	WCHAR* pBufWChar;
	char* pBufChar;

	pBufChar = CharFileToCharW(cpInFileName);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	pBufWChar = Utf8CharToWCharConv(pBufChar);
	free(pBufChar);
	return pBufWChar;
}

WCHAR* WCharFileToWCharW(const WCHAR* cpInFileName)
{
	FILE* pInFilePointer;
	size_t iBufferSize;
	size_t nNotBom = 2;
	size_t nRet = 0;
	WCHAR* pBufWChar;
#ifndef _NODLL
	errno_t error;
#endif

	iBufferSize = (size_t)FileToFileSizeW(cpInFileName);
	IF_UNLIKELY(iBufferSize < 2) {
		return NULL;
	}

	pBufWChar = (WCHAR*)malloc(iBufferSize + sizeof(WCHAR));
	IF_UNLIKELY(pBufWChar == NULL) {
		return NULL;
	}

#ifndef _NODLL
	error = _wfopen_s(&pInFilePointer, cpInFileName, L"rb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = _wfopen(cpInFileName, L"rb")) == NULL)
#endif
	{
		free(pBufWChar);
		return NULL;
	}

	nRet = fread(pBufWChar, 2, 1, pInFilePointer);
	IF_UNLIKELY(!nRet)
	{
		free(pBufWChar);
		fclose(pInFilePointer);
		return NULL;
	}

	if (CHAR_COMP_TO_INT_2CHAR(pBufWChar, 0xFF, 0xFE))
	{
		nNotBom = 0;
		iBufferSize -= 2;
	}

	*(WCHAR*)((unsigned char*)pBufWChar + iBufferSize) = '\0';
	nRet = fread(pBufWChar + nNotBom, (size_t)iBufferSize, 1, pInFilePointer);
	IF_UNLIKELY(!nRet)
	{
		free(pBufWChar);
		pBufWChar = NULL;
	}
	fclose(pInFilePointer);
	return pBufWChar;
}

WCHAR* WCharFileToWCharA(const char* cpInFileName)
{
	FILE* pInFilePointer;
	size_t iBufferSize;
	size_t nNotBom = 2;
	size_t nRet = 0;
	WCHAR* pBufWChar;
#ifndef _NODLL
	errno_t error;
#endif

	iBufferSize = (size_t)FileToFileSizeA(cpInFileName);
	IF_UNLIKELY(iBufferSize < 2) {
		return NULL;
	}

	pBufWChar = (WCHAR*)malloc(iBufferSize + sizeof(WCHAR));
	IF_UNLIKELY(pBufWChar == NULL) {
		return NULL;
	}

#ifndef _NODLL
	error = fopen_s(&pInFilePointer, cpInFileName, "rb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = fopen(cpInFileName, "rb")) == NULL)
#endif
	{
		free(pBufWChar);
		return NULL;
	}

	nRet = fread(pBufWChar, 2, 1, pInFilePointer);
	IF_UNLIKELY(!nRet)
	{
		free(pBufWChar);
		fclose(pInFilePointer);
		return NULL;
	}

	if (CHAR_COMP_TO_2CHAR(pBufWChar, 0xFF, 0xFE))
	{
		nNotBom = 0;
		iBufferSize -= 2;
	}

	*(WCHAR*)((unsigned char*)pBufWChar + iBufferSize) = '\0';
	nRet = fread(pBufWChar + nNotBom, (size_t)iBufferSize, 1, pInFilePointer);
	IF_UNLIKELY(!nRet)
	{
		free(pBufWChar);
		pBufWChar = NULL;
	}

	fclose(pInFilePointer);
	return pBufWChar;
}

char* WCharFileToCharW(const WCHAR* cpInFileName)
{
	WCHAR* pBufWChar;
	char* pBufChar;

	pBufWChar = WCharFileToWCharW(cpInFileName);
	IF_UNLIKELY(pBufWChar == NULL) {
		return NULL;
	}
	pBufChar = WCharToCharConv(pBufWChar);
	free(pBufWChar);
	return pBufChar;
}

char* WCharFileToCharA(const char* cpInFileName)
{
	WCHAR* pBufWCharFileName;
	char* pBufChar;

	pBufWCharFileName = CharToWCharConv(cpInFileName);
	IF_UNLIKELY(pBufWCharFileName == NULL) {
		return 0;
	}
	pBufChar = WCharFileToCharW(pBufWCharFileName);
	free(pBufWCharFileName);
	return pBufChar;
}

size_t CharToCharFileA(const char* cpInChar, const char* cpInFileName)
{
	const size_t iBufferSize = strlen(cpInChar);
	return CharToFileSizeA(cpInChar, cpInFileName, iBufferSize);
}

size_t CharToCharFileW(const char* cpInChar, const WCHAR* cpInFileName)
{
	const size_t iBufferSize = strlen(cpInChar);
	return CharToFileSizeW(cpInChar, cpInFileName, iBufferSize);
}

size_t CharToWCharFileA(const char* cpInChar, const char* cpInFileName)
{
	WCHAR* pBufWChar;
	size_t nRet = FALSE;

	pBufWChar = CharToWCharConv(cpInChar);
	IF_UNLIKELY(pBufWChar == NULL) {
		return nRet;
	}
	nRet = WCharToWCharFileA(pBufWChar, cpInFileName);
	free(pBufWChar);
	return nRet;
}

size_t CharToWCharFileW(const char* cpInChar, const WCHAR* cpInFileName)
{
	WCHAR* pBufWChar;
	size_t nRet = FALSE;

	pBufWChar = CharToWCharConv(cpInChar);
	IF_UNLIKELY(pBufWChar == NULL) {
		return nRet;
	}
	nRet = WCharToWCharFileW(pBufWChar, cpInFileName);
	free(pBufWChar);
	return nRet;
}

size_t CharToUtf8CharFileA(const char* cpInChar, const char* cpInFileName)
{
	size_t iBufferSize;
	size_t nRet = FALSE;
	char* pBufUtf8Char;

	pBufUtf8Char = CharToUtf8CharConv(cpInChar);
	IF_UNLIKELY(pBufUtf8Char == NULL) {
		return nRet;
	}
	iBufferSize = strlen(pBufUtf8Char);
	nRet = CharToFileSizeA(pBufUtf8Char, cpInFileName, iBufferSize);
	free(pBufUtf8Char);
	return nRet;
}

size_t CharToUtf8CharFileW(const char* cpInChar, const WCHAR* cpInFileName)
{
	size_t iBufferSize;
	size_t nRet = FALSE;
	char* pBufUtf8Char;

	pBufUtf8Char = CharToUtf8CharConv(cpInChar);
	IF_UNLIKELY(pBufUtf8Char == NULL) {
		return nRet;
	}
	iBufferSize = strlen(pBufUtf8Char);
	nRet = CharToFileSizeW(pBufUtf8Char, cpInFileName, iBufferSize);
	free(pBufUtf8Char);
	return nRet;
}

size_t BinaryToFileA(const void* cpInBinary, const char* cpInFileName, size_t inSize)
{
	FILE* pInFilePointer;
	size_t nRet = FALSE;
#ifndef _NODLL
	errno_t error;

	error = fopen_s(&pInFilePointer, cpInFileName, "wb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = fopen(cpInFileName, "wb")) == NULL)
#endif
	{
		return nRet;
	}

	nRet = fwrite(cpInBinary, inSize, 1, pInFilePointer);
	fclose(pInFilePointer);
	return nRet;
}

size_t CharToFileSizeA(const char* cpInChar, const char* cpInFileName, size_t inSize)
{
	return BinaryToFileA((BYTE*)cpInChar, cpInFileName, inSize);
}

size_t CharToFileA(const char* cpInChar, const char* cpInFileName)
{
	return CharToCharFileA(cpInChar, cpInFileName);
}

size_t BinaryToFileW(const void* cpInBinary, const WCHAR* cpInFileName, size_t inSize)
{
	FILE* pInFilePointer;
	size_t nRet = FALSE;
#ifndef _NODLL
	errno_t error;

	error = _wfopen_s(&pInFilePointer, cpInFileName, L"wb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = _wfopen(cpInFileName, L"wb")) == NULL)
#endif
	{
		return nRet;
	}

	nRet = fwrite(cpInBinary, inSize, 1, pInFilePointer);
	fclose(pInFilePointer);
	return nRet;
}

size_t CharToFileSizeW(const char* cpInChar, const WCHAR* cpInFileName, size_t inSize)
{
	return BinaryToFileW((BYTE*)cpInChar, cpInFileName, inSize);
}

size_t CharToFileW(const char* cpInChar, const WCHAR* cpInFileName)
{
	return CharToCharFileW(cpInChar, cpInFileName);
}

size_t WCharToCharFileW(const WCHAR* cpInWChar, const WCHAR* cpInFileName)
{
	size_t iBufferSize;
	size_t nRet = FALSE;
	char* pBufChar;

	pBufChar = WCharToCharConv(cpInWChar);
	IF_UNLIKELY(pBufChar == NULL) {
		return nRet;
	}
	iBufferSize = strlen(pBufChar);
	nRet = CharToFileSizeW(pBufChar, cpInFileName, iBufferSize);
	free(pBufChar);
	return nRet;
}

size_t WCharToCharFileA(const WCHAR* cpInWChar, const char* cpInFileName)
{
	size_t iBufferSize;
	size_t nRet = FALSE;
	char* pBufChar;

	pBufChar = WCharToCharConv(cpInWChar);
	IF_UNLIKELY(pBufChar == NULL) {
		return nRet;
	}
	iBufferSize = strlen(pBufChar);
	nRet = CharToFileSizeA(pBufChar, cpInFileName, iBufferSize);
	free(pBufChar);
	return nRet;
}

size_t WCharToWCharFileW(const WCHAR* cpInWChar, const WCHAR* cpInFileName)
{
	FILE* pInFilePointer;
	const BYTE bBuf[] = { 0xFF, 0xFE };
	size_t iBufferSize = wcslen(cpInWChar) * sizeof(WCHAR);
	size_t nRet = FALSE;
#ifndef _NODLL
	errno_t error;

	error = _wfopen_s(&pInFilePointer, cpInFileName, L"wb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = _wfopen(cpInFileName, L"wb")) == NULL)
#endif
	{
		return nRet;
	}

	nRet = fwrite(bBuf, sizeof(bBuf), 1, pInFilePointer);
	IF_UNLIKELY(!nRet) {
		return nRet;
	}
	nRet = fwrite(cpInWChar, iBufferSize, 1, pInFilePointer);
	fclose(pInFilePointer);
	return nRet;
}

size_t WCharToWCharFileA(const WCHAR* cpInWChar, const char* cpInFileName)
{
	FILE* pInFilePointer;
	const BYTE bBuf[] = { 0xFF, 0xFE };
	size_t iBufferSize = wcslen(cpInWChar) * sizeof(WCHAR);
	size_t nRet = FALSE;
#ifndef _NODLL
	errno_t error;

	error = fopen_s(&pInFilePointer, cpInFileName, "wb");
	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = fopen(cpInFileName, "wb")) == NULL)
#endif
	{
		return nRet;
	}

	nRet = fwrite(bBuf, sizeof(bBuf), 1, pInFilePointer);
	IF_UNLIKELY(!nRet) {
		return nRet;
	}
	nRet = fwrite(cpInWChar, iBufferSize, 1, pInFilePointer);
	fclose(pInFilePointer);
	return nRet;
}

size_t WCharToUtf8CharFileW(const WCHAR* cpInWChar, const WCHAR* cpInFileName)
{
	size_t iBufferSize;
	size_t nRet = FALSE;
	char* pBufUtf8Char;

	pBufUtf8Char = WCharToUtf8CharConv(cpInWChar);
	IF_UNLIKELY(pBufUtf8Char == NULL) {
		return nRet;
	}
	iBufferSize = strlen(pBufUtf8Char);
	nRet = CharToFileSizeW(pBufUtf8Char, cpInFileName, iBufferSize);
	free(pBufUtf8Char);
	return nRet;
}

size_t WCharToUtf8CharFileA(const WCHAR* cpInWChar, const char* cpInFileName)
{
	size_t iBufferSize;
	size_t nRet = FALSE;
	char* pBufUtf8Char;

	pBufUtf8Char = WCharToUtf8CharConv(cpInWChar);
	IF_UNLIKELY(pBufUtf8Char == NULL) {
		return nRet;
	}
	iBufferSize = strlen(pBufUtf8Char);
	nRet = CharToFileSizeA(pBufUtf8Char, cpInFileName, iBufferSize);
	free(pBufUtf8Char);
	return nRet;
}


#define HEXCHAR_UPPER const char cpHexCharUpper[] = "0123456789ABCDEF"
#define HEXCHAR_LOWER const char cpHexCharLower[] = "0123456789abcdef"

char* SetCharToHexChar(const char cpInChar, char* outBuf)
{
	HEXCHAR_UPPER;
	outBuf[0] = cpHexCharUpper[(cpInChar & 0xF0) >> 4];
	outBuf[1] = cpHexCharUpper[(cpInChar & 0x0F)];
	outBuf[2] = '\0';
	return outBuf;
}

WCHAR* SetCharToHexWChar(const char cpInChar, WCHAR* outBuf)
{
	HEXCHAR_UPPER;
	outBuf[0] = cpHexCharUpper[(cpInChar & 0xF0) >> 4];
	outBuf[1] = cpHexCharUpper[(cpInChar & 0x0F)];
	outBuf[2] = '\0';
	return outBuf;
}

char* SetCharToHexLowerChar(const char cpInChar, char* outBuf)
{
	HEXCHAR_LOWER;
	outBuf[0] = cpHexCharLower[(cpInChar & 0xF0) >> 4];
	outBuf[1] = cpHexCharLower[(cpInChar & 0x0F)];
	outBuf[2] = '\0';
	return outBuf;
}

WCHAR* SetCharToHexLowerWChar(const char cpInChar, WCHAR* outBuf)
{
	HEXCHAR_LOWER;
	outBuf[0] = cpHexCharLower[(cpInChar & 0xF0) >> 4];
	outBuf[1] = cpHexCharLower[(cpInChar & 0x0F)];
	outBuf[2] = '\0';
	return outBuf;
}

size_t CharToRtfEncodeCharLength(const char* cpInChar)
{
	char* ptr = NULL;
	size_t nDstCount = 0;
	int nLang1041 = 0;

	ptr = (char*)cpInChar;
	while (*ptr != '\0')
	{
		if (_ismbblead(*ptr))
		{
			if (nLang1041 == 0)
			{
				nLang1041 = 1;
				nDstCount += sizeof("\\lang1041") - 1;
			}
			nDstCount += 4;
			ptr++;
			nDstCount += 4;
		}
		else
		{
			if (nLang1041 == 1)
			{
				if (*ptr == ' ')
				{
					nDstCount++;
					nLang1041 = 0;
					nDstCount += sizeof("\\lang17") - 1;
					ptr++;
					continue;
				}
				else
				{
					nLang1041 = 0;
					nDstCount += sizeof("\\lang17") - 1;
				}
			}
			nDstCount++;
			if (*ptr == '\\') {
				nDstCount++;
			}
		}
		ptr++;
	}
	return nDstCount;
}

char* CharToRtfEncodeChar(const char* cpInChar, char* pOutChar)
{
	HEXCHAR_LOWER;
	char* ptr = NULL;
	char* pDst = NULL;
	size_t iBufferSize = 0;
	int nLang1041 = 0;

	IF_UNLIKELY(pOutChar == NULL)
	{
		iBufferSize = CharToRtfEncodeCharLength(cpInChar);
		pOutChar = (char*)malloc(iBufferSize + 1);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}
	ptr = (char*)cpInChar;
	pDst = pOutChar;

	while (*ptr != '\0')
	{
		if (_ismbblead(*ptr))
		{
			if (nLang1041 == 0)
			{
				nLang1041 = 1;
				pDst = qstrcpy(pDst, "\\lang1041");
			}
			*pDst++ = '\\';
			*pDst++ = '\'';
			*pDst++ = cpHexCharLower[(*ptr & 0xF0) >> 4];
			*pDst++ = cpHexCharLower[(*ptr & 0x0F)];

			ptr++;
			*pDst++ = '\\';
			*pDst++ = '\'';
			*pDst++ = cpHexCharLower[(*ptr & 0xF0) >> 4];
			*pDst++ = cpHexCharLower[(*ptr & 0x0F)];
		}
		else
		{
			if (nLang1041 == 1)
			{
				if (*ptr == ' ')
				{
					*pDst++ = ' ';
					nLang1041 = 0;
					pDst = qstrcpy(pDst, "\\lang17");
					ptr++;
					continue;
				}
				else
				{
					nLang1041 = 0;
					pDst = qstrcpy(pDst, "\\lang17");
				}
			}
			*pDst++ = *ptr;
			if (*ptr == '\\') {
				*pDst++ = '\\';
			}
		}
		ptr++;
	}
	*pDst = '\0';
	return pOutChar;
}

char* WCharToRtfEncodeChar(const WCHAR* cpInWChar, char* pOutChar)
{
	char* pBufChar = WCharToCharConv(cpInWChar);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	pOutChar = CharToRtfEncodeChar(pBufChar, pOutChar);
	free(pBufChar);
	return pOutChar;
}

size_t qmbslen(const char* cpInChar)
{
	WCHAR* pBufWChar;
	size_t iBufferSize;

	pBufWChar = CharToWCharConv(cpInChar);
	IF_UNLIKELY(pBufWChar == NULL) {
		return 0;
	}
	iBufferSize = wcslen(pBufWChar);
	free(pBufWChar);
	return iBufferSize;
}

char* BinaryToCharToUpper(char* pOutChar, const BYTE* cpInBinary, const DWORD dwLength)
{
	return BinaryToChar(pOutChar, cpInBinary, dwLength, TRUE);
}

char* BinaryToCharToLower(char* pOutChar, const BYTE* cpInBinary, const DWORD dwLength)
{
	return BinaryToChar(pOutChar, cpInBinary, dwLength, FALSE);
}

WCHAR* BinaryToWCharToUpper(WCHAR* pOutWChar, const BYTE* cpInBinary, const DWORD dwLength)
{
	return BinaryToWChar(pOutWChar, cpInBinary, dwLength, TRUE);
}

WCHAR* BinaryToWCharToLower(WCHAR* pOutWChar, const BYTE* cpInBinary, const DWORD dwLength)
{
	return BinaryToWChar(pOutWChar, cpInBinary, dwLength, FALSE);
}

char* BinaryToChar(char* pOutChar, const BYTE* cpInBinary, const DWORD dwLength, BOOL bIsUpper)
{
	const char* cpHexLU[2] = { "0123456789abcdef", "0123456789ABCDEF" };
	const char* cpHexChar = cpHexLU[(bIsUpper) & 1];
	DWORD dwCount;
	char* p;
	size_t* aligned_p;
	BYTE* b;
	size_t* aligned_b;

	IF_UNLIKELY(pOutChar == NULL)
	{
		pOutChar = (char*)malloc(((size_t)dwLength + 1) * 2);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}

	p = pOutChar;
	b = (BYTE*)cpInBinary;
	dwCount = dwLength;

	if (!UNALIGNED2(p, b) && !TOO_SMALL(dwCount))
	{
		aligned_p = (size_t*)p;
		aligned_b = (size_t*)b;

		while (dwCount >= sizeof(size_t))
		{
			const size_t qwHashByte = *aligned_b++;
			size_t bByte;
			size_t bVal;

#if SIZE_MAX == 0xffffffffffffffffui64
			bVal = qwHashByte & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 8;

			bVal = (qwHashByte >> 8) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 16;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 24;

			bVal = (qwHashByte >> 16) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 32;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 40;

			bVal = (qwHashByte >> 24) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 48;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 56;
			*aligned_p++ = bByte;

			bVal = (qwHashByte >> 32) & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 8;

			bVal = (qwHashByte >> 40) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 16;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 24;

			bVal = (qwHashByte >> 48) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 32;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 40;

			bVal = (qwHashByte >> 56) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 48;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 56;
			*aligned_p++ = bByte;
#else
			bVal = qwHashByte & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 8;

			bVal = (qwHashByte >> 8) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 16;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 24;
			*aligned_p++ = bByte;

			bVal = (qwHashByte >> 16) & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 8;

			bVal = (qwHashByte >> 24) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 16;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 24;
			*aligned_p++ = bByte;
#endif

			dwCount -= sizeof(size_t);
		}

		p = (char*)aligned_p;
		b = (BYTE*)aligned_b;
	}

	while (dwCount--)
	{
		const BYTE byHashByte = *b++;
		*p++ = cpHexChar[byHashByte >> 4];
		*p++ = cpHexChar[byHashByte & 0x0F];
	}

	*p = '\0';
	return pOutChar;
}

WCHAR* BinaryToWChar(WCHAR* pOutWChar, const BYTE* cpInBinary, const DWORD dwLength, BOOL bIsUpper)
{
	const char* cpHexLU[2] = { "0123456789abcdef", "0123456789ABCDEF" };
	const char* cpHexChar = cpHexLU[(bIsUpper) & 1];
	DWORD dwCount;
	WCHAR* p;
	size_t* aligned_p;
	BYTE* b;
	size_t* aligned_b;

	IF_UNLIKELY(pOutWChar == NULL)
	{
		pOutWChar = (WCHAR*)malloc(((size_t)dwLength + 1) * 2 * sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}

	p = pOutWChar;
	b = (BYTE*)cpInBinary;
	dwCount = dwLength;

	if (!UNALIGNED2(p, b) && !TOO_SMALL(dwCount))
	{
		aligned_p = (size_t*)p;
		aligned_b = (size_t*)b;

		while (dwCount >= sizeof(size_t))
		{
			const size_t qwHashByte = *aligned_b++;
			size_t bByte;
			size_t bVal;

#if SIZE_MAX == 0xffffffffffffffffui64
			bVal = qwHashByte & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;

			bVal = (qwHashByte >> 8) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 32;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 48;
			*aligned_p++ = bByte;

			bVal = (qwHashByte >> 16) & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;

			bVal = (qwHashByte >> 24) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 32;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 48;
			*aligned_p++ = bByte;

			bVal = (qwHashByte >> 32) & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;

			bVal = ((qwHashByte >> 40) & 0xFF);
			bByte += (size_t)cpHexChar[bVal >> 4] << 32;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 48;
			*aligned_p++ = bByte;

			bVal = (qwHashByte >> 48) & 0xFF;
			bByte = cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;

			bVal = (qwHashByte >> 56) & 0xFF;
			bByte += (size_t)cpHexChar[bVal >> 4] << 32;
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 48;
			*aligned_p++ = bByte;
#else
			bVal = qwHashByte & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;
			*aligned_p++ = bByte;

			bVal = (qwHashByte >> 8) & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;
			*aligned_p++ = bByte;

			bVal = (qwHashByte >> 16) & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;
			*aligned_p++ = bByte;

			bVal = (qwHashByte >> 24) & 0xFF;
			bByte = (size_t)cpHexChar[bVal >> 4];
			bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;
			*aligned_p++ = bByte;
#endif

			dwCount -= sizeof(size_t);
		}

		p = (WCHAR*)aligned_p;
		b = (BYTE*)aligned_b;
	}

	while (dwCount--)
	{
		const BYTE byHashByte = *b++;
		*p++ = cpHexChar[byHashByte >> 4];
		*p++ = cpHexChar[byHashByte & 0x0F];
	}

	*p = '\0';
	return pOutWChar;
}

char* AlignedBinaryToCharToUpper(char* pOutChar, const BYTE* cpAlignedInBinary, const DWORD dwLength)
{
	return AlignedBinaryToChar(pOutChar, cpAlignedInBinary, dwLength, TRUE);
}

char* AlignedBinaryToCharToLower(char* pOutChar, const BYTE* cpAlignedInBinary, const DWORD dwLength)
{
	return AlignedBinaryToChar(pOutChar, cpAlignedInBinary, dwLength, FALSE);
}

WCHAR* AlignedBinaryToWCharToUpper(WCHAR* pOutWChar, const BYTE* cpAlignedInBinary, const DWORD dwLength)
{
	return AlignedBinaryToWChar(pOutWChar, cpAlignedInBinary, dwLength, TRUE);
}

WCHAR* AlignedBinaryToWCharToLower(WCHAR* pOutWChar, const BYTE* cpAlignedInBinary, const DWORD dwLength)
{
	return AlignedBinaryToWChar(pOutWChar, cpAlignedInBinary, dwLength, FALSE);
}

char* AlignedBinaryToChar(char* pOutChar, const BYTE* cpAlignedInBinary, const DWORD dwLength, BOOL bIsUpper)
{
	const char* cpHexLU[2] = { "0123456789abcdef", "0123456789ABCDEF" };
	const char* cpHexChar = cpHexLU[(bIsUpper) & 1];
	DWORD dwCount;
	size_t* aligned_p;
	size_t* aligned_b;

	dwCount = dwLength;
	aligned_p = (size_t*)pOutChar;
	aligned_b = (size_t*)cpAlignedInBinary;

	while (dwCount > 0)
	{
		const size_t qwHashByte = *aligned_b++;
		size_t bByte;
		size_t bVal;

#if SIZE_MAX == 0xffffffffffffffffui64
		bVal = qwHashByte & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 8;

		bVal = (qwHashByte >> 8) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 16;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 24;

		bVal = (qwHashByte >> 16) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 32;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 40;

		bVal = (qwHashByte >> 24) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 48;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 56;
		*aligned_p++ = bByte;

		bVal = (qwHashByte >> 32) & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 8;

		bVal = (qwHashByte >> 40) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 16;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 24;

		bVal = (qwHashByte >> 48) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 32;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 40;

		bVal = (qwHashByte >> 56) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 48;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 56;
		*aligned_p++ = bByte;
#else
		bVal = qwHashByte & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 8;

		bVal = (qwHashByte >> 8) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 16;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 24;
		*aligned_p++ = bByte;

		bVal = (qwHashByte >> 16) & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 8;

		bVal = (qwHashByte >> 24) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 16;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 24;
		*aligned_p++ = bByte;
#endif

		dwCount -= sizeof(size_t);
	}

	*((char*)aligned_p) = '\0';
	return pOutChar;
}

WCHAR* AlignedBinaryToWChar(WCHAR* pOutWChar, const BYTE* cpAlignedInBinary, const DWORD dwLength, BOOL bIsUpper)
{
	const char* cpHexLU[2] = { "0123456789abcdef", "0123456789ABCDEF" };
	const char* cpHexChar = cpHexLU[(bIsUpper) & 1];
	DWORD dwCount;
	size_t* aligned_p;
	size_t* aligned_b;

	dwCount = dwLength;
	aligned_p = (size_t*)pOutWChar;
	aligned_b = (size_t*)cpAlignedInBinary;

	while (dwCount > 0)
	{
		const size_t qwHashByte = *aligned_b++;
		size_t bByte;
		size_t bVal;

#if SIZE_MAX == 0xffffffffffffffffui64
		bVal = qwHashByte & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;

		bVal = (qwHashByte >> 8) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 32;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 48;
		*aligned_p++ = bByte;

		bVal = (qwHashByte >> 16) & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;

		bVal = (qwHashByte >> 24) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 32;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 48;
		*aligned_p++ = bByte;

		bVal = (qwHashByte >> 32) & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;

		bVal = ((qwHashByte >> 40) & 0xFF);
		bByte += (size_t)cpHexChar[bVal >> 4] << 32;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 48;
		*aligned_p++ = bByte;

		bVal = (qwHashByte >> 48) & 0xFF;
		bByte = cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;

		bVal = (qwHashByte >> 56) & 0xFF;
		bByte += (size_t)cpHexChar[bVal >> 4] << 32;
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 48;
		*aligned_p++ = bByte;
#else
		bVal = qwHashByte & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;
		*aligned_p++ = bByte;

		bVal = (qwHashByte >> 8) & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;
		*aligned_p++ = bByte;

		bVal = (qwHashByte >> 16) & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;
		*aligned_p++ = bByte;

		bVal = (qwHashByte >> 24) & 0xFF;
		bByte = (size_t)cpHexChar[bVal >> 4];
		bByte += (size_t)cpHexChar[bVal & 0x0F] << 16;
		*aligned_p++ = bByte;
#endif

		dwCount -= sizeof(size_t);
	}

	*((WCHAR*)aligned_p) = '\0';
	return pOutWChar;
}

#define ATOX_TABLE const unsigned char AToHexTable[] = \
{ \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0, \
	 0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, \
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 \
} \

unsigned char* CharToHex(const char* cpInChar, unsigned char* pOutHex, size_t nInSize)
{
	return CharToBinary(pOutHex, cpInChar, nInSize);
}

BYTE* CharToBinary(BYTE* pOutBinary, const char* cpInChar, size_t nInCharSize)
{
	ATOX_TABLE;
	size_t nCount;
	char* p;
	size_t* aligned_p;
	BYTE* b;
	size_t* aligned_b;

	if (nInCharSize == (size_t)-1) {
		nInCharSize = strlen(cpInChar);
	}

	IF_UNLIKELY(pOutBinary == NULL)
	{
		pOutBinary = (BYTE*)malloc(((size_t)nInCharSize));
		IF_UNLIKELY(pOutBinary == NULL) {
			return NULL;
		}
	}

	p = (char*)cpInChar;
	b = (BYTE*)pOutBinary;
	nCount = nInCharSize;

	if (!UNALIGNED2(p, b) && !TOO_SMALL(nCount))
	{
		aligned_p = (size_t*)p;
		aligned_b = (size_t*)b;

		while (nCount >= sizeof(size_t))
		{
			char szBuf[sizeof(size_t) * 2];
			size_t nHashByte;

			memcpy(szBuf, aligned_p, sizeof(szBuf));
			nHashByte = (size_t)AToHexTable[szBuf[1]];
			nHashByte += (size_t)AToHexTable[szBuf[0]] << 4;
			nHashByte += (size_t)AToHexTable[szBuf[3]] << 8;
			nHashByte += (size_t)AToHexTable[szBuf[2]] << 12;
			nHashByte += (size_t)AToHexTable[szBuf[5]] << 16;
			nHashByte += (size_t)AToHexTable[szBuf[4]] << 20;
			nHashByte += (size_t)AToHexTable[szBuf[7]] << 24;
			nHashByte += (size_t)AToHexTable[szBuf[6]] << 28;

#if SIZE_MAX == 0xffffffffffffffffui64
			nHashByte += (size_t)AToHexTable[szBuf[1 + 8]] << (0 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[0 + 8]] << (4 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[3 + 8]] << (8 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[2 + 8]] << (12 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[5 + 8]] << (16 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[4 + 8]] << (20 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[7 + 8]] << (24 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[6 + 8]] << (28 + 32);
#endif

			*aligned_b++ = nHashByte;
			aligned_p += 2;

#if SIZE_MAX == 0xffffffffffffffffui64
			nCount -= (size_t)8 * 2;
#else
			nCount -= 4 * 2;
#endif

		}

		p = (char*)aligned_p;
		b = (BYTE*)aligned_b;
	}

	while (nCount)
	{
		char szBuf[2];
		long nHashByte;

		memcpy(szBuf, p, sizeof(szBuf));
		nHashByte = AToHexTable[szBuf[1]];
		nHashByte += AToHexTable[szBuf[0]] << 4;

		*b++ = (BYTE)nHashByte;
		p += 2;
		nCount -= 2;
	}
	return pOutBinary;
}

unsigned char* WCharToHex(const WCHAR* cpInWChar, unsigned char* pOutHex, size_t nInSize)
{
	return WCharToBinary(pOutHex, cpInWChar, nInSize);
}

BYTE* WCharToBinary(BYTE* pOutBinary, const WCHAR* cpInWChar, size_t nInWCharSize)
{
	ATOX_TABLE;
	size_t nCount;
	WCHAR* p;
	size_t* aligned_p;
	BYTE* b;
	size_t* aligned_b;


	if (nInWCharSize == (size_t)-1) {
		nInWCharSize = wcslen(cpInWChar);
	}

	IF_UNLIKELY(pOutBinary == NULL)
	{
		pOutBinary = (BYTE*)malloc(((size_t)nInWCharSize));
		IF_UNLIKELY(pOutBinary == NULL) {
			return NULL;
		}
	}

	p = (WCHAR*)cpInWChar;
	b = (BYTE*)pOutBinary;
	nCount = nInWCharSize;

	if (!UNALIGNED2(p, b) && !TOO_SMALL(nCount))
	{
		aligned_p = (size_t*)p;
		aligned_b = (size_t*)b;

		while (nCount > sizeof(size_t))
		{
			WCHAR szBuf[sizeof(size_t) * 2];
			size_t nHashByte;

			memcpy(szBuf, aligned_p, sizeof(szBuf));
			nHashByte = (size_t)AToHexTable[szBuf[1]];
			nHashByte += (size_t)AToHexTable[szBuf[0]] << 4;
			nHashByte += (size_t)AToHexTable[szBuf[3]] << 8;
			nHashByte += (size_t)AToHexTable[szBuf[2]] << 12;
			nHashByte += (size_t)AToHexTable[szBuf[5]] << 16;
			nHashByte += (size_t)AToHexTable[szBuf[4]] << 20;
			nHashByte += (size_t)AToHexTable[szBuf[7]] << 24;
			nHashByte += (size_t)AToHexTable[szBuf[6]] << 28;

#if SIZE_MAX == 0xffffffffffffffffui64
			nHashByte += (size_t)AToHexTable[szBuf[1 + 8]] << (0 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[0 + 8]] << (4 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[3 + 8]] << (8 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[2 + 8]] << (12 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[5 + 8]] << (16 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[4 + 8]] << (20 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[7 + 8]] << (24 + 32);
			nHashByte += (size_t)AToHexTable[szBuf[6 + 8]] << (28 + 32);
#endif

			*aligned_b++ = nHashByte;

#if WCHAR_MAX > 0xFFFF
			aligned_p += (2 * 4);
#else
			aligned_p += (2 * 2);// aligned_p += (2 * sizeof(WCHAR)); warning C6305
#endif

#if SIZE_MAX == 0xffffffffffffffffui64
			nCount -= (size_t)8 * 2;
#else
			nCount -= 4 * 2;
#endif

		}

		p = (WCHAR*)aligned_p;
		b = (BYTE*)aligned_b;
	}

	while (nCount > 0)
	{
		WCHAR szBuf[2];
		long nHashByte;

		memcpy(szBuf, p, sizeof(szBuf));
		nHashByte = AToHexTable[szBuf[1]];
		nHashByte += AToHexTable[szBuf[0]] << 4;

		*b++ = (BYTE)nHashByte;
		p += 2;
		nCount -= 2;
	}
	return pOutBinary;
}

char* CharToCopy(const char* cpInChar)
{
	return CharToCopyToPadding(cpInChar, 0);
}

char* CharToCopyToPadding(const char* cpInChar, size_t nPaddingSize)
{
	char* cpBufChar;
	size_t iBufferSize;

	IF_UNLIKELY(cpInChar == NULL) {
		return NULL;
	}
	iBufferSize = strlen(cpInChar);
	cpBufChar = (char*)malloc((iBufferSize + nPaddingSize + 1));
	IF_UNLIKELY(cpBufChar == NULL) {
		return NULL;
	}
	qstrcpy(cpBufChar, cpInChar);
	return cpBufChar;
}

char* CharToCopy2(const char* cpInChar, const char* cpCharArg1)
{
	return CharToCopy2ToPadding(cpInChar,cpCharArg1, 0);
}

char* CharToCopy2ToPadding(const char* cpInChar, const char* cpCharArg1, size_t nPaddingSize)
{
	char* cpBufChar;
	char* pTmp;
	size_t iBufferSize;

	iBufferSize = strlen(cpInChar);
	if (cpCharArg1 == NULL) {
		return CharToCopyToPadding(cpInChar, nPaddingSize);
	}
	iBufferSize += strlen(cpCharArg1);
	cpBufChar = (char*)malloc((iBufferSize + nPaddingSize + 1));
	IF_UNLIKELY(cpBufChar == NULL) {
		return NULL;
	}
	pTmp = qstrcpy(cpBufChar, cpInChar);
	qstrcpy(pTmp, cpCharArg1);
	return cpBufChar;
}

char* CharToCopy3(const char* cpInChar, const char* cpCharArg1, const char* cpCharArg2)
{
	return CharToCopy3ToPadding(cpInChar, cpCharArg1, cpCharArg2, 0);
}

char* CharToCopy3ToPadding(const char* cpInChar, const char* cpCharArg1, const char* cpCharArg2, size_t nPaddingSize)
{
	char* cpBufChar;
	char* pTmp;
	size_t iBufferSize;

	iBufferSize = strlen(cpInChar);
	if (cpCharArg1 == NULL) {
		return CharToCopyToPadding(cpInChar, nPaddingSize);
	}
	iBufferSize += strlen(cpCharArg1);
	if (cpCharArg2 == NULL) {
		return CharToCopy2ToPadding(cpInChar, cpCharArg1, nPaddingSize);
	}
	iBufferSize += strlen(cpCharArg2);
	cpBufChar = (char*)malloc((iBufferSize + nPaddingSize + 1));
	IF_UNLIKELY(cpBufChar == NULL) {
		return NULL;
	}
	pTmp = qstrcpy(cpBufChar, cpInChar);
	pTmp = qstrcpy(pTmp, cpCharArg1);
	qstrcpy(pTmp, cpCharArg2);
	return cpBufChar;
}

char* CharToCopy4(const char* cpInChar, const char* cpCharArg1, const char* cpCharArg2, const char* cpCharArg3)
{
	char* cpBufChar;
	char* pTmp;
	size_t iBufferSize;

	iBufferSize = strlen(cpInChar);
	if (cpCharArg1 == NULL) {
		return CharToCopy(cpInChar);
	}
	iBufferSize += strlen(cpCharArg1);
	if (cpCharArg2 == NULL) {
		return CharToCopy2(cpInChar, cpCharArg1);
	}
	iBufferSize += strlen(cpCharArg2);
	if (cpCharArg3 == NULL) {
		return CharToCopy3(cpInChar, cpCharArg1, cpCharArg2);
	}
	iBufferSize += strlen(cpCharArg3);
	cpBufChar = (char*)malloc((iBufferSize + 1));
	IF_UNLIKELY(cpBufChar == NULL) {
		return NULL;
	}
	pTmp = qstrcpy(cpBufChar, cpInChar);
	pTmp = qstrcpy(pTmp, cpCharArg1);
	pTmp = qstrcpy(pTmp, cpCharArg2);
	qstrcpy(pTmp, cpCharArg3);
	return cpBufChar;
}

char* CharToCopy5(const char* cpInChar, const char* cpCharArg1, const char* cpCharArg2, const char* cpCharArg3, const char* cpCharArg4)
{
	char* cpBufChar;
	char* pTmp;
	size_t iBufferSize;

	iBufferSize = strlen(cpInChar);
	if (cpCharArg1 == NULL) {
		return CharToCopy(cpInChar);
	}
	iBufferSize += strlen(cpCharArg1);
	if (cpCharArg2 == NULL) {
		return CharToCopy2(cpInChar, cpCharArg1);
	}
	iBufferSize += strlen(cpCharArg2);
	if (cpCharArg3 == NULL) {
		return CharToCopy3(cpInChar, cpCharArg1, cpCharArg2);
	}
	iBufferSize += strlen(cpCharArg3);
	if (cpCharArg4 == NULL) {
		return CharToCopy4(cpInChar, cpCharArg1, cpCharArg2, cpCharArg3);
	}
	iBufferSize += strlen(cpCharArg4);
	cpBufChar = (char*)malloc((iBufferSize + 1));
	IF_UNLIKELY(cpBufChar == NULL) {
		return NULL;
	}
	pTmp = qstrcpy(cpBufChar, cpInChar);
	pTmp = qstrcpy(pTmp, cpCharArg1);
	pTmp = qstrcpy(pTmp, cpCharArg2);
	pTmp = qstrcpy(pTmp, cpCharArg3);
	qstrcpy(pTmp, cpCharArg4);
	return cpBufChar;
}

char* CharToCopy6(const char* cpInChar, const char* cpCharArg1, const char* cpCharArg2, const char* cpCharArg3, const char* cpCharArg4, const char* cpCharArg5)
{
	char* cpBufChar;
	char* pTmp;
	size_t iBufferSize;

	iBufferSize = strlen(cpInChar);
	if (cpCharArg1 == NULL) {
		return CharToCopy(cpInChar);
	}
	iBufferSize += strlen(cpCharArg1);
	if (cpCharArg2 == NULL) {
		return CharToCopy2(cpInChar, cpCharArg1);
	}
	iBufferSize += strlen(cpCharArg2);
	if (cpCharArg3 == NULL) {
		return CharToCopy3(cpInChar, cpCharArg1, cpCharArg2);
	}
	iBufferSize += strlen(cpCharArg3);
	if (cpCharArg4 == NULL) {
		return CharToCopy4(cpInChar, cpCharArg1, cpCharArg2, cpCharArg3);
	}
	iBufferSize += strlen(cpCharArg4);
	if (cpCharArg5 == NULL) {
		return CharToCopy5(cpInChar, cpCharArg1, cpCharArg2, cpCharArg3, cpCharArg4);
	}
	iBufferSize += strlen(cpCharArg5);
	cpBufChar = (char*)malloc((iBufferSize + 1));
	IF_UNLIKELY(cpBufChar == NULL) {
		return NULL;
	}
	pTmp = qstrcpy(cpBufChar, cpInChar);
	pTmp = qstrcpy(pTmp, cpCharArg1);
	pTmp = qstrcpy(pTmp, cpCharArg2);
	pTmp = qstrcpy(pTmp, cpCharArg3);
	pTmp = qstrcpy(pTmp, cpCharArg4);
	qstrcpy(pTmp, cpCharArg5);
	return cpBufChar;
}


WCHAR* WCharToCopy(const WCHAR* cpInWChar)
{
	return WCharToCopyToPadding(cpInWChar, 0);
}

WCHAR* WCharToCopyToPadding(const WCHAR* cpInWChar, size_t nPaddingSize)
{
	WCHAR* cpBufWChar;
	size_t iBufferSize;

	iBufferSize = wcslen(cpInWChar);
	cpBufWChar = (WCHAR*)malloc((iBufferSize + nPaddingSize + 1) * sizeof(WCHAR));
	IF_UNLIKELY(cpBufWChar == NULL) {
		return NULL;
	}
	qwcscpy(cpBufWChar, cpInWChar);
	return cpBufWChar;
}

WCHAR* WCharToCopy2(const WCHAR* cpInWChar, const WCHAR* cpWCharArg1)
{
	return WCharToCopy2ToPadding(cpInWChar, cpWCharArg1, 0);
}

WCHAR* WCharToCopy2ToPadding(const WCHAR* cpInWChar, const WCHAR* cpWCharArg1, size_t nPaddingSize)
{
	WCHAR* cpBufWChar;
	WCHAR* pTmp;
	size_t iBufferSize;

	iBufferSize = wcslen(cpInWChar);
	if (cpWCharArg1 == NULL) {
		return WCharToCopyToPadding(cpInWChar, nPaddingSize);
	}
	iBufferSize += wcslen(cpWCharArg1);
	cpBufWChar = (WCHAR*)malloc((iBufferSize + nPaddingSize + 1) * sizeof(WCHAR));
	IF_UNLIKELY(cpBufWChar == NULL) {
		return NULL;
	}
	pTmp = qwcscpy(cpBufWChar, cpInWChar);
	qwcscpy(pTmp, cpWCharArg1);
	return cpBufWChar;
}

WCHAR* WCharToCopy3(const WCHAR* cpInWChar, const WCHAR* cpWCharArg1, const WCHAR* cpWCharArg2)
{
	return WCharToCopy3ToPadding(cpInWChar, cpWCharArg1, cpWCharArg2, 0);
}

WCHAR* WCharToCopy3ToPadding(const WCHAR* cpInWChar, const WCHAR* cpWCharArg1, const WCHAR* cpWCharArg2, size_t nPaddingSize)
{
	WCHAR* cpBufWChar;
	WCHAR* pTmp;
	size_t iBufferSize;

	iBufferSize = wcslen(cpInWChar);
	if (cpWCharArg1 == NULL) {
		return WCharToCopyToPadding(cpInWChar, nPaddingSize);
	}
	iBufferSize += wcslen(cpWCharArg1);
	if (cpWCharArg2 == NULL) {
		return WCharToCopy2ToPadding(cpInWChar, cpWCharArg1, nPaddingSize);
	}
	iBufferSize += wcslen(cpWCharArg2);
	cpBufWChar = (WCHAR*)malloc((iBufferSize + nPaddingSize + 1) * sizeof(WCHAR));
	IF_UNLIKELY(cpBufWChar == NULL) {
		return NULL;
	}
	pTmp = qwcscpy(cpBufWChar, cpInWChar);
	pTmp = qwcscpy(pTmp, cpWCharArg1);
	qwcscpy(pTmp, cpWCharArg2);
	return cpBufWChar;
}

WCHAR* WCharToCopy4(const WCHAR* cpInWChar, const WCHAR* cpWCharArg1, const WCHAR* cpWCharArg2, const WCHAR* cpWCharArg3)
{
	WCHAR* cpBufWChar;
	WCHAR* pTmp;
	size_t iBufferSize;

	iBufferSize = wcslen(cpInWChar);
	if (cpWCharArg1 == NULL) {
		return WCharToCopy(cpInWChar);
	}
	iBufferSize += wcslen(cpWCharArg1);
	if (cpWCharArg2 == NULL) {
		return WCharToCopy2(cpInWChar, cpWCharArg1);
	}
	iBufferSize += wcslen(cpWCharArg2);
	if (cpWCharArg3 == NULL) {
		return WCharToCopy3(cpInWChar, cpWCharArg1, cpWCharArg2);
	}
	iBufferSize += wcslen(cpWCharArg3);
	cpBufWChar = (WCHAR*)malloc((iBufferSize + 1) * sizeof(WCHAR));
	IF_UNLIKELY(cpBufWChar == NULL) {
		return NULL;
	}
	pTmp = qwcscpy(cpBufWChar, cpInWChar);
	pTmp = qwcscpy(pTmp, cpWCharArg1);
	pTmp = qwcscpy(pTmp, cpWCharArg2);
	qwcscpy(pTmp, cpWCharArg3);
	return cpBufWChar;
}

WCHAR* WCharToCopy5(const WCHAR* cpInWChar, const WCHAR* cpWCharArg1, const WCHAR* cpWCharArg2, const WCHAR* cpWCharArg3, const WCHAR* cpWCharArg4)
{
	WCHAR* cpBufWChar;
	WCHAR* pTmp;
	size_t iBufferSize;

	iBufferSize = wcslen(cpInWChar);
	if (cpWCharArg1 == NULL) {
		return WCharToCopy(cpInWChar);
	}
	iBufferSize += wcslen(cpWCharArg1);
	if (cpWCharArg2 == NULL) {
		return WCharToCopy2(cpInWChar, cpWCharArg1);
	}
	iBufferSize += wcslen(cpWCharArg2);
	if (cpWCharArg3 == NULL) {
		return WCharToCopy3(cpInWChar, cpWCharArg1, cpWCharArg2);
	}
	iBufferSize += wcslen(cpWCharArg3);
	if (cpWCharArg4 == NULL) {
		return WCharToCopy4(cpInWChar, cpWCharArg1, cpWCharArg2, cpWCharArg3);
	}
	iBufferSize += wcslen(cpWCharArg4);
	cpBufWChar = (WCHAR*)malloc((iBufferSize + 1) * sizeof(WCHAR));
	IF_UNLIKELY(cpBufWChar == NULL) {
		return NULL;
	}
	pTmp = qwcscpy(cpBufWChar, cpInWChar);
	pTmp = qwcscpy(pTmp, cpWCharArg1);
	pTmp = qwcscpy(pTmp, cpWCharArg2);
	pTmp = qwcscpy(pTmp, cpWCharArg3);
	qwcscpy(pTmp, cpWCharArg4);
	return cpBufWChar;
}

WCHAR* WCharToCopy6(const WCHAR* cpInWChar, const WCHAR* cpWCharArg1, const WCHAR* cpWCharArg2, const WCHAR* cpWCharArg3, const WCHAR* cpWCharArg4, const WCHAR* cpWCharArg5)
{
	WCHAR* cpBufWChar;
	WCHAR* pTmp;
	size_t iBufferSize;

	iBufferSize = wcslen(cpInWChar);
	if (cpWCharArg1 == NULL) {
		return WCharToCopy(cpInWChar);
	}
	iBufferSize += wcslen(cpWCharArg1);
	if (cpWCharArg2 == NULL) {
		return WCharToCopy2(cpInWChar, cpWCharArg1);
	}
	iBufferSize += wcslen(cpWCharArg2);
	if (cpWCharArg3 == NULL) {
		return WCharToCopy3(cpInWChar, cpWCharArg1, cpWCharArg2);
	}
	iBufferSize += wcslen(cpWCharArg3);
	if (cpWCharArg4 == NULL) {
		return WCharToCopy4(cpInWChar, cpWCharArg1, cpWCharArg2, cpWCharArg3);
	}
	iBufferSize += wcslen(cpWCharArg4);
	if (cpWCharArg5 == NULL) {
		return WCharToCopy5(cpInWChar, cpWCharArg1, cpWCharArg2, cpWCharArg3, cpWCharArg4);
	}
	iBufferSize += wcslen(cpWCharArg5);
	cpBufWChar = (WCHAR*)malloc((iBufferSize + 1) * sizeof(WCHAR));
	IF_UNLIKELY(cpBufWChar == NULL) {
		return NULL;
	}
	pTmp = qwcscpy(cpBufWChar, cpInWChar);
	pTmp = qwcscpy(pTmp, cpWCharArg1);
	pTmp = qwcscpy(pTmp, cpWCharArg2);
	pTmp = qwcscpy(pTmp, cpWCharArg3);
	pTmp = qwcscpy(pTmp, cpWCharArg4);
	qwcscpy(pTmp, cpWCharArg5);
	return cpBufWChar;
}


size_t CharToCharLength(const char* cpInChar)
{
	return strlen(cpInChar);
}

char* CharToCharConv(const char* cpInChar)
{
	return CharToCharConv2(cpInChar, NULL);
}

char* CharToCharConv2(const char* cpInChar, char* pOutChar)
{
	IF_UNLIKELY(pOutChar == NULL)
	{
		const size_t nCharSize = CharToCharLength(cpInChar);

		pOutChar = (char*)malloc((nCharSize + 1));
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}
	qstrcpy(pOutChar, cpInChar);
	return pOutChar;
}

size_t CharToWCharLength(const char* cpInChar)
{
	unsigned char* pSrc;
	size_t DstCount = 0;

	pSrc = (unsigned char*)cpInChar;
	while (*pSrc)
	{
		if (*pSrc >= 0x80)
		{
			DstCount += sizeof(WCHAR);
			++pSrc;
		}
		else {
			DstCount += sizeof(WCHAR);
		}
		++pSrc;
	}
	return DstCount;
}

WCHAR* CharToWCharConv(const char* cpInChar)
{
	return CharToWCharConv2(cpInChar, NULL);
}

WCHAR* CharToWCharConv2(const char* cpInChar, WCHAR* pOutWChar)
{
	WCHAR* pDst;
	unsigned char* pSrc;

	IF_UNLIKELY(pOutWChar == NULL)
	{
		const size_t nWCharSize = CharToWCharLength(cpInChar);
		pOutWChar = (WCHAR*)malloc((nWCharSize + 1) * sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}

	pSrc = (unsigned char*)cpInChar;
	pDst = pOutWChar;

	while (*pSrc)
	{
		const int nRet = qmbtowc(pDst, (char*)pSrc);
		pDst++;
		pSrc += nRet;
	}
	*pDst = '\0';
	return pOutWChar;
}

int qmbtowc(wchar_t* pDst, const char* pSrc)
{
	wchar_t w;
	const int nRet = mbtowc(&w, pSrc, MB_CUR_MAX);

	IF_UNLIKELY(nRet > 0) {
		*pDst = w;
	}
	else
	{
		*pDst = '_';
		return sizeof(wchar_t);
	}
	return nRet;
}

int qwctomb(char* pDst, const wchar_t* pSrc)
{
	char c[8] = { 0 };
	char* ptr = c;
	int nRet;

#ifndef _NODLL
	const errno_t err = wctomb_s(&nRet, c, MB_CUR_MAX, *pSrc);

	IF_UNLIKELY(err) {
		nRet = 0;
	}
#else
	nRet = wctomb(c, *pSrc);
#endif

	switch (nRet)
	{
	case 4:
		*pDst++ = *ptr++;
		SWITCH_FALLTHROUGH
	case 3:
		*pDst++ = *ptr++;
		SWITCH_FALLTHROUGH
	case 2:
		*pDst++ = *ptr++;
		SWITCH_FALLTHROUGH
	case 1:
		*pDst++ = *ptr++;
		SWITCH_FALLTHROUGH
	default:
		nRet = 1;
		*pDst = '_';
	}
	return nRet;
}

size_t WCharToCharLength(const WCHAR* cpInWChar)
{
	WCHAR* pSrc;
	size_t DstCount = 0;

	pSrc = (WCHAR*)cpInWChar;
	while (*pSrc)
	{
		if (*pSrc >= 0x80) {
			DstCount += 2;
		}
		else {
			++DstCount;
		}
		++pSrc;
	}
	return DstCount;
}

char* WCharToCharConv(const WCHAR* cpInWChar)
{
	return WCharToCharConv2(cpInWChar, NULL);
}

char* WCharToCharConv2(const WCHAR* cpInWChar, char* pOutChar)
{
	unsigned char* pDst;
	WCHAR* pSrc;

	IF_UNLIKELY(pOutChar == NULL)
	{
		const size_t nCharSize = WCharToCharLength(cpInWChar);
		pOutChar = (char*)malloc(nCharSize + 1);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}

	pSrc = (WCHAR*)cpInWChar;
	pDst = (unsigned char*)pOutChar;

	while (*pSrc)
	{
		if (*pSrc >= 0x80)
		{
			const int nRet = qwctomb((char*)pDst, pSrc);
			pDst += nRet;

			if (((*pSrc) & ~((1 << 10) - 1)) == 0xD800) {
				++pSrc;
			}
		}
		else {
			*pDst++ = (unsigned char)*pSrc;
		}
		++pSrc;
	}
	*pDst = '\0';
	return pOutChar;
}

size_t WCharToWCharLength(const WCHAR* cpInWChar)
{
	return wcslen(cpInWChar);
}


WCHAR* WCharToWCharConv(const WCHAR* cpInWChar)
{
	return WCharToWCharConv2(cpInWChar, NULL);
}

WCHAR* WCharToWCharConv2(const WCHAR* cpInWChar, WCHAR* pOutWChar)
{
	IF_UNLIKELY(pOutWChar == NULL)
	{
		const size_t nCharSize = WCharToWCharLength(cpInWChar);
		pOutWChar = (WCHAR*)malloc((nCharSize + 1) * sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}
	qwcscpy(pOutWChar, cpInWChar);
	return pOutWChar;
}

size_t CharLength(const char* cpInChar)
{
	unsigned char* pSrc;
	size_t DstCount = 0;

	pSrc = (unsigned char*)cpInChar;
	while (*pSrc)
	{
		if (*pSrc >= 0x80)
		{
			++pSrc;
			DstCount += 2;
		}
		else {
			++DstCount;
		}
		++pSrc;
	}
	return DstCount;
}

size_t CharToUtf8CharLength(const char* cpInChar)
{
	unsigned char* pSrc;
	size_t DstCount = 0;

	pSrc = (unsigned char*)cpInChar;
	while (*pSrc)
	{
		if (*pSrc >= 0x80)
		{
			DstCount += 3;
			++pSrc;
		}
		else {
			++DstCount;
		}
		++pSrc;
	}
	return DstCount;
}


char* CharToUtf8CharConv(const char* cpInChar)
{
	return CharToUtf8CharConv2(cpInChar, NULL);
}

char* CharToUtf8CharConv2(const char* cpInChar, char* pOutUtf8Char)
{
	unsigned char* pDst;
	unsigned char* pSrc;

	IF_UNLIKELY(pOutUtf8Char == NULL)
	{
		const size_t nUtf8CharSize = CharToUtf8CharLength(cpInChar);
		pOutUtf8Char = (char*)malloc(nUtf8CharSize + 1);
		IF_UNLIKELY(pOutUtf8Char == NULL) {
			return NULL;
		}
	}

	pSrc = (unsigned char*)cpInChar;
	pDst = (unsigned char*)pOutUtf8Char;

	while (*pSrc)
	{
		if (*pSrc >= 0x80)
		{
			WCHAR wc;
			const int nRet = mbtowc(&wc, (char*)pSrc, MB_CUR_MAX);

			IF_UNLIKELY(!nRet) {
				wc = '_';
			}

			if (wc <= 0x07FF)
			{
				*pDst++ = (unsigned char)(((wc >> 6) & 0x1F) + 0xC0);
				*pDst++ = (unsigned char)(((wc) & 0x3F) + 0x80);
			}
			else
			{
				*pDst++ = (unsigned char)(((wc >> 12) & 0x0F) + 0xE0);
				*pDst++ = (unsigned char)(((wc >> 6) & 0x3F) + 0x80);
				*pDst++ = (unsigned char)(((wc) & 0x3F) + 0x80);
			}
			++pSrc;
		}
		else {
			*pDst++ = *pSrc;
		}
		++pSrc;
	}
	*pDst = '\0';
	return pOutUtf8Char;
}

size_t Utf8CharToCharLength(const char* cpInUtf8Char)
{
	unsigned char* pSrc;
	size_t DstCount = 0;

	pSrc = (unsigned char*)cpInUtf8Char;
	while (*pSrc)
	{
		if ((*pSrc & 0x80) == 0x00) {	// 1byte
			++DstCount;
		}
		else if ((*pSrc & 0xF0) == 0xE0) // 3byte
		{
			if (*pSrc == 0xE0 && *(pSrc + 1) == 0x80)
			{
				pSrc += 3;
				continue;
			}
			DstCount += 2;
			pSrc += 2;
		}
		else if ((*pSrc & 0xF8) == 0xF0) // 4byte
		{
			++DstCount;
			pSrc += 3;
		}
		else if ((*pSrc & 0xE0) == 0xC0) // 2byte
		{
			if (*pSrc <= 0xC1)
			{
				pSrc += 2;
				continue;
			}
			DstCount += 2;
			++pSrc;
		}
		else if ((*pSrc & 0xFC) == 0xF8) { // 5byte -> error
			pSrc += 4;
		}
		else if ((*pSrc & 0xFE) == 0xFC) { // 6byte -> error
			pSrc += 5;
		}
		++pSrc;
	}
	return DstCount;
}


char* Utf8CharToCharConv(const char* cpInUtf8Char)
{
	return Utf8CharToCharConv2(cpInUtf8Char, NULL);
}

char* Utf8CharToCharConv2(const char* cpInUtf8Char, char* pOutChar)
{
	unsigned char* pDst;
	unsigned char* pSrc;

	IF_UNLIKELY(pOutChar == NULL)
	{
		const size_t nCharSize = Utf8CharToCharLength(cpInUtf8Char);
		pOutChar = (char*)malloc(nCharSize + 1);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}

	pSrc = (unsigned char*)cpInUtf8Char;
	pDst = (unsigned char*)pOutChar;

	while (*pSrc)
	{
		WCHAR wc = 0;

		if ((*pSrc & 0x80) == 0x00) // 1byte
		{
			*pDst++ = *pSrc++;
			continue;
		}
		else if ((*pSrc & 0xF0) == 0xE0) // 3byte
		{
			if (*pSrc == 0xE0 && *(pSrc + 1) == 0x80)
			{
				pSrc += 3;
				continue;
			}
			wc = (*pSrc & 0x0F) << 12;
			++pSrc;
			wc += (*pSrc & 0x3F) << 6;
			++pSrc;
			wc += (*pSrc & 0x3F);
		}
		else if ((*pSrc & 0xF8) == 0xF0) // 4byte
		{
			*pDst++ = '_';
			pSrc += 3;
		}
		else if ((*pSrc & 0xE0) == 0xC0) // 2byte
		{
			if (*pSrc <= 0xC1)
			{
				pSrc += 2;
				continue;
			}
			wc = (*pSrc & 0x1F) << 6;
			++pSrc;
			wc += (*pSrc & 0x3F);
		}
		else if ((*pSrc & 0xFC) == 0xF8) {// 5byte -> error
			pSrc += 4;
		}
		else if ((*pSrc & 0xFE) == 0xFC) {// 6byte -> error
			pSrc += 5;
		}

		if (wc > 0)
		{
			const int nRet = qwctomb((char*)pDst, &wc);
			pDst += nRet;
		}
		++pSrc;
	}
	*pDst = '\0';
	return (char*)pOutChar;
}

size_t Utf8CharToWCharLength(const char* cpInUtf8Char)
{
	unsigned char* pSrc;
	size_t DstCount = 0;

	pSrc = (unsigned char*)cpInUtf8Char;
	while (*pSrc)
	{
		if ((*pSrc & 0x80) == 0x00) {	// 1byte
			DstCount += 2;
		}
		else if ((*pSrc & 0xF0) == 0xE0) // 3byte
		{
			if (*pSrc == 0xE0 && *(pSrc + 1) == 0x80)
			{
				pSrc += 3;
				continue;
			}
			DstCount += 2;
			pSrc += 2;
		}
		else if ((*pSrc & 0xF8) == 0xF0) // 4byte
		{
			if (*pSrc == 0xF0 && *(pSrc + 1) == 0x80 && *(pSrc + 2) == 0x80)
			{
				pSrc += 4;
				continue;
			}
			DstCount += 4;
			pSrc += 3;
		}
		else if ((*pSrc & 0xE0) == 0xC0) // 2byte
		{
			if (*pSrc <= 0xC1)
			{
				pSrc += 2;
				continue;
			}
			DstCount += 2;
			++pSrc;
		}
		else if ((*pSrc & 0xFC) == 0xF8) { // 5byte -> error
			pSrc += 4;
		}
		else if ((*pSrc & 0xFE) == 0xFC) { // 6byte -> error
			pSrc += 5;
		}
		++pSrc;
	}
	return DstCount;
}


WCHAR* Utf8CharToWCharConv(const char* cpInUtf8Char)
{
	return Utf8CharToWCharConv2(cpInUtf8Char, NULL);
}

WCHAR* Utf8CharToWCharConv2(const char* cpInUtf8Char, WCHAR* pOutWChar)
{
	WCHAR* pDst;
	unsigned char* pSrc;

	IF_UNLIKELY(pOutWChar == NULL)
	{
		const size_t nWCharSize = Utf8CharToWCharLength(cpInUtf8Char);
		pOutWChar = (WCHAR*)malloc(nWCharSize + sizeof(WCHAR));
		IF_UNLIKELY(pOutWChar == NULL) {
			return NULL;
		}
	}

	pSrc = (unsigned char*)cpInUtf8Char;
	pDst = pOutWChar;

	while (*pSrc)
	{
		if ((*pSrc & 0x80) == 0x00) {	// 1byte
			*pDst++ = *pSrc;
		}
		else if ((*pSrc & 0xF0) == 0xE0) // 3byte
		{
			if (*pSrc == 0xE0 && *(pSrc + 1) == 0x80)
			{
				pSrc += 3;
				continue;
			}
			*pDst = (*pSrc & 0x0F) << 12;
			++pSrc;
			*pDst += (*pSrc & 0x3F) << 6;
			++pSrc;
			*pDst += (*pSrc & 0x3F);
			++pDst;
		}
		else if ((*pSrc & 0xF8) == 0xF0) // 4byte
		{
			unsigned int surrogate;

			if (*pSrc == 0xF0 && *(pSrc + 1) == 0x80 && *(pSrc + 2) == 0x80)
			{
				pSrc += 4;
				continue;
			}

			surrogate = ((unsigned int)(*pSrc) & 0x07) << 18;
			++pSrc;
			surrogate += (*pSrc & 0x3F) << 12;
			++pSrc;
			surrogate += (*pSrc & 0x3F) << 6;
			++pSrc;
			surrogate += (*pSrc & 0x3F);
			surrogate -= 0x10000;

			*pDst++ = (WCHAR)((surrogate >> 10) + 0xD800);
			*pDst++ = (WCHAR)((surrogate & 0x03FF) + 0xDC00);
		}
		else if ((*pSrc & 0xE0) == 0xC0) // 2byte
		{
			if (*pSrc <= 0xC1)
			{
				pSrc += 2;
				continue;
			}
			*pDst = (*pSrc & 0x1F) << 6;
			++pSrc;
			*pDst += (*pSrc & 0x3F);
			++pDst;
		}
		else if ((*pSrc & 0xFC) == 0xF8) { // 5byte -> error
			pSrc += 4;
		}
		else if ((*pSrc & 0xFE) == 0xFC) { // 6byte -> error
			pSrc += 5;
		}
		++pSrc;
	}
	*pDst = '\0';
	return pOutWChar;
}

size_t WCharToUtf8CharLength(const WCHAR* cpInWChar)
{
	WCHAR* pSrc;
	size_t DstCount = 0;

	pSrc = (WCHAR*)cpInWChar;
	while (*pSrc)
	{
		if (*pSrc <= 0x007F) {
			++DstCount;
		}
		else if (*pSrc <= 0x07FF) {
			DstCount += 2;
		}
		else
		{
			if (((*pSrc) & ~((1 << 10) - 1)) == 0xD800)
			{
				++pSrc;
				if (((*pSrc) & ~((1 << 10) - 1)) == 0xDC00) {
					DstCount += 4;
				}
			}
			else if (!(((*pSrc) & ~((1 << 10) - 1)) == 0xDC00)) {
				DstCount += 3;
			}
		}
		++pSrc;
	}
	return DstCount;
}


char* WCharToUtf8CharConv(const WCHAR* cpInWChar)
{
	return WCharToUtf8CharConv2(cpInWChar, NULL);
}

char* WCharToUtf8CharConv2(const WCHAR* cpInWChar, char* pOutUtf8Char)
{
	unsigned char* pDst;
	WCHAR* pSrc;

	IF_UNLIKELY(pOutUtf8Char == NULL)
	{
		const size_t nOutUtf8CharSize = WCharToUtf8CharLength(cpInWChar);
		pOutUtf8Char = (char*)malloc(nOutUtf8CharSize + 1);
		IF_UNLIKELY(pOutUtf8Char == NULL) {
			return NULL;
		}
	}

	pSrc = (WCHAR*)cpInWChar;
	pDst = (unsigned char*)pOutUtf8Char;

	while (*pSrc)
	{
		if (*pSrc <= 0x007F) {
			*pDst++ = (unsigned char)(*pSrc & 0x7F);
		}
		else if (*pSrc <= 0x07FF)
		{
			*pDst++ = (unsigned char)(((*pSrc >> 6) & 0x1F) + 0xC0);
			*pDst++ = (unsigned char)(((*pSrc) & 0x3F) + 0x80);
		}
		else if (*pSrc <= 0xD7FF)
		{
			*pDst++ = (unsigned char)(((*pSrc >> 12) & 0x0F) + 0xE0);
			*pDst++ = (unsigned char)(((*pSrc >> 6) & 0x3F) + 0x80);
			*pDst++ = (unsigned char)(((*pSrc) & 0x3F) + 0x80);
		}
		else
		{
			if (((*pSrc) & ~((1 << 10) - 1)) == 0xD800)
			{
				unsigned int surrogate = ((unsigned int)(*pSrc) - 0xD800) << 10;

				++pSrc;
				if (((*pSrc) & ~((1 << 10) - 1)) == 0xDC00)
				{
					surrogate += (*pSrc - 0xDC00);
					surrogate += 0x10000;

					*pDst++ = (unsigned char)(((surrogate >> 18) & 0x07) + 0xF0);
					*pDst++ = (unsigned char)(((surrogate >> 12) & 0x3F) + 0x80);
					*pDst++ = (unsigned char)(((surrogate >> 6) & 0x3F) + 0x80);
					*pDst++ = (unsigned char)(((surrogate) & 0x3F) + 0x80);
				}

			}
			else if (!(((*pSrc) & ~((1 << 10) - 1)) == 0xDC00))
			{
				*pDst++ = (unsigned char)(((*pSrc >> 12) & 0x0F) + 0xE0);
				*pDst++ = (unsigned char)(((*pSrc >> 6) & 0x3F) + 0x80);
				*pDst++ = (unsigned char)(((*pSrc) & 0x3F) + 0x80);
			}
		}
		++pSrc;
	}
	*pDst = '\0';
	return pOutUtf8Char;
}


char* GetSystemDirectoryFileNameToCharA(const char* cpFileName)
{
	UINT nRet = FALSE;
	UINT nLen;
	char* pPath;
	char* pTmp;

	nLen = GetSystemDirectoryA(NULL, 0);
	IF_UNLIKELY(!nLen) {
		return FALSE;
	}
	nLen += (UINT)strlen(cpFileName) + 1;
	pPath = (char*)malloc(nLen * sizeof(char));
	IF_UNLIKELY(pPath == NULL) {
		return FALSE;
	}
	nRet = GetSystemDirectoryA(pPath, nLen);
	IF_UNLIKELY(!nRet)
	{
		free(pPath);
		return NULL;
	}
	pTmp = pPath + nRet;
	*pTmp++ = '\\';
	qstrcpy(pTmp, cpFileName);
	return pPath;
}

WCHAR* GetSystemDirectoryFileNameToWCharW(const WCHAR* cpFileName)
{
	UINT nRet = FALSE;
	UINT nLen;
	WCHAR* pPath;
	WCHAR* pTmp;

	nLen = GetSystemDirectoryW(NULL, 0);
	IF_UNLIKELY(!nLen) {
		return FALSE;
	}
	nLen += (UINT)wcslen(cpFileName) + 1;
	pPath = (WCHAR*)malloc(nLen * sizeof(WCHAR));
	IF_UNLIKELY(pPath == NULL) {
		return FALSE;
	}
	nRet = GetSystemDirectoryW(pPath, nLen);
	IF_UNLIKELY(!nRet)
	{
		free(pPath);
		return NULL;
	}
	pTmp = pPath + nRet;
	*pTmp++ = '\\';
	qwcscpy(pTmp, cpFileName);
	return pPath;
}

char* GetSystemDirectoryFileNameToCharW(const WCHAR* cpFileName)
{
	char* pBufFileName;
	char* pBufChar;

	pBufFileName = WCharToCharConv(cpFileName);
	IF_UNLIKELY(pBufFileName == NULL) {
		return NULL;
	}
	pBufChar = GetSystemDirectoryFileNameToCharA(pBufFileName);
	CharToFree(pBufFileName);
	return pBufChar;
}

WCHAR* GetSystemDirectoryFileNameToWCharA(const char* cpFileName)
{
	WCHAR* pBufFileName;
	WCHAR* pBufWChar;

	pBufFileName = CharToWCharConv(cpFileName);
	IF_UNLIKELY(pBufFileName == NULL) {
		return NULL;
	}
	pBufWChar = GetSystemDirectoryFileNameToWCharW(pBufFileName);
	WCharToFree(pBufFileName);
	return pBufWChar;
}

void CharToFree(char* pInChar)
{
	free(pInChar);
	pInChar = NULL;
}

void WCharToFree(WCHAR* pInWChar)
{
	free(pInWChar);
	pInWChar = NULL;
}

WCHAR* RemoveUnicodeChar(const WCHAR* cpInWChar)
{
	WCHAR* pBufWChar;
	char* pBufChar;

	pBufChar = WCharToCharConv(cpInWChar);
	IF_UNLIKELY(pBufChar == NULL) {
		return NULL;
	}
	pBufWChar = CharToWCharConv(pBufChar);
	CharToFree(pBufChar);
	return pBufWChar;
}

char* CharToUpper(char* pInChar)
{
	char* pDst = pInChar;
	size_t* aligned_dst;

	if (!UNALIGNED1(pDst))
	{
		aligned_dst = (size_t*)pDst;

		while (!DETECTNULL(*aligned_dst))
		{
			size_t aligned_src = *aligned_dst;
			size_t aligned_val;

			aligned_val = toupper((int)(aligned_src & 0xFF));
			aligned_val += (size_t)toupper((int)((aligned_src >> 8) & 0xFF)) << 8;
			aligned_val += (size_t)toupper((int)((aligned_src >> 16) & 0xFF)) << 16;
			aligned_val += (size_t)toupper((int)((aligned_src >> 24) & 0xFF)) << 24;

#if SIZE_MAX == 0xffffffffffffffffui64
			aligned_val += (size_t)toupper((int)((aligned_src >> 32) & 0xFF)) << 32;
			aligned_val += (size_t)toupper((int)((aligned_src >> 40) & 0xFF)) << 40;
			aligned_val += (size_t)toupper((int)((aligned_src >> 48) & 0xFF)) << 48;
			aligned_val += (size_t)toupper((int)((aligned_src >> 56) & 0xFF)) << 56;
#endif

			*aligned_dst++ = aligned_val;
		}

		pDst = (char*)aligned_dst;
	}

	while ((*pDst++ = (char)toupper(*(unsigned char*)pDst))) {
		;
	}
	return pInChar;
}

char* CharToLower(char* pInChar)
{
	char* pDst = pInChar;
	size_t* aligned_dst;

	if (!UNALIGNED1(pDst))
	{
		aligned_dst = (size_t*)pDst;

		while (!DETECTNULL(*aligned_dst))
		{
			size_t aligned_src = *aligned_dst;
			size_t aligned_val;

			aligned_val = tolower((int)(aligned_src & 0xFF));
			aligned_val += (size_t)tolower((int)((aligned_src >> 8) & 0xFF)) << 8;
			aligned_val += (size_t)tolower((int)((aligned_src >> 16) & 0xFF)) << 16;
			aligned_val += (size_t)tolower((int)((aligned_src >> 24) & 0xFF)) << 24;

#if SIZE_MAX == 0xffffffffffffffffui64
			aligned_val += (size_t)tolower((int)((aligned_src >> 32) & 0xFF)) << 32;
			aligned_val += (size_t)tolower((int)((aligned_src >> 40) & 0xFF)) << 40;
			aligned_val += (size_t)tolower((int)((aligned_src >> 48) & 0xFF)) << 48;
			aligned_val += (size_t)tolower((int)((aligned_src >> 56) & 0xFF)) << 56;
#endif

			*aligned_dst++ = aligned_val;
		}

		pDst = (char*)aligned_dst;
	}

	while ((*pDst++ = (char)tolower(*(unsigned char*)pDst))) {
		;
	}
	return pInChar;
}

char* CharSizeToUpper(char* pInChar, size_t nInSize)
{
	char* pDst = pInChar;

	while (nInSize > 0)
	{
		nInSize--;
		if (*pDst != '\0') {
			*pDst = (char)toupper(*(unsigned char*)pDst);
		}
		else {
			break;
		}
		pDst++;
	}
	return pInChar;
}

char* CharSizeToLower(char* pInChar, size_t nInSize)
{
	char* pDst = pInChar;

	while (nInSize > 0)
	{
		nInSize--;
		if (*pDst != '\0') {
			*pDst = (char)tolower(*(unsigned char*)pDst);
		}
		else {
			break;
		}
		pDst++;
	}
	return pInChar;
}

WCHAR* WCharToUpper(WCHAR* pInWChar)
{
	WCHAR* pDst = pInWChar;

	while ((*pDst++ = towupper(*pDst))) {
		;
	}
	return pInWChar;
}

WCHAR* WCharToLower(WCHAR* pInWChar)
{
	WCHAR* pDst = pInWChar;

	while ((*pDst++ = towlower(*pDst))) {
		;
	}
	return pInWChar;
}

WCHAR* WCharSizeToUpper(WCHAR* pInWChar, size_t nInSize)
{
	WCHAR* pDst = pInWChar;

	while (nInSize > 0)
	{
		nInSize--;
		if (*pDst != '\0') {
			*pDst = towupper(*pDst);
		}
		else {
			break;
		}
		pDst++;
	}
	return pInWChar;
}

WCHAR* WCharSizeToLower(WCHAR* pInWChar, size_t nInSize)
{
	WCHAR* pDst = pInWChar;

	while (nInSize > 0)
	{
		nInSize--;
		if (*pDst != '\0') {
			*pDst = towlower(*pDst);
		}
		else {
			break;
		}
		pDst++;
	}
	return pInWChar;
}

size_t CharSizeToFWrite(FILE* fp, const void* cpInChar, size_t nSize)
{
	return fwrite(cpInChar, nSize, 1, fp);
}

size_t CharToFWrite(FILE* fp, const char* cpInChar)
{
	return fwrite(cpInChar, strlen(cpInChar), 1, fp);
}

size_t CharToCharFWrite(FILE* fp, const char* cpInChar)
{
	return fwrite(cpInChar, strlen(cpInChar), 1, fp);
}

size_t CharToWCharFWrite(FILE* fp, const char* cpInChar)
{
	WCHAR* pBufWChar = NULL;
	size_t nRet = 0;

	pBufWChar = CharToWCharConv(cpInChar);
	IF_LIKELY(pBufWChar != NULL)
	{
		const unsigned char nBom[] = { 0xff, 0xfe };
		nRet = fwrite(nBom, sizeof(nBom), 1, fp);
		nRet = fwrite(pBufWChar, wcslen(pBufWChar), 1, fp);
		free(pBufWChar);
	}
	return nRet;
}

size_t CharToUtf8CharFWrite(FILE* fp, const char* cpInChar)
{
	char* pBufUtf8Char = NULL;
	size_t nRet = 0;

	pBufUtf8Char = CharToUtf8CharConv(cpInChar);
	IF_LIKELY(pBufUtf8Char != NULL)
	{
		nRet = fwrite(pBufUtf8Char, strlen(pBufUtf8Char), 1, fp);
		free(pBufUtf8Char);
	}
	return nRet;
}

size_t WCharToCharFWrite(FILE* fp, const WCHAR* cpInWChar)
{
	char* pBufChar = NULL;
	size_t nRet = 0;

	pBufChar = WCharToCharConv(cpInWChar);
	IF_LIKELY(pBufChar != NULL)
	{
		nRet = fwrite(pBufChar, strlen(pBufChar), 1, fp);
		free(pBufChar);
	}
	return nRet;
}

size_t WCharToFWrite(FILE* fp, const WCHAR* cpInWChar)
{
	return fwrite(cpInWChar, wcslen(cpInWChar), 1, fp);
}

size_t WCharToUtf8CharFWrite(FILE* fp, const WCHAR* cpInWChar)
{
	char* pBufUtf8Char = NULL;
	size_t nRet = 0;

	pBufUtf8Char = WCharToUtf8CharConv(cpInWChar);
	IF_LIKELY(pBufUtf8Char != NULL)
	{
		nRet = fwrite(pBufUtf8Char, strlen(pBufUtf8Char), 1, fp);
		free(pBufUtf8Char);
	}
	return nRet;
}

size_t Utf8CharToFWrite(FILE* fp, const char* cpInUtf8Char)
{
	char* pBufChar = NULL;
	size_t nRet = 0;

	pBufChar = Utf8CharToCharConv(cpInUtf8Char);
	IF_LIKELY(pBufChar != NULL)
	{
		nRet = fwrite(pBufChar, strlen(pBufChar), 1, fp);
		free(pBufChar);
	}
	return nRet;
}

size_t Utf8CharToWCharFWrite(FILE* fp, const char* cpInUtf8Char)
{
	WCHAR* pBufWChar = NULL;
	size_t nRet = 0;

	pBufWChar = Utf8CharToWCharConv(cpInUtf8Char);
	IF_LIKELY(pBufWChar != NULL)
	{
		const unsigned char nBom[] = { 0xff, 0xfe };
		nRet = fwrite(nBom, sizeof(nBom), 1, fp);
		nRet = fwrite(pBufWChar, wcslen(pBufWChar), 1, fp);
		free(pBufWChar);
	}
	return nRet;
}

size_t Utf8CharToUtf8CharFWrite(FILE* fp, const char* cpInUtf8Char)
{
	return fwrite(cpInUtf8Char, strlen(cpInUtf8Char), 1, fp);
}

BOOL CharSizeToWriteFile(HANDLE hFile, const void* cpInChar, DWORD nLength)
{
	DWORD dwWriteByte = 0;
	return  WriteFile(hFile, cpInChar, nLength, &dwWriteByte, NULL);
}

BOOL CharToWriteFile(HANDLE hFile, const char* cpInChar)
{
	DWORD dwWriteByte = 0;
	return WriteFile(hFile, cpInChar, (DWORD)strlen(cpInChar), &dwWriteByte, NULL);
}

BOOL CharToUtf8CharWriteFile(HANDLE hFile, const char* cpInChar)
{
	char* pBufUtf8Char = NULL;
	DWORD dwWriteByte = 0;
	BOOL nRet = 0;

	pBufUtf8Char = CharToUtf8CharConv(cpInChar);
	IF_LIKELY(pBufUtf8Char != NULL)
	{
		nRet = WriteFile(hFile, pBufUtf8Char, (DWORD)strlen(pBufUtf8Char), &dwWriteByte, NULL);
		free(pBufUtf8Char);
	}
	return nRet;
}

BOOL WCharToCharWriteFile(HANDLE hFile, const WCHAR* cpInWChar)
{
	char* pBufChar = NULL;
	DWORD dwWriteByte = 0;
	BOOL nRet = 0;

	pBufChar = WCharToCharConv(cpInWChar);
	IF_LIKELY(pBufChar != NULL)
	{
		nRet = WriteFile(hFile, pBufChar, (DWORD)strlen(pBufChar), &dwWriteByte, NULL);
		free(pBufChar);
	}
	return nRet;
}

BOOL WCharToWriteFile(HANDLE hFile, const WCHAR* cpInWChar)
{
	DWORD dwWriteByte = 0;
	return WriteFile(hFile, cpInWChar, (DWORD)wcslen(cpInWChar) * sizeof(WCHAR), &dwWriteByte, NULL);
}

BOOL Utf8CharToCharWriteFile(HANDLE hFile, const char* cpInUtf8Char)
{
	char* pBufChar = NULL;
	DWORD dwWriteByte = 0;
	BOOL nRet = 0;

	pBufChar = Utf8CharToCharConv(cpInUtf8Char);
	IF_LIKELY(pBufChar != NULL)
	{
		nRet = WriteFile(hFile, pBufChar, (DWORD)strlen(pBufChar), &dwWriteByte, NULL);
		free(pBufChar);
	}
	return nRet;
}

BOOL Utf8CharToWCharWriteFile(HANDLE hFile, const char* cpInUrf8Char)
{
	WCHAR* pBufWChar = NULL;
	DWORD dwWriteByte = 0;
	BOOL nRet = 0;

	pBufWChar = Utf8CharToWCharConv(cpInUrf8Char);
	IF_LIKELY(pBufWChar != NULL)
	{
		nRet = WriteFile(hFile, pBufWChar, (DWORD)wcslen(pBufWChar), &dwWriteByte, NULL);
		free(pBufWChar);
	}
	return nRet;
}

BOOL Utf8CharToWriteFile(HANDLE hFile, const char* cpInUtf8Char)
{
	DWORD dwWriteByte = 0;
	return WriteFile(hFile, cpInUtf8Char, (DWORD)strlen(cpInUtf8Char) * sizeof(char), &dwWriteByte, NULL);
}

BOOL CharToWCharWriteFile(HANDLE hFile, const char* cpInChar)
{
	WCHAR* pBufWChar = NULL;
	DWORD dwWriteByte = 0;
	BOOL nRet = 0;

	pBufWChar = CharToWCharConv(cpInChar);
	IF_LIKELY(pBufWChar != NULL)
	{
		nRet = WriteFile(hFile, pBufWChar, (DWORD)wcslen(pBufWChar), &dwWriteByte, NULL);
		free(pBufWChar);
	}
	return nRet;
}

BOOL WCharToUtf8CharWriteFile(HANDLE hFile, const WCHAR* cpInWChar)
{
	char* pBufUtf8Char = NULL;
	DWORD dwWriteByte = 0;
	BOOL nRet = 0;

	pBufUtf8Char = WCharToUtf8CharConv(cpInWChar);
	IF_LIKELY(pBufUtf8Char != NULL)
	{
		nRet = WriteFile(hFile, pBufUtf8Char, (DWORD)strlen(pBufUtf8Char), &dwWriteByte, NULL);
		free(pBufUtf8Char);
	}
	return nRet;
}

BOOL GetCharIsBool(const char* cpInChar)
{
	if (cpInChar != NULL)
	{
		switch (*cpInChar)
		{
		case 't':
		case 'T':
			if (_strnicmp(cpInChar, "TRUE", 4) == 0) {
				return TRUE;
			}
			break;
		default:
			if (*cpInChar != '0' &&
				!(*cpInChar == 0x82 && *(cpInChar + 1) == 0x4F))
			{
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

BOOL GetWCharIsBool(const WCHAR* cpInWChar)
{
	if (cpInWChar != NULL && *cpInWChar != '\0')
	{
		switch (*cpInWChar)
		{
		case 't':
		case 'T':
			if (_wcsnicmp(cpInWChar, L"TRUE", 4) == 0) {
				return TRUE;
			}
			break;
		default:
			if (*cpInWChar != '0' && *cpInWChar != 0xFF10) {
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

size_t CharToHashFileLine(const char* cpInChar, size_t nInSize, char* pOutFileName, unsigned char* pOutHash, size_t nOutHashSize)
{
	char* pPos = (char*)cpInChar;
	char* pHash = NULL;
	char* pFileName = NULL;
	size_t nHashSize = 0;

	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}

	if (nInSize < 8)
	{
HASHLINE_ERR:
		if (pOutFileName != NULL) {
			*pOutFileName = '\0';
		}
		memset(pOutHash, 0, nOutHashSize);
		return 0;
	}

	pPos = CharToSpaceSkip(pPos);
	if (*pPos == '\0') {
		goto HASHLINE_ERR;
	}

	if (isxdigit((unsigned char)*pPos))
	{
		size_t i;

		pHash = pPos;
		pPos++;
		nHashSize++;

		for (i = 0; i < nInSize; i++)
		{
			if (!isxdigit((unsigned char)*pPos)) {
				break;
			}
			nHashSize++;
			pPos++;
		}

		if (nHashSize < 8) {
			goto HASHLINE_ERR;
		}

		pFileName = pPos;
	}
	else
	{
		pPos = (char*)cpInChar + nInSize - 1;

		while (isspace((unsigned char)*pPos))
		{
			if (pPos == cpInChar) {
				goto HASHLINE_ERR;
			}
			pPos--;
		}

		while (pPos >= cpInChar)
		{
			if (!isxdigit((unsigned char)*pPos))
			{
				pPos++;
				break;
			}
			nHashSize++;
			pPos--;
		}

		if (nHashSize >= 8)
		{
			pHash = pPos;

			if (pPos == cpInChar) {
				pFileName = NULL;
			}
			else {
				pFileName = (char*)cpInChar;
			}
			goto FILE_CHK_END;
		}
		else if (nHashSize > 0) {
			goto HASHLINE_ERR;
		}

		while (pPos >= cpInChar)
		{
			if (CHAR_COMP_TO_3CHAR(pPos, '>', ':', ' '))//*pPos == L'>' && *(pPos + 1) == L':' && *(pPos + 2) == L' ')
			{
				pFileName = pPos + 3;
				pHash = (char*)cpInChar;
				nHashSize = 0;
				pPos--;

				while (pPos >= cpInChar)
				{
					if (!iswxdigit(*pPos))
					{
						if (*pPos == '<')
						{
							pHash = pPos + 1;
							pPos = pFileName;
							goto FILE_CHK_END;
						}
						break;
					}
					nHashSize++;
					pPos--;
				}
				break;
			}
			pPos--;
		}
		goto HASHLINE_ERR;
	}
FILE_CHK_END:

	if (nHashSize == nOutHashSize * 2) {
		CharToHex(pHash, pOutHash, nHashSize);
	}
	else {
		CharToHex(pHash, pOutHash, nOutHashSize);
	}
	nHashSize /= 2;

	if (pFileName == NULL)
	{
NO_FILENAME:
		if (pOutFileName != NULL) {
			*pOutFileName = '\0';
		}
		return nHashSize;
	}

	if (pFileName == cpInChar)
	{
		char szExtBuffer[sizeof(size_t)] = { 0 };
		const unsigned int* aligned_nExt = (unsigned int*)szExtBuffer;

		pPos = pFileName;
		memcpy(szExtBuffer, pFileName, sizeof(szExtBuffer));
		CharSizeToUpper(szExtBuffer, sizeof(szExtBuffer));

		switch (*aligned_nExt & 0x00FFFFFF)
		{
		case MAKE_CHAR_TO_INT_3CHAR('X', 'X', 'H'):
			szExtBuffer[0] = pFileName[3];
			szExtBuffer[1] = pFileName[4];
			szExtBuffer[2] = pFileName[5];
			if (szExtBuffer[0] == '3')
			{
				if (szExtBuffer[1] == '2') {
					pPos += 5;
				}
				else {
					pPos += 4;
				}
			}
			else if ((*aligned_nExt & 0x0000FFFF) == MAKE_CHAR_TO_INT_2CHAR('6', '4')) {
				pPos += 5;
			}
			else if ((*aligned_nExt & 0x00FFFFFF) == MAKE_CHAR_TO_INT_3CHAR('1', '2', '8')) {
				pPos += 6;
			}
			break;
		case MAKE_CHAR_TO_INT_3CHAR('C', 'R', 'C'):
			if (szExtBuffer[3] == '3')
			{
				memcpy(szExtBuffer, pFileName + 4, sizeof(szExtBuffer));
				CharSizeToUpper(szExtBuffer, sizeof(szExtBuffer));
				if (szExtBuffer[0] == '2')
				{
					if (szExtBuffer[1] == 'C') {
						pPos += 6;
					}
					else {
						pPos += 5;
					}
				}
			}
			break;
		case MAKE_CHAR_TO_INT_3CHAR('M', 'D', '2'):
			pPos += 3;
			break;
		case MAKE_CHAR_TO_INT_3CHAR('M', 'D', '4'):
			pPos += 3;
			break;
		case MAKE_CHAR_TO_INT_3CHAR('M', 'D', '5'):
			pPos += 3;
			break;
		case MAKE_CHAR_TO_INT_3CHAR('S', 'H', 'A'):
			switch (szExtBuffer[3])
			{
			case '1':
				pPos += 4;
				break;
			case '2':
				memcpy(szExtBuffer, pFileName + 4, sizeof(szExtBuffer));
				switch (*aligned_nExt & 0x0000FFFF)
				{
				case MAKE_CHAR_TO_INT_2CHAR('2', '4'):
					pPos += 6;
					break;
				case MAKE_CHAR_TO_INT_2CHAR('5', '6'):
					if (*aligned_nExt == MAKE_CHAR_TO_INT_4CHAR('5', '6', '-', '1'))
					{
						memcpy(szExtBuffer, pFileName + 8, sizeof(szExtBuffer));
						if ((*aligned_nExt & 0x0000FFFF) == MAKE_CHAR_TO_INT_2CHAR('9', '2')) {
							pPos += 10;
						}
					}
					else {
						pPos += 6;
					}
					break;
				}
				break;
			case '3':
				memcpy(szExtBuffer, pFileName + 4, sizeof(szExtBuffer));
				switch (*aligned_nExt)
				{
				case MAKE_CHAR_TO_INT_4CHAR('-', '2', '2', '4'):
				case MAKE_CHAR_TO_INT_4CHAR('-', '2', '5', '6'):
				case MAKE_CHAR_TO_INT_4CHAR('-', '3', '8', '4'):
				case MAKE_CHAR_TO_INT_4CHAR('-', '5', '1', '2'):
					pPos += 8;
					break;
				default:
					switch (*aligned_nExt & 0x0000FFFF)
					{
					case MAKE_CHAR_TO_INT_2CHAR('8', '4'):
						pPos += 6;
						break;
					}
				}
				break;
			case '5':
				memcpy(szExtBuffer, pFileName + 4, sizeof(szExtBuffer));
				switch (*aligned_nExt)
				{
				case MAKE_CHAR_TO_INT_4CHAR('1', '2', '-', '2'):
					memcpy(szExtBuffer, pFileName + 8, sizeof(szExtBuffer));
					switch (*aligned_nExt & 0x0000FFFF)
					{
					case MAKE_CHAR_TO_INT_2CHAR('2', '4'):
					case MAKE_CHAR_TO_INT_2CHAR('5', '6'):
						pPos += 10;
						break;
					}
					break;
				default:
					switch (*aligned_nExt & 0x0000FFFF)
					{
					case MAKE_CHAR_TO_INT_2CHAR('1', '2'):
						pPos += 6;
						break;
					}
				}
				break;
			}
			break;
		}


		if (*pPos != '(') {
			goto NO_FILENAME;
		}
		pPos++;

		if (pOutFileName != NULL)
		{
			char* pOutFileName_p = pOutFileName;

			while (*pPos != '\0')
			{
				if (*pPos == ')')
				{
					char* pTmp = pPos + 1;

					if (isspace((unsigned char)*pTmp)) {
						pTmp++;
					}

					if (*(pTmp) == '=' && isspace((unsigned char)*(pTmp + 1)))
					{
						*pOutFileName_p = '\0';
						break;
					}
				}
				*pOutFileName_p++ = *pPos++;
			}
		}
	}
	else
	{
		pPos = CharToSpaceSkip(pPos);
		if (pOutFileName != NULL) {
			qstrcpy(pOutFileName, pPos);
		}
	}
	return nHashSize;
}

size_t WCharToHashFileLine(const WCHAR* cpInWChar, size_t nInSize, WCHAR* pOutFileName, unsigned char* pOutHash, size_t nOutHashSize)
{
	WCHAR* pPos = (WCHAR*)cpInWChar;
	WCHAR* pHash = NULL;
	WCHAR* pFileName = NULL;
	size_t nHashSize = 0;

	if (nInSize == (size_t)-1) {
		nInSize = wcslen(cpInWChar);
	}

	if (nInSize < 8)
	{
HASHLINE_ERR:
		if (pOutFileName != NULL) {
			*pOutFileName = '\0';
		}
		memset(pOutHash, 0, nOutHashSize);
		return 0;
	}

	pPos = WCharToSpaceSkip(pPos);
	if (*pPos == '\0') {
		goto HASHLINE_ERR;
	}

	if (iswxdigit(*pPos))
	{
		size_t i;

		pHash = pPos;
		pPos++;
		nHashSize++;

		for (i = 0; i < nInSize; i++)
		{
			if (!iswxdigit(*pPos)) {
				break;
			}
			nHashSize++;
			pPos++;
		}

		if (nHashSize < 8) {
			goto HASHLINE_ERR;
		}

		pFileName = pPos;
	}
	else
	{
		pPos = (WCHAR*)cpInWChar + nInSize - 1;
		while (iswspace(*pPos))
		{
			if (pPos == cpInWChar) {
				goto HASHLINE_ERR;
			}
			pPos--;
		}

		while (pPos >= cpInWChar)
		{
			if (!iswxdigit(*pPos))
			{
				pPos++;
				break;
			}
			nHashSize++;
			pPos--;
		}

		if (nHashSize >= 8)
		{
			pHash = pPos;

			if (pPos == cpInWChar) {
				pFileName = NULL;
			}
			else {
				pFileName = (WCHAR*)cpInWChar;
			}
			goto FILE_CHK_END;
		}
		else if (nHashSize > 0) {
			goto HASHLINE_ERR;
		}

		while (pPos >= cpInWChar)
		{
			if (WCHAR_COMP_TO_3CHAR(pPos, '>', ':', ' '))//*pPos == L'>' && *(pPos + 1) == L':' && *(pPos + 2) == L' ')
			{
				pFileName = pPos + 3;
				pHash = (WCHAR*)cpInWChar;
				nHashSize = 0;
				pPos--;

				while (pPos >= cpInWChar)
				{
					if (!iswxdigit(*pPos))
					{
						if (*pPos == '<')
						{
							pHash = pPos + 1;
							pPos = pFileName;
							goto FILE_CHK_END;
						}
						break;
					}
					nHashSize++;
					pPos--;
				}
				break;
			}
			pPos--;
		}

		goto HASHLINE_ERR;
	}
FILE_CHK_END:

	if (nHashSize == nOutHashSize * 2) {
		WCharToHex(pHash, pOutHash, nHashSize);
	}
	else {
		WCharToHex(pHash, pOutHash, nOutHashSize);
	}
	nHashSize /= 2;

	if (pFileName == NULL)
	{
NO_FILENAME:
		if (pOutFileName != NULL) {
			*pOutFileName = '\0';
		}
		return nHashSize;
	}

	if (pFileName == cpInWChar)
	{
		WCHAR szExtBuffer[sizeof(size_t)] = { 0 };
		const unsigned __int64* aligned_nExt = (unsigned __int64*)szExtBuffer;

		pPos = pFileName;
		memcpy(szExtBuffer, pFileName, sizeof(szExtBuffer));
		WCharSizeToUpper(szExtBuffer, SIZEOF_NUM(szExtBuffer));

		switch (*aligned_nExt & 0x0000FFFFFFFFFFFF)
		{
		case MAKE_WCHAR_TO_INT_3CHAR('X', 'X', 'H'):
			szExtBuffer[0] = pFileName[3];
			szExtBuffer[1] = pFileName[4];
			szExtBuffer[2] = pFileName[5];
			if (szExtBuffer[0] == '3')
			{
				if (szExtBuffer[1] == '2') {
					pPos += 5;
				}
				else {
					pPos += 4;
				}
			}
			else if ((*aligned_nExt & 0x00000000FFFFFFFF) == MAKE_WCHAR_TO_INT_2CHAR('6', '4')) {
				pPos += 5;
			}
			else if ((*aligned_nExt & 0x0000FFFFFFFFFFFF) == MAKE_WCHAR_TO_INT_3CHAR('1', '2', '8')) {
				pPos += 6;
			}
			break;
		case MAKE_WCHAR_TO_INT_3CHAR('C', 'R', 'C'):
			if (szExtBuffer[3] == '3')
			{
				memcpy(szExtBuffer, pFileName + 4, sizeof(szExtBuffer));
				WCharSizeToUpper(szExtBuffer, SIZEOF_NUM(szExtBuffer));
				if (szExtBuffer[0] == '2')
				{
					if (szExtBuffer[1] == 'C') {
						pPos += 6;
					}
					else {
						pPos += 5;
					}
				}
			}
			break;
		case MAKE_WCHAR_TO_INT_3CHAR('M', 'D', '2'):
			pPos += 3;
			break;
		case MAKE_WCHAR_TO_INT_3CHAR('M', 'D', '4'):
			pPos += 3;
			break;
		case MAKE_WCHAR_TO_INT_3CHAR('M', 'D', '5'):
			pPos += 3;
			break;
		case MAKE_WCHAR_TO_INT_3CHAR('S', 'H', 'A'):
			switch (szExtBuffer[3])
			{
			case '1':
				pPos += 4;
				break;
			case '2':
				memcpy(szExtBuffer, pFileName + 4, sizeof(szExtBuffer));

				switch (*aligned_nExt & 0x00000000FFFFFFFF)
				{
				case MAKE_WCHAR_TO_INT_2CHAR('2', '4'):
					pPos += 6;
					break;
				case MAKE_WCHAR_TO_INT_2CHAR('5', '6'):
					if (*aligned_nExt == MAKE_WCHAR_TO_INT_4CHAR('5', '6', '-', '1'))
					{
						memcpy(szExtBuffer, pFileName + 8, sizeof(szExtBuffer));
						if ((*aligned_nExt & 0x00000000FFFFFFFF) == MAKE_WCHAR_TO_INT_2CHAR('9', '2')) {
							pPos += 10;
						}
					}
					else {
						pPos += 6;
					}
					break;
				}
				break;
			case '3':
				memcpy(szExtBuffer, pFileName + 4, sizeof(szExtBuffer));

				switch (*aligned_nExt)
				{
				case MAKE_WCHAR_TO_INT_4CHAR('-', '2', '2', '4'):
				case MAKE_WCHAR_TO_INT_4CHAR('-', '2', '5', '6'):
				case MAKE_WCHAR_TO_INT_4CHAR('-', '3', '8', '4'):
				case MAKE_WCHAR_TO_INT_4CHAR('-', '5', '1', '2'):
					pPos += 8;
					break;
				default:
					switch (*aligned_nExt & 0x00000000FFFFFFFF)
					{
					case MAKE_WCHAR_TO_INT_2CHAR('8', '4'):
						pPos += 6;
						break;
					}
				}
				break;
			case '5':
				memcpy(szExtBuffer, pFileName + 4, sizeof(szExtBuffer));

				switch (*aligned_nExt)
				{
				case MAKE_WCHAR_TO_INT_4CHAR('1', '2', '-', '2'):
					memcpy(szExtBuffer, pFileName + 8, sizeof(szExtBuffer));

					switch (*aligned_nExt & 0xFFFFFFFF)
					{
					case MAKE_WCHAR_TO_INT_2CHAR('2', '4'):
					case MAKE_WCHAR_TO_INT_2CHAR('5', '6'):
						pPos += 10;
						break;
					}
					break;
				default:
					switch (*aligned_nExt & 0xFFFFFFFF)
					{
					case MAKE_WCHAR_TO_INT_2CHAR('1', '2'):
						pPos += 6;
						break;
					}
				}
				break;
			}
			break;
		}


		if (*pPos != '(') {
			goto NO_FILENAME;
		}
		pPos++;

		if (pOutFileName != NULL)
		{
			WCHAR* pOutFileName_p = pOutFileName;

			while (*pPos != '\0')
			{
				if (*pPos == ')')
				{
					WCHAR* pTmp = pPos + 1;

					if (iswspace(*pTmp)) {
						pTmp++;
					}

					if (*(pTmp) == '=' && iswspace(*(pTmp + 1)))
					{
						*pOutFileName_p = '\0';
						break;
					}
				}
				*pOutFileName_p++ = *pPos++;
			}
		}
	}
	else
	{
		pPos = WCharToSpaceSkip(pPos);
		if (pOutFileName != NULL) {
			qwcscpy(pOutFileName, pPos);
		}
	}
	return nHashSize;
}

char* CharToSpaceSkip(const char* cpInChar)
{
	char* pPos = (char*)cpInChar;

	while (pPos != NULL)
	{
		if (!isspace((unsigned char)*pPos)) {
			break;
		}
		pPos++;
	}
	return pPos;
}

WCHAR* WCharToSpaceSkip(const WCHAR* cpInWChar)
{
	WCHAR* pPos = (WCHAR*)cpInWChar;

	while (pPos != NULL)
	{
		if (!iswspace(*pPos)) {
			break;
		}
		pPos++;
	}
	return pPos;
}

size_t CharToDeleteReturnCode(char* pInChar, size_t nInSize)
{
	char* pPos;
	const char szReturnCode[] = { '\r', '\n' };

	if (nInSize == (size_t)-1) {
		nInSize = strlen(pInChar);
	}

	pPos = pInChar + nInSize - 2;
	if (memcmp(pPos, szReturnCode, sizeof(szReturnCode)) == 0)
	{
		*pPos++ = '\0';
		*pPos = '\0';
		nInSize -= 2;
	}
	else
	{
		pPos++;
		if (*pPos == '\r' || *pPos == '\n')
		{
			*pPos = '\0';
			nInSize--;
		}
	}
	return nInSize;
}

size_t WCharToDeleteReturnCode(WCHAR* pInWChar, size_t nInSize)
{
	WCHAR* pPos;
	const WCHAR szReturnCode[] = {'\r', '\n'};

	if (nInSize == (size_t)-1) {
		nInSize = wcslen(pInWChar);
	}
	if (nInSize < 1) {
		return 0;
	}

	pPos = pInWChar + nInSize - 2;
	if (memcmp(pPos, szReturnCode, sizeof(szReturnCode)) == 0)
	{
		*pPos++ = '\0';
		*pPos = '\0';
		nInSize -= 2;
	}
	else
	{
		pPos++;
		if (*pPos == '\r' || *pPos == '\n')
		{
			*pPos = '\0';
			nInSize--;
		}
	}
	return nInSize;
}


char* CharToEnvString(const char* cpInChar)
{
	char* pOutChar;
	size_t nInSize;
	int nRet;

	nInSize = _MAX_PATH * 4;
	pOutChar = (char*)malloc(nInSize);
	if (pOutChar == NULL) {
		return NULL;
	}

	nRet = ExpandEnvironmentStringsA(cpInChar, pOutChar, (UINT)nInSize);
	if (!nRet) {
		qstrcpy(pOutChar, cpInChar);
	}
	return pOutChar;
}

char* CharToEnvString2(const char* cpInChar, char* pOutChar, size_t nInSize)
{
	int nRet;

	if (pOutChar == NULL)
	{
		nInSize = _MAX_PATH * 4;
		pOutChar = (char*)malloc(nInSize);
		if (pOutChar == NULL) {
			return NULL;
		}
	}
	if (nInSize == -1) {
		nInSize = strlen(cpInChar);
	}

	nRet = ExpandEnvironmentStringsA(cpInChar, pOutChar, (UINT)nInSize);
	if (!nRet) {
		qstrcpy(pOutChar, cpInChar);
	}
	return pOutChar;
}


WCHAR* WCharToEnvString(const WCHAR* cpInWChar)
{
	WCHAR* pOutWChar;
	size_t nInSize;
	int nRet;

	nInSize = _MAX_PATH * 4;
	pOutWChar = (WCHAR*)malloc(nInSize * sizeof(WCHAR));
	if (pOutWChar == NULL) {
		return NULL;
	}

	nRet = ExpandEnvironmentStringsW(cpInWChar, pOutWChar, (UINT)nInSize);
	if (!nRet) {
		qwcscpy(pOutWChar, cpInWChar);
	}
	return pOutWChar;
}

WCHAR* WCharToEnvString2(const WCHAR* cpInWChar, WCHAR* pOutWChar, size_t nInSize)
{
	int nRet;

	if (pOutWChar == NULL)
	{
		nInSize = _MAX_PATH * 4;
		pOutWChar = (WCHAR*)malloc(nInSize * sizeof(WCHAR));
		if (pOutWChar == NULL) {
			return NULL;
		}
	}
	if (nInSize == -1) {
		nInSize = wcslen(cpInWChar);
	}

	nRet = ExpandEnvironmentStringsW(cpInWChar, pOutWChar, (UINT)nInSize);
	if (!nRet) {
		qwcscpy(pOutWChar, cpInWChar);
	}
	return pOutWChar;
}

char* qstrcpy(char* _Destination, const char* _Source)
{
	char* dst = _Destination;
	const char* src = _Source;
	size_t* aligned_dst;
	const size_t* aligned_src;

	/* If SRC or DEST is unaligned, then copy bytes.  */
	if (!UNALIGNED2(src, dst))
	{
		aligned_dst = (size_t*)dst;
		aligned_src = (size_t*)src;

		/* SRC and DEST are both "long int" aligned, try to do "long int"
		   sized copies.  */
		while (!DETECTNULL(*aligned_src)) {
			*aligned_dst++ = *aligned_src++;
		}
		dst = (char*)aligned_dst;
		src = (char*)aligned_src;
	}

	while ((*dst++ = *src++)) {
		;
	}
	return dst - 1;
}

WCHAR* qwcscpy(WCHAR* _Destination, const WCHAR* _Source)
{
	while (*_Source) {
		*_Destination++ = *_Source++;
	}
	*_Destination = '\0';
	return _Destination;
}

char* qstrncpy(char* _Destination, const char* _Source, size_t _Count)
{
	char* dst = _Destination;
	const char* src = _Source;
	size_t* aligned_dst;
	const size_t* aligned_src;
	size_t n = _Count;

	/* If SRC and DEST is aligned and count large enough, then copy words.  */
	if (!UNALIGNED2(src, dst) && !TOO_SMALL(_Count))
	{
		aligned_dst = (size_t*)dst;
		aligned_src = (size_t*)src;

		/* SRC and DEST are both "long int" aligned, try to do "long int"
	   sized copies.  */
		while (_Count >= sizeof(size_t) && !DETECTNULL(*aligned_src))
		{
			_Count -= sizeof(size_t);
			*aligned_dst++ = *aligned_src++;
		}

		dst = (char*)aligned_dst;
		src = (char*)aligned_src;
	}

	while (_Count > 0)
	{
		--_Count;
		if ((*dst++ = *src++) == '\0') {
			break;
		}
	}

	if (n > 0 && *(dst - 1) != '\0') {
		*dst = '\0';
	}

	while (_Count-- > 0) {
		*dst++ = '\0';
	}
	return dst;
}

WCHAR* qwcsncpy(WCHAR* _Destination, const WCHAR* _Source, size_t _Count)
{
	WCHAR* dscan = _Destination;
	size_t n = _Count;

	while (_Count > 0)
	{
		--_Count;
		if ((*dscan++ = *_Source++) == '\0') {
			break;
		}
	}

	if (n > 0 && *(dscan - 1) != '\0') {
		*dscan = '\0';
	}

	while (_Count-- > 0) {
		*dscan++ = '\0';
	}
	return dscan;
}

/* Given a file name, S of length S_LEN, that is not NUL-terminated,
   modify it in place, performing the equivalent of this sed substitution:
   's/\\n/\n/g;s/\\r/\r/g;s/\\\\/\\/g' i.e., replacing each "\\n" string
   with a newline, each "\\r" string with a carriage return,
   and each "\\\\" with a single backslash, NUL-terminate it and return S.
   If S is not a valid escaped file name, i.e., if it ends with an odd number
   of backslashes or if it contains a backslash followed by anything other
   than "n" or another backslash, return NULL.  */

static char* filename_unescape(char* s, size_t s_len)
{
	char* dst = s;
	size_t i;

	for (i = 0; i < s_len; i++)
	{
		switch (s[i])
		{
		case '\\':
			if (i == s_len - 1)
			{
				/* File name ends with an unescaped backslash: invalid.  */
				return NULL;
			}
			++i;
			switch (s[i])
			{
			case 'n':
				*dst++ = '\n';
				break;
			case 'r':
				*dst++ = '\r';
				break;
			case '\\':
				*dst++ = '\\';
				break;
			default:
				/* Only '\', 'n' or 'r' may follow a backslash.  */
				return NULL;
			}
			break;
		case '\0':
			/* The file name may not contain a NUL.  */
			return NULL;
		default:
			*dst++ = s[i];
			break;
		}
	}
	if (dst < s + s_len) {
		*dst = '\0';
	}
	return s;
}

static WCHAR* filename_wunescape(WCHAR* s, size_t s_len)
{
	WCHAR* dst = s;
	size_t i;

	for (i = 0; i < s_len; i++)
	{
		switch (s[i])
		{
		case '\\':
			if (i == s_len - 1)
			{
				/* File name ends with an unescaped backslash: invalid.  */
				return NULL;
			}
			++i;
			switch (s[i])
			{
			case 'n':
				*dst++ = '\n';
				break;
			case 'r':
				*dst++ = '\r';
				break;
			case '\\':
				*dst++ = '\\';
				break;
			default:
				/* Only '\', 'n' or 'r' may follow a backslash.  */
				return NULL;
			}
			break;
		case '\0':
			/* The file name may not contain a NUL.  */
			return NULL;
		default:
			*dst++ = s[i];
			break;
		}
	}
	if (dst < s + s_len) {
		*dst = '\0';
	}
	return s;
}

BOOL bsd_split_3A(char* s, size_t s_len, unsigned char** hex_digest, char** file_name, BOOL escaped_filename)
{
	size_t i;

	if (s_len == 0) {
		return FALSE;
	}

	/* Find end of filename. The BSD 'md5' and 'sha1' commands do not escape
	   filenames, so search backwards for the last ')'. */
	i = s_len - 1;
	while (i && s[i] != ')') {
		i--;
	}

	if (s[i] != ')') {
		return FALSE;
	}

	*file_name = s;

	if (escaped_filename && filename_unescape(s, i) == NULL) {
		return FALSE;
	}

	s[i++] = '\0';

	while (ISWHITE(s[i])) {
		i++;
	}

	if (s[i] != '=') {
		return FALSE;
	}

	i++;

	while (ISWHITE(s[i])) {
		i++;
	}

	*hex_digest = (unsigned char*)&s[i];
	return TRUE;
}

BOOL bsd_split_3W(WCHAR* s, size_t s_len, WCHAR** hex_digest, WCHAR** file_name, BOOL escaped_filename)
{
	size_t i;

	if (s_len == 0) {
		return FALSE;
	}

	/* Find end of filename. The BSD 'md5' and 'sha1' commands do not escape
	   filenames, so search backwards for the last ')'. */
	i = s_len - 1;
	while (i && s[i] != ')') {
		i--;
	}

	if (s[i] != ')') {
		return FALSE;
	}

	*file_name = s;

	if (escaped_filename && filename_wunescape(s, i) == NULL) {
		return FALSE;
	}

	s[i++] = '\0';

	while (ISWHITE(s[i])) {
		i++;
	}

	if (s[i] != '=') {
		return FALSE;
	}

	i++;

	while (ISWHITE(s[i])) {
		i++;
	}

	*hex_digest = (WCHAR*)&s[i];
	return TRUE;
}

static int bsd_reversed = -1;

BOOL split_3A(char* s, size_t s_len,	unsigned char** hex_digest, int* binary, char** file_name,
	const char* digest_type_string, const size_t min_digest_line_length, const size_t digest_hex_bytes)
{
	BOOL escaped_filename = FALSE;
	size_t algo_name_len;
	size_t i = 0;

	while (ISWHITE(s[i])) {
		++i;
	}

	/* Check for BSD-style checksum line. */
	algo_name_len = strlen(digest_type_string);
	if (strncmp(s + i, digest_type_string, algo_name_len) == 0)
	{
		if (s[i + algo_name_len] == ' ') {
			++i;
		}
		if (s[i + algo_name_len] == '(')
		{
			*binary = 0;
			return bsd_split_3A(s + i + algo_name_len + 1,
				s_len - (i + algo_name_len + 1),
				hex_digest, file_name, escaped_filename);
		}
		return FALSE;
	}

	/* Ignore this line if it is too short.
	   Each line must have at least `min_digest_line_length - 1' (or one more, if
	   the first is a backslash) more characters to contain correct message digest
	   information.  */
	if (s_len - i < min_digest_line_length + (s[i] == '\\')) {
		return FALSE;
	}

	if (s[i] == '\\')
	{
		++i;
		escaped_filename = TRUE;
	}
	*hex_digest = (unsigned char*)&s[i];

	/* The first field has to be the n-character hexadecimal
	   representation of the message digest.  If it is not followed
	   immediately by a white space it's an error.  */
	i += digest_hex_bytes;
	if (!ISWHITE(s[i])) {
		return FALSE;
	}

	s[i++] = '\0';


	if (s[i] != ' ' && s[i] != '*') {
		return FALSE;
	}
	*binary = (s[i++] == '*');

	/* All characters between the type indicator and end of line are
	   significant -- that includes leading and trailing white space.  */
	*file_name = &s[i];

	/* If "bsd reversed" format detected.  */
	if ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
	{
		/* Don't allow mixing bsd and standard formats,
		   to minimize security issues with attackers
		   renaming files with leading spaces.
		   This assumes that with bsd format checksums
		   that the first file name does not have
		   a leading ' ' or '*'.  */
		if (bsd_reversed == 0) {
			return FALSE;
		}
		bsd_reversed = 1;
	}
	else if (bsd_reversed != 1)
	{
		bsd_reversed = 0;
		*binary = (s[i++] == '*');
	}

	/* All characters between the type indicator and end of line are
	   significant -- that includes leading and trailing white space.  */
	*file_name = &s[i];

	if (escaped_filename) {
		return filename_unescape(&s[i], s_len - i) != NULL;
	}
	return TRUE;
}

BOOL split_3W(WCHAR* s, size_t s_len, WCHAR** hex_digest, int* binary, WCHAR** file_name,
	const WCHAR* digest_type_string, const size_t min_digest_line_length, const size_t digest_hex_bytes)
{
	BOOL escaped_filename = FALSE;
	size_t algo_name_len;
	size_t i = 0;

	while (ISWHITE(s[i])) {
		++i;
	}

	/* Check for BSD-style checksum line. */
	algo_name_len = wcslen(digest_type_string);
	if (wcsncmp(s + i, digest_type_string, algo_name_len) == 0)
	{
		if (s[i + algo_name_len] == ' ') {
			++i;
		}
		if (s[i + algo_name_len] == '(')
		{
			*binary = 0;
			return bsd_split_3W(s + i + algo_name_len + 1,
				s_len - (i + algo_name_len + 1),
				hex_digest, file_name, escaped_filename);
		}
		return FALSE;
	}

	/* Ignore this line if it is too short.
	   Each line must have at least `min_digest_line_length - 1' (or one more, if
	   the first is a backslash) more characters to contain correct message digest
	   information.  */
	if (s_len - i < min_digest_line_length + (s[i] == '\\')) {
		return FALSE;
	}

	if (s[i] == '\\')
	{
		++i;
		escaped_filename = TRUE;
	}
	*hex_digest = (WCHAR*)&s[i];

	/* The first field has to be the n-character hexadecimal
	   representation of the message digest.  If it is not followed
	   immediately by a white space it's an error.  */
	i += digest_hex_bytes;
	if (!ISWHITE(s[i])) {
		return FALSE;
	}

	s[i++] = '\0';


	/* If "bsd reversed" format detected.  */
	if ((s_len - i == 1) || (s[i] != ' ' && s[i] != '*'))
	{
		/* Don't allow mixing bsd and standard formats,
		   to minimize security issues with attackers
		   renaming files with leading spaces.
		   This assumes that with bsd format checksums
		   that the first file name does not have
		   a leading ' ' or '*'.  */
		if (bsd_reversed == 0) {
			return FALSE;
		}
		bsd_reversed = 1;
	}
	else if (bsd_reversed != 1)
	{
		bsd_reversed = 0;
		*binary = (s[i++] == '*');
	}

	/* All characters between the type indicator and end of line are
	   significant -- that includes leading and trailing white space.  */
	*file_name = &s[i];

	if (escaped_filename) {
		return filename_wunescape(&s[i], s_len - i) != NULL;
	}
	return TRUE;
}

size_t CharToGetLine(char** ppOutChar, size_t* npInCount, const char *cpInChar, size_t nInCharSize)
{
	char* ptr;
	cpInChar += *npInCount;
	*ppOutChar = (char*)cpInChar;
	ptr = (char*)cpInChar;
	nInCharSize -= *npInCount;

	if (nInCharSize == 0) {
		nInCharSize = strlen(cpInChar);
	}

	while (nInCharSize--)
	{
		if (*ptr == '\n')
		{
			*ptr = '\0';
			if (*(ptr - 1) == '\r') {
				*(ptr - 1) = '\0';
			}
			*npInCount = ptr - cpInChar;
			return ptr - cpInChar;
		}
		ptr++;
	}
	*npInCount = ptr - cpInChar;
	return ptr - cpInChar;
}

size_t WCharToGetLine(WCHAR** ppOutWChar, size_t* npInCount, const WCHAR* cpInWChar, size_t nInCharSize)
{
	WCHAR* ptr;
	cpInWChar += *npInCount;
	*ppOutWChar = (WCHAR*)cpInWChar;
	ptr = (WCHAR*)cpInWChar;
	nInCharSize -= *npInCount;

	if (nInCharSize == 0) {
		nInCharSize = wcslen(cpInWChar);
	}

	while (nInCharSize--)
	{
		if (*ptr == '\n')
		{
			*ptr = '\0';
			if (*(ptr - 1) == '\r') {
				*(ptr - 1) = '\0';
			}
			*npInCount = ptr - cpInWChar;
			return ptr - cpInWChar;
		}
		ptr++;
	}
	*npInCount = ptr - cpInWChar;
	return ptr - cpInWChar;
}
