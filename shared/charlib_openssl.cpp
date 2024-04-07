#include <stdio.h>
#include <string.h>

#include <share.h>
#include "charlib.h"
#include "charlib_base64.h"
#include "charlib_openssl.h"

#define XORKEY1 'M'
#define XORKEY2 'O'

const unsigned char iv[] =
{
	0xE3 ^ XORKEY1,
	0x81 ^ XORKEY2,
	0x8B ^ XORKEY1,
	0xE3 ^ XORKEY2,
	0x81 ^ XORKEY1,
	0x95 ^ XORKEY2,
	0x4A ^ XORKEY1,
	0x75 ^ XORKEY2,
	0x6E ^ XORKEY1,
	0x63 ^ XORKEY2,
	0x68 ^ XORKEY1,
	0x61 ^ XORKEY2,
	0x72 ^ XORKEY1,
	0x6C ^ XORKEY2,
	0x69 ^ XORKEY1,
	0x62 ^ XORKEY2
};


unsigned __int64 FileToFileAes256EncryptCore(FILE* fpInFile, FILE* fpOutFile, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64* nFileReadingSize)
{
	EVP_CIPHER_CTX* cpCTX = NULL;
	size_t deciv[(sizeof(iv) + 8) / sizeof(size_t)] = { 0 };
	size_t byHash[32 / sizeof(size_t)] = { 0 };
	__int64 nFileReading = 0;
	unsigned char* pFileWriteBuf;
	int nOutLength = 0;
	unsigned char* key;
	size_t nRet = 0;
	int nIsMalloc = 0;
	int nPaddingSize = 0;

	if (pFileReadBuf == NULL)
	{
		nFileReadBufSize = 0xFFFF;
		pFileReadBuf = (unsigned char*)malloc(nFileReadBufSize);
		if (pFileReadBuf == NULL) {
			return 0;
		}
		nIsMalloc++;
	}
	pFileWriteBuf = (unsigned char*)malloc(nFileReadBufSize);
	if (pFileWriteBuf == NULL){
		goto ENCRYPT_ERR;
	}
	nIsMalloc++;

	if (nFileReadingSize == NULL) {
		nFileReadingSize = &nFileReading;
	}
	*nFileReadingSize = 0;


	if (1)
	{
		size_t* src = (size_t*)iv;
		size_t* dsc = (size_t*)deciv;
		size_t xorkey;

		xorkey = XORKEY1;
		xorkey += (size_t)XORKEY2 << 8;
		xorkey += (size_t)XORKEY1 << 16;
		xorkey += (size_t)XORKEY2 << 24;

#if SIZE_MAX == 0xffffffffffffffffui64
		xorkey += (size_t)XORKEY1 << 32;
		xorkey += (size_t)XORKEY2 << 40;
		xorkey += (size_t)XORKEY1 << 48;
		xorkey += (size_t)XORKEY2 << 56;
#endif

		*dsc++ = xorkey ^ *src++;
		*dsc++ = xorkey ^ *src++;

#if SIZE_MAX != 0xffffffffffffffffui64
		*dsc++ = xorkey ^ *src++;
		*dsc++ = xorkey ^ *src++;

		*dsc++ = xorkey;
#endif

		*dsc++ = xorkey;
	}


	key = CharToDigest(deciv, sizeof(iv), EVP_sha256(), (unsigned char*)byHash);
	if (key == NULL) {
		goto ENCRYPT_ERR;
	}

	cpCTX = EVP_CIPHER_CTX_new();
	if (cpCTX == FALSE) {
		goto ENCRYPT_ERR;
	}

	nRet = EVP_CIPHER_CTX_init(cpCTX);
	if (nRet == FALSE) {
		goto ENCRYPT_ERR;
	}

	nRet = EVP_EncryptInit_ex(cpCTX, EVP_aes_256_cbc(), NULL, key, (unsigned char*)deciv);
	if (nRet == FALSE) {
		goto ENCRYPT_ERR;
	}

	nRet = fwrite(iv, 1, sizeof(iv), fpOutFile);
	if (!nRet) {
		goto ENCRYPT_ERR;
	}

	while (1)
	{
		const size_t nFileRead = fread(pFileReadBuf, 1, nFileReadBufSize, fpInFile);
		if (!nFileRead) {
			break;
		}
		*nFileReadingSize += nFileRead;

		nRet = EVP_EncryptUpdate(cpCTX, pFileWriteBuf, &nOutLength, (unsigned char*)pFileReadBuf, (int)nFileRead);
		if (nRet == FALSE) {
			goto ENCRYPT_ERR;
		}
		if (!nOutLength) {
			continue;
		}

		nRet = fwrite(pFileWriteBuf, 1, nOutLength, fpOutFile);
		if (!nRet) {
			goto ENCRYPT_ERR;
		}
	}

	nRet = EVP_EncryptFinal_ex(cpCTX, pFileWriteBuf + nOutLength, &nPaddingSize);
	if (nRet == FALSE) {
		goto ENCRYPT_ERR;
	}

	if (nPaddingSize)
	{
		nRet = fwrite(pFileWriteBuf + nOutLength, 1, nPaddingSize, fpOutFile);
		if (!nRet) {
			goto ENCRYPT_ERR;
		}
	}

	SecureZeroMemory(deciv, sizeof(iv));
	SecureZeroMemory(byHash, sizeof(byHash));
	free(pFileWriteBuf);
	EVP_CIPHER_CTX_cleanup(cpCTX);
	EVP_CIPHER_CTX_free(cpCTX);

#if _MSC_VER > 1300
	return _ftelli64(fpOutFile);
#else
	return ftell(fpOutFile);
#endif

ENCRYPT_ERR:
	SecureZeroMemory(deciv, sizeof(iv));
	SecureZeroMemory(byHash, sizeof(byHash));
	if (nIsMalloc--)
	{
		free(pFileWriteBuf);
		if (nIsMalloc) {
			free(pFileReadBuf);
		}
	}
	EVP_CIPHER_CTX_cleanup(cpCTX);
	EVP_CIPHER_CTX_free(cpCTX);
	return 0;
}

unsigned __int64 FileToFileAes256EncryptA(const char* cpInFileName, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64* nFileReadingSize)
{
	FILE* fpInFile = NULL;
	FILE* fpOutFile = NULL;
	char* pOutFileName = NULL;
	unsigned __int64 nOutLength = 0;

	pOutFileName = CharToCopy2ToPadding(cpInFileName, ".aes", 1);
	if (pOutFileName == NULL) {
		return 0;
	}

	if ((fpInFile = _fsopen(cpInFileName, "rb", _SH_DENYWR)) == NULL) {
		goto ENCRYPT_ERR;
	}

	if ((fpOutFile = _fsopen(pOutFileName, "wb", _SH_DENYRW)) == NULL) {
		goto ENCRYPT_ERR;
	}

	nOutLength = FileToFileAes256EncryptCore(fpInFile, fpOutFile, pFileReadBuf, nFileReadBufSize, nFileReadingSize);

	free(pOutFileName);
	if (fpInFile) {
		fclose(fpInFile);
	}
	if (fpOutFile) {
		fclose(fpOutFile);
	}
	return nOutLength;

ENCRYPT_ERR:
	free(pOutFileName);
	if (fpInFile) {
		fclose(fpInFile);
	}
	if (fpOutFile) {
		fclose(fpOutFile);
	}
	return 0;
}

unsigned __int64 FileToFileAes256EncryptW(const WCHAR* cpInFileName, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64* nFileReadingSize)
{
	FILE* fpInFile = NULL;
	FILE* fpOutFile = NULL;
	WCHAR* pOutFileName = NULL;
	unsigned __int64 nOutLength = 0;

	pOutFileName = WCharToCopy2ToPadding(cpInFileName, L".aes", 1);
	if (pOutFileName == NULL) {
		return 0;
	}

	if ((fpInFile = _wfsopen(cpInFileName, L"rb", _SH_DENYWR)) == NULL) {
		goto ENCRYPT_ERR;
	}

	if ((fpOutFile = _wfsopen(pOutFileName, L"wb", _SH_DENYRW)) == NULL) {
		goto ENCRYPT_ERR;
	}

	nOutLength = FileToFileAes256EncryptCore(fpInFile, fpOutFile, pFileReadBuf, nFileReadBufSize, nFileReadingSize);

	free(pOutFileName);
	if (fpInFile) {
		fclose(fpInFile);
	}
	if (fpOutFile) {
		fclose(fpOutFile);
	}
	return nOutLength;

ENCRYPT_ERR:
	free(pOutFileName);
	if (fpInFile) {
		fclose(fpInFile);
	}
	if (fpOutFile) {
		fclose(fpOutFile);
	}
	return 0;
}

unsigned __int64 FileToFileAes256DecryptCore(FILE* fpInFile, FILE* fpOutFile, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64* nFileReadingSize)
{
	EVP_CIPHER_CTX* cpCTX = NULL;
	size_t deciv[(sizeof(iv) + 8) / sizeof(size_t)] = { 0 };
	size_t byHash[32 / sizeof(size_t)] = { 0 };
	__int64 nFileReading = 0;
	unsigned char* pFileWriteBuf = NULL;
	int nOutLength = 0;
	unsigned char* key = NULL;
	size_t nRet = 0;
	int nIsMalloc = 0;
	int nPaddingSize = 0;


	if (pFileReadBuf == NULL)
	{
		nFileReadBufSize = 0xFFFF;
		pFileReadBuf = (unsigned char*)malloc(nFileReadBufSize);
		if (pFileReadBuf == NULL) {
			return 0;
		}
		nIsMalloc++;
	}
	pFileWriteBuf = (unsigned char*)malloc(nFileReadBufSize);
	if (pFileWriteBuf == NULL) {
		goto DECRYPT_ERR;
	}
	nIsMalloc++;

	if (nFileReadingSize == NULL) {
		nFileReadingSize = &nFileReading;
	}
	*nFileReadingSize = 0;


	if (1)
	{
		size_t* src = (size_t*)iv;
		size_t* dsc = (size_t*)deciv;
		size_t xorkey;

		xorkey = XORKEY1;
		xorkey += (size_t)XORKEY2 << 8;
		xorkey += (size_t)XORKEY1 << 16;
		xorkey += (size_t)XORKEY2 << 24;

#if SIZE_MAX == 0xffffffffffffffffui64
		xorkey += (size_t)XORKEY1 << 32;
		xorkey += (size_t)XORKEY2 << 40;
		xorkey += (size_t)XORKEY1 << 48;
		xorkey += (size_t)XORKEY2 << 56;
#endif

		*dsc++ = xorkey ^ *src++;
		*dsc++ = xorkey ^ *src++;

#if SIZE_MAX != 0xffffffffffffffffui64
		*dsc++ = xorkey ^ *src++;
		*dsc++ = xorkey ^ *src++;

		*dsc++ = xorkey;
#endif

		*dsc++ = xorkey;
	}


	key = CharToDigest(deciv, sizeof(iv), EVP_sha256(), (unsigned char*)byHash);
	if (key == NULL) {
		goto DECRYPT_ERR;
	}

	cpCTX = EVP_CIPHER_CTX_new();
	if (cpCTX == FALSE) {
		goto DECRYPT_ERR;
	}

	nRet = EVP_CIPHER_CTX_init(cpCTX);
	if (nRet == FALSE) {
		goto DECRYPT_ERR;
	}

	nRet = EVP_DecryptInit_ex(cpCTX, EVP_aes_256_cbc(), NULL, key, (unsigned char*)deciv);
	if (nRet == FALSE) {
		goto DECRYPT_ERR;
	}

	nRet = fread(pFileReadBuf, 1, sizeof(iv), fpInFile);
	if (nRet < 16) {
		goto DECRYPT_ERR;
	}
	*nFileReadingSize += nRet;

	if (memcmp(pFileReadBuf, iv, sizeof(iv)) != 0) {
		goto DECRYPT_ERR;
	}

	while (1)
	{
		const size_t nFileRead = fread(pFileReadBuf, 1, nFileReadBufSize, fpInFile);
		if (!nFileRead) {
			break;
		}
		*nFileReadingSize += nFileRead;

		nRet = EVP_DecryptUpdate(cpCTX, pFileWriteBuf, &nOutLength, (unsigned char*)pFileReadBuf, (int)nFileRead);
		if (nRet == FALSE) {
			goto DECRYPT_ERR;
		}
		if (!nOutLength) {
			continue;
		}

		nRet = fwrite(pFileWriteBuf, 1, nOutLength, fpOutFile);
		if (!nRet) {
			goto DECRYPT_ERR;
		}
	}

	nRet = EVP_DecryptFinal_ex(cpCTX, pFileWriteBuf + nOutLength, &nPaddingSize);
	if (nRet == FALSE) {
		goto DECRYPT_ERR;
	}

	if (nPaddingSize)
	{
		nRet = fwrite(pFileWriteBuf + nOutLength, 1, nPaddingSize, fpOutFile);
		if (!nRet) {
			goto DECRYPT_ERR;
		}
	}

	SecureZeroMemory(deciv, sizeof(iv));
	SecureZeroMemory(byHash, sizeof(byHash));
	free(pFileWriteBuf);
	EVP_CIPHER_CTX_cleanup(cpCTX);
	EVP_CIPHER_CTX_free(cpCTX);

#if _MSC_VER > 1300
	return _ftelli64(fpOutFile);
#else
	return ftell(fpOutFile);
#endif

DECRYPT_ERR:
	SecureZeroMemory(deciv, sizeof(iv));
	SecureZeroMemory(byHash, sizeof(byHash));
	if (nIsMalloc--)
	{
		free(pFileWriteBuf);
		if (nIsMalloc) {
			free(pFileReadBuf);
		}
	}
	EVP_CIPHER_CTX_cleanup(cpCTX);
	EVP_CIPHER_CTX_free(cpCTX);
	return 0;
}

unsigned __int64 FileToFileAes256DecryptA(const char* cpInFileName, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64* nFileReadingSize)
{
	FILE* fpInFile = NULL;
	FILE* fpOutFile = NULL;
	char* pOutFileName = NULL;
	char* pTemp;
	unsigned __int64 nOutLength = 0;

	pOutFileName = CharToCopyToPadding(cpInFileName, 1);
	if (pOutFileName == NULL) {
		return 0;
	}
	pTemp = GetCharToExtension(pOutFileName);
	if (pTemp == NULL) {
		goto DECRYPT_ERR;
	}
	*pTemp = '\0';

	if ((fpInFile = _fsopen(cpInFileName, "rb", _SH_DENYWR)) == NULL) {
		goto DECRYPT_ERR;
	}

	if ((fpOutFile = _fsopen(pOutFileName, "wb", _SH_DENYRW)) == NULL) {
		goto DECRYPT_ERR;
	}

	nOutLength = FileToFileAes256DecryptCore(fpInFile, fpOutFile, pFileReadBuf, nFileReadBufSize, nFileReadingSize);

	free(pOutFileName);
	if (fpInFile) {
		fclose(fpInFile);
	}
	if (fpOutFile) {
		fclose(fpOutFile);
	}
	return nOutLength;

DECRYPT_ERR:
	free(pOutFileName);
	if (fpInFile) {
		fclose(fpInFile);
	}
	if (fpOutFile) {
		fclose(fpOutFile);
	}
	return 0;
}

unsigned __int64 FileToFileAes256DecryptW(const WCHAR* cpInFileName, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64* nFileReadingSize)
{
	FILE* fpInFile = NULL;
	FILE* fpOutFile = NULL;
	WCHAR* pOutFileName = NULL;
	WCHAR* pTemp;
	unsigned __int64 nOutLength = 0;

	pOutFileName = WCharToCopyToPadding(cpInFileName, 1);
	if (pOutFileName == NULL) {
		return 0;
	}
	pTemp = GetWCharToExtension(pOutFileName);
	if (pTemp == NULL) {
		goto DECRYPT_ERR;
	}
	*pTemp = '\0';

	if ((fpInFile = _wfsopen(cpInFileName, L"rb", _SH_DENYWR)) == NULL) {
		goto DECRYPT_ERR;
	}

	if ((fpOutFile = _wfsopen(pOutFileName, L"wb", _SH_DENYRW)) == NULL) {
		goto DECRYPT_ERR;
	}

	nOutLength = FileToFileAes256DecryptCore(fpInFile, fpOutFile, pFileReadBuf, nFileReadBufSize, nFileReadingSize);

	free(pOutFileName);
	if (fpInFile) {
		fclose(fpInFile);
	}
	if (fpOutFile) {
		fclose(fpOutFile);
	}
	return nOutLength;

DECRYPT_ERR:
	free(pOutFileName);
	if (fpInFile) {
		fclose(fpInFile);
	}
	if (fpOutFile) {
		fclose(fpOutFile);
	}
	return 0;
}

char* Aes256EncryptToBase64(const void* cpIn, const size_t nInLength, char* pOutEncrypt)
{
	unsigned char* byAes;
	int nAesLength = 0;

	byAes = Aes256Encrypt(cpIn, nInLength, NULL, &nAesLength);
	if (byAes == NULL) {
		return NULL;
	}
	pOutEncrypt = BinaryToBase64(byAes, nAesLength, pOutEncrypt);
	free(byAes);
	return pOutEncrypt;
}

void* Base64ToAes256Decrypt(const char* cpInEncrypt, void* pOut, int* nOutLength)
{
	unsigned char* byAes;
	size_t nAesLength = 0;

	byAes = (unsigned char*)Base64ToBinary(cpInEncrypt, NULL, &nAesLength);
	if (byAes == NULL) {
		return NULL;
	}
	pOut = Aes256Decrypt(byAes, nAesLength, (unsigned char*)pOut, nOutLength);
	free(byAes);
	return pOut;
}

unsigned char* Aes256Encrypt(const void* cpIn, const size_t nInLength, unsigned char* pOutEncrypt, int* nOutLength)
{
	EVP_CIPHER_CTX* cpCTX = NULL;
	size_t deciv[(sizeof(iv) + 8) / sizeof(size_t)] = { 0 };
	size_t byHash[32 / sizeof(size_t)] = { 0 };
	unsigned char* key = NULL;
	int nRet;
	int nIsMalloc = 0;
	int nPaddingSize = 0;

	if (pOutEncrypt == NULL)
	{
		pOutEncrypt = (unsigned char*)malloc(nInLength + EVP_MAX_BLOCK_LENGTH);
		if (pOutEncrypt == NULL) {
			return NULL;
		}
		nIsMalloc++;
	}

	if (1)
	{
		size_t* src = (size_t*)iv;
		size_t* dsc = (size_t*)deciv;
		size_t xorkey;

		xorkey = XORKEY1;
		xorkey += (size_t)XORKEY2 << 8;
		xorkey += (size_t)XORKEY1 << 16;
		xorkey += (size_t)XORKEY2 << 24;

#if SIZE_MAX == 0xffffffffffffffffui64
		xorkey += (size_t)XORKEY1 << 32;
		xorkey += (size_t)XORKEY2 << 40;
		xorkey += (size_t)XORKEY1 << 48;
		xorkey += (size_t)XORKEY2 << 56;
#endif

		*dsc++ = xorkey ^ *src++;
		*dsc++ = xorkey ^ *src++;

#if SIZE_MAX != 0xffffffffffffffffui64
		*dsc++ = xorkey ^ *src++;
		*dsc++ = xorkey ^ *src++;

		*dsc++ = xorkey;
#endif

		*dsc++ = xorkey;
	}


	key = CharToDigest(deciv, sizeof(iv), EVP_sha256(), (unsigned char*)byHash);
	if (key == NULL) {
		goto ENCRYPT_ERR;
	}

	cpCTX = EVP_CIPHER_CTX_new();
	if (cpCTX == NULL) {
		goto ENCRYPT_ERR;
	}

	nRet = EVP_CIPHER_CTX_init(cpCTX);
	if (nRet == FALSE) {
		goto ENCRYPT_ERR;
	}

	nRet = EVP_EncryptInit_ex(cpCTX, EVP_aes_256_cbc(), NULL, key, (unsigned char*)deciv);
	if (nRet == FALSE) {
		goto ENCRYPT_ERR;
	}


	nRet = EVP_EncryptUpdate(cpCTX, pOutEncrypt, nOutLength, (unsigned char*)cpIn, (int)nInLength);
	if (nRet == FALSE) {
		goto ENCRYPT_ERR;
	}

	nRet = EVP_EncryptFinal_ex(cpCTX, pOutEncrypt + *nOutLength, &nPaddingSize);
	if (nRet == FALSE) {
		goto ENCRYPT_ERR;
	}
	*nOutLength += nPaddingSize;

	SecureZeroMemory(deciv, sizeof(iv));
	SecureZeroMemory(byHash, sizeof(byHash));
	EVP_CIPHER_CTX_cleanup(cpCTX);
	EVP_CIPHER_CTX_free(cpCTX);

	return pOutEncrypt;

ENCRYPT_ERR:
	SecureZeroMemory(deciv, sizeof(iv));
	SecureZeroMemory(byHash, sizeof(byHash));
	if (nIsMalloc) {
		free(pOutEncrypt);
	}
	EVP_CIPHER_CTX_cleanup(cpCTX);
	EVP_CIPHER_CTX_free(cpCTX);
	return NULL;
}

void* Aes256Decrypt(const unsigned char* cpInEncrypt, const size_t nInLength, unsigned char* pOut, int* nOutLength)
{
	EVP_CIPHER_CTX* cpCTX = NULL;
	size_t deciv[(sizeof(iv) + 8) / sizeof(size_t)] = { 0 };
	size_t byHash[32 / sizeof(size_t)] = { 0 };
	unsigned char* key = NULL;
	int nRet = 0;
	int nIsMalloc = 0;
	int nPaddingSize = 0;

	if (pOut == NULL)
	{
		pOut = (unsigned char*)malloc(nInLength);
		if (pOut == NULL) {
			return NULL;
		}
		nIsMalloc++;
	}

	if (1)
	{
		size_t* src = (size_t*)iv;
		size_t* dsc = (size_t*)deciv;
		size_t xorkey;

		xorkey = XORKEY1;
		xorkey += (size_t)XORKEY2 << 8;
		xorkey += (size_t)XORKEY1 << 16;
		xorkey += (size_t)XORKEY2 << 24;

#if SIZE_MAX == 0xffffffffffffffffui64
		xorkey += (size_t)XORKEY1 << 32;
		xorkey += (size_t)XORKEY2 << 40;
		xorkey += (size_t)XORKEY1 << 48;
		xorkey += (size_t)XORKEY2 << 56;
#endif

		*dsc++ = xorkey ^ *src++;
		*dsc++ = xorkey ^ *src++;

#if SIZE_MAX != 0xffffffffffffffffui64
		*dsc++ = xorkey ^ *src++;
		*dsc++ = xorkey ^ *src++;

		*dsc++ = xorkey;
#endif

		*dsc++ = xorkey;
	}


	key = CharToDigest(deciv, sizeof(iv), EVP_sha256(), (unsigned char*)byHash);
	if (key == NULL) {
		goto DECRYPT_ERR;
	}

	cpCTX = EVP_CIPHER_CTX_new();
	if (cpCTX == NULL) {
		goto DECRYPT_ERR;
	}

	nRet = EVP_CIPHER_CTX_init(cpCTX);
	if (nRet == FALSE) {
		goto DECRYPT_ERR;
	}

	nRet = EVP_DecryptInit_ex(cpCTX, EVP_aes_256_cbc(), NULL, key, (unsigned char*)deciv);
	if (nRet == FALSE) {
		goto DECRYPT_ERR;
	}

	nRet = EVP_DecryptUpdate(cpCTX, (unsigned char*)pOut, nOutLength, (unsigned char*)cpInEncrypt, (int)nInLength);
	if (nRet == FALSE) {
		goto DECRYPT_ERR;
	}

	nRet = EVP_DecryptFinal_ex(cpCTX, pOut + *nOutLength, &nPaddingSize);
	if (nRet == FALSE) {
		goto DECRYPT_ERR;
	}
	memset(pOut + *nOutLength + nPaddingSize, 0, nInLength - *nOutLength - nPaddingSize);
	*nOutLength += nPaddingSize;

	SecureZeroMemory(deciv, sizeof(iv));
	SecureZeroMemory(byHash, sizeof(byHash));
	EVP_CIPHER_CTX_cleanup(cpCTX);
	EVP_CIPHER_CTX_free(cpCTX);

	return pOut;

DECRYPT_ERR:
	SecureZeroMemory(deciv, sizeof(iv));
	SecureZeroMemory(byHash, sizeof(byHash));
	if (nIsMalloc) {
		free(pOut);
	}
	EVP_CIPHER_CTX_cleanup(cpCTX);
	EVP_CIPHER_CTX_free(cpCTX);
	return NULL;
}

unsigned char* CharToAes256Encrypt(const char* cpInChar, int* nOutLength, unsigned char* pOutEncrypt)
{
	int nInCharSize = (int)strlen(cpInChar);
	return (unsigned char*)Aes256Decrypt((const unsigned char*)cpInChar, nInCharSize, pOutEncrypt, nOutLength);
}

unsigned char* WCharToAes256Encrypt(const WCHAR* cpInWChar, int* nOutLength, unsigned char* pOutEncrypt)
{
	int nInWCharSize = (int)wcslen(cpInWChar);
	return (unsigned char*)Aes256Decrypt((const unsigned char*)cpInWChar, nInWCharSize, pOutEncrypt, nOutLength);
}

char* CharToAes256EncryptToBase64(const char* cpInChar, char* pOutEncrypt)
{
	int nInCharSize = (int)strlen(cpInChar);
	return Aes256EncryptToBase64(cpInChar, nInCharSize, pOutEncrypt);
}

char* WCharToAes256EncryptToBase64(const WCHAR* cpInWChar, char* pOutEncrypt)
{
	int nInWCharSize = (int)wcslen(cpInWChar);
	return Aes256EncryptToBase64(cpInWChar, nInWCharSize * sizeof(WCHAR), pOutEncrypt);
}

char* Aes256DecryptToChar(const unsigned char* cpInEncrypt, const size_t nInLength, char* pOut)
{
	int nOutCharSize = 0;
	pOut = (char*)Aes256Decrypt(cpInEncrypt, nInLength, (unsigned char*)pOut, &nOutCharSize);
	if (pOut == NULL) {
		return NULL;
	}
	pOut[nOutCharSize + 1] = '\0';
	return pOut;
}

WCHAR* Aes256DecryptToWChar(const unsigned char* cpInEncrypt, const size_t nInLength, WCHAR* pOut)
{
	int nOutWCharSize = 0;
	pOut = (WCHAR*)Aes256Decrypt(cpInEncrypt, nInLength, (unsigned char*)pOut, &nOutWCharSize);
	if (pOut == NULL) {
		return NULL;
	}
	pOut[((size_t)nOutWCharSize + 1) / sizeof(WCHAR)] = '\0';
	return pOut;
}

char* Base64ToAes256DecryptToChar(const char* cpInEncrypt, char* pOut)
{
	int nOutCharSize = 0;
	pOut = (char*)Base64ToAes256Decrypt(cpInEncrypt, (unsigned char*)pOut, &nOutCharSize);
	if (pOut == NULL) {
		return NULL;
	}
	pOut[nOutCharSize + 1] = '\0';
	return pOut;
}

WCHAR* Base64ToAes256DecryptToWChar(const char* cpInEncrypt, WCHAR* pOut)
{
	int nOutWCharSize = 0;
	pOut = (WCHAR*)Base64ToAes256Decrypt(cpInEncrypt, (unsigned char*)pOut, &nOutWCharSize);
	if (pOut == NULL) {
		return NULL;
	}
	pOut[((size_t)nOutWCharSize + 1) / sizeof(WCHAR)] = '\0';
	return pOut;
}

unsigned char* FileToDigestCore(FILE* fpFile, const EVP_MD* cpDigestType, unsigned char* pOutHash, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64 *nFileReadingSize)
{
	EVP_MD_CTX* mdCTX = NULL;
	__int64 nFileReading = 0;
	int nIsMalloc = 0;
	int nRet = 0;

	if (pOutHash == NULL)
	{
		pOutHash = (unsigned char*)malloc(EVP_MD_size(cpDigestType));
		if (pOutHash == NULL) {
			return NULL;
		}
		nIsMalloc++;
	}

	if (pFileReadBuf == NULL)
	{
		nFileReadBufSize = 0xFFFF;
		pFileReadBuf = (unsigned char*)malloc(nFileReadBufSize);
		if (pFileReadBuf == NULL) {
			goto DIGEST_ERR;
		}
		nIsMalloc++;
	}

	if (nFileReadBufSize == 0) {
		nFileReadBufSize = 0x3FFFF;
	}

	if (nFileReadingSize == NULL) {
		nFileReadingSize = &nFileReading;
	}
	*nFileReadingSize = 0;

	mdCTX = EVP_MD_CTX_new();
	if (mdCTX == NULL) {
		goto DIGEST_ERR;
	}

	nRet = EVP_DigestInit_ex(mdCTX, cpDigestType, NULL);
	if (nRet == FALSE) {
		goto DIGEST_ERR;
	}

	while (1)
	{
		const size_t nFileRead = fread(pFileReadBuf, 1, nFileReadBufSize, fpFile);
		if (!nFileRead) {
			break;
		}
		*nFileReadingSize += nFileRead;

		nRet = EVP_DigestUpdate(mdCTX, pFileReadBuf, nFileRead);
		if (nRet == FALSE) {
			goto DIGEST_ERR;
		}
	}

	nRet = EVP_DigestFinal_ex(mdCTX, pOutHash, NULL);
	EVP_MD_CTX_free(mdCTX);
	return pOutHash;

DIGEST_ERR:
	if (nIsMalloc--)
	{
		free(pOutHash);
		if (nIsMalloc) {
			free(pFileReadBuf);
		}
	}
	EVP_MD_CTX_free(mdCTX);
	return NULL;
}

unsigned char* FileToDigestA(const char* cpInFileName, const EVP_MD* cpDigestType, unsigned char* pOutHash, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64* nFileReadingSize)
{
	FILE* fpInFile;
	unsigned char *nDigest = NULL;

	if ((fpInFile = _fsopen(cpInFileName, "rb", _SH_DENYWR)) != NULL)
	{
		nDigest = FileToDigestCore(fpInFile, cpDigestType, pOutHash, pFileReadBuf, nFileReadBufSize, nFileReadingSize);
		fclose(fpInFile);
	}
	return nDigest;
}

unsigned char* FileToDigestW(const WCHAR* cpInFileName, const EVP_MD* cpDigestType, unsigned char* pOutHash, unsigned char* pFileReadBuf, size_t nFileReadBufSize, __int64* nFileReadingSize)
{
	FILE* fpInFile;
	unsigned char* nDigest = NULL;

	if ((fpInFile = _wfsopen(cpInFileName, L"rb", _SH_DENYWR)) != NULL)
	{
		nDigest = FileToDigestCore(fpInFile, cpDigestType, pOutHash, pFileReadBuf, nFileReadBufSize, nFileReadingSize);
		fclose(fpInFile);
	}
	return nDigest;
}

unsigned char* CharToDigest(const void* cpIn, const size_t nLength, const EVP_MD* cpDigestType, unsigned char* pOutHash)
{
	EVP_MD_CTX* mdCTX;
	int nRet = 0;
	int nIsMalloc = 0;

	if (pOutHash == NULL)
	{
		pOutHash = (unsigned char*)malloc(EVP_MD_size(cpDigestType));
		if (pOutHash == NULL) {
			return NULL;
		}
		nIsMalloc++;
	}

	mdCTX = EVP_MD_CTX_new();
	if (mdCTX == NULL) {
		goto DIGEST_ERR;
	}

	nRet = EVP_DigestInit_ex(mdCTX, cpDigestType, NULL);
	if (nRet == FALSE) {
		goto DIGEST_ERR;
	}

	nRet = EVP_DigestUpdate(mdCTX, cpIn, nLength);
	if (nRet == FALSE) {
		goto DIGEST_ERR;
	}

	nRet = EVP_DigestFinal_ex(mdCTX, pOutHash, NULL);
	if (nRet == FALSE) {
		goto DIGEST_ERR;
	}
	EVP_MD_CTX_free(mdCTX);
	return pOutHash;

DIGEST_ERR:
	if (nIsMalloc) {
		free(pOutHash);
	}
	EVP_MD_CTX_free(mdCTX);
	return NULL;
}

#define CHAR_TO_HASH_CHAR_FUNC(NAME, BITLEN, DIGEST_FUNC_NAME) \
char* CharTo ## NAME ## Char(const void* cpInChar, const size_t nLength, char* pOutChar) \
{ \
	size_t byHash[(BITLEN) / 8 / sizeof(size_t)] = { 0 }; \
	unsigned char* bRet; \
 \
	bRet = CharToDigest(cpInChar, nLength, ## DIGEST_FUNC_NAME ##, (unsigned char*)byHash); \
	if (bRet == NULL) { \
		return NULL; \
	} \
	return BinaryToChar(pOutChar, (unsigned char*)byHash, sizeof(byHash), FALSE); \
} \
 \
char* CharTo ## NAME ## Char(const char* cpInChar, char* pOutChar) \
{ \
	return CharTo ## NAME ## Char(cpInChar, strlen(cpInChar), pOutChar); \
} \
 \
char* WCharTo ## NAME ## Char(const WCHAR* cpInWChar, char* pOutChar) \
{ \
	return CharTo ## NAME ## Char(cpInWChar, wcslen(cpInWChar) * sizeof(WCHAR), pOutChar); \
} \
 \
char* WCharToUtf8CharTo ## NAME ## Char(const WCHAR* cpInWChar, char* pOutChar) \
{ \
	char* pBufChar; \
 \
	pBufChar = WCharToUtf8CharConv(cpInWChar); \
	if (pBufChar == NULL) { \
		return NULL; \
	} \
	pOutChar = CharTo ## NAME ## Char(pBufChar, strlen(pBufChar), pOutChar); \
	free(pBufChar); \
	return pOutChar; \
} \
 \
char* CharTo ## NAME ## ToBase64Char(const void* cpIn, const size_t nLength, char* pOutChar, size_t nOutCharSize) \
{ \
	size_t byHash[(BITLEN) / 8 / sizeof(size_t)] = { 0 }; \
	unsigned char* bRet; \
 \
	bRet = CharToDigest(cpIn, nLength, ## DIGEST_FUNC_NAME ##, (unsigned char*)byHash); \
	if (bRet == NULL) { \
		return NULL; \
	} \
	CharSizeToBase64v2((char*)byHash, sizeof(byHash), pOutChar); \
	return pOutChar; \
} \
 \
char* WCharToUtf8CharTo ## NAME ## ToBase64WChar(const WCHAR* cpInWChar, char* pOutChar) \
{ \
	char* pBufChar; \
 \
	pBufChar = WCharToUtf8CharConv(cpInWChar); \
	if (pBufChar == NULL) { \
		return NULL; \
	} \
	pOutChar = CharTo ## NAME ## ToBase64Char(pBufChar, strlen(pBufChar), pOutChar); \
	free(pBufChar); \
	return pOutChar; \
} \
 \
WCHAR* CharTo ## NAME ## WChar(const void* cpInChar, const size_t nLength, WCHAR* pOutWChar) \
{ \
	size_t byHash[(BITLEN) / 8 / sizeof(size_t)] = { 0 }; \
	unsigned char* bRet; \
 \
	bRet = CharToDigest(cpInChar, nLength, ## DIGEST_FUNC_NAME ##, (unsigned char*)byHash); \
	if (bRet == NULL) { \
		return NULL; \
	} \
	return BinaryToWChar(pOutWChar, (unsigned char*)byHash, sizeof(byHash), FALSE); \
} \
 \
WCHAR* CharTo ## NAME ## WChar(const char* cpInChar, WCHAR* pOutWChar) \
{ \
	return CharTo ## NAME ## WChar(cpInChar, strlen(cpInChar), pOutWChar); \
} \
 \
WCHAR* WCharTo ## NAME ## WChar(const WCHAR* cpInWChar, WCHAR* pOutWChar) \
{ \
	return CharTo ## NAME ## WChar(cpInWChar, wcslen(cpInWChar) * sizeof(WCHAR), pOutWChar); \
} \
 \
WCHAR* WCharToUtf8CharTo ## NAME ## WChar(const WCHAR* cpInWChar, WCHAR* pOutWChar) \
{ \
	char* pBufChar; \
 \
	pBufChar = WCharToUtf8CharConv(cpInWChar); \
	if (pBufChar == NULL) { \
		return NULL; \
	} \
	pOutWChar = CharTo ## NAME ## WChar(pBufChar, strlen(pBufChar), pOutWChar); \
	free(pBufChar); \
	return pOutWChar; \
} \
WCHAR* CharTo ## NAME ## ToBase64WChar(const void* cpIn, const size_t nLength, WCHAR* pOutWChar, size_t nOutCharSize) \
{ \
	size_t byHash[(BITLEN) / 8 / sizeof(size_t)] = { 0 }; \
	unsigned char* bRet; \
 \
	bRet = CharToDigest(cpIn, nLength, ## DIGEST_FUNC_NAME ##, (unsigned char*)byHash); \
	if (bRet == NULL) { \
		return NULL; \
	} \
	CharSizeToBase64ToWCharv2((char*)byHash, sizeof(byHash), pOutWChar); \
	return pOutWChar; \
} \
 \
WCHAR* WCharToUtf8CharTo ## NAME ## ToBase64WChar(const WCHAR* cpInWChar, WCHAR* pOutWChar) \
{ \
	char* pBufChar; \
 \
	pBufChar = WCharToUtf8CharConv(cpInWChar); \
	if (pBufChar == NULL) { \
		return NULL; \
	} \
	pOutWChar = CharTo ## NAME ## ToBase64WChar(pBufChar, strlen(pBufChar), pOutWChar); \
	free(pBufChar); \
	return pOutWChar; \
} \


CHAR_TO_HASH_CHAR_FUNC(MD5, 128, EVP_md5())
CHAR_TO_HASH_CHAR_FUNC(SHA1, 160, EVP_sha1())
CHAR_TO_HASH_CHAR_FUNC(SHA224, 224, EVP_sha224())
CHAR_TO_HASH_CHAR_FUNC(SHA256, 256, EVP_sha256())
CHAR_TO_HASH_CHAR_FUNC(SHA384, 384, EVP_sha384())
CHAR_TO_HASH_CHAR_FUNC(SHA512, 512, EVP_sha512())
CHAR_TO_HASH_CHAR_FUNC(SHA512_224, 224, EVP_sha512_224())
CHAR_TO_HASH_CHAR_FUNC(SHA512_256, 256, EVP_sha512_256())
CHAR_TO_HASH_CHAR_FUNC(SHA3_224, 224, EVP_sha3_224())
CHAR_TO_HASH_CHAR_FUNC(SHA3_256, 224, EVP_sha3_256())
CHAR_TO_HASH_CHAR_FUNC(SHA3_384, 384, EVP_sha3_384())
CHAR_TO_HASH_CHAR_FUNC(SHA3_512, 512, EVP_sha3_512())
