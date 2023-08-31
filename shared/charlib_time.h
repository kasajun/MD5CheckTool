#pragma once
#ifndef __CHARLIB_TIME_H__
#define __CHARLIB_TIME_H__

#include "charlib.h"

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

#else


#endif
#endif
