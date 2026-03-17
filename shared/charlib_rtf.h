#pragma once
#ifndef __CHARLIB_RTF_H_86E40D81_89D2_4411_8448_64367ADB5807__
#define __CHARLIB_RTF_H_86E40D81_89D2_4411_8448_64367ADB5807__

#include "charlib.h"

#ifdef  __cplusplus
extern "C" {
#endif

	char* CharToRtfEncodeChar(const char* cpInChar, char* pOutChar);
	size_t CharToRtfEncodeCharLength(const char* cpInChar);

	char* WCharToRtfEncodeChar(const WCHAR* cpInWChar, char* pOutChar);
	size_t WCharToRtfEncodeCharLength(const WCHAR* cpInWChar);


#ifdef _UNICODE
#define TCharToRtfEncodeChar WCharToRtfEncodeChar
#define TCharToRtfEncodeCharLength WCharToRtfEncodeCharLength
#else
#define TCharToRtfEncodeChar CharToRtfEncodeChar
#define TCharToRtfEncodeCharLength CharToRtfEncodeCharLength
#endif

#ifdef  __cplusplus
}
#endif
#endif
