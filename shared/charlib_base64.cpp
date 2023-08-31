#include "charlib_base64.h"
#include "c20.h"


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
	return Base64Decode((char*)pOutChar, cpBase64Char);
}

size_t Base64Decode(char* pOutChar, const char* cpBase64Char)
{
	size_t nbytesdecoded = 0;
	const unsigned char* bufin;
	unsigned char* bufout;
	size_t nprbytes;

	bufout = (unsigned char*)pOutChar;
	bufin = (const unsigned char*)cpBase64Char;

LINEFEED_LOOP:
	if (1)
	{
		const unsigned char* bufinstart = bufin;
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

size_t Base64ToWCharDecode(char* pOutChar, const WCHAR* cpBase64WChar)
{
	size_t nbytesdecoded = 0;
	const WCHAR* bufin;
	unsigned char* bufout;
	size_t nprbytes;

	bufout = (unsigned char*)pOutChar;
	bufin = (const WCHAR*)cpBase64WChar;

LINEFEED_LOOP:
	if (1)
	{
		const WCHAR* bufinstart = bufin;
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
	return Base64DecodeLength((const char*)cpBase64Char);
}

size_t Base64DecodeLength(const char* cpBase64Char)
{
	size_t nbytesdecoded = 0;
	const unsigned char* bufin;
	bufin = (const unsigned char*)cpBase64Char;

LINEFEED_LOOP:
	if (1)
	{
		const unsigned char* bufinstart = bufin;
		while (pr2six[*(bufin++)] <= 63);

		const size_t nprbytes = (bufin - bufinstart) - 1;
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
		while (pr2six[*(bufin++)] <= 63);

		const size_t nprbytes = (bufin - bufinstart) - 1;
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
	return Base64Encode(pOutChar, (const char*)cpInChar, nInSize);
}

size_t Base64Encode(char* pOutChar, const char* cpInChar, size_t nInSize)
{
	size_t i;
	const char* pSrc = cpInChar;
	char* pDst;

	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}

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

size_t Base64EncodeToWChar(WCHAR* pOutWChar, const char* cpInChar, size_t nInSize)
{
	size_t i;
	const char* pSrc = cpInChar;
	WCHAR* pDst;

	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}

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

size_t Base64EncodeLength(const char* cpInChar, size_t nInSize)
{
	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}
	return ((nInSize + 2) / 3 * 4) + 1;
}

size_t Base64EncodeToWCharLength(const WCHAR* cpInWChar, size_t nInSize)
{
	if (nInSize == (size_t)-1) {
		nInSize = wcslen(cpInWChar);
	}
	return ((nInSize + 2) / 3 * 4) + 1;
}
 
size_t Base64EncodeLength(const unsigned char* cpInChar, size_t nInSize)
{
	return Base64EncodeLength((const char*)cpInChar, nInSize);
}

size_t Base64FileNameEncode(char* pOutChar, const char* cpInChar, size_t nInSize)
{
	size_t i;
	const char* pSrc = cpInChar;
	char* pDst;

	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}

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

	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}

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
	IF_UNLIKELY(pOutBase64Char == NULL)
	{
		const size_t nOutBase64CharSize = Base64EncodeLength((const char*)cpIn, nInSize);
		pOutBase64Char = (char*)malloc(nOutBase64CharSize);
		IF_UNLIKELY(pOutBase64Char == NULL) {
			return NULL;
		}
	}
	Base64Encode(pOutBase64Char, (const char*)cpIn, nInSize);
	return pOutBase64Char;
}

WCHAR* BinaryToBase64ToWChar(const void* cpIn, size_t nInSize, WCHAR* pOutBase64WChar)
{
	IF_UNLIKELY(pOutBase64WChar == NULL)
	{
		const size_t nOutBase64CharSize = Base64EncodeLength((const char*)cpIn, nInSize);
		pOutBase64WChar = (WCHAR*)malloc(nOutBase64CharSize * sizeof(WCHAR));
		IF_UNLIKELY(pOutBase64WChar == NULL) {
			return NULL;
		}
	}
	Base64EncodeToWChar(pOutBase64WChar, (const char*)cpIn, nInSize);
	return pOutBase64WChar;
}

char* CharToBase64(const char* cpInChar, size_t nInSize, char* pOutBase64Char)
{
	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}
	return BinaryToBase64(cpInChar, nInSize, pOutBase64Char);
}

WCHAR* CharToBase64ToWChar(const char* cpInChar, size_t nInSize, WCHAR* pOutBase64WChar)
{
	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}
	return BinaryToBase64ToWChar(cpInChar, nInSize, pOutBase64WChar);
}

char* WCharToBase64(const WCHAR* cpInWChar, size_t nInSize)
{
	if (nInSize == (size_t)-1) {
		nInSize = wcslen(cpInWChar) * sizeof(WCHAR);
	}
	return CharToBase64((char*)cpInWChar, nInSize);
}

WCHAR* WCharToBase64ToWChar(const WCHAR* cpInWChar, size_t nInSize)
{
	if (nInSize == (size_t)-1) {
		nInSize = wcslen(cpInWChar) * sizeof(WCHAR);
	}
	return CharToBase64ToWChar((char*)cpInWChar, nInSize);
}

size_t Base64LineFeedEncodeLength(const char* cpInChar, size_t nInSize)
{
	size_t nLineFeed;
	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}
	nLineFeed = INT_CEIL(nInSize, 48) * 2;
	return Base64EncodeLength(cpInChar, nInSize) + nLineFeed;
}

char* BinaryToBase64LineFeed(const void* cpIn, size_t nInSize)
{
	char* pBase64Char;
	size_t nBase64CharSize;
	size_t nRet;
	size_t nCount;
	char* pSrc;
	char* pDst;

	nBase64CharSize = Base64LineFeedEncodeLength((char*)cpIn, nInSize);
	pBase64Char = (char*)malloc(nBase64CharSize);
	IF_UNLIKELY(pBase64Char == NULL) {
		return NULL;
	}
	pSrc = (char*)cpIn;
	pDst = pBase64Char;
	nCount = nInSize;
	while (nCount >= 48)
	{
		Base64Encode(pDst, pSrc, 48);
		pSrc += 48;
		pDst += 64;
		*pDst++ = '\r';
		*pDst++ = '\n';
		nCount -= 48;
	}
	if (nCount > 0)
	{
		nRet = Base64Encode(pDst, pSrc, nCount);
		pDst += nRet - 1;
		*pDst++ = '\r';
		*pDst++ = '\n';
	}
	*pDst++ = '\0';
	return pBase64Char;
}

char* CharToBase64LineFeed(const char* cpInChar, size_t nInSize)
{
	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}
	return BinaryToBase64LineFeed(cpInChar, nInSize);
}

WCHAR* BinaryToBase64LineFeedToWChar(const void* cpIn, size_t nInSize)
{
	WCHAR* pBase64WChar;
	size_t nBase64WCharSize;
	size_t nRet;
	size_t nCount;
	char* pSrc;
	WCHAR* pDst;

	nBase64WCharSize = Base64LineFeedEncodeLength((const char*)cpIn, nInSize);
	pBase64WChar = (WCHAR*)malloc(nBase64WCharSize * sizeof(WCHAR));
	IF_UNLIKELY(pBase64WChar == NULL) {
		return NULL;
	}
	pSrc = (char*)cpIn;
	pDst = pBase64WChar;
	nCount = nInSize;
	while (nCount >= 48)
	{
		Base64EncodeToWChar(pDst, pSrc, 48);
		pSrc += 48;
		pDst += 64;
		*pDst++ = '\r';
		*pDst++ = '\n';
		nCount -= 48;
	}
	if (nCount > 0)
	{
		nRet = Base64EncodeToWChar(pDst, pSrc, nCount);
		pDst += nRet - 1;
		*pDst++ = '\r';
		*pDst++ = '\n';
	}
	*pDst++ = '\0';
	return pBase64WChar;
}

WCHAR* CharToBase64LineFeedToWChar(const char* cpInChar, size_t nInSize)
{
	if (nInSize == (size_t)-1) {
		nInSize = strlen(cpInChar);
	}
	return BinaryToBase64LineFeedToWChar(cpInChar, nInSize);
}

void* Base64ToBinary(const char* cpInBase64Char, void* pOut, size_t *pOutSize)
{
	*pOutSize = Base64DecodeLength(cpInBase64Char);
	IF_UNLIKELY(pOut == NULL)
	{
		pOut = (char*)malloc(*pOutSize);
		IF_UNLIKELY(pOut == NULL) {
			return NULL;
		}
	}
	*pOutSize = Base64Decode((char*)pOut, cpInBase64Char);
	return pOut;
}

char* Base64ToChar(const char* cpInBase64Char, char* pOutChar)
{
	size_t nOutCharSize;

	nOutCharSize = Base64DecodeLength(cpInBase64Char);
	IF_UNLIKELY(pOutChar == NULL)
	{
		pOutChar = (char*)malloc(nOutCharSize + 1);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}
	Base64Decode(pOutChar, cpInBase64Char);
	return pOutChar;
}

char* WCharToBase64ToChar(const WCHAR* cpInBase64WChar, char* pOutChar)
{
	size_t nOutCharSize;

	nOutCharSize = Base64ToWCharDecodeLength(cpInBase64WChar);
	IF_UNLIKELY(pOutChar == NULL)
	{
		pOutChar = (char*)malloc(nOutCharSize + 1);
		IF_UNLIKELY(pOutChar == NULL) {
			return NULL;
		}
	}
	Base64ToWCharDecode(pOutChar, cpInBase64WChar);
	return pOutChar;
}
