#pragma once
#ifndef __CHARLIB_TIME_H_3CCAC007_874F_416F_846C_12C634E05FFC__
#define __CHARLIB_TIME_H_3CCAC007_874F_416F_846C_12C634E05FFC__

#include "charlib.h"

#ifdef  __cplusplus
extern "C" {
#endif

FILETIME* FILETIMEToAddMilliseconds(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToAddSecond(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToAddMinute(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToAddHour(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToAddDay(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToSubMilliseconds(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToSubSecond(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToSubMinute(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToSubHour(FILETIME* ftFileTime, unsigned __int64 ullAddTime);
FILETIME* FILETIMEToSubDay(FILETIME* ftFileTime, unsigned __int64 ullAddTime);

char* LOCALFILETIMEToChar(const FILETIME* cpFileTime, const char* cpTimeFormat);
WCHAR* LOCALFILETIMEToWChar(const FILETIME* cpFileTime, const WCHAR* cpTimeFormat);

char* UINT64ToLOCALFILETIMEToChar(const unsigned __int64 cFileTime, const char* cpTimeFormat);
WCHAR* UINT64ToLOCALFILETIMEToWChar(const unsigned __int64 cFileTime, const WCHAR* cpTimeFormat);

char* FILETIMEToChar(const FILETIME* cpFileTime, const char* cpTimeFormat);
WCHAR* FILETIMEToWChar(const FILETIME* cpFileTime, const WCHAR* cpTimeFormat);

char* UINT64ToFILETIMEToChar(const unsigned __int64 cFileTime, const char* cpTimeFormat);
WCHAR* UINT64ToFILETIMEToWChar(const unsigned __int64 cFileTime, const WCHAR* cpTimeFormat);

char* SYSTEMTIMEToChar(const SYSTEMTIME* cpFileTime, const char* cpTimeFormat);
WCHAR* SYSTEMTIMEToWChar(const SYSTEMTIME* cpFileTime, const WCHAR* cpTimeFormat);

char* Time64ToChar(__time64_t inTime, const char* cpTimeFormat);
WCHAR* Time64ToWChar(__time64_t inTime, const WCHAR* cpTimeFormat);

#ifdef _UNICODE
#define LOCALFILETIMEToTChar LOCALFILETIMEToWChar
#define UINT64ToLOCALFILETIMEToTChar UINT64ToLOCALFILETIMEToWChar
#define FILETIMEToTChar FILETIMEToWChar
#define UINT64ToFILETIMEToTChar UINT64ToFILETIMEToWChar
#define SYSTEMTIMEToTChar SYSTEMTIMEToWChar
#define Time64ToTChar Time64ToWChar
#else
#define LOCALFILETIMEToTChar LOCALFILETIMEToChar
#define UINT64ToLOCALFILETIMEToTChar UINT64ToLOCALFILETIMEToChar
#define FILETIMEToTChar FILETIMEToChar
#define UINT64ToFILETIMEToTChar UINT64ToFILETIMEToChar
#define SYSTEMTIMEToTChar SYSTEMTIMEToChar
#define Time64ToTChar Time64ToChar
#endif

#ifdef  __cplusplus
}
#endif
#endif
