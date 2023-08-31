#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CHARLIB_URL_H_A327EAA7_692B_4b3b_A0BE_B2AD9A52B7F3__
#define __CHARLIB_URL_H_A327EAA7_692B_4b3b_A0BE_B2AD9A52B7F3__

#include "charlib.h"

size_t CharToUrlEncodeLength(const char* cpInChar);

char* CharToUrlEncode(const char* cpInChar);
char* Utf8CharToUrlEncodeChar(const char* cpInChar);
char* WCharToUrlEncodeChar(const WCHAR* cpInWChar);

WCHAR* WCharToUrlEncodeWChar(const WCHAR* cpInChar);
WCHAR* CharToUrlEncodeWChar(const char* cpInChar);
WCHAR* Utf8CharToUrlEncodeWChar(const char* cpInChar);

#ifdef _UNICODE
#define TCharToUrlEncodeChar WCharToUrlEncodeChar
#define TCharToUrlEncodeWChar WCharToUrlEncodeWChar
#define TCharToUrlEncodeTChar WCharToUrlEncodeWChar
#else
#define TCharToUrlEncodeChar CharToUrlEncodeChar
#define TCharToUrlEncodeWChar CharToUrlEncodeWChar
#define TCharToUrlEncodeTChar CharToUrlEncodeChar
#endif
#endif
