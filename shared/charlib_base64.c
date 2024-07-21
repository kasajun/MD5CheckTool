#include "charlib_base64.h"


static const unsigned char pr2six[] =
{
	/* ASCII table */
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 63, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};


size_t Base64Decode(unsigned char* pOutChar, const char* cpBase64Char)
{
	size_t nbytesdecoded = 0;
	const unsigned char* bufin;
	unsigned char* bufout;

	bufout = pOutChar;
	bufin = (const unsigned char*)cpBase64Char;

LINEFEED_LOOP:
	if (1)
	{
		const unsigned char* bufinstart = bufin;
		size_t nprbytes;

		while (pr2six[*(bufin++)] <= 63);

		nprbytes = (bufin - bufinstart) - 1;
		nbytesdecoded += ((nprbytes + 3) / 4) * 3;
		bufin = bufinstart;

		while (nprbytes > 4)
		{
			*(bufout++) =
				(unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
			*(bufout++) =
				(unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
			*(bufout++) =
				(unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
			bufin += 4;
			nprbytes -= 4;
		}

		/* Note: (nprbytes == 1) would be an error, so just ingore that case */
		if (nprbytes > 1) {
			*(bufout++) = (unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
		}
		else if (nprbytes > 2) {
			*(bufout++) = (unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
		}
		else if (nprbytes > 3) {
			*(bufout++) = (unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
		}

		*bufout = '\0';
		nbytesdecoded -= (4 - nprbytes) & 3;

		if (bufin[4] == '\r' && bufin[5] == '\n' && bufin[6] != '\0')
		{
			bufin += 5;
			goto LINEFEED_LOOP;
		}
	}
	return nbytesdecoded;
}

size_t Base64ToWCharDecode(unsigned char* pOutChar, const WCHAR* cpBase64WChar)
{
	size_t nbytesdecoded = 0;
	const WCHAR* bufin;
	unsigned char* bufout;

	bufout = pOutChar;
	bufin = (const WCHAR*)cpBase64WChar;

LINEFEED_LOOP:
	if (1)
	{
		const WCHAR* bufinstart = bufin;
		size_t nprbytes;

		while (pr2six[*(bufin++)] <= 63);

		nprbytes = (bufin - bufinstart) - 1;
		nbytesdecoded += ((nprbytes + 3) / 4) * 3;
		bufin = bufinstart;

		while (nprbytes > 4)
		{
			*(bufout++) = (unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
			*(bufout++) = (unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
			*(bufout++) = (unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
			bufin += 4;
			nprbytes -= 4;
		}

		/* Note: (nprbytes == 1) would be an error, so just ingore that case */
		if (nprbytes > 1) {
			*(bufout++) = (unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
		}
		else if (nprbytes > 2) {
			*(bufout++) = (unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
		}
		else if (nprbytes > 3) {
			*(bufout++) = (unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
		}

		*bufout = '\0';
		nbytesdecoded -= (4 - nprbytes) & 3;

		if (bufin[4] == '\r' && bufin[5] == '\n' && bufin[6] != '\0')
		{
			bufin += 5;
			goto LINEFEED_LOOP;
		}
	}
	return nbytesdecoded;
}


size_t Base64DecodeLength(const unsigned char* cpBase64Char)
{
	size_t nbytesdecoded = 0;
	const unsigned char* bufin;
	bufin = (const unsigned char*)cpBase64Char;

LINEFEED_LOOP:
	if (1)
	{
		const unsigned char* bufinstart = bufin;
		size_t nprbytes;

		while (pr2six[*(bufin++)] <= 63);

		nprbytes = (bufin - bufinstart) - 1;
		nbytesdecoded += ((nprbytes + 3) / 4) * 3;
		if (*(bufin - 1) == '\r' && *bufin++ == '\n' && *bufin != '\0') {
			goto LINEFEED_LOOP;
		}
	}
	return nbytesdecoded + 1;
}

size_t Base64ToWCharDecodeLength(const WCHAR* cpBase64WChar)
{
	size_t nbytesdecoded;
	const WCHAR* bufin;
	bufin = (const WCHAR*)cpBase64WChar;

LINEFEED_LOOP:
	if (1)
	{
		const WCHAR* bufinstart = bufin;
		size_t nprbytes;

		while (pr2six[*(bufin++)] <= 63);

		nprbytes = (bufin - bufinstart) - 1;
		nbytesdecoded = ((nprbytes + 3) / 4) * 3;
		if (*(bufin - 1) == '\r' && *bufin++ == '\n' && *bufin != '\0') {
			goto LINEFEED_LOOP;
		}
	}
	return nbytesdecoded + 1;
}

static const char Base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char Base64FileNameTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";


size_t Base64Encode(char* pOutChar, const unsigned char* cpInChar, size_t nInSize)
{
	size_t i;
	const char* pSrc = (const char*)cpInChar;
	char* pDst;

	pDst = pOutChar;
	for (i = 0; i < (nInSize - 2); i += 3, pSrc += 3)
	{
		*pDst++ = Base64Table[(*(pSrc) >> 2) & 0x3F];
		*pDst++ = Base64Table[((*(pSrc) & 0x03) << 4) | ((int)(*(pSrc + 1) & 0xF0) >> 4)];
		*pDst++ = Base64Table[((*(pSrc + 1) & 0x0F) << 2) | ((int)(*(pSrc + 2) & 0xC0) >> 6)];
		*pDst++ = Base64Table[(*(pSrc + 2)) & 0x3F];
	}

	if (i < nInSize)
	{
		*pDst++ = Base64Table[(*(pSrc) >> 2) & 0x3F];
		if (i == (nInSize - 1))
		{
			*pDst++ = Base64Table[((*(pSrc) & 0x03) << 4)];
			*pDst++ = '=';
		}
		else
		{
			*pDst++ = Base64Table[((*(pSrc) & 0x03) << 4) | ((int)(*(pSrc + 1) & 0xF0) >> 4)];
			*pDst++ = Base64Table[((*(pSrc + 1) & 0x0F) << 2)];
		}
		*pDst++ = '=';
	}
	*pDst++ = '\0';

	return pDst - pOutChar;
}

size_t Base64EncodeToWChar(WCHAR* pOutWChar, const unsigned char* cpInChar, size_t nInSize)
{
	size_t i;
	const char* pSrc = (const char*)cpInChar;
	WCHAR* pDst;

	pDst = pOutWChar;
	for (i = 0; i < (nInSize - 2); i += 3, pSrc += 3)
	{
		*pDst++ = Base64Table[(*(pSrc) >> 2) & 0x3F];
		*pDst++ = Base64Table[((*(pSrc) & 0x03) << 4) | ((int)(*(pSrc + 1) & 0xF0) >> 4)];
		*pDst++ = Base64Table[((*(pSrc + 1) & 0x0F) << 2) | ((int)(*(pSrc + 2) & 0xC0) >> 6)];
		*pDst++ = Base64Table[(*(pSrc + 2)) & 0x3F];
	}

	if (i < nInSize)
	{
		*pDst++ = Base64Table[(*(pSrc) >> 2) & 0x3F];
		if (i == (nInSize - 1))
		{
			*pDst++ = Base64Table[((*(pSrc) & 0x03) << 4)];
			*pDst++ = '=';
		}
		else
		{
			*pDst++ = Base64Table[((*(pSrc) & 0x03) << 4) | ((int)(*(pSrc + 1) & 0xF0) >> 4)];
			*pDst++ = Base64Table[((*(pSrc + 1) & 0x0F) << 2)];
		}
		*pDst++ = '=';
	}
	*pDst++ = '\0';

	return pDst - pOutWChar;
}


size_t Base64EncodeLength(const char* cpInChar)
{
	size_t nInSize = strlen(cpInChar);
	return Base64EncodeLength2(nInSize);
}

size_t Base64EncodeLength2(size_t nInSize)
{
	return ((nInSize + 2) / 3 * 4) + 1;
}


size_t Base64EncodeToWCharLength(const WCHAR* cpInWChar)
{
	size_t nInSize = wcslen(cpInWChar);
	return Base64EncodeToWCharLength2(nInSize);
}

size_t Base64EncodeToWCharLength2(size_t nInSize)
{
	return ((nInSize + 2) / 3 * 4) + 1;
}
 

size_t Base64FileNameEncode(char* pOutChar, const char* cpInChar, size_t nInSize)
{
	size_t i;
	const char* pSrc = cpInChar;
	char* pDst;

	pDst = pOutChar;
	for (i = 0; i < (nInSize - 2); i += 3, pSrc += 3)
	{
		*pDst++ = Base64FileNameTable[(*(pSrc) >> 2) & 0x3F];
		*pDst++ = Base64FileNameTable[((*(pSrc) & 0x03) << 4) | ((int)(*(pSrc + 1) & 0xF0) >> 4)];
		*pDst++ = Base64FileNameTable[((*(pSrc + 1) & 0x0F) << 2) | ((int)(*(pSrc + 2) & 0xC0) >> 6)];
		*pDst++ = Base64FileNameTable[(*(pSrc + 2)) & 0x3F];
	}

	if (i < nInSize)
	{
		*pDst++ = Base64FileNameTable[(*(pSrc) >> 2) & 0x3F];
		if (i == (nInSize - 1)) {
			*pDst++ = Base64FileNameTable[((*(pSrc) & 0x03) << 4)];
		}
		else
		{
			*pDst++ = Base64FileNameTable[((*(pSrc) & 0x03) << 4) | ((int)(*(pSrc + 1) & 0xF0) >> 4)];
			*pDst++ = Base64FileNameTable[((*(pSrc + 1) & 0x0F) << 2)];
		}
	}
	*pDst++ = '\0';

	return pDst - pOutChar;
}

size_t Base64FileNameEncodeToWChar(WCHAR* pOutChar, const char* cpInChar, size_t nInSize)
{
	size_t i;
	const char* pSrc = cpInChar;
	WCHAR* pDst;

	pDst = pOutChar;
	for (i = 0; i < (nInSize - 2); i += 3, pSrc += 3)
	{
		*pDst++ = Base64FileNameTable[(*(pSrc) >> 2) & 0x3F];
		*pDst++ = Base64FileNameTable[((*(pSrc) & 0x03) << 4) | ((int)(*(pSrc + 1) & 0xF0) >> 4)];
		*pDst++ = Base64FileNameTable[((*(pSrc + 1) & 0x0F) << 2) | ((int)(*(pSrc + 2) & 0xC0) >> 6)];
		*pDst++ = Base64FileNameTable[(*(pSrc + 2)) & 0x3F];
	}

	if (i < nInSize)
	{
		*pDst++ = Base64FileNameTable[(*(pSrc) >> 2) & 0x3F];
		if (i == (nInSize - 1)) {
			*pDst++ = Base64FileNameTable[((*(pSrc) & 0x03) << 4)];
		}
		else
		{
			*pDst++ = Base64FileNameTable[((*(pSrc) & 0x03) << 4) | ((int)(*(pSrc + 1) & 0xF0) >> 4)];
			*pDst++ = Base64FileNameTable[((*(pSrc + 1) & 0x0F) << 2)];
		}
	}
	*pDst++ = '\0';

	return pDst - pOutChar;
}

char* BinaryToBase64(const void* cpIn, size_t nInSize, char* pOutBase64Char)
{
	if (pOutBase64Char == NULL)
	{
		size_t nOutBase64CharSize = Base64EncodeLength2(nInSize);
		pOutBase64Char = (char*)malloc(nOutBase64CharSize);
		if (pOutBase64Char == NULL) {
			return NULL;
		}
	}
	Base64Encode(pOutBase64Char, (const unsigned char*)cpIn, nInSize);
	return pOutBase64Char;
}

WCHAR* BinaryToBase64ToWChar(const void* cpIn, size_t nInSize, WCHAR* pOutBase64WChar)
{
	if (pOutBase64WChar == NULL)
	{
		size_t nOutBase64CharSize = Base64EncodeLength2(nInSize);
		pOutBase64WChar = (WCHAR*)malloc(nOutBase64CharSize * sizeof(WCHAR));
		if (pOutBase64WChar == NULL) {
			return NULL;
		}
	}
	Base64EncodeToWChar(pOutBase64WChar, (const unsigned char*)cpIn, nInSize);
	return pOutBase64WChar;
}


char* CharToBase64(const char* cpInChar)
{
	size_t nInSize = strlen(cpInChar);
	return BinaryToBase64(cpInChar, nInSize, NULL);
}

char* CharToBase64v2(const char* cpInChar, char* pOutBase64Char)
{
	size_t nInSize = strlen(cpInChar);
	return BinaryToBase64(cpInChar, nInSize, pOutBase64Char);
}

char* CharSizeToBase64(const char* cpInChar, size_t nInSize)
{
	return BinaryToBase64(cpInChar, nInSize, NULL);
}

char* CharSizeToBase64v2(const char* cpInChar, size_t nInSize, char* pOutBase64Char)
{
	return BinaryToBase64(cpInChar, nInSize, pOutBase64Char);
}


WCHAR* CharToBase64ToWChar(const char* cpInChar)
{
	size_t nInSize = strlen(cpInChar);
	return BinaryToBase64ToWChar(cpInChar, nInSize, NULL);
}

WCHAR* CharToBase64ToWCharv2(const char* cpInChar, WCHAR* pOutBase64WChar)
{
	size_t nInSize = strlen(cpInChar);
	return BinaryToBase64ToWChar(cpInChar, nInSize, pOutBase64WChar);
}

WCHAR* CharSizeToBase64ToWChar(const char* cpInChar, size_t nInSize)
{
	return BinaryToBase64ToWChar(cpInChar, nInSize, NULL);
}

WCHAR* CharSizeToBase64ToWCharv2(const char* cpInChar, size_t nInSize, WCHAR* pOutBase64WChar)
{
	return BinaryToBase64ToWChar(cpInChar, nInSize, pOutBase64WChar);
}


char* WCharToBase64(const WCHAR* cpInWChar)
{
	size_t nInSize = wcslen(cpInWChar) * sizeof(WCHAR);
	return BinaryToBase64(cpInWChar, nInSize, NULL);
}

char* WCharToBase64v2(const WCHAR* cpInWChar, char* pOutBase64Char)
{
	size_t nInSize = wcslen(cpInWChar) * sizeof(WCHAR);
	return BinaryToBase64(cpInWChar, nInSize, pOutBase64Char);
}

char* WCharSizeToBase64(const WCHAR* cpInWChar, size_t nInSize)
{
	return BinaryToBase64(cpInWChar, nInSize, NULL);
}

char* WCharSizeToBase64v2(const WCHAR* cpInWChar, size_t nInSize, char* pOutBase64Char)
{
	return BinaryToBase64(cpInWChar, nInSize, pOutBase64Char);
}


WCHAR* WCharToBase64ToWChar(const WCHAR* cpInWChar)
{
	size_t nInSize = wcslen(cpInWChar) * sizeof(WCHAR);
	return BinaryToBase64ToWChar((char*)cpInWChar, nInSize, NULL);
}

WCHAR* WCharToBase64ToWCharv2(const WCHAR* cpInWChar, WCHAR* pOutBase64WChar)
{
	size_t nInSize = wcslen(cpInWChar) * sizeof(WCHAR);
	return BinaryToBase64ToWChar((char*)cpInWChar, nInSize, pOutBase64WChar);
}

WCHAR* WCharSizeToBase64ToWChar(const WCHAR* cpInWChar, size_t nInSize)
{
	return BinaryToBase64ToWChar((char*)cpInWChar, nInSize, NULL);
}

WCHAR* WCharSizeToBase64ToWCharv2(const WCHAR* cpInWChar, size_t nInSize, WCHAR* pOutBase64WChar)
{
	return BinaryToBase64ToWChar((char*)cpInWChar, nInSize, pOutBase64WChar);
}


void* Base64ToBinary(const char* cpInBase64Char, void* pOut, size_t *pnOutSize)
{
	size_t nOutSize;

	if (pnOutSize == NULL) {
		return NULL;
	}
	nOutSize = Base64DecodeLength((const unsigned char*)cpInBase64Char);

	if (pOut == NULL)
	{
		pOut = malloc(nOutSize);
		if (pOut == NULL) {
			return NULL;
		}
	}
	*pnOutSize = Base64Decode((unsigned char*)pOut, cpInBase64Char);
	return pOut;
}


char* Base64ToCharA(const char* cpInBase64Char)
{
	char* pOutChar;
	size_t nOutCharSize;

	nOutCharSize = Base64DecodeLength((const unsigned char*)cpInBase64Char);
	pOutChar = (char*)malloc(nOutCharSize + 1);
	if (pOutChar == NULL) {
		return NULL;
	}
	Base64Decode((unsigned char*)pOutChar, cpInBase64Char);
	return pOutChar;
}

char* Base64ToCharv2A(const char* cpInBase64Char, char* pOutChar)
{
	size_t nOutCharSize;

	nOutCharSize = Base64DecodeLength((const unsigned char*)cpInBase64Char);
	if (pOutChar == NULL)
	{
		pOutChar = (char*)malloc(nOutCharSize + 1);
		if (pOutChar == NULL) {
			return NULL;
		}
	}
	Base64Decode((unsigned char*)pOutChar, cpInBase64Char);
	return pOutChar;
}


WCHAR* Base64ToWCharA(const char* cpInBase64Char)
{
	WCHAR* pOutWChar;
	size_t nOutWCharSize;

	nOutWCharSize = Base64DecodeLength((const unsigned char*)cpInBase64Char);
	pOutWChar = (WCHAR*)malloc((nOutWCharSize + 1) * sizeof(WCHAR));
	if (pOutWChar == NULL) {
		return NULL;
	}
	Base64Decode((unsigned char*)pOutWChar, cpInBase64Char);
	return pOutWChar;
}

WCHAR* Base64ToWCharv2A(const char* cpInBase64Char, WCHAR* pOutWChar)
{
	size_t nOutWCharSize;

	nOutWCharSize = Base64DecodeLength((const unsigned char*)cpInBase64Char);
	if (pOutWChar == NULL)
	{
		pOutWChar = (WCHAR*)malloc((nOutWCharSize + 1) * sizeof(WCHAR));
		if (pOutWChar == NULL) {
			return NULL;
		}
	}
	Base64Decode((unsigned char*)pOutWChar, cpInBase64Char);
	return pOutWChar;
}


char* Base64ToCharW(const WCHAR* cpInBase64WChar)
{
	char* pOutChar;
	size_t nOutCharSize;

	nOutCharSize = Base64ToWCharDecodeLength(cpInBase64WChar);
	pOutChar = (char*)malloc(nOutCharSize + 1);
	if (pOutChar == NULL) {
		return NULL;
	}
	Base64ToWCharDecode((unsigned char*)pOutChar, cpInBase64WChar);
	return pOutChar;
}

char* Base64ToCharv2W(const WCHAR* cpInBase64WChar, char* pOutChar)
{
	size_t nOutCharSize;

	nOutCharSize = Base64ToWCharDecodeLength(cpInBase64WChar);
	if (pOutChar == NULL)
	{
		pOutChar = (char*)malloc(nOutCharSize + 1);
		if (pOutChar == NULL) {
			return NULL;
		}
	}
	Base64ToWCharDecode((unsigned char*)pOutChar, cpInBase64WChar);
	return pOutChar;
}


WCHAR* Base64ToWCharW(const WCHAR* cpInBase64WChar)
{
	WCHAR* pOutWChar;
	size_t nOutWCharSize;

	nOutWCharSize = Base64ToWCharDecodeLength(cpInBase64WChar);
	pOutWChar = (WCHAR*)malloc((nOutWCharSize + 1) * sizeof(WCHAR));
	if (pOutWChar == NULL) {
		return NULL;
	}
	Base64ToWCharDecode((unsigned char*)pOutWChar, cpInBase64WChar);
	return pOutWChar;
}

WCHAR* Base64ToWCharv2W(const WCHAR* cpInBase64WChar, WCHAR* pOutWChar)
{
	size_t nOutWCharSize;

	nOutWCharSize = Base64ToWCharDecodeLength(cpInBase64WChar);
	if (pOutWChar == NULL)
	{
		pOutWChar = (WCHAR*)malloc((nOutWCharSize + 1) * sizeof(WCHAR));
		if (pOutWChar == NULL) {
			return NULL;
		}
	}
	Base64ToWCharDecode((unsigned char*)pOutWChar, cpInBase64WChar);
	return pOutWChar;
}


size_t Base64LineFeedEncodeLength(const char* cpInChar)
{
	size_t nInSize = strlen(cpInChar);
	size_t nLineFeed = INT_CEIL(nInSize, 48) * 2;
	return Base64EncodeLength2(nInSize) + nLineFeed;
}

size_t Base64LineFeedEncodeLength2(size_t nInSize)
{
	size_t nLineFeed = INT_CEIL(nInSize, 48) * 2;
	return Base64EncodeLength2(nInSize) + nLineFeed;
}

char* BinaryToBase64LineFeed(const void* cpIn, size_t nInSize, char* pOutBase64Char)
{
	size_t nBase64CharSize;
	size_t nRet;
	size_t nCount;
	char* pSrc;
	char* pDst;

	nBase64CharSize = Base64LineFeedEncodeLength2(nInSize);
	if (pOutBase64Char == NULL)
	{
		pOutBase64Char = (char*)malloc(nBase64CharSize);
		if (pOutBase64Char == NULL) {
			return NULL;
		}
	}

	pSrc = (char*)cpIn;
	pDst = pOutBase64Char;
	nCount = nInSize;
	while (nCount >= 48)
	{
		Base64Encode(pDst, (const unsigned char*)pSrc, 48);
		pSrc += 48;
		pDst += 64;
		*pDst++ = '\r';
		*pDst++ = '\n';
		nCount -= 48;
	}
	if (nCount > 0)
	{
		nRet = Base64Encode(pDst, (const unsigned char*)pSrc, nCount);
		pDst += nRet - 1;
		*pDst++ = '\r';
		*pDst++ = '\n';
	}
	*pDst++ = '\0';
	return pOutBase64Char;
}

WCHAR* BinaryToBase64LineFeedToWChar(const void* cpIn, size_t nInSize, WCHAR* pOutBase64WChar)
{
	size_t nBase64WCharSize;
	size_t nRet;
	size_t nCount;
	char* pSrc;
	WCHAR* pDst;

	nBase64WCharSize = Base64LineFeedEncodeLength2(nInSize);
	if (pOutBase64WChar == NULL)
	{
		pOutBase64WChar = (WCHAR*)malloc(nBase64WCharSize * sizeof(WCHAR));
		if (pOutBase64WChar == NULL) {
			return NULL;
		}
	}
	pSrc = (char*)cpIn;
	pDst = pOutBase64WChar;
	nCount = nInSize;
	while (nCount >= 48)
	{
		Base64EncodeToWChar(pDst, (const unsigned char*)pSrc, 48);
		pSrc += 48;
		pDst += 64;
		*pDst++ = '\r';
		*pDst++ = '\n';
		nCount -= 48;
	}
	if (nCount > 0)
	{
		nRet = Base64EncodeToWChar(pDst, (const unsigned char*)pSrc, nCount);
		pDst += nRet - 1;
		*pDst++ = '\r';
		*pDst++ = '\n';
	}
	*pDst++ = '\0';
	return pOutBase64WChar;
}


char* CharToBase64LineFeed(const char* cpInChar)
{
	size_t nInSize = strlen(cpInChar);
	return BinaryToBase64LineFeed(cpInChar, nInSize, NULL);
}

char* CharToBase64LineFeedv2(const char* cpInChar, char* pOutChar)
{
	size_t nInSize = strlen(cpInChar);
	return BinaryToBase64LineFeed(cpInChar, nInSize, pOutChar);
}

char* CharSizeToBase64LineFeed(const char* cpInChar, size_t nInSize)
{
	return BinaryToBase64LineFeed(cpInChar, nInSize, NULL);
}

char* CharSizeToBase64LineFeedv2(const char* cpInChar, size_t nInSize, char* pOutChar)
{
	return BinaryToBase64LineFeed(cpInChar, nInSize, pOutChar);
}


WCHAR* CharToBase64LineFeedToWChar(const char* cpInChar)
{
	size_t nInSize = strlen(cpInChar);
	return BinaryToBase64LineFeedToWChar(cpInChar, nInSize, NULL);
}

WCHAR* CharToBase64LineFeedToWCharv2(const char* cpInChar, WCHAR* pOutWChar)
{
	size_t nInSize = strlen(cpInChar);
	return BinaryToBase64LineFeedToWChar(cpInChar, nInSize, pOutWChar);
}

WCHAR* CharSizeToBase64LineFeedToWChar(const char* cpInChar, size_t nInSize)
{
	return BinaryToBase64LineFeedToWChar(cpInChar, nInSize, NULL);
}

WCHAR* CharSizeToBase64LineFeedToWCharv2(const char* cpInChar, size_t nInSize, WCHAR* pOutWChar)
{
	return BinaryToBase64LineFeedToWChar(cpInChar, nInSize, pOutWChar);
}
