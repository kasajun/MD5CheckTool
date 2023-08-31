#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CHARLIB_BASE64_H_A9C8B0F6_1A34_436e_87E1_8B78BE25CEF9__
#define __CHARLIB_BASE64_H_A9C8B0F6_1A34_436e_87E1_8B78BE25CEF9__

#include "charlib.h"

size_t Base64Decode(char* pOutChar, const char* cpBase64Char);
size_t Base64DecodeLength(const char* cpBase64Char);

size_t Base64Decode(unsigned char* pOutChar, const char* cpBase64Char);
size_t Base64DecodeLength(const unsigned char* cpBase64Char);

size_t Base64ToWCharDecode(char* pOutChar, const WCHAR* cpBase64WChar);
size_t Base64ToWCharDecodeLength(const WCHAR* cpBase64WChar);

size_t Base64Encode(char* pOutChar, const unsigned char* cpInChar, size_t nInSize);

size_t Base64Encode(char* pOutChar, const char* pInChar, size_t nInSize = (size_t)-1);
size_t Base64EncodeLength(const char* pChar, size_t nInSize = (size_t)-1);

size_t Base64EncodeToWChar(WCHAR* pOutWChar, const char* cpInChar, size_t nInSize = (size_t)-1);
size_t Base64EncodeToWCharLength(const WCHAR* pWChar, size_t nInSize = (size_t)-1);

size_t Base64FileNameEncode(char* pOutChar, const char* cpInChar, size_t nInSize = (size_t)-1);
size_t Base64FileNameEncodeToWChar(WCHAR* pOutChar, const char* cpInChar, size_t nInSize = (size_t)-1);

char* CharToBase64(const char* pChar, size_t nInSize = (size_t)-1, char* pOutBase64Char = NULL);
WCHAR* CharToBase64ToWChar(const char* pChar, size_t nInSize = (size_t)-1, WCHAR* pOutBase64WChar = NULL);

char* BinaryToBase64(const void* cpIn, size_t nInSize, char* pOutBase64Char = NULL);
WCHAR* BinaryToBase64ToWChar(const void* cpIn, size_t nInSize, WCHAR* pOutBase64WChar = NULL);

char* WCharToBase64(const WCHAR* cpWChar, size_t nInSize = (size_t)-1);
WCHAR* WCharToBase64ToWChar(const WCHAR* cpWChar, size_t nInSize = (size_t)-1);

char* Base64ToChar(const char* cpInBase64Char, char* pOutChar = NULL);
char* WCharToBase64ToChar(const WCHAR* cpInBase64WChar, char* pOutChar = NULL);

void* Base64ToBinary(const char* cpInBase64Char, void* pOut, size_t* pOutSize);

size_t Base64LineFeedEncodeLength(const char* cpChar, size_t nInSize = (size_t)-1);
char* CharToBase64LineFeed(const char* cpChar, size_t nInSize = (size_t)-1);
char* BinaryToBase64LineFeed(const void* cpIn, size_t nInSize);

WCHAR* CharToBase64LineFeedToWChar(const char* cpChar, size_t nInSize = (size_t)-1);
WCHAR* BinaryToBase64LineFeedToWChar(const void* cpIn, size_t nInSize = (size_t)-1);

#ifdef _UNICODE

#else

#endif
#endif
