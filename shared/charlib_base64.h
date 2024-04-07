#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CHARLIB_BASE64_H_A9C8B0F6_1A34_436e_87E1_8B78BE25CEF9__
#define __CHARLIB_BASE64_H_A9C8B0F6_1A34_436e_87E1_8B78BE25CEF9__

#include "charlib.h"


#ifdef  __cplusplus
extern "C" {
#endif

size_t Base64Decode(unsigned char* pOutChar, const char* cpBase64Char);
size_t Base64DecodeLength(const unsigned char* cpBase64Char);

size_t Base64ToWCharDecode(unsigned char* pOutChar, const WCHAR* cpBase64WChar);
size_t Base64ToWCharDecodeLength(const WCHAR* cpBase64WChar);


size_t Base64Encode(char* pOutChar, const unsigned char* cpInChar, size_t nInSize);
size_t Base64EncodeLength(const char* pChar);
size_t Base64EncodeLength2(size_t nInSize);

size_t Base64EncodeToWChar(WCHAR* pOutWChar, const unsigned char* cpInChar, size_t nInSize);
size_t Base64EncodeToWCharLength(const WCHAR* pWChar);
size_t Base64EncodeToWCharLength2(size_t nInSize);


size_t Base64FileNameEncode(char* pOutChar, const char* cpInChar, size_t nInSize);
size_t Base64FileNameEncodeToWChar(WCHAR* pOutChar, const char* cpInChar, size_t nInSize);


char* BinaryToBase64(const void* cpIn, size_t nInSize, char* pOutBase64Char);
WCHAR* BinaryToBase64ToWChar(const void* cpIn, size_t nInSize, WCHAR* pOutBase64WChar);


char* CharToBase64(const char* pChar);
char* CharToBase64v2(const char* pChar, char* pOutBase64Char);
char* CharSizeToBase64(const char* pChar, size_t nInSize);
char* CharSizeToBase64v2(const char* pChar, size_t nInSize, char* pOutBase64Char);

WCHAR* CharToBase64ToWChar(const char* pChar);
WCHAR* CharToBase64ToWCharv2(const char* pChar, WCHAR* pOutBase64WChar);
WCHAR* CharSizeToBase64ToWChar(const char* pChar, size_t nInSize);
WCHAR* CharSizeToBase64ToWCharv2(const char* pChar, size_t nInSize, WCHAR* pOutBase64WChar);


char* WCharToBase64(const WCHAR* cpWChar);
char* WCharToBase64v2(const WCHAR* cpWChar, char* pOutBase64Char);
char* WCharSizeToBase64(const WCHAR* cpWChar, size_t nInSize);
char* WCharSizeToBase64v2(const WCHAR* cpWChar, size_t nInSize, char* pOutBase64Char);

WCHAR* WCharToBase64ToWChar(const WCHAR* cpWChar);
WCHAR* WCharToBase64ToWCharv2(const WCHAR* cpWChar, WCHAR* pOutBase64WChar);
WCHAR* WCharSizeToBase64ToWChar(const WCHAR* cpWChar, size_t nInSize);
WCHAR* WCharSizeToBase64ToWCharv2(const WCHAR* cpWChar, size_t nInSize, WCHAR* pOutBase64WChar);


char* Base64ToCharA(const char* cpInBase64Char);
char* Base64ToCharv2A(const char* cpInBase64Char, char* pOutChar);

WCHAR* Base64ToWCharA(const char* cpInBase64Char);
WCHAR* Base64ToWCharv2A(const char* cpInBase64Char, WCHAR* pOutWChar);


char* Base64ToCharW(const WCHAR* cpInBase64WChar);
char* Base64ToCharv2W(const WCHAR* cpInBase64WChar, char* pOutChar);

WCHAR* Base64ToWCharW(const WCHAR* cpInBase64Char);
WCHAR* Base64ToWCharv2W(const WCHAR* cpInBase64Char, WCHAR* pOutWChar);


void* Base64ToBinary(const char* cpInBase64Char, void* pOut, size_t* pOutSize);


size_t Base64LineFeedEncodeLength(const char* cpChar);
size_t Base64LineFeedEncodeLength2(size_t nInSize);

char* BinaryToBase64LineFeed(const void* cpIn, size_t nInSize, char* pOutBase64Char);
WCHAR* BinaryToBase64LineFeedToWChar(const void* cpIn, size_t nInSize, WCHAR* pOutBase64WChar);

char* CharToBase64LineFeed(const char* cpChar);
char* CharToBase64LineFeedv2(const char* cpChar, char* pOutChar);
char* CharSizeToBase64LineFeed(const char* cpChar, size_t nInSize);
char* CharSizeToBase64LineFeedv2(const char* cpChar, size_t nInSize, char* pOutChar);

WCHAR* CharToBase64LineFeedToWChar(const char* cpInChar);
WCHAR* CharToBase64LineFeedToWCharv2(const char* cpInChar, WCHAR* pOutWChar);
WCHAR* CharSizeToBase64LineFeedToWChar(const char* cpInChar, size_t nInSize);
WCHAR* CharSizeToBase64LineFeedToWCharv2(const char* cpInChar, size_t nInSize, WCHAR* pOutWChar);


#define BinaryToBase64ToChar BinaryToBase64

#define CharToBase64ToChar CharToBase64
#define CharToBase64ToCharv2 CharToBase64v2
#define CharSizeToBase64ToChar CharSizeToBase64
#define CharSizeToBase64ToCharv2 CharSizeToBase64v2

#define WCharToBase64ToChar WCharToBase64
#define WCharToBase64ToCharv2 WCharToBase64v2
#define WCharSizeToBase64ToChar WCharSizeToBase64
#define WCharSizeToBase64ToCharv2 WCharSizeToBase64v2


#define CharToBase64LineFeedToChar CharToBase64LineFeed
#define CharToBase64LineFeedToCharv2ToTChar CharToBase64LineFeedv2
#define CharSizeToBase64LineFeedToChar CharSizeToBase64LineFeed
#define CharSizeToBase64LineFeedToCharv2 CharSizeToBase64LineFeedv2

#define WCharToBase64LineFeedToChar WCharToBase64LineFeed
#define WCharToBase64LineFeedToCharv2ToTChar WCharToBase64LineFeedv2
#define WCharSizeToBase64LineFeedToChar WCharSizeToBase64LineFeed
#define WCharSizeToBase64LineFeedToCharv2 WCharSizeToBase64LineFeedv2


#ifdef _UNICODE

#define BinaryToBase64ToTChar BinaryToBase64ToWChar

#define CharToBase64ToTChar CharToBase64ToWChar
#define CharToBase64ToTCharv2 CharToBase64ToWCharv2
#define CharSizeToBase64ToTChar CharSizeToBase64ToWChar
#define CharSizeToBase64ToTCharv2 CharSizeToBase64ToWCharv2

#define Base64ToChar Base64ToCharW
#define Base64ToCharv2 Base64ToCharv2W
#define Base64ToWChar Base64ToWCharW
#define Base64ToWCharv2 Base64ToWCharv2W

#define CharToBase64LineFeedToTChar CharToBase64LineFeedToWChar
#define CharToBase64LineFeedToTCharv2ToTChar CharToBase64LineFeedToWCharv2
#define CharSizeToBase64LineFeedToTChar CharSizeToBase64LineFeedToWChar
#define CharSizeToBase64LineFeedToTCharv2 CharSizeToBase64LineFeedToWCharv2

#else

#define BinaryToBase64ToTChar BinaryToBase64

#define CharToBase64ToTChar CharToBase64
#define CharToBase64ToTCharv2 CharToBase64v2
#define CharSizeToBase64ToTChar CharSizeToBase64
#define CharSizeToBase64ToTCharv2 CharSizeToBase64v2

#define Base64ToChar Base64ToCharA
#define Base64ToCharv2 Base64ToCharv2A
#define Base64ToWChar Base64ToWCharA
#define Base64ToWCharv2 Base64ToWCharv2A

#define CharToBase64LineFeedToTChar CharToBase64LineFeed
#define CharToBase64LineFeedToTCharv2ToTChar CharToBase64LineFeedv2
#define CharSizeToBase64LineFeedToTChar CharSizeToBase64LineFeed
#define CharSizeToBase64LineFeedToTCharv2 CharSizeToBase64LineFeedv2

#endif

#ifdef  __cplusplus
}
#endif
#endif
