#include "BinaryView.h"
#include "c20.h"
#include <mbctype.h>


#pragma warning(disable:4996)
const char cpHexChar[] = "0123456789ABCDEF";


#ifndef WCHAR_UNALIGNED
#define WCHAR_UNALIGNED(X) ((size_t)(X) & (sizeof(WCHAR) - 1))
#endif


size_t VariableView_CoreA(char* pOutChar, QWORD nInVariable, DWORD dwType, int nIsHex, int nIsVariable)
{
	char* ptr = pOutChar;
	char szNum[33];

	if (nIsVariable)
	{
		*ptr++ = ' ';
		*ptr++ = '=';
		*ptr++ = ' ';
	}

	switch (dwType)
	{
	case ARRAYVIEW_TYPE_BYTE:
		if (nIsHex)
		{
			BYTE b;

			*ptr++ = '0';
			*ptr++ = 'x';
			b = (BYTE)nInVariable;
			*ptr++ = cpHexChar[(b & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b & 0x0F)];
		}
		else
		{
			_itoa((char)nInVariable, szNum, 10);
			ptr = qstrcpy(ptr, szNum);
		}
		break;
	case ARRAYVIEW_TYPE_WORD:
		if (nIsHex)
		{
			BYTE b[sizeof(WORD)];

			*ptr++ = '0';
			*ptr++ = 'x';
			memcpy(b, &nInVariable, sizeof(WORD));
			*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[1] & 0x0F)];
			*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[0] & 0x0F)];
		}
		else
		{
			_itoa((short)nInVariable, szNum, 10);
			ptr = qstrcpy(ptr, szNum);
		}
		break;
	case 0:
	case ARRAYVIEW_TYPE_DWORD:
		if (nIsHex)
		{
			BYTE b[sizeof(DWORD)];

			*ptr++ = '0';
			*ptr++ = 'x';
			memcpy(b, &nInVariable, sizeof(DWORD));
			*ptr++ = cpHexChar[(b[3] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[3] & 0x0F)];
			*ptr++ = cpHexChar[(b[2] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[2] & 0x0F)];
			*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[1] & 0x0F)];
			*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[0] & 0x0F)];
		}
		else
		{
			_itoa((int)nInVariable, szNum, 10);
			ptr = qstrcpy(ptr, szNum);
		}
		break;
	case ARRAYVIEW_TYPE_QWORD:
		if (nIsHex)
		{
			BYTE b[sizeof(unsigned __int64)];

			*ptr++ = '0';
			*ptr++ = 'x';
			memcpy(b, &nInVariable, sizeof(unsigned __int64));
			*ptr++ = cpHexChar[(b[7] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[7] & 0x0F)];
			*ptr++ = cpHexChar[(b[6] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[6] & 0x0F)];
			*ptr++ = cpHexChar[(b[5] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[5] & 0x0F)];
			*ptr++ = cpHexChar[(b[4] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[4] & 0x0F)];
			*ptr++ = cpHexChar[(b[3] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[3] & 0x0F)];
			*ptr++ = cpHexChar[(b[2] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[2] & 0x0F)];
			*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[1] & 0x0F)];
			*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[0] & 0x0F)];
		}
		else
		{
			_i64toa((__int64)nInVariable, szNum, 10);
			ptr = qstrcpy(ptr, szNum);
		}
		break;
	case ARRAYVIEW_TYPE_CHAR:
		{
			char* c = (char*)nInVariable;

			if (c)
			{ 
				*ptr++ = '\"';
				ptr = qstrcpy(ptr, c);
				*ptr++ = '\"';
			}
			else
			{
				*ptr++ = '(';
				*ptr++ = 'N';
				*ptr++ = 'U';
				*ptr++ = 'L';
				*ptr++ = 'L';
				*ptr++ = ')';
			}
		}
		break;
	case ARRAYVIEW_TYPE_WCHAR:
		{
			WCHAR* w = (WCHAR*)nInVariable;

			if (w)
			{
				WCHAR* wptr = w;

				*ptr++ = 'L';
				*ptr++ = '\"';
				while (*wptr)
				{
					if (*wptr >= 0x80)
					{
						const int nRet = qwctomb((char*)ptr, wptr);
						ptr += nRet;

						if (((*wptr) & ~((1 << 10) - 1)) == 0xD800) {
							wptr++;
						}
					}
					else {
						*ptr++ = (BYTE)*wptr;
					}
					wptr++;
				}
				*ptr++ = '\"';
			}
			else
			{
				*ptr++ = '(';
				*ptr++ = 'N';
				*ptr++ = 'U';
				*ptr++ = 'L';
				*ptr++ = 'L';
				*ptr++ = ')';
			}
		}
		break;
	case ARRAYVIEW_TYPE_DOUBLE:
		{
			double d = *(double*)nInVariable;
			sprintf(szNum, "%.2f", d);
			ptr = qstrcpy(ptr, szNum);
		}
		break;
	}
	if (nIsVariable) {
		*ptr++ = ';';
	}
	*ptr++ = '\r';
	*ptr++ = '\n';
	*ptr = '\0';
	return ptr - pOutChar;
}

size_t VariableView_CoreW(WCHAR* pOutWChar, QWORD nInVariable, DWORD dwType, int nIsHex, int nIsVariable)
{
	WCHAR* ptr = pOutWChar;
	WCHAR szNum[33];

	if (nIsVariable)
	{
		*ptr++ = ' ';
		*ptr++ = '=';
		*ptr++ = ' ';
	}

	switch (dwType)
	{
	case ARRAYVIEW_TYPE_BYTE:
		if (nIsHex)
		{
			BYTE b;

			*ptr++ = '0';
			*ptr++ = 'x';
			b = (BYTE)nInVariable;
			*ptr++ = cpHexChar[(b & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b & 0x0F)];
		}
		else
		{
			_itow((char)nInVariable, szNum, 10);
			ptr = qwcscpy(ptr, szNum);
		}
		break;
	case ARRAYVIEW_TYPE_WORD:
		if (nIsHex)
		{
			BYTE b[sizeof(WORD)];

			*ptr++ = '0';
			*ptr++ = 'x';
			memcpy(b, &nInVariable, sizeof(WORD));
			*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[1] & 0x0F)];
			*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[0] & 0x0F)];
		}
		else
		{
			_itow((short)nInVariable, szNum, 10);
			ptr = qwcscpy(ptr, szNum);
		}
		break;
	case 0:
	case ARRAYVIEW_TYPE_DWORD:
		if (nIsHex)
		{
			BYTE b[sizeof(DWORD)];

			*ptr++ = '0';
			*ptr++ = 'x';
			memcpy(b, &nInVariable, sizeof(DWORD));
			*ptr++ = cpHexChar[(b[3] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[3] & 0x0F)];
			*ptr++ = cpHexChar[(b[2] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[2] & 0x0F)];
			*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[1] & 0x0F)];
			*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[0] & 0x0F)];
		}
		else
		{
			_itow((int)nInVariable, szNum, 10);
			ptr = qwcscpy(ptr, szNum);
		}
		break;
	case ARRAYVIEW_TYPE_QWORD:
		if (nIsHex)
		{
			BYTE b[sizeof(unsigned __int64)];

			*ptr++ = '0';
			*ptr++ = 'x';
			memcpy(b, &nInVariable, sizeof(unsigned __int64));
			*ptr++ = cpHexChar[(b[7] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[7] & 0x0F)];
			*ptr++ = cpHexChar[(b[6] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[6] & 0x0F)];
			*ptr++ = cpHexChar[(b[5] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[5] & 0x0F)];
			*ptr++ = cpHexChar[(b[4] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[4] & 0x0F)];
			*ptr++ = cpHexChar[(b[3] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[3] & 0x0F)];
			*ptr++ = cpHexChar[(b[2] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[2] & 0x0F)];
			*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[1] & 0x0F)];
			*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
			*ptr++ = cpHexChar[(b[0] & 0x0F)];
		}
		else
		{
			_i64tow((__int64)nInVariable, szNum, 10);
			ptr = qwcscpy(ptr, szNum);
		}
		break;
	case ARRAYVIEW_TYPE_CHAR:
		{
			char* c = (char*)nInVariable;

			if (c)
			{
				char* cptr = c;

				*ptr++ = '\"';
				while (*cptr)
				{
					const int nRet = qmbtowc(ptr, (char*)cptr);
					ptr++;
					cptr += nRet;
				}
				*ptr++ = '\"';
			}
			else
			{
				*ptr++ = '(';
				*ptr++ = 'N';
				*ptr++ = 'U';
				*ptr++ = 'L';
				*ptr++ = 'L';
				*ptr++ = ')';
			}
		}
		break;
	case ARRAYVIEW_TYPE_WCHAR:
		{
			WCHAR* w = (WCHAR*)nInVariable;

			if (w)
			{
				*ptr++ = 'L';
				*ptr++ = '\"';
				ptr = qwcscpy(ptr, w);
				*ptr++ = '\"';
			}
			else
			{
				*ptr++ = '(';
				*ptr++ = 'N';
				*ptr++ = 'U';
				*ptr++ = 'L';
				*ptr++ = 'L';
				*ptr++ = ')';
			}
		}
		break;
	case ARRAYVIEW_TYPE_DOUBLE:
		{
			double d = *(double*)nInVariable;
			swprintf(szNum, L"%.2f", d);
			ptr = qwcscpy(ptr, szNum);
		}
		break;
	}
	if (nIsVariable) {
		*ptr++ = ';';
	}
	*ptr++ = '\r';
	*ptr++ = '\n';
	*ptr = '\0';
	return ptr - pOutWChar;
}

size_t VariableViewA(char* pOutChar, QWORD nInVariable, DWORD dwType, int nIsHex, int nIsVariable)
{
	if (pOutChar == NULL) {
		return FALSE;
	}
	return VariableView_CoreA(pOutChar, nInVariable, dwType, nIsHex, nIsVariable);
}

size_t VariableViewW(WCHAR* pOutWChar, QWORD nInVariable, DWORD dwType, int nIsHex, int nIsVariable)
{
	if (pOutWChar == NULL) {
		return FALSE;
	}
	return VariableView_CoreW(pOutWChar, nInVariable, dwType, nIsHex, nIsVariable);
}

size_t VariableView_LengthA(QWORD nInVariable, DWORD dwType, int nIsHex, int nIsVariable)
{
	char szNum[33];
	size_t nRet = 7;

	if (nIsVariable) {
		nRet -= 4;
	}

	switch (dwType)
	{
	case ARRAYVIEW_TYPE_BYTE:
		if(nIsHex)
		{
			nRet += sizeof(BYTE) * 2;
			nRet += 2;
		}
		else
		{
			_itoa((char)nInVariable, szNum, 10);
			nRet += strlen(szNum);
		}
		break;
	case ARRAYVIEW_TYPE_WORD:
		if(nIsHex)
		{
			nRet += sizeof(WORD) * 2;
			nRet += 2;
		}
		else
		{
			_itoa((short)nInVariable, szNum, 10);
			nRet += strlen(szNum);
		}
		break;
	case ARRAYVIEW_TYPE_DWORD:
		if(nIsHex)
		{
			nRet += sizeof(DWORD) * 2;
			nRet += 2;
		}
		else
		{
			_itoa((int)nInVariable, szNum, 10);
			nRet += strlen(szNum);
		}
		break;
	case ARRAYVIEW_TYPE_QWORD:
		if(nIsHex)
		{
			nRet += sizeof(unsigned __int64) * 2;
			nRet += 2;
		}
		else
		{
			_i64toa((__int64)nInVariable, szNum, 10);
			nRet += strlen(szNum);
		}
		break;
	case ARRAYVIEW_TYPE_CHAR:
		{
			char* c = (char*)nInVariable;

			if (c)
			{
				nRet += strlen(c);
				nRet += 2;
			}
			else {
				nRet += 6;
			}
		}
		break;
	case ARRAYVIEW_TYPE_WCHAR:
		{
			WCHAR* c = (WCHAR*)nInVariable;

			if (c)
			{
				nRet += WCharToCharLength(c);
				nRet += 3;
			}
			else {
				nRet += 6;
			}
		}
		break;
	case ARRAYVIEW_TYPE_DOUBLE:
		{
			double d = *(double*)nInVariable;
			sprintf(szNum, "%.2f", d);
			nRet += strlen(szNum);
		}
		break;
	}
	return nRet;
}

size_t VariableView_LengthW(QWORD nInVariable, DWORD dwType, int nIsHex, int nIsVariable)
{
	WCHAR szNum[33];
	size_t nRet = 7;

	if (nIsVariable) {
		nRet -= 4;
	}

	switch (dwType)
	{
	case ARRAYVIEW_TYPE_BYTE:
		if(nIsHex)
		{
			nRet += sizeof(BYTE) * 2;
			nRet += 2;
		}
		else
		{
			_itow((char)nInVariable, szNum, 10);
			nRet += wcslen(szNum);
		}
		break;
	case ARRAYVIEW_TYPE_WORD:
		if(nIsHex)
		{
			nRet += sizeof(WORD) * 2;
			nRet += 2;
		}
		else
		{
			_itow((short)nInVariable, szNum, 10);
			nRet += wcslen(szNum);
		}
		break;
	case ARRAYVIEW_TYPE_DWORD:
		if(nIsHex)
		{
			nRet += sizeof(DWORD) * 2;
			nRet += 2;
		}
		else
		{
			_itow((int)nInVariable, szNum, 10);
			nRet += wcslen(szNum);
		}
		break;
	case ARRAYVIEW_TYPE_QWORD:
		if(nIsHex)
		{
			nRet += sizeof(unsigned __int64) * 2;
			nRet += 2;
		}
		else
		{
			_i64tow((__int64)nInVariable, szNum, 10);
			nRet += wcslen(szNum);
		}
		break;
	case ARRAYVIEW_TYPE_CHAR:
		{
			char* c = (char*)nInVariable;

			if (c)
			{
				nRet += CharToWCharLength(c);
				nRet += 2;
			}
			else {
				nRet += 6;
			}
		}
		break;
	case ARRAYVIEW_TYPE_WCHAR:
		{
			WCHAR* c = (WCHAR*)nInVariable;

			if (c)
			{
				nRet += wcslen(c);
				nRet += 3;
			}
			else {
				nRet += 6;
			}
		}
		break;
	case ARRAYVIEW_TYPE_DOUBLE:
		{
			double d = *(double*)nInVariable;
			swprintf(szNum, L"%.2f", d);
			nRet += wcslen(szNum);
		}
		break;
	}
	return nRet;
}

size_t CharToVariableViewA(char* pOutChar, const char* nInChar, int nIsVariable)
{
	return VariableViewA(pOutChar, (size_t)nInChar, ARRAYVIEW_TYPE_CHAR, 0, nIsVariable);
}

size_t WCharToVariableViewA(char* pOutChar, const WCHAR* nInWChar, int nIsVariable)
{
	return VariableViewA(pOutChar, (size_t)nInWChar, ARRAYVIEW_TYPE_CHAR, 0, nIsVariable);
}

size_t CharToVariableViewW(WCHAR* pOutWChar, const char* nInChar, int nIsVariable)
{
	return VariableViewW(pOutWChar, (size_t)nInChar, ARRAYVIEW_TYPE_WCHAR, 0, nIsVariable);
}

size_t WCharToVariableViewW(WCHAR* pOutWChar, const WCHAR* nInWChar, int nIsVariable)
{
	return VariableViewW(pOutWChar, (size_t)nInWChar, ARRAYVIEW_TYPE_WCHAR, 0, nIsVariable);
}

size_t DoubleToVariableViewA(char* pOutChar, double nInDouble, int nIsVariable)
{
	return VariableViewA(pOutChar, (size_t)&nInDouble, ARRAYVIEW_TYPE_DOUBLE, 0, nIsVariable);
}

size_t DoubleToVariableViewW(WCHAR* pOutWChar, double nInDouble, int nIsVariable)
{
	return VariableViewW(pOutWChar, (size_t)&nInDouble, ARRAYVIEW_TYPE_DOUBLE, 0, nIsVariable);
}

char* VariableViewToCharA(QWORD nInVariable, DWORD dwType, int nIsHex, int nIsVariable)
{
	char* pOutChar;
	size_t nLen;

	nLen = VariableView_LengthA(nInVariable, dwType, nIsHex, nIsVariable);
	pOutChar = (char*)malloc(nLen);
	IF_UNLIKELY(pOutChar == NULL) {
		return NULL;
	}
	VariableView_CoreA(pOutChar, nInVariable, dwType, nIsHex, nIsVariable);
	return pOutChar;
}

WCHAR* VariableViewToWCharW(QWORD nInVariable, DWORD dwType, int nIsHex, int nIsVariable)
{
	WCHAR* pOutWChar;
	size_t nLen;

	nLen = VariableView_LengthW(nInVariable, dwType, nIsHex, nIsVariable);
	pOutWChar = (WCHAR*)malloc(nLen * sizeof(WCHAR));
	IF_UNLIKELY(pOutWChar == NULL) {
		return NULL;
	}
	VariableView_CoreW(pOutWChar, nInVariable, dwType, nIsHex, nIsVariable);
	return pOutWChar;
}

char* CharToVariableViewAToCharA(const char* nInChar, int nIsVariable)
{
	return VariableViewToCharA((size_t)nInChar, ARRAYVIEW_TYPE_CHAR, FALSE, nIsVariable);
}

char* WCharToVariableViewAToCharA(const WCHAR* nInWChar, int nIsVariable)
{
	return VariableViewToCharA((size_t)nInWChar, ARRAYVIEW_TYPE_WCHAR, FALSE, nIsVariable);
}

WCHAR* CharToVariableViewWToWCharW(const char* nInChar, int nIsVariable)
{
	return VariableViewToWCharW((size_t)nInChar, ARRAYVIEW_TYPE_CHAR, FALSE, nIsVariable);
}

WCHAR* WCharToVariableViewWToWCharW(const WCHAR* nInWChar, int nIsVariable)
{
	return VariableViewToWCharW((size_t)nInWChar, ARRAYVIEW_TYPE_WCHAR, FALSE, nIsVariable);
}

char* DoubleToVariableViewToCharA(double nInDouble, int nIsVariable)
{
	return VariableViewToCharA((size_t)&nInDouble, ARRAYVIEW_TYPE_DOUBLE, FALSE, nIsVariable);
}

WCHAR* DoubleToVariableViewToWCharW(double nInDouble, int nIsVariable)
{
	return VariableViewToWCharW((size_t)&nInDouble, ARRAYVIEW_TYPE_DOUBLE, FALSE, nIsVariable);
}


size_t ArrayView_CoreA(char* pOutChar, const VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex)
{
	char* ptr = pOutChar;
	BYTE* btr = (BYTE*)pInArray;
	char szNum[33];

	*ptr++ = '[';
	*ptr++ = ']';
	*ptr++ = ' ';
	*ptr++ = '=';
	*ptr++ = ' ';
	*ptr++ = '{';
	*ptr++ = '\r';
	*ptr++ = '\n';

	switch (dwType)
	{
	case ARRAYVIEW_TYPE_BYTE:
		if (nIsHex)
		{
			BYTE b;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = '0';
				*ptr++ = 'x';
				b = *btr++;
				*ptr++ = cpHexChar[(b & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b & 0x0F)];
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		else
		{
			BYTE b;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				b = *btr++;
				_itoa((char)b, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_WORD:
		if (nIsHex)
		{
			BYTE b[sizeof(WORD)];
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = '0';
				*ptr++ = 'x';
				memcpy(b, btr, sizeof(WORD));
				btr += sizeof(WORD);
				*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[1] & 0x0F)];
				*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[0] & 0x0F)];
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		else
		{
			WORD w;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&w, btr, sizeof(WORD));
				btr += sizeof(WORD);
				_itoa((short)w, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_DWORD:
		if (nIsHex)
		{
			BYTE b[sizeof(DWORD)];
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = '0';
				*ptr++ = 'x';
				memcpy(b, btr, sizeof(DWORD));
				btr += sizeof(DWORD);
				*ptr++ = cpHexChar[(b[3] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[3] & 0x0F)];
				*ptr++ = cpHexChar[(b[2] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[2] & 0x0F)];
				*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[1] & 0x0F)];
				*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[0] & 0x0F)];
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		else
		{
			DWORD d;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&d, btr, sizeof(DWORD));
				btr += sizeof(DWORD);
				_itoa((int)d, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_QWORD:
		if (nIsHex)
		{
			BYTE b[sizeof(unsigned __int64)];
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = '0';
				*ptr++ = 'x';
				memcpy(b, btr, sizeof(unsigned __int64));
				btr += sizeof(unsigned __int64);
				*ptr++ = cpHexChar[(b[7] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[7] & 0x0F)];
				*ptr++ = cpHexChar[(b[6] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[6] & 0x0F)];
				*ptr++ = cpHexChar[(b[5] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[5] & 0x0F)];
				*ptr++ = cpHexChar[(b[4] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[4] & 0x0F)];
				*ptr++ = cpHexChar[(b[3] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[3] & 0x0F)];
				*ptr++ = cpHexChar[(b[2] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[2] & 0x0F)];
				*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[1] & 0x0F)];
				*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[0] & 0x0F)];
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		else
		{
			unsigned __int64 q;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&q, btr, sizeof(unsigned __int64));
				btr += sizeof(unsigned __int64);
				_i64toa(q, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_CHAR:
		{
			char* c;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&c, btr, sizeof(char*));
				btr += sizeof(char*);
				if (c)
				{ 
					*ptr++ = '\"';
					ptr = qstrcpy(ptr, c);
					*ptr++ = '\"';
				}
				else
				{
					*ptr++ = '(';
					*ptr++ = 'N';
					*ptr++ = 'U';
					*ptr++ = 'L';
					*ptr++ = 'L';
					*ptr++ = ')';
				}
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_WCHAR:
		{
			WCHAR* w;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = 'L';
				*ptr++ = '\"';
				memcpy(&w, btr, sizeof(WCHAR*));
				btr += sizeof(WCHAR*);
				if (w)
				{
					WCHAR* wptr = w;

					while (*wptr)
					{
						if (*wptr >= 0x80)
						{
							const int nRet = qwctomb((char*)ptr, wptr);
							ptr += nRet;

							if (((*wptr) & ~((1 << 10) - 1)) == 0xD800) {
								wptr++;
							}
						}
						else {
							*ptr++ = (BYTE)*wptr;
						}
						wptr++;
					}
					*ptr++ = '\"';
				}
				else
				{
					*ptr++ = '(';
					*ptr++ = 'N';
					*ptr++ = 'U';
					*ptr++ = 'L';
					*ptr++ = 'L';
					*ptr++ = ')';
				}
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itoa((int)i, szNum, 10);
				ptr = qstrcpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;

	}
	*ptr++ = '}';
	*ptr++ = ';';
	*ptr++ = '\r';
	*ptr++ = '\n';
	*ptr = '\0';
	return ptr - pOutChar;
}

size_t ArrayView_CoreW(WCHAR* pOutWChar, const VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex)
{
	WCHAR* ptr = pOutWChar;
	BYTE* btr = (BYTE*)pInArray;
	WCHAR szNum[33];

	*ptr++ = '[';
	*ptr++ = ']';
	*ptr++ = ' ';
	*ptr++ = '=';
	*ptr++ = ' ';
	*ptr++ = '{';
	*ptr++ = '\r';
	*ptr++ = '\n';

	switch (dwType)
	{
	case ARRAYVIEW_TYPE_BYTE:
		if (nIsHex)
		{
			BYTE b;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = '0';
				*ptr++ = 'x';
				b = *btr++;
				*ptr++ = cpHexChar[(b & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b & 0x0F)];
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		else
		{
			BYTE b;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				b = *btr++;
				_itow((char)b, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_WORD:
		if (nIsHex)
		{
			BYTE b[sizeof(WORD)];
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = '0';
				*ptr++ = 'x';
				memcpy(b, btr, sizeof(WORD));
				btr += sizeof(WORD);
				*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[1] & 0x0F)];
				*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[0] & 0x0F)];
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		else
		{
			WORD w;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&w, btr, sizeof(WORD));
				btr += sizeof(WORD);
				_itow((short)w, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_DWORD:
		if (nIsHex)
		{
			BYTE b[sizeof(DWORD)];
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = '0';
				*ptr++ = 'x';
				memcpy(b, btr, sizeof(DWORD));
				btr += sizeof(DWORD);
				*ptr++ = cpHexChar[(b[3] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[3] & 0x0F)];
				*ptr++ = cpHexChar[(b[2] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[2] & 0x0F)];
				*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[1] & 0x0F)];
				*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[0] & 0x0F)];
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		else
		{
			DWORD d;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&d, btr, sizeof(DWORD));
				btr += sizeof(DWORD);
				_itow((int)d, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_QWORD:
		if (nIsHex)
		{
			BYTE b[sizeof(unsigned __int64)];
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				*ptr++ = '0';
				*ptr++ = 'x';
				memcpy(b, btr, sizeof(unsigned __int64));
				btr += sizeof(unsigned __int64);
				*ptr++ = cpHexChar[(b[7] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[7] & 0x0F)];
				*ptr++ = cpHexChar[(b[6] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[6] & 0x0F)];
				*ptr++ = cpHexChar[(b[5] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[5] & 0x0F)];
				*ptr++ = cpHexChar[(b[4] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[4] & 0x0F)];
				*ptr++ = cpHexChar[(b[3] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[3] & 0x0F)];
				*ptr++ = cpHexChar[(b[2] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[2] & 0x0F)];
				*ptr++ = cpHexChar[(b[1] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[1] & 0x0F)];
				*ptr++ = cpHexChar[(b[0] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(b[0] & 0x0F)];
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		else
		{
			unsigned __int64 q;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&q, btr, sizeof(unsigned __int64));
				btr += sizeof(unsigned __int64);
				_i64tow(q, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_CHAR:
		{
			char* c;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&c, btr, sizeof(char*));
				btr += sizeof(char*);
				if (c)
				{
					char* cptr = c;

					*ptr++ = '\"';
					while (*cptr)
					{
						const int nRet = qmbtowc(ptr, (char*)cptr);
						ptr++;
						cptr += nRet;
					}
					*ptr++ = '\"';
				}
				else
				{
					*ptr++ = '(';
					*ptr++ = 'N';
					*ptr++ = 'U';
					*ptr++ = 'L';
					*ptr++ = 'L';
					*ptr++ = ')';
				}
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;
	case ARRAYVIEW_TYPE_WCHAR:
		{
			WCHAR* w;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				*ptr++ = '\t';
				memcpy(&w, btr, sizeof(WCHAR*));
				btr += sizeof(WCHAR*);
				if (w)
				{
					*ptr++ = 'L';
					*ptr++ = '\"';
					ptr = qwcscpy(ptr, w);
					*ptr++ = '\"';
				}
				else
				{
					*ptr++ = '(';
					*ptr++ = 'N';
					*ptr++ = 'U';
					*ptr++ = 'L';
					*ptr++ = 'L';
					*ptr++ = ')';
				}
				*ptr++ = ',';
				*ptr++ = ' ';
				*ptr++ = '/';
				*ptr++ = '/';
				*ptr++ = '[';
				_itow((int)i, szNum, 10);
				ptr = qwcscpy(ptr, szNum);
				*ptr++ = ']';
				*ptr++ = '\r';
				*ptr++ = '\n';
			}
		}
		break;

	}
	*ptr++ = '}';
	*ptr++ = ';';
	*ptr++ = '\r';
	*ptr++ = '\n';
	*ptr = '\0';
	return ptr - pOutWChar;
}

size_t ArrayViewA(char* pOutChar, const VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex)
{
	if (pOutChar == NULL) {
		return FALSE;
	}
	return ArrayView_CoreA(pOutChar, pInArray, dwType, nArraySize, nIsHex);
}

size_t ArrayViewW(WCHAR* pOutWChar, const VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex)
{
	if (pOutWChar == NULL) {
		return FALSE;
	}
	return ArrayView_CoreW(pOutWChar, pInArray, dwType, nArraySize, nIsHex);
}

size_t ArrayView_LengthA(const VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex)
{
	BYTE* btr = (BYTE*)pInArray;
	char szNum[33];
	size_t nRet = 13;

	switch (dwType)
	{
	case ARRAYVIEW_TYPE_BYTE:
		if(nIsHex)
		{
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				nRet += 2;// sizeof(BYTE) * 2;
				nRet += 12;
			}
		}
		else
		{
			BYTE b;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				b = *btr++;
				_itoa(b, szNum, 10);
				nRet += strlen(szNum);
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_WORD:
		if(nIsHex)
		{
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				nRet += 4;// sizeof(WORD) * 2;
				nRet += 12;
			}
		}
		else
		{
			WORD w;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				memcpy(&w, btr, sizeof(WORD));
				btr += 2;// sizeof(WORD);
				_itoa((short)w, szNum, 10);
				nRet += strlen(szNum);
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_DWORD:
		if(nIsHex)
		{
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				nRet += 8;// sizeof(DWORD) * 2;
				nRet += 12;
			}
		}
		else
		{
			DWORD d;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				memcpy(&d, btr, sizeof(DWORD));
				btr += 4;// sizeof(DWORD);
				_itoa((int)d, szNum, 10);
				nRet += strlen(szNum);
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_QWORD:
		if(nIsHex)
		{
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				nRet += 16;// sizeof(unsigned __int64) * 2;
				nRet += 12;
			}
		}
		else
		{
			unsigned __int64 q;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				memcpy(&q, btr, sizeof(unsigned __int64));
				btr += 8;// sizeof(unsigned __int64);
				_i64toa((__int64)q, szNum, 10);
				nRet += strlen(szNum);
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_CHAR:
		{
			char* c;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				memcpy(&c, btr, sizeof(char*));
				btr += sizeof(char*);
				if (c)
				{
					nRet += strlen(c);
					nRet += 2;
				}
				else {
					nRet += 6;
				}
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_WCHAR:
		{
			WCHAR* c;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itoa((int)i, szNum, 10);
				nRet += strlen(szNum);
				memcpy(&c, btr, sizeof(WCHAR*));
				btr += sizeof(WCHAR*);
				if (c)
				{
					nRet += WCharToCharLength(c);
					nRet += 3;
				}
				else {
					nRet += 6;
				}
				nRet += 10;
			}
		}
		break;
	}
	return nRet;
}

size_t ArrayView_LengthW(const VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex)
{
	BYTE* btr = (BYTE*)pInArray;
	WCHAR szNum[33];
	size_t nRet = 13;

	switch (dwType)
	{
	case ARRAYVIEW_TYPE_BYTE:
		if(nIsHex)
		{
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				nRet += 2;
				nRet += 12;
			}
		}
		else
		{
			BYTE b;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				b = *btr++;
				_itow(b, szNum, 10);
				nRet += wcslen(szNum);
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_WORD:
		if(nIsHex)
		{
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				nRet += 4;// sizeof(WORD) * 2;
				nRet += 12;
			}
		}
		else
		{
			WORD w;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				memcpy(&w, btr, sizeof(WORD));
				btr += sizeof(WORD);
				_itow((short)w, szNum, 10);
				nRet += wcslen(szNum);
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_DWORD:
		if(nIsHex)
		{
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				nRet += 8;// sizeof(DWORD) * 2;
				nRet += 12;
			}
		}
		else
		{
			DWORD d;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				memcpy(&d, btr, sizeof(DWORD));
				btr += 4;// sizeof(DWORD);
				_itow((int)d, szNum, 10);
				nRet += wcslen(szNum);
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_QWORD:
		if(nIsHex)
		{
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				nRet += 16;// sizeof(unsigned __int64) * 2;
				nRet += 12;
			}
		}
		else
		{
			unsigned __int64 q;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				memcpy(&q, btr, sizeof(unsigned __int64));
				btr += 8;// sizeof(unsigned __int64);
				_i64tow((__int64)q, szNum, 10);
				nRet += wcslen(szNum);
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_CHAR:
		{
			char* c;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				memcpy(&c, btr, sizeof(char*));
				btr += sizeof(char*);
				if (c)
				{
					nRet += CharToWCharLength(c);
					nRet += 2;
				}
				else {
					nRet += 6;
				}
				nRet += 10;
			}
		}
		break;
	case ARRAYVIEW_TYPE_WCHAR:
		{
			WCHAR* c;
			size_t i;

			for (i = 0; i < nArraySize; i++)
			{
				_itow((int)i, szNum, 10);
				nRet += wcslen(szNum);
				memcpy(&c, btr, sizeof(WCHAR*));
				btr += sizeof(WCHAR*);
				if (c)
				{
					nRet += wcslen(c);
					nRet += 3;
				}
				else {
					nRet += 6;
				}
				nRet += 10;
			}
		}
		break;
	}
	return nRet;
}

char* ArrayViewToCharA(const VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex)
{
	char* pOutChar;
	size_t nLen;

	nLen = ArrayView_LengthA(pInArray, dwType, nArraySize, nIsHex);
	pOutChar = (char*)malloc(nLen);
	if (pOutChar == NULL) {
		return NULL;
	}
	ArrayView_CoreA(pOutChar, pInArray, dwType, nArraySize, nIsHex);
	return pOutChar;
}

WCHAR* ArrayViewToWCharW(const VOID* pInArray, DWORD dwType, size_t nArraySize, int nIsHex)
{
	WCHAR* pOutWChar;
	size_t nLen;

	nLen = ArrayView_LengthW(pInArray, dwType, nArraySize, nIsHex);
	pOutWChar = (WCHAR*)malloc(nLen * sizeof(WCHAR));
	if (pOutWChar == NULL) {
		return NULL;
	}
	ArrayView_CoreW(pOutWChar, pInArray, dwType, nArraySize, nIsHex);
	return pOutWChar;
}


size_t BitView_CoreA(char* pOutChar, size_t nBitNum, size_t nSize)
{
	char* ptr = pOutChar;
	BYTE* b = (BYTE*)&nBitNum;
	size_t i = 0;

	*ptr++ = ' ';
	*ptr++ = '=';
	*ptr++ = ' ';
	*ptr++ = '0';
	*ptr++ = 'x';

	switch (nSize)
	{
#if defined(_M_X64)
	case 8:
		*ptr++ = cpHexChar[(*(b + 7) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 7) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 6) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 6) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 5) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 5) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 4) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 4) & 0x0F)];
		SWITCH_FALLTHROUGH
#endif
	case 4:
		*ptr++ = cpHexChar[(*(b + 3) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 3) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 2) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 2) & 0x0F)];
		SWITCH_FALLTHROUGH
	case 2:
		*ptr++ = cpHexChar[(*(b + 1) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 1) & 0x0F)];
		SWITCH_FALLTHROUGH
	default:
		*ptr++ = cpHexChar[(*(b + 0) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 0) & 0x0F)];
	}

	*ptr++ = ';';
	*ptr++ = '\r';
	*ptr++ = '\n';
	memcpy(ptr, "-----------------------------------------------------------\r\n", 27);
	ptr += 27;
	*ptr++ = '\r';
	*ptr++ = '\n';
	memcpy(ptr, "+BITVIEW  7 6 5 4  3 2 1 0 \r\n", 29);
	ptr += 29;
	memcpy(ptr, "-----------------------------------------------------------\r\n", 27);
	ptr += 27;
	*ptr++ = '\r';
	*ptr++ = '\n';

	for (; i < nSize; i++)
	{
		*ptr++ = '[';
		*ptr++ = cpHexChar[i & (sizeof(size_t) - 1)];
		*ptr++ = ']';
		*ptr++ = ' ';
		*ptr++ = '0';
		*ptr++ = 'x';
		*ptr++ = cpHexChar[(*(b + i) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + i) & 0x0F)];
		*ptr++ = ' ';
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x80) >> 7];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x40) >> 6];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x20) >> 5];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x10) >> 4];
		*ptr++ = ' ';
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x08) >> 3];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x04) >> 2];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x02) >> 1];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x01)];
		*ptr++ = ' ';
		*ptr++ = '\r';
		*ptr++ = '\n';
	}
	memcpy(ptr, "-----------------------------------------------------------\r\n", 27);
	ptr += 27;
	*ptr++ = '\r';
	*ptr++ = '\n';
	*ptr = '\0';
	return ptr - pOutChar;
}

size_t BitView_CoreW(WCHAR* pOutWChar, size_t nBitNum, size_t nSize)
{
	WCHAR* ptr = pOutWChar;
	BYTE* b = (BYTE*)&nBitNum;
	size_t i = 0;


	*ptr++ = ' ';
	*ptr++ = '=';
	*ptr++ = ' ';
	*ptr++ = '0';
	*ptr++ = 'x';

	switch (nSize)
	{

#if defined(_M_X64)
	case 8:
		*ptr++ = cpHexChar[(*(b + 7) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 7) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 6) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 6) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 5) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 5) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 4) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 4) & 0x0F)];
		SWITCH_FALLTHROUGH
#endif
	case 4:
		*ptr++ = cpHexChar[(*(b + 3) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 3) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 2) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 2) & 0x0F)];
		SWITCH_FALLTHROUGH
	case 2:
		*ptr++ = cpHexChar[(*(b + 1) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 1) & 0x0F)];
		SWITCH_FALLTHROUGH
	default:
		*ptr++ = cpHexChar[(*(b + 0) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 0) & 0x0F)];
	}

	*ptr++ = ';';
	*ptr++ = '\r';
	*ptr++ = '\n';
	memcpy(ptr, L"-----------------------------------------------------------\r\n", 27 * sizeof(WCHAR));
	ptr += 27;
	*ptr++ = '\r';
	*ptr++ = '\n';
	memcpy(ptr, L"+BITVIEW  7 6 5 4  3 2 1 0 \r\n", 29 * sizeof(WCHAR));
	ptr += 29;
	memcpy(ptr, L"-----------------------------------------------------------\r\n", 27 * sizeof(WCHAR));
	ptr += 27;
	*ptr++ = '\r';
	*ptr++ = '\n';

	for (; i < nSize; i++)
	{
		*ptr++ = '[';
		*ptr++ = cpHexChar[i & (sizeof(size_t) - 1)];
		*ptr++ = ']';
		*ptr++ = ' ';
		*ptr++ = '0';
		*ptr++ = 'x';
		*ptr++ = cpHexChar[(*(b + i) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + i) & 0x0F)];
		*ptr++ = ' ';
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x80) >> 7];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x40) >> 6];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x20) >> 5];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x10) >> 4];
		*ptr++ = ' ';
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x08) >> 3];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x04) >> 2];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x02) >> 1];
		*ptr++ = ' ';
		*ptr++ = cpHexChar[(*(b + i) & 0x01)];
		*ptr++ = ' ';
		*ptr++ = '\r';
		*ptr++ = '\n';
	}
	memcpy(ptr, L"-----------------------------------------------------------\r\n", 27 * sizeof(WCHAR));
	ptr += 27;
	*ptr++ = '\r';
	*ptr++ = '\n';
	*ptr = '\0';
	return ptr - pOutWChar;
}

size_t BitView_Length(size_t nBitNum)
{
	nBitNum = nBitNum & (sizeof(size_t) * 2 - 1);
	return (nBitNum * 31) + ((size_t)27 * 3) + 30 + 14;
}

size_t BitViewA(char* pOutChar, size_t nBitNum, size_t nSize)
{
	if (pOutChar == NULL) {
		return FALSE;
	}
	return BitView_CoreA(pOutChar, nBitNum, nSize);
}

size_t BitViewW(WCHAR* pOutWChar, size_t nBitNum, size_t nSize)
{
	if (pOutWChar == NULL) {
		return FALSE;
	}
	return BitView_CoreW(pOutWChar, nBitNum, nSize);
}

char* BitViewToCharA(size_t nBitNum, size_t nSize)
{
	char* pOutChar;
	size_t nLen;

	nLen = BitView_Length(nSize);
	pOutChar = (char*)malloc(nLen);
	IF_UNLIKELY(pOutChar == NULL) {
		return NULL;
	}
	BitView_CoreA(pOutChar, nBitNum, nSize);
	return pOutChar;
}

WCHAR* BitViewToWCharW(size_t nBitNum, size_t nSize)
{
	WCHAR* pOutWChar;
	size_t nLen;

	nLen = BitView_Length(nSize);
	pOutWChar = (WCHAR*)malloc(nLen * sizeof(WCHAR));
	IF_UNLIKELY(pOutWChar == NULL) {
		return NULL;
	}
	BitView_CoreW(pOutWChar, nBitNum, nSize);
	return pOutWChar;
}

size_t BinaryView_CoreA(char* pOutChar, const BYTE* cpInByte, size_t nLen, int nIsWChar, size_t nStart)
{
	char szChar[20] = { 0 };
	BYTE* btr = (BYTE*)cpInByte + nStart;
	char* ptr = pOutChar;
	size_t i = 0;
	size_t nOverChar = 0;
	const size_t nLoop = INT_CEIL(nLen, 16) * 16;

	*ptr++ = ';';
	*ptr++ = '\r';
	*ptr++ = '\n';

	IF_UNLIKELY(nLen == 0)
	{
		*ptr++ = '\0';
		return ptr - pOutChar;
	}

	memcpy(ptr, "-----------------------------------------------------------------------------\r\n", 79);
	ptr += 79;
	memcpy(ptr, "+ADDRESS  +0 +1 +2 +3 +4 +5 +6 +7  +8 +9 +A +B +C +D +E +F  0123456789ABCDEF \r\n", 79);
	ptr += 79;
	memcpy(ptr, "-----------------------------------------------------------------------------\r\n", 79);
	ptr += 79;

	for (i = 0; i < nLoop; i += 16)
	{
		const size_t e = nLen - i;
		DWORD d = BSwap32((DWORD)(i + nStart));
		BYTE* b = (BYTE*)&d;

		if (e >= 16)
		{
			if (e > 17) {
				memcpy(szChar, btr, 18);
			}
			else if (e > 16)
			{
				memcpy(szChar, btr, 17);
				memset(szChar + 16, 0, 1);
			}
			else
			{
				memcpy(szChar, btr, 16);
				memset(szChar + 16, 0, 2);
			}
			btr += 16;
		}
		else
		{
			memset(szChar, 0, 18);
			memcpy(szChar, btr, e);
			btr += e;
		}

		*ptr++ = cpHexChar[(*(b + 0) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 0) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 1) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 1) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 2) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 2) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 3) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 3) & 0x0F)];
		*ptr++ = ' ';

		if (e >= 16)
		{
			size_t y = 0;

			for (; y < 16; y++)
			{
				IF_UNLIKELY(y % 8 == 0) {
					*ptr++ = ' ';
				}

				*ptr++ = cpHexChar[(szChar[y] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(szChar[y] & 0x0F)];
				*ptr++ = ' ';
			}
		}
		else
		{
			size_t y = 0;

			for (; y < 16; y++)
			{
				IF_UNLIKELY(y % 8 == 0) {
					*ptr++ = ' ';
				}

				if (y < e)
				{
					*ptr++ = cpHexChar[(szChar[y] & 0xF0) >> 4];
					*ptr++ = cpHexChar[(szChar[y] & 0x0F)];
					*ptr++ = ' ';
				}
				else
				{
					*ptr++ = ' ';
					*ptr++ = ' ';
					*ptr++ = ' ';
				}
			}
		}

		*ptr++ = ' ';

		if (nIsWChar)
		{
			size_t y = 0;

			for (; y < 16; y++)
			{
				if (szChar[y] == '\0')
				{
					*ptr++ = ' ';
					continue;
				}
				*ptr++ = szChar[y];
			}
		}
		else
		{
			size_t y = nOverChar;

			for (; nOverChar > 0; nOverChar--) {
				*ptr++ = ' ';
			}

			for (; y < 16; y++)
			{
				IF_UNLIKELY(szChar[y] == '\0')
				{
					*ptr++ = ' ';
					continue;
				}
				if (_ismbblead(szChar[y]) && szChar[y + 1] != '\0')
				{
					*ptr++ = szChar[y];
					y++;
					IF_UNLIKELY(y > 15) {
						nOverChar++;
					}
				}
				*ptr++ = szChar[y];
			}
		}

		*ptr++ = ' ';
		*ptr++ = '\r';
		*ptr++ = '\n';
	}

	memcpy(ptr, "-----------------------------------------------------------------------------\r\n", 80);
	ptr += 80;
	return ptr - pOutChar;
}


size_t BinaryView_CoreW(WCHAR* pOutWChar, const BYTE* cpInByte, size_t nLen, int nIsWChar, size_t nStart)
{
	char szChar[20] = { 0 };
	BYTE* btr = (BYTE*)cpInByte + nStart;
	WCHAR* ptr = pOutWChar;
	size_t i = 0;
	size_t nOverChar = 0;
	const size_t nLoop = INT_CEIL(nLen, 16) * 16;

	*ptr++ = ';';
	*ptr++ = '\r';
	*ptr++ = '\n';

	IF_UNLIKELY(nLen == 0)
	{
		*ptr++ = '\0';
		return ptr - pOutWChar;
	}

	memcpy(ptr, L"-----------------------------------------------------------------------------\r\n", 79 * sizeof(WCHAR));
	ptr += 79;
	memcpy(ptr, L"+ADDRESS  +0 +1 +2 +3 +4 +5 +6 +7  +8 +9 +A +B +C +D +E +F  0123456789ABCDEF \r\n", 79 * sizeof(WCHAR));
	ptr += 79;
	memcpy(ptr, L"-----------------------------------------------------------------------------\r\n", 79 * sizeof(WCHAR));
	ptr += 79;

	for (i = 0; i < nLoop; i += 16)
	{
		const size_t e = nLen - i;
		DWORD d = BSwap32((DWORD)(i + nStart));
		BYTE* b = (BYTE*)&d;

		if (e >= 16)
		{
			if (e > 17) {
				memcpy(szChar, btr, 18);
			}
			else if (e > 16)
			{
				memcpy(szChar, btr, 17);
				memset(szChar + 16, 0, 1);
			}
			else
			{
				memcpy(szChar, btr, 16);
				memset(szChar + 16, 0, 2);
			}
			btr += 16;
		}
		else
		{
			memset(szChar, 0, 18);
			memcpy(szChar, btr, e);
			btr += e;
		}

		*ptr++ = cpHexChar[(*(b + 0) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 0) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 1) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 1) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 2) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 2) & 0x0F)];
		*ptr++ = cpHexChar[(*(b + 3) & 0xF0) >> 4];
		*ptr++ = cpHexChar[(*(b + 3) & 0x0F)];
		*ptr++ = ' ';

		if (e >= 16)
		{
			size_t y = 0;

			for (; y < 16; y++)
			{
				IF_UNLIKELY(y % 8 == 0) {
					*ptr++ = ' ';
				}

				*ptr++ = cpHexChar[(szChar[y] & 0xF0) >> 4];
				*ptr++ = cpHexChar[(szChar[y] & 0x0F)];
				*ptr++ = ' ';
			}
		}
		else
		{
			size_t y = 0;

			for (; y < 16; y++)
			{
				IF_UNLIKELY(y % 8 == 0) {
					*ptr++ = ' ';
				}

				if (y < e)
				{
					*ptr++ = cpHexChar[(szChar[y] & 0xF0) >> 4];
					*ptr++ = cpHexChar[(szChar[y] & 0x0F)];
					*ptr++ = ' ';
				}
				else
				{
					*ptr++ = ' ';
					*ptr++ = ' ';
					*ptr++ = ' ';
				}
			}
		}

		*ptr++ = ' ';

		if (nIsWChar)
		{
			WCHAR* w = (WCHAR*)szChar;
			size_t y = 0;

			for (; y < 8; y++)
			{
				if (w[y] == '\0')
				{
					*ptr++ = ' ';
					*ptr++ = ' ';
					continue;
				}
				*ptr++ = w[y];
				if (w[y] < 0x0100) {
					*ptr++ = ' ';
				}
			}
		}
		else
		{
			size_t y = nOverChar;
			size_t n = 0;

			for (; nOverChar > 0; nOverChar--) {
				*ptr++ = ' ';
			}

			for (; y < 16;)
			{
				if (szChar[y] == '\0')
				{
					*ptr++ = ' ';
					y++;
					continue;
				}
				n = qmbtowc(ptr, &szChar[y]);
				y += n;
				ptr++;
				IF_UNLIKELY(y > 15) {
					nOverChar = y - 16;
				}
			}
		}

		*ptr++ = ' ';
		*ptr++ = '\r';
		*ptr++ = '\n';
	}

	memcpy(ptr, L"-----------------------------------------------------------------------------\r\n", 80 * sizeof(WCHAR));
	ptr += 80;
	return ptr - pOutWChar;
}

size_t BinaryView_Length(size_t nLen)
{
	if (nLen == 0) {
		return 4;
	}
	return nLen * 4 + INT_CEIL(nLen, 16) * 14 + INT_CEIL(nLen, 8) + (size_t)79 * 3 + 77 + 12;
}

char* CharToBinaryViewToCharA(const char* cpInChar, size_t nLen, size_t nStart)
{
	if (nLen == (size_t)-1) {
		nLen = strlen(cpInChar);
	}
	return BinaryViewToCharA((BYTE*)cpInChar, nLen, FALSE, nStart);
}

WCHAR* CharToBinaryViewToWCharW(const char* cpInChar, size_t nLen, size_t nStart)
{
	if (nLen == (size_t)-1) {
		nLen = strlen(cpInChar);
	}
	return BinaryViewToWCharW((BYTE*)cpInChar, nLen, FALSE, nStart);
}

char* WCharToBinaryViewToCharA(const WCHAR* cpInWChar, size_t nLen, size_t nStart)
{
	if (nLen == (size_t)-1) {
		nLen = wcslen(cpInWChar) * sizeof(WCHAR);
	}
	return BinaryViewToCharA((BYTE*)cpInWChar, nLen, TRUE, nStart);
}

WCHAR* WCharToBinaryViewToWCharW(const WCHAR* cpInWChar, size_t nLen, size_t nStart)
{
	if (nLen == (size_t)-1) {
		nLen = wcslen(cpInWChar) * sizeof(WCHAR);
	}
	return BinaryViewToWCharW((BYTE*)cpInWChar, nLen, TRUE, nStart);
}

char* BinaryViewToCharA(const BYTE* cpInByte, size_t nLen, int nIsWChar, size_t nStart)
{
	char* pOutChar = NULL;
	size_t nSize = 0;

	nSize = BinaryView_Length(nLen);
	pOutChar = (char*)malloc(nSize);
	IF_UNLIKELY(pOutChar == NULL) {
		return NULL;
	}
	BinaryView_CoreA(pOutChar, cpInByte, nLen, nIsWChar, nStart);
	return pOutChar;
}

WCHAR* BinaryViewToWCharW(const BYTE* cpInByte, size_t nLen, int nIsWChar, size_t nStart)
{
	WCHAR* pOutWChar = NULL;
	size_t nSize = 0;

	nSize = BinaryView_Length(nLen);
	pOutWChar = (WCHAR*)malloc(nSize * sizeof(WCHAR));
	IF_UNLIKELY(pOutWChar == NULL) {
		return NULL;
	}
	BinaryView_CoreW(pOutWChar, cpInByte, nLen, nIsWChar, nStart);
	return pOutWChar;
}

VOID BinaryViewFree(VOID* pFree)
{
	free(pFree);
	return;
}

size_t CharToBinaryViewA(char* pOutChar, const char* cpInChar, size_t nLen, size_t nStart)
{
	if (nLen == (size_t)-1) {
		nLen = strlen(cpInChar);
	}
	return BinaryView_CoreA(pOutChar, (BYTE*)cpInChar, nLen, FALSE, nStart);
}

size_t WCharToBinaryViewA(char* pOutChar, const WCHAR* cpInWChar, size_t nLen, size_t nStart)
{
	if (nLen == (size_t)-1) {
		nLen = wcslen(cpInWChar);
	}
	return BinaryView_CoreA(pOutChar, (BYTE*)cpInWChar, nLen * sizeof(WCHAR), TRUE, nStart);
}

size_t CharToBinaryViewW(WCHAR* pOutWChar, const char* cpInChar, size_t nLen, size_t nStart)
{
	if (nLen == (size_t)-1) {
		nLen = strlen(cpInChar);
	}
	return BinaryView_CoreW(pOutWChar, (BYTE*)cpInChar, nLen, FALSE, nStart);
}

size_t WCharToBinaryViewW(WCHAR* pOutWChar, const WCHAR* cpInWChar, size_t nLen, size_t nStart)
{
	if (nLen == (size_t)-1) {
		nLen = wcslen(cpInWChar);
	}
	return BinaryView_CoreW(pOutWChar, (BYTE*)cpInWChar, nLen * sizeof(WCHAR), TRUE, nStart);
}

size_t BinaryViewA(char* pOutChar, const BYTE* cpInByte, size_t nLen, size_t nStart)
{
	return BinaryView_CoreA(pOutChar, cpInByte, nLen, FALSE, nStart);
}

size_t BinaryViewW(WCHAR* pOutWChar, const BYTE* cpInByte, size_t nLen, size_t nStart)
{
	return BinaryView_CoreW(pOutWChar, cpInByte, nLen, FALSE, nStart);
}
