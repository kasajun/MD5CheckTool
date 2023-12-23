// clipboard.cpp

#include "clipboard.h"
#include "charlib.h"
#include "c20.h"


#define START_HTML 182

static char* ito010a(char* pText, int nValue)
{
	char szBuf[33] = "";
	char* p;
	size_t len;
#if _MSC_VER > 1300
	errno_t err;
#endif

	p = pText;

#if _MSC_VER > 1300
	err = _itoa_s(nValue, szBuf, 10);
	IF_LIKELY(err == 0)
	{
#else
	_itoa(nValue, szBuf, 10);
#endif

		len = 10 - strlen(szBuf);
		for (size_t i = 0; i < len; ++i) {
			*p++ = '0';
		}
		qstrcpy(p, szBuf);

#if _MSC_VER > 1300
	}
	else {
		qstrcpy(p, "0000000000");
	}
#endif
	return p;
}

static int _010atoi(const char* pText)
{
	char* p;

	p = (char*)pText;
	while (*p != '\0')
	{
		if (*p != '0') {
			break;
		}
		++p;
	}
	return atoi(p);
}

BOOL SetClipboardTextA(HWND hWnd, const char* pText, const char* pRtfText, const char* pHtmlText)
{
	WCHAR* pWCharText;
	HGLOBAL hg1 = NULL;
	HGLOBAL hg2 = NULL;
	int nRet = 0;

	pWCharText = CharToWCharConv(pText);
	IF_UNLIKELY(pWCharText == NULL) {
		return FALSE;
	}

	IF_UNLIKELY(!OpenClipboard(hWnd))
	{
		free(pWCharText);
		return FALSE;
	}

	nRet = (int)strlen(pText);
	hg1 = GlobalAlloc(GHND, (size_t)nRet + 1);
	nRet = (int)wcslen(pWCharText) * sizeof(WCHAR);
	hg2 = GlobalAlloc(GHND, (size_t)nRet + sizeof(WCHAR));

	EmptyClipboard();
	IF_LIKELY(hg1 != NULL)
	{
		char* pMem = (char*)GlobalLock(hg1);

		IF_UNLIKELY(pMem == NULL) {
			goto CF_TEXT_END;
		}
		qstrcpy(pMem, pText);
		GlobalUnlock(hg1);
		IF_UNLIKELY(SetClipboardData(CF_TEXT, hg1) == NULL) {
			GlobalFree(hg1);
		}
	}
CF_TEXT_END:

	IF_LIKELY(hg2 != NULL)
	{
		WCHAR* pMem = (WCHAR*)GlobalLock(hg2);

		IF_UNLIKELY(pMem == NULL) {
			goto CF_UNICODETEXT_END;
		}
		qwcscpy(pMem, pWCharText);
		GlobalUnlock(hg2);
		IF_UNLIKELY(SetClipboardData(CF_UNICODETEXT, hg2) == NULL) {
			GlobalFree(hg2);
		}
	}
CF_UNICODETEXT_END:

	if (pRtfText != NULL)
	{
		HGLOBAL hg3 = NULL;
		char* pMem;
		UINT uFormat;

		nRet = (int)strlen(pRtfText);
		hg3 = GlobalAlloc(GHND, (size_t)nRet + 1);
		IF_UNLIKELY(hg3 == NULL) {
			goto CF_RTFTEXT_END;
		}

		pMem = (char*)GlobalLock(hg3);
		IF_UNLIKELY(pMem == NULL) {
			goto CF_RTFTEXT_END;
		}
		qstrcpy(pMem, pRtfText);
		GlobalUnlock(hg3);
		uFormat = RegisterClipboardFormatA("Rich Text Format");
		IF_LIKELY(uFormat != 0)
		{
			IF_UNLIKELY(SetClipboardData(uFormat, hg3) == NULL)
			{
				GlobalFree(hg3);
				goto CF_RTFTEXT_END;
			}
		}
		uFormat = RegisterClipboardFormatA("RTF As Text");
		IF_LIKELY(uFormat != 0)
		{
			IF_UNLIKELY(SetClipboardData(uFormat, hg3) == NULL)
			{
				GlobalFree(hg3);
				goto CF_RTFTEXT_END;
			}
		}
	}
CF_RTFTEXT_END:

	if (pHtmlText != NULL)
	{
		HGLOBAL hg3 = NULL;
		char* pMem;

		nRet = (int)(strlen(pHtmlText) + START_HTML);
		hg3 = GlobalAlloc(GHND, (size_t)nRet + 1);
		IF_UNLIKELY(hg3 == NULL) {
			goto CF_HTMLTEXT_END;
		}

		pMem = (char*)GlobalLock(hg3);
		IF_LIKELY(pMem != NULL)
		{
			char szNum[12];

			const int nStartHTML = START_HTML;
			const int nEndHTML = nRet;
			const char* pStartFragment = strstr(pHtmlText, "<!--StartFragment-->");
			const int nStartFragment = pStartFragment != NULL ? (int)(pStartFragment - pHtmlText + nStartHTML + 22) : -1;
			const char* pEndFragment = strstr(pHtmlText, "<!--EndFragment-->");
			const int nEndFragment = pEndFragment != NULL ? (int)(pEndFragment - pHtmlText + nStartHTML - 1) : -1;
			char* pHtml = pMem;
			UINT uFormat;

			pHtml = qstrcpy(pHtml, "Version:1.0\r\nStartHTML:");
			//sprintf_s(szNum, "%010d", nStartHTML);
			ito010a(szNum, nStartHTML);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nEndHTML:");
			//sprintf_s(szNum, "%010d", nEndHTML);
			ito010a(szNum, nEndHTML);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nStartFragment:");
			//sprintf_s(szNum, "%010d", nStartFragment);
			ito010a(szNum, nStartFragment);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nEndFragment:");
			//sprintf_s(szNum, "%010d", nEndFragment);
			ito010a(szNum, nEndFragment);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nStartSelection:");
			//sprintf_s(szNum, "%010d", nStartFragment);
			ito010a(szNum, nStartFragment);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nEndSelection:");
			//sprintf_s(szNum, "%010d", nEndFragment);
			ito010a(szNum, nEndFragment);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nSourceURL:about:blank");
			pHtml = qstrcpy(pHtml, "\r\n\r\n");
			qstrcpy(pHtml, pHtmlText);
			GlobalUnlock(hg3);
			uFormat = RegisterClipboardFormatA("HTML Format");
			if (uFormat == NULL) {
				goto CF_HTMLTEXT_END;
			}
			if (SetClipboardData(uFormat, hg3) == NULL) {
				GlobalFree(hg3);
			}
		}
	}
CF_HTMLTEXT_END:

	CloseClipboard();
	free(pWCharText);
	return nRet;
}

BOOL SetClipboardTextW(HWND hWnd, const WCHAR* pText, const char* pRtfText, const char* pHtmlText)
{
	char* pCharText;
	HGLOBAL hg1 = NULL;
	HGLOBAL hg2 = NULL;
	int nRet = 0;

	pCharText = WCharToCharConv(pText);
	IF_UNLIKELY(pCharText == NULL) {
		return FALSE;
	}

	IF_UNLIKELY(!OpenClipboard(hWnd))
	{
		free(pCharText);
		return FALSE;
	}

	nRet = (int)wcslen(pText) * sizeof(WCHAR);
	hg1 = GlobalAlloc(GHND, (size_t)nRet + sizeof(WCHAR));
	nRet = (int)strlen(pCharText);
	hg2 = GlobalAlloc(GHND, (size_t)nRet + 1);

	EmptyClipboard();
	IF_LIKELY(hg2 != NULL)
	{
		char* pMem = (char*)GlobalLock(hg2);

		IF_UNLIKELY(pMem == NULL){
			goto CF_TEXT_END;
		}
		qstrcpy(pMem, pCharText);
		GlobalUnlock(hg2);
		if (SetClipboardData(CF_TEXT, hg2) == NULL) {
			GlobalFree(hg2);
		}
	}
CF_TEXT_END:

	IF_LIKELY(hg1 != NULL)
	{
		WCHAR* pMem = (WCHAR*)GlobalLock(hg1);

		IF_UNLIKELY(pMem == NULL) {
			goto CF_UNICODETEXT_END;
		}
		qwcscpy(pMem, pText);
		GlobalUnlock(hg1);
		IF_UNLIKELY(SetClipboardData(CF_UNICODETEXT, hg1) == NULL) {
			GlobalFree(hg1);
		}
	}
CF_UNICODETEXT_END:

	if (pRtfText != NULL)
	{
		HGLOBAL hg3 = NULL;
		char* pMem;
		UINT uFormat;

		nRet = (int)strlen(pRtfText);
		hg3 = GlobalAlloc(GHND, (size_t)nRet + 1);
		IF_UNLIKELY(hg3 == NULL) {
			goto CF_RTFTEXT_END;
		}
		pMem = (char*)GlobalLock(hg3);
		IF_UNLIKELY(pMem == NULL) {
			goto CF_RTFTEXT_END;
		}
		qstrcpy(pMem, pRtfText);
		GlobalUnlock(hg3);
		uFormat = RegisterClipboardFormatW(L"Rich Text Format");
		IF_LIKELY(uFormat != NULL)
		{
			IF_UNLIKELY(SetClipboardData(uFormat, hg3) == NULL)
			{
				GlobalFree(hg3);
				goto CF_RTFTEXT_END;
			}
		}
		uFormat = RegisterClipboardFormatW(L"RTF As Text");
		IF_LIKELY(uFormat != NULL)
		{
			IF_UNLIKELY(SetClipboardData(uFormat, hg3) == NULL)
			{
				GlobalFree(hg3);
				goto CF_RTFTEXT_END;
			}
		}
	}
CF_RTFTEXT_END:

	if (pHtmlText != NULL)
	{
		HGLOBAL hg3 = NULL;
		char* pMem;

		nRet = (int)(strlen(pHtmlText) + START_HTML);
		hg3 = GlobalAlloc(GHND, (size_t)nRet + 1);
		IF_UNLIKELY(hg3 == NULL) {
			goto CF_HTMLTEXT_END;
		}

		pMem = (char*)GlobalLock(hg3);
		IF_LIKELY(pMem != NULL)
		{
			char szNum[12];

			const int nStartHTML = START_HTML;
			const int nEndHTML = nRet;
			const char* pStartFragment = strstr(pHtmlText, "<!--StartFragment-->");
			const int nStartFragment = pStartFragment != NULL ? (int)(pStartFragment - pHtmlText + nStartHTML + 22) : -1;
			const char* pEndFragment = strstr(pHtmlText, "<!--EndFragment-->");
			const int nEndFragment = pEndFragment != NULL ? (int)(pEndFragment - pHtmlText + nStartHTML - 1) : -1;
			char* pHtml = pMem;
			UINT uFormat;

			pHtml = qstrcpy(pHtml, "Version:1.0\r\nStartHTML:");
			//sprintf_s(szNum, "%010d", nStartHTML);
			ito010a(szNum, nStartHTML);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nEndHTML:");
			//sprintf_s(szNum, "%010d", nEndHTML);
			ito010a(szNum, nEndHTML);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nStartFragment:");
			//sprintf_s(szNum, "%010d", nStartFragment);
			ito010a(szNum, nStartFragment);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nEndFragment:");
			//sprintf_s(szNum, "%010d", nEndFragment);
			ito010a(szNum, nEndFragment);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nStartSelection:");
			//sprintf_s(szNum, "%010d", nStartFragment);
			ito010a(szNum, nStartFragment);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nEndSelection:");
			//sprintf_s(szNum, "%010d", nEndFragment);
			ito010a(szNum, nEndFragment);
			pHtml = qstrcpy(pHtml, szNum);
			pHtml = qstrcpy(pHtml, "\r\nSourceURL:about:blank");
			pHtml = qstrcpy(pHtml, "\r\n\r\n");
			qstrcpy(pHtml, pHtmlText);
			GlobalUnlock(hg3);
			uFormat = RegisterClipboardFormatW(L"HTML Format");
			IF_LIKELY(uFormat != NULL)
			{
				IF_UNLIKELY(SetClipboardData(uFormat, hg3) == NULL) {
					GlobalFree(hg3);
				}
			}
		}
	}
CF_HTMLTEXT_END:

	CloseClipboard();
	free(pCharText);
	return nRet;
}

DWORD GetClipboardTextA(HWND hWnd, char* lpText, size_t size)
{
	HANDLE hMem = NULL;
	char* pText;
	int nRet = 0;

	if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) {
		return FALSE;
	}

	if (!OpenClipboard(hWnd)) {
		return FALSE;
	}

	hMem = GetClipboardData(CF_TEXT);
	IF_UNLIKELY(hMem == NULL)
	{
		CloseClipboard();
		return FALSE;
	}

	pText = (char*)GlobalLock(hMem);
	IF_UNLIKELY(pText == NULL)
	{
		GlobalUnlock(hMem);
		CloseClipboard();
		return FALSE;
	}

	nRet = (int)strlen(pText);
	if (lpText != NULL)
	{
		qstrncpy(lpText, pText, size);
		if ((size_t)nRet > size) {
			*(lpText + (size - 1)) = '\0';
		}
	}

	GlobalUnlock(hMem);
	CloseClipboard();
	return nRet;
}

DWORD GetClipboardTextW(HWND hWnd, WCHAR* lpText, size_t size)
{
	HANDLE hMem = NULL;
	WCHAR* pText;
	int nRet = 0;

	if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) {
		return FALSE;
	}

	if (!OpenClipboard(hWnd)) {
		return FALSE;
	}

	hMem = GetClipboardData(CF_UNICODETEXT);
	IF_UNLIKELY(hMem == NULL)
	{
		CloseClipboard();
		return FALSE;
	}

	pText = (WCHAR*)GlobalLock(hMem);
	IF_UNLIKELY(pText == NULL)
	{
		GlobalUnlock(hMem);
		CloseClipboard();
		return FALSE;
	}

	nRet = (int)wcslen(pText);
	if (lpText != NULL)
	{
		qwcsncpy(lpText, pText, size);
		if ((size_t)nRet > size) {
			*(lpText + (size - 1)) = '\0';
		}
	}

	GlobalUnlock(hMem);
	CloseClipboard();
	return nRet;
}

char* GetClipboardMallocTextA(HWND hWnd)
{
	HANDLE hMem = NULL;
	char* pText;
	char* pBuf;
	int nRet = 0;

	if (!IsClipboardFormatAvailable(CF_TEXT)) {
		return NULL;
	}

	if (!OpenClipboard(hWnd)) {
		return NULL;
	}

	hMem = GetClipboardData(CF_TEXT);
	IF_UNLIKELY(hMem == NULL)
	{
		CloseClipboard();
		return NULL;
	}

	pText = (char*)GlobalLock(hMem);
	IF_UNLIKELY(pText == NULL)
	{
		GlobalUnlock(hMem);
		CloseClipboard();
		return NULL;
	}

	nRet = (int)strlen(pText);
	pBuf = (char*)malloc(((size_t)nRet + 1) * sizeof(char));
	IF_LIKELY(pBuf != NULL)
	{
		qstrncpy(pBuf, pText, nRet);
		*(pBuf + nRet) = '\0';
	}

	GlobalUnlock(hMem);
	CloseClipboard();
	return pBuf;
}

WCHAR* GetClipboardMallocTextW(HWND hWnd)
{
	HANDLE hMem = NULL;
	WCHAR* pText;
	WCHAR* pBuf;
	int nRet = 0;

	if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) {
		return NULL;
	}

	if (!OpenClipboard(hWnd)) {
		return NULL;
	}

	hMem = GetClipboardData(CF_UNICODETEXT);
	IF_UNLIKELY(hMem == NULL)
	{
		CloseClipboard();
		return NULL;
	}

	pText = (WCHAR*)GlobalLock(hMem);
	IF_UNLIKELY(pText == NULL)
	{
		GlobalUnlock(hMem);
		CloseClipboard();
		return NULL;
	}

	nRet = (int)wcslen(pText);
	pBuf = (WCHAR*)malloc(((size_t)nRet + 1) * sizeof(WCHAR));
	IF_LIKELY(pBuf != NULL)
	{
		qwcsncpy(pBuf, pText, nRet);
		*(pBuf + nRet) = '\0';
	}

	GlobalUnlock(hMem);
	CloseClipboard();
	return pBuf;
}
