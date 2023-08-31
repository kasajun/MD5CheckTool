#include "charlib_time.h"
#include "c20.h"


#define CHAR_TIME_FORMAT	"%Y/%m/%d %H:%M:%S"
#define WCHAR_TIME_FORMAT	L"%Y/%m/%d %H:%M:%S"

FILETIME* FILETIMEToAddMilliseconds(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime += (ullAddTime * (1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToAddSecond(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime += (ullAddTime * (1000 * 1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToAddMinute(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime += (ullAddTime * ((unsigned __int64)60 * 1000 * 1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToAddHour(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime += (ullAddTime * ((unsigned __int64)60 * 60 * 1000 * 1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToAddDay(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime += (ullAddTime * ((unsigned __int64)24 * 60 * 60 * 1000 * 1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToSubMilliseconds(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime -= (ullAddTime * (1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToSubSecond(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime -= (ullAddTime * (1000 * 1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToSubMinute(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime -= (ullAddTime * ((unsigned __int64)60 * 1000 * 1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToSubHour(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime -= (ullAddTime * ((unsigned __int64)60 * 60 * 1000 * 1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

FILETIME* FILETIMEToSubDay(FILETIME* ftFileTime, unsigned __int64 ullAddTime)
{
	unsigned __int64 ullFileTime = ((unsigned __int64)ftFileTime->dwHighDateTime << 32) + ftFileTime->dwLowDateTime;
	ullFileTime -= (ullAddTime * ((unsigned __int64)24 * 60 * 60 * 1000 * 1000 * 10));
	ftFileTime->dwLowDateTime = (DWORD)(ullFileTime & 0xFFFFFFFF);
	ftFileTime->dwHighDateTime = (DWORD)(ullFileTime >> 32);
	return ftFileTime;
}

char* LOCALFILETIMEToChar(const FILETIME* cpFileTime, const char* cpTimeFormat)
{
	FILETIME ltFileTime;

	FileTimeToLocalFileTime(cpFileTime, &ltFileTime);
	return FILETIMEToChar(&ltFileTime, cpTimeFormat);
}

WCHAR* LOCALFILETIMEToWChar(const FILETIME* cpFileTime, const WCHAR* cpTimeFormat)
{
	FILETIME ltFileTime;

	FileTimeToLocalFileTime(cpFileTime, &ltFileTime);
	return FILETIMEToWChar(&ltFileTime, cpTimeFormat);
}

char* UINT64ToLOCALFILETIMEToChar(const unsigned __int64 cFileTime, const char* cpTimeFormat)
{
	FILETIME ftFileTime;
	FILETIME ltFileTime;

	ftFileTime.dwHighDateTime = (DWORD)(cFileTime >> 32);
	ftFileTime.dwLowDateTime = (DWORD)(cFileTime & 0xFFFFFFFF);
	FileTimeToLocalFileTime(&ftFileTime, &ltFileTime);
	return FILETIMEToChar(&ltFileTime, cpTimeFormat);
}

WCHAR* UINT64ToLOCALFILETIMEToWChar(const unsigned __int64 cFileTime, const WCHAR* cpTimeFormat)
{
	FILETIME ftFileTime;
	FILETIME ltFileTime;

	ftFileTime.dwHighDateTime = (DWORD)(cFileTime >> 32);
	ftFileTime.dwLowDateTime = (DWORD)(cFileTime & 0xFFFFFFFF);
	FileTimeToLocalFileTime(&ftFileTime, &ltFileTime);
	return FILETIMEToWChar(&ltFileTime, cpTimeFormat);
}

char* FILETIMEToChar(const FILETIME* cpFileTime, const char* cpTimeFormat)
{
	SYSTEMTIME stFileTime;

	FileTimeToSystemTime(cpFileTime, &stFileTime);
	return SYSTEMTIMEToChar(&stFileTime, cpTimeFormat);
}

WCHAR* FILETIMEToWChar(const FILETIME* cpFileTime, const WCHAR* cpTimeFormat)
{
	SYSTEMTIME stFileTime;

	FileTimeToSystemTime(cpFileTime, &stFileTime);
	return SYSTEMTIMEToWChar(&stFileTime, cpTimeFormat);
}

char* UINT64ToFILETIMEToChar(const unsigned __int64 cFileTime, const char* cpTimeFormat)
{
	FILETIME ftFileTime;
	SYSTEMTIME stFileTime;

	ftFileTime.dwHighDateTime = (DWORD)(cFileTime >> 32);
	ftFileTime.dwLowDateTime = (DWORD)(cFileTime & 0xFFFFFFFF);
	FileTimeToSystemTime(&ftFileTime, &stFileTime);
	return SYSTEMTIMEToChar(&stFileTime, cpTimeFormat);
}

WCHAR* UINT64ToFILETIMEToWChar(const unsigned __int64 cFileTime, const WCHAR* cpTimeFormat)
{
	FILETIME ftFileTime;
	SYSTEMTIME stFileTime;

	ftFileTime.dwHighDateTime = (DWORD)(cFileTime >> 32);
	ftFileTime.dwLowDateTime = (DWORD)(cFileTime & 0xFFFFFFFF);
	FileTimeToSystemTime(&ftFileTime, &stFileTime);
	return SYSTEMTIMEToWChar(&stFileTime, cpTimeFormat);
}

char* SYSTEMTIMEToChar(const SYSTEMTIME* cpFileTime, const char* cpTimeFormat)
{
	struct tm tmFileTime;
	__time64_t tiFileTime;

	memset(&tmFileTime, 0, sizeof(tmFileTime));
	tmFileTime.tm_year = cpFileTime->wYear - 1900;
	tmFileTime.tm_mon = cpFileTime->wMonth - 1;
	tmFileTime.tm_mday = cpFileTime->wDay;
	tmFileTime.tm_hour = cpFileTime->wHour;
	tmFileTime.tm_min = cpFileTime->wMinute;
	tmFileTime.tm_sec = cpFileTime->wSecond;

#if _MSC_VER > 1300
	tiFileTime = _mktime64(&tmFileTime);
#else
	tiFileTime = mktime(&tmFileTime);
#endif

	return Time64ToChar(tiFileTime, cpTimeFormat);
}

WCHAR* SYSTEMTIMEToWChar(const SYSTEMTIME* cpFileTime, const WCHAR* cpTimeFormat)
{
	struct tm tmFileTime;
	__time64_t tiFileTime;

	memset(&tmFileTime, 0, sizeof(tmFileTime));
	tmFileTime.tm_year = cpFileTime->wYear - 1900;
	tmFileTime.tm_mon = cpFileTime->wMonth - 1;
	tmFileTime.tm_mday = cpFileTime->wDay;
	tmFileTime.tm_hour = cpFileTime->wHour;
	tmFileTime.tm_min = cpFileTime->wMinute;
	tmFileTime.tm_sec = cpFileTime->wSecond;

#if _MSC_VER > 1300
	tiFileTime = _mktime64(&tmFileTime);
#else
	tiFileTime = mktime(&tmFileTime);
#endif

	return Time64ToWChar(tiFileTime, cpTimeFormat);
}

char* Time64ToChar(__time64_t inTime, const char* cpTimeFormat)
{
	const char* pTimeFormat;
	char* cpBufChar;
	size_t iBufferSize;
	__time64_t tiTime;

	if (inTime) {
		tiTime = inTime;
	}
	else
	{

#if _MSC_VER > 1300
		tiTime = _time64(0);
#else
		tiTime = time(0);
#endif

	}

	if (cpTimeFormat != NULL) {
		pTimeFormat = cpTimeFormat;
	}
	else {
		pTimeFormat = CHAR_TIME_FORMAT;
	}

	iBufferSize = 100;
	cpBufChar = (char*)malloc(iBufferSize);
	IF_UNLIKELY(cpBufChar == NULL) {
		return NULL;
	}

#if _MSC_VER > 1300
#ifndef _NODLL

	struct tm tm;
	errno_t err;

	err = _localtime64_s(&tm, &tiTime);
	IF_UNLIKELY(err)
	{
		free(cpBufChar);
		return 0;
	}
	strftime(cpBufChar, iBufferSize, pTimeFormat, &tm);

#else

	struct tm* ptm;
	ptm = _localtime64(&tiTime);
	strftime(cpBufChar, iBufferSize, pTimeFormat, ptm);

#endif
#else

	struct tm* ptm;
	ptm = localtime(&tiTime);
	strftime(cpBufChar, iBufferSize, pTimeFormat, ptm);

#endif

	return cpBufChar;
}

WCHAR* Time64ToWChar(__time64_t inTime, const WCHAR* cpTimeFormat)
{
	const WCHAR* pTimeFormat;
	WCHAR* cpBufWChar;
	size_t iBufferSize;
	__time64_t tiTime;

	if (inTime) {
		tiTime = inTime;
	}
	else 
	{

#if _MSC_VER > 1300
		tiTime = _time64(0);
#else
		tiTime = time(0);
#endif

	}

	if (cpTimeFormat != NULL) {
		pTimeFormat = cpTimeFormat;
	}
	else {
		pTimeFormat = WCHAR_TIME_FORMAT;
	}

	iBufferSize = 100;
	cpBufWChar = (WCHAR*)malloc(iBufferSize * sizeof(WCHAR));
	IF_UNLIKELY(cpBufWChar == NULL) {
		return NULL;
	}

#if _MSC_VER > 1300
#ifndef _NODLL

	struct tm tm;
	errno_t err;

	err = _localtime64_s(&tm, &tiTime);
	IF_UNLIKELY(err)
	{
		free(cpBufWChar);
		return 0;
	}
	wcsftime(cpBufWChar, iBufferSize, pTimeFormat, &tm);

#else

	struct tm* ptm;
	ptm = _localtime64(&tiTime);
	wcsftime(cpBufWChar, iBufferSize, pTimeFormat, ptm);

#endif
#else

	struct tm* ptm;
	ptm = localtime(&tiTime);
	wcsftime(cpBufWChar, iBufferSize, pTimeFormat, ptm);

#endif
	return cpBufWChar;
}
