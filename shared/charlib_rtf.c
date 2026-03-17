#include "charlib_rtf.h"
#include <mbstring.h>

#define HEXCHAR_LOWER const char cpHexCharLower[16] = "0123456789abcdef"


size_t CharToRtfEncodeCharLength(const char* cpInChar)
{
	char* ptr = NULL;
	size_t nDstCount = 0;
	int nLang = 17;

	ptr = (char*)cpInChar;
	while (*ptr != '\0')
	{
		if (_ismbblead(*ptr))
		{
			if (nLang != 1041)
			{
				nLang = 1041;
				nDstCount += sizeof("\\lang1041") - 1;
			}
			nDstCount += 9;
		}
		else
		{
			char nCode = '\0';
			int nReturnCode = 0;

			switch (*ptr)
			{
			case '{':
			case '}':
			case '\\':
				nCode = *ptr;
				break;
			case '\r':
			case '\n':
				if ((*ptr + 1) == '\n') {
					ptr++;
				}
				nReturnCode++;
				break;
			}

			if (nCode != 0)
			{
				if (nLang != 1041)
				{
					nLang = 1041;
					nDstCount += sizeof("\\lang1041") - 1;
				}
				nDstCount += 4;
			}
			else if (nReturnCode)
			{
				if (nLang != 17)
				{
					nLang = 17;
					nDstCount += sizeof("\\lang17") - 1;
				}
				nDstCount += sizeof("\\line") - 1;
			}
			else
			{
				if (nLang != 17)
				{
					nLang = 17;
					nDstCount += sizeof("\\lang17") - 1;
					if (*ptr != ' ') {
						nDstCount++;
					}
				}
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
	int nLang = 17;

	if (pOutChar == NULL)
	{
		iBufferSize = CharToRtfEncodeCharLength(cpInChar);
		pOutChar = (char*)malloc(iBufferSize + 1);
		if (pOutChar == NULL) {
			return NULL;
		}
	}
	ptr = (char*)cpInChar;
	pDst = pOutChar;

	while (*ptr != '\0')
	{
		if (_ismbblead(*ptr))
		{
			if (nLang != 1041)
			{
				nLang = 1041;
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
			char nCode = '\0';
			int nReturnCode = 0;

			switch (*ptr)
			{
			case '{':
			case '}':
			case '\\':
				nCode = *ptr;
				break;
			case '\r':
			case '\n':
				if ((*ptr + 1) == '\n') {
					ptr++;
				}
				nReturnCode++;
				break;
			}

			if (nCode != 0)
			{
				if (nLang != 1041)
				{
					nLang = 1041;
					pDst = qstrcpy(pDst, "\\lang1041");
				}

				*pDst++ = '\\';
				*pDst++ = '\'';
				*pDst++ = cpHexCharLower[(nCode & 0xF0) >> 4];
				*pDst++ = cpHexCharLower[(nCode & 0x0F)];
			}
			else if (nReturnCode)
			{
				if (nLang != 17)
				{
					nLang = 17;
					pDst = qstrcpy(pDst, "\\lang17");
				}
				pDst = qstrcpy(pDst, "\\line");
			}
			else
			{
				if (nLang != 17)
				{
					nLang = 17;
					pDst = qstrcpy(pDst, "\\lang17");
					if (*ptr != ' ') {
						*pDst++ = ' ';
					}
				}
				*pDst++ = *ptr;
			}
		}
		ptr++;
	}
	*pDst = '\0';
	return pOutChar;
}

size_t WCharToRtfEncodeCharLength(const WCHAR* cpInWChar)
{
	WCHAR* ptr = NULL;
	size_t nDstCount = 0;
	int nLang = 17;
	int nUnicode = 0;

	ptr = (WCHAR*)cpInWChar;
	while (*ptr != '\0')
	{
		if (*ptr > 0x7f)
		{
			if (((*ptr) & ~((1 << 10) - 1)) == 0xD800)
			{
				char szCode[8];

				if (!nUnicode)
				{
					nUnicode++;
					nDstCount += sizeof("\\f10") - 1;
				}

				if (nLang != 17)
				{
					nLang = 17;
					nDstCount += sizeof("\\lang17") - 1;
				}

#ifndef _NODLL
				_itoa_s(*(short*)ptr, szCode, sizeof(szCode), 10);
#else
				_itoa(*(short*)ptr, szCode, 10);
#endif
				nDstCount += strlen(szCode) + 3;
				ptr++;

#ifndef _NODLL
				_itoa_s(*(short*)ptr, szCode, sizeof(szCode), 10);
#else
				_itoa(*(short*)ptr, szCode, 10);
#endif
				nDstCount += strlen(szCode) + 3;
			}
			else
			{
				char szCode[4];
				const int nRet = WideCharToMultiByte(CP_ACP, 0, ptr, 1, szCode, sizeof(szCode), NULL, NULL);

				if (nRet)
				{
					if (nUnicode)
					{
						nUnicode = 0;
						nDstCount += sizeof("\\f0") - 1;
					}

					if (nLang != 1041)
					{
						nLang = 1041;
						nDstCount += sizeof("\\lang1041") - 1;
					}


					nDstCount += 4;

					if (nRet > 1) {
						nDstCount += 4;
					}
				}
			}
		}
		else
		{
			char nCode = '\0';
			int nReturnCode = 0;

			switch (*ptr)
			{
			case '{':
			case '}':
			case '\\':
				nCode = (char)*ptr;
				break;
			case '\r':
			case '\n':
				if ((*ptr + 1) == '\n') {
					ptr++;
				}
				nReturnCode++;
				break;
			}

			if (nUnicode)
			{
				nUnicode = 0;
				nDstCount += sizeof("\\f0") - 1;
			}

			if (nCode != 0)
			{
				if (nLang != 1041)
				{
					nLang = 1041;
					nDstCount += sizeof("\\lang1041") - 1;
				}
				nDstCount += 4;
			}
			else if (nReturnCode)
			{
				if (nLang != 17)
				{
					nLang = 17;
					nDstCount += sizeof("\\lang17") - 1;
				}
				nDstCount += sizeof("\\line\r\n") - 1;
			}
			else
			{
				if (nLang != 17)
				{
					nLang = 17;
					nDstCount += sizeof("\\lang17") - 1;
					if (*ptr != ' ') {
						nDstCount++;
					}
				}
				nDstCount++;
			}
		}
		ptr++;
	}
	return nDstCount;
}

char* WCharToRtfEncodeChar(const WCHAR* cpInWChar, char* pOutChar)
{
	HEXCHAR_LOWER;
	WCHAR* ptr = NULL;
	char* pDst = NULL;
	size_t iBufferSize = 0;
	int nLang = 17;
	int nUnicode = 0;

	if (pOutChar == NULL)
	{
		iBufferSize = WCharToRtfEncodeCharLength(cpInWChar);
		pOutChar = (char*)malloc(iBufferSize + 1);
		if (pOutChar == NULL) {
			return NULL;
		}
	}
	ptr = (WCHAR*)cpInWChar;
	pDst = pOutChar;

	while (*ptr != '\0')
	{
		if (*ptr > 0x7f)
		{
			if (((*ptr) & ~((1 << 10) - 1)) == 0xD800)
			{
				char szCode[8];

				if (!nUnicode)
				{
					nUnicode++;
					pDst = qstrcpy(pDst, "\\f10");
				}

				if (nLang != 17)
				{
					nLang = 17;
					pDst = qstrcpy(pDst, "\\lang17");
				}

#ifndef _NODLL
				_ltoa_s(*(short*)ptr, szCode, sizeof(szCode), 10);
#else
				_ltoa(*(short*)ptr, szCode, 10);
#endif
				* pDst++ = '\\';
				*pDst++ = 'u';
				pDst = qstrcpy(pDst, szCode);
				*pDst++ = '?';
				ptr++;

#ifndef _NODLL
				_ltoa_s(*(short*)ptr, szCode, sizeof(szCode), 10);
#else
				_ltoa(*(short*)ptr, szCode, 10);
#endif
				* pDst++ = '\\';
				*pDst++ = 'u';
				pDst = qstrcpy(pDst, szCode);
				*pDst++ = '?';
			}
			else
			{
				char szCode[4];
				const int nRet = WideCharToMultiByte(CP_ACP, 0, ptr, 1, szCode, sizeof(szCode), NULL, NULL);

				if (nRet)
				{
					if (nUnicode)
					{
						nUnicode = 0;
						pDst = qstrcpy(pDst, "\\f0");
					}

					if (nLang != 1041)
					{
						nLang = 1041;
						pDst = qstrcpy(pDst, "\\lang1041");
					}

					*pDst++ = '\\';
					*pDst++ = '\'';
					*pDst++ = cpHexCharLower[(szCode[0] & 0xF0) >> 4];
					*pDst++ = cpHexCharLower[(szCode[0] & 0x0F)];

					if (nRet > 1)
					{
						*pDst++ = '\\';
						*pDst++ = '\'';
						*pDst++ = cpHexCharLower[(szCode[1] & 0xF0) >> 4];
						*pDst++ = cpHexCharLower[(szCode[1] & 0x0F)];
					}
				}
			}
		}
		else
		{
			char nCode = '\0';
			int nReturnCode = 0;

			switch (*ptr)
			{
			case '{':
			case '}':
			case '\\':
				nCode = (char)*ptr;
				break;
			case '\r':
			case '\n':
				if ((*ptr + 1) == '\n') {
					ptr++;
				}
				nReturnCode++;
				break;
			}

			if (nUnicode)
			{
				nUnicode = 0;
				pDst = qstrcpy(pDst, "\\f0");
			}

			if (nCode != 0)
			{
				if (nLang != 1041)
				{
					nLang = 1041;
					pDst = qstrcpy(pDst, "\\lang1041");
				}

				*pDst++ = '\\';
				*pDst++ = '\'';
				*pDst++ = cpHexCharLower[(nCode & 0xF0) >> 4];
				*pDst++ = cpHexCharLower[(nCode & 0x0F)];
			}
			else if (nReturnCode)
			{
				if (nLang != 17)
				{
					nLang = 17;
					pDst = qstrcpy(pDst, "\\lang17");
				}
				pDst = qstrcpy(pDst, "\\line\r\n");
			}
			else
			{
				if (nLang != 17)
				{
					nLang = 17;
					pDst = qstrcpy(pDst, "\\lang17");
					if (*ptr != ' ') {
						*pDst++ = ' ';
					}
				}
				*pDst++ = (char)*ptr;
			}
		}
		ptr++;
	}
	*pDst = '\0';
	return pOutChar;
}
