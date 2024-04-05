#pragma once
#ifndef __OPENSSLCHARLIB_H__
#define __OPENSSLCHARLIB_H__
#include <windows.h>
#include <openssl/evp.h>

//#pragma comment(lib, "libcrypto.lib")

#define AES256_DATA_SIZE(n) ((n) + (16 - (n) % 16))


unsigned char* Aes256Encrypt(const void* cpIn, const size_t nInLength, unsigned char* pOutEncrypt, int* nOutLength);
void* Aes256Decrypt(const unsigned char* cpInEncrypt, const size_t nInLength, unsigned char* pOut, int* nOutLength);

unsigned __int64 FileToFileAes256EncryptCore(FILE* fpInFile, FILE* fpOutFile, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);
unsigned __int64 FileToFileAes256EncryptA(const char* cpInFileName, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);
unsigned __int64 FileToFileAes256EncryptW(const WCHAR* cpInFileName, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);

unsigned __int64 FileToFileAes256DecryptCore(FILE* fpInFile, FILE* fpOutFile, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);
unsigned __int64 FileToFileAes256DecryptA(const char* cpInFileName, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);
unsigned __int64 FileToFileAes256DecryptW(const WCHAR* cpInFileName, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);

unsigned char* CharToAes256Encrypt(const char* cpInChar, int* nOutLength, unsigned char* pOutEncrypt = NULL);
unsigned char* WCharToAes256Encrypt(const WCHAR* cpInWChar, int* nOutLength, unsigned char* pOutEncrypt = NULL);
char* CharToAes256EncryptToBase64(const char* cpInChar, char* pOutEncrypt = NULL);
char* WCharToAes256EncryptToBase64(const WCHAR* cpInWChar, char* pOutEncrypt = NULL);

char* Aes256DecryptToChar(const unsigned char* cpInEncrypt, const size_t nInLength, char* pOut = NULL);
WCHAR* Aes256DecryptToWChar(const unsigned char* cpInEncrypt, const size_t nInLength, WCHAR* pOut = NULL);
char* Base64ToAes256DecryptToChar(const char* cpInEncrypt, char* pOut = NULL);
WCHAR* Base64ToAes256DecryptToWChar(const char* cpInEncrypt, WCHAR* pOut = NULL);

unsigned char* FileToDigestCore(FILE* fpFile, const EVP_MD* cpDigestType, unsigned char* pOutHash = NULL, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);
unsigned char* FileToDigestA(const char* cpInFileName, const EVP_MD* cpDigestType, unsigned char* pOutHash = NULL, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);
unsigned char* FileToDigestW(const WCHAR* cpInFileName, const EVP_MD* cpDigestType, unsigned char* pOutHash = NULL, unsigned char* pFileReadBuf = NULL, size_t nFileReadBufSize = 0, __int64* nFileReadingSize = NULL);
unsigned char* CharToDigest(const void* cpIn, const size_t nLength, const EVP_MD* cpDigestType = EVP_sha256(), unsigned char* pOutHash = NULL);

#define CHAR_TO_HASH_CHAR_EXPORT_FUNC(NAME) \
char* CharTo ## NAME ## Char(const void* cpInChar, const size_t nLength, char* pOutChar = NULL); \
char* CharTo ## NAME ## Char(const char* cpInChar, char* pOutChar = NULL); \
char* WCharTo ## NAME ## Char(const WCHAR* cpInWChar, char* pOutChar = NULL); \
char* WCharToUtf8CharTo ## NAME ## Char(const WCHAR* cpInWChar, char* pOutChar = NULL); \
char* CharTo ## NAME ## ToBase64Char(const void* cpInChar, const size_t nLength, char* pOutChar = NULL, size_t outCharSize = (size_t)-1); \
char* WCharToUtf8CharTo ## NAME ## ToBase64Char(const WCHAR* cpInWChar, char* pOutChar = NULL); \
 \
WCHAR* CharTo ## NAME ## WChar(const void* cpInChar, const size_t nLength, WCHAR* pOutWChar = NULL); \
WCHAR* CharTo ## NAME ## WChar(const char* cpInChar, WCHAR* pOutWChar = NULL); \
WCHAR* WCharTo ## NAME ## WChar(const WCHAR* cpInWChar, WCHAR* pOutWChar = NULL); \
WCHAR* WCharToUtf8CharTo ## NAME ## WChar(const WCHAR* cpInWChar, WCHAR* pOutWChar = NULL); \
WCHAR* CharTo ## NAME ## ToBase64WChar(const void* cpInChar, const size_t nLength, WCHAR* pOutWChar = NULL, size_t outCharSize = (size_t)-1); \
WCHAR* WCharToUtf8CharTo ## NAME ## ToBase64WChar(const WCHAR* cpInWChar, WCHAR* pOutWChar = NULL); \

CHAR_TO_HASH_CHAR_EXPORT_FUNC(MD5)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA1)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA224)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA256)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA384)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA512)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA512_224)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA512_256)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA3_224)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA3_256)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA3_384)
CHAR_TO_HASH_CHAR_EXPORT_FUNC(SHA3_512)

#ifdef _UNICODE
#define FileToFileAes256Encrypt FileToFileAes256EncryptW
#define FileToFileAes256Decrypt FileToFileAes256DecryptW
#define FileToDigest			FileToDigestW

#else
#define FileToFileAes256Encrypt FileToFileAes256EncryptA
#define FileToFileAes256Decrypt FileToFileAes256DecryptA
#define FileToDigest			FileToDigestA

#endif
#endif
