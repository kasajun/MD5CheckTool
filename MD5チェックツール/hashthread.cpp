// hashthread.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "hashthread.h"
#include <stdio.h>
#include <process.h>
#include <mbstring.h>
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#include <mmsystem.h>
#endif
#include "../shared/c20.h"
#include "../shared/charlib_base64.h"
#include "../shared/cpuid.h"
#include "../shared/md2.h"
#include "../shared/md4.h"
#include "../shared/md5.h"
#include "../shared/md5_noasm.h"
#include "../shared/sha1.h"
#include "../shared/sha1_noasm.h"
#include "../shared/sha256.h"
#include "../shared/sha256_noasm.h"
#include "../shared/sha512.h"
#include "../shared/sha512_noasm.h"
#include "../shared/sha3.h"
#include "../shared/sha3_noasm.h"
#include "../shared/crc32.h"
#include "../shared/crc32c.h"
#include "../shared/xxhash.h"
#include "../shared/os.h"


#ifndef CALG_SHA_256
#define ALG_SID_SHA_256					12
#define ALG_SID_SHA_384					13
#define ALG_SID_SHA_512					14
#define CALG_SHA_256					(ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA_256)
#define CALG_SHA_384					(ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA_384)
#define CALG_SHA_512					(ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA_512)
#define MS_ENH_RSA_AES_PROV_A			"Microsoft Enhanced RSA and AES Cryptographic Provider"
#define MS_ENH_RSA_AES_PROV_W			L"Microsoft Enhanced RSA and AES Cryptographic Provider"
#define MS_ENH_RSA_AES_PROV_XP_A		"Microsoft Enhanced RSA and AES Cryptographic Provider (Prototype)"
#define MS_ENH_RSA_AES_PROV_XP_W		L"Microsoft Enhanced RSA and AES Cryptographic Provider (Prototype)"
#ifdef _UNICODE
#define MS_ENH_RSA_AES_PROV_XP			MS_ENH_RSA_AES_PROV_XP_W
#define MS_ENH_RSA_AES_PROV				MS_ENH_RSA_AES_PROV_W
#else
#define MS_ENH_RSA_AES_PROV_XP			MS_ENH_RSA_AES_PROV_XP_A
#define MS_ENH_RSA_AES_PROV				MS_ENH_RSA_AES_PROV_A
#endif
#define PROV_REPLACE_OWF				23
#define PROV_RSA_AES					24
#endif

typedef struct WinCrypt_st {
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
} WinCrypt_CTX;

BYTE* byBuf;
BYTE* szNoneBuf;


int Hash_MD5_Init(void* mdCTX)
{
	return MD5_Init((MD5_CTX*)mdCTX);
}
int Hash_MD5_WinCrypt_Init(void* mdCTX)
{
	int nRet = CryptAcquireContext(&((WinCrypt_CTX*)mdCTX)->hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET);
	IF_LIKELY(nRet) {
		return CryptCreateHash(((WinCrypt_CTX*)mdCTX)->hCryptProv, CALG_MD5, 0, 0, &((WinCrypt_CTX*)mdCTX)->hHash);
	}
	return FALSE;
}

int Hash_SHA1_Init(void* mdCTX)
{
	return SHA1_Init((SHA_CTX*)mdCTX);
}
int Hash_SHA1_WinCrypt_Init(void* mdCTX)
{
	int nRet = CryptAcquireContext(&((WinCrypt_CTX*)mdCTX)->hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET);
	IF_LIKELY(nRet) {
		return CryptCreateHash(((WinCrypt_CTX*)mdCTX)->hCryptProv, CALG_SHA1, 0, 0, &((WinCrypt_CTX*)mdCTX)->hHash);
	}
	return FALSE;
}

int Hash_SHA256_192_Init(void* mdCTX)
{
	return SHA256_192_Init((SHA256_CTX*)mdCTX);
}

int Hash_SHA224_Init(void* mdCTX)
{
	return SHA224_Init((SHA256_CTX*)mdCTX);
}

int Hash_SHA256_Init(void* mdCTX)
{
	return SHA256_Init((SHA256_CTX*)mdCTX);
}
int Hash_SHA256_WinCrypt_Init(void* mdCTX)
{
	int nRet = CryptAcquireContext(&((WinCrypt_CTX*)mdCTX)->hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET);
	IF_LIKELY(nRet) {
		return CryptCreateHash(((WinCrypt_CTX*)mdCTX)->hCryptProv, CALG_SHA_256, 0, 0, &((WinCrypt_CTX*)mdCTX)->hHash);
	}
	return FALSE;
}

int Hash_SHA384_Init(void* mdCTX)
{
	return SHA384_Init((SHA512_CTX*)mdCTX);
}
int Hash_SHA384_WinCrypt_Init(void* mdCTX)
{
	int nRet = CryptAcquireContext(&((WinCrypt_CTX*)mdCTX)->hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET);
	IF_LIKELY(nRet) {
		return CryptCreateHash(((WinCrypt_CTX*)mdCTX)->hCryptProv, CALG_SHA_384, 0, 0, &((WinCrypt_CTX*)mdCTX)->hHash);
	}
	return FALSE;
}

int Hash_SHA512_Init(void* mdCTX)
{
	return SHA512_Init((SHA512_CTX*)mdCTX);
}
int Hash_SHA512_WinCrypt_Init(void* mdCTX)
{
	int nRet = CryptAcquireContext(&((WinCrypt_CTX*)mdCTX)->hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET);
	IF_LIKELY(nRet) {
		return CryptCreateHash(((WinCrypt_CTX*)mdCTX)->hCryptProv, CALG_SHA_512, 0, 0, &((WinCrypt_CTX*)mdCTX)->hHash);
	}
	return FALSE;
}

int Hash_SHA512_224_Init(void* mdCTX)
{
	return SHA512_224_Init((SHA512_CTX*)mdCTX);
}

int Hash_SHA512_256_Init(void* mdCTX)
{
	return SHA512_256_Init((SHA512_CTX*)mdCTX);
}

int Hash_SHA3_224_Init(void* mdCTX)
{
	return SHA3_224_Init((KECCAK1600_CTX*)mdCTX);
}

int Hash_SHA3_256_Init(void* mdCTX)
{
	return SHA3_256_Init((KECCAK1600_CTX*)mdCTX);
}

int Hash_SHA3_384_Init(void* mdCTX)
{
	return SHA3_384_Init((KECCAK1600_CTX*)mdCTX);
}

int Hash_SHA3_512_Init(void* mdCTX)
{
	return SHA3_512_Init((KECCAK1600_CTX*)mdCTX);
}

int Hash_crc32_Init(void* mdCTX)
{
	return crc32_Init((unsigned int*)mdCTX);
}

int Hash_crc32c_Init(void* mdCTX)
{
	return crc32c_Init((unsigned int*)mdCTX);
}

int Hash_XXH32_Init(void* mdCTX)
{
	return XXH32_Init((XXH32_state_t*)mdCTX);
}

int Hash_XXH64_Init(void* mdCTX)
{
	return XXH64_Init((XXH64_state_t*)mdCTX);
}

int Hash_XXH3_Init(void* mdCTX)
{
	return XXH3_Init((XXH3_state_t*)mdCTX);
}

int Hash_XXH128_Init(void* mdCTX)
{
	return XXH128_Init((XXH3_state_t*)mdCTX);
}

int Hash_MD2_Init(void* mdCTX)
{
	return MD2_Init((MD2_CTX*)mdCTX);
}

int Hash_MD4_Init(void* mdCTX)
{
	return MD4_Init((MD4_CTX*)mdCTX);
}

int Hash_None_Init(void* mdCTX)
{
	UNREFERENCED_PARAMETER(mdCTX);
	return 1;
}


#define Hash_MD5_WinCrypt_Update Hash_WinCrypt_Update
#define Hash_SHA1_WinCrypt_Update Hash_WinCrypt_Update
#define Hash_SHA256_WinCrypt_Update Hash_WinCrypt_Update
#define Hash_SHA384_WinCrypt_Update Hash_WinCrypt_Update
#define Hash_SHA512_WinCrypt_Update Hash_WinCrypt_Update

int Hash_WinCrypt_Update(void* mdCTX, const void* data, size_t len)
{
	return CryptHashData(((WinCrypt_CTX*)mdCTX)->hHash, (const BYTE*)data, (DWORD)len, 0);
}

int Hash_MD5_Update(void* mdCTX, const void* data, size_t len)
{
	return MD5_Update((MD5_CTX*)mdCTX, data, len);
}
int Hash_MD5_NoAsm_Update(void* mdCTX, const void* data, size_t len)
{
	return MD5_NOASM_Update((MD5_CTX*)mdCTX, data, len);
}

int Hash_SHA1_Update(void* mdCTX, const void* data, size_t len)
{
	return SHA1_Update((SHA_CTX*)mdCTX, data, len);
}

int Hash_SHA1_NoAsm_Update(void* mdCTX, const void* data, size_t len)
{
	return SHA1_NOASM_Update((SHA_CTX*)mdCTX, data, len);
}

int Hash_SHA256_Update(void* mdCTX, const void* data, size_t len)
{
	return SHA256_Update((SHA256_CTX*)mdCTX, data, len);
}

int Hash_SHA256_NoAsm_Update(void* mdCTX, const void* data, size_t len)
{
	return SHA256_NOASM_Update((SHA256_CTX*)mdCTX, data, len);
}

int Hash_SHA512_Update(void* mdCTX, const void* data, size_t len)
{
	return SHA512_Update((SHA512_CTX*)mdCTX, data, len);
}

int Hash_SHA512_NoAsm_Update(void* mdCTX, const void* data, size_t len)
{
	return SHA512_NOASM_Update((SHA512_CTX*)mdCTX, data, len);
}

int Hash_SHA3_Update(void* mdCTX, const void* data, size_t len)
{
	return SHA3_Update((KECCAK1600_CTX*)mdCTX, data, len);
}

int Hash_SHA3_NoAsm_Update(void* mdCTX, const void* data, size_t len)
{
	return SHA3_NOASM_Update((KECCAK1600_CTX*)mdCTX, data, len);
}

int Hash_crc32_Update(void* mdCTX, const void* data, size_t len)
{
	return crc32_Update((unsigned int*)mdCTX, data, len);
}

int Hash_crc32c_Update(void* mdCTX, const void* data, size_t len)
{
	return crc32c_Update((unsigned int*)mdCTX, data, len);
}
int Hash_crc32c_SSE4_2_Update(void* mdCTX, const void* data, size_t len)
{
	return crc32c_SSE4_2_Update((unsigned int*)mdCTX, data, len);
}

int Hash_XXH32_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH32_Update((XXH32_state_t*)mdCTX, data, len);
}

int Hash_XXH64_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH64_Update((XXH64_state_t*)mdCTX, data, len);
}

int Hash_XXH3_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH3_Update((XXH3_state_t*)mdCTX, data, len);
}

int Hash_XXH128_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH128_Update((XXH3_state_t*)mdCTX, data, len);
}

int Hash_MD2_Update(void* mdCTX, const void* data, size_t len)
{
	return MD2_Update((MD2_CTX*)mdCTX, data, len);
}

int Hash_MD4_Update(void* mdCTX, const void* data, size_t len)
{
	return MD4_Update((MD4_CTX*)mdCTX, data, len);
}

int Hash_None_Update(void* mdCTX, const void* data, size_t len)
{
	UNREFERENCED_PARAMETER(mdCTX);
	memcpy(szNoneBuf, data, len);
	return 1;
}


int Hash_MD5_Final(unsigned char* md, void* mdCTX)
{
	return MD5_Final(md, (MD5_CTX*)mdCTX);
}
int Hash_MD5_NoAsm_Final(unsigned char* md, void* mdCTX)
{
	return MD5_NOASM_Final(md, (MD5_CTX*)mdCTX);
}
int Hash_MD5_WinCrypt_Final(unsigned char* md, void* mdCTX)
{
	DWORD dwHashLen = HASH_MD5_LEN;
	return CryptGetHashParam(((WinCrypt_CTX*)mdCTX)->hHash, HP_HASHVAL, md, &dwHashLen, 0);
}

int Hash_SHA1_Final(unsigned char* md, void* mdCTX)
{
	return SHA1_Final(md, (SHA_CTX*)mdCTX);
}
int Hash_SHA1_NoAsm_Final(unsigned char* md, void* mdCTX)
{
	return SHA1_NOASM_Final(md, (SHA_CTX*)mdCTX);
}
int Hash_SHA1_WinCrypt_Final(unsigned char* md, void* mdCTX)
{
	DWORD dwHashLen = HASH_SHA1_LEN;
	return CryptGetHashParam(((WinCrypt_CTX*)mdCTX)->hHash, HP_HASHVAL, md, &dwHashLen, 0);
}

int Hash_SHA256_Final(unsigned char* md, void* mdCTX)
{
	return SHA256_Final(md, (SHA256_CTX*)mdCTX);
}
int Hash_SHA256_NoAsm_Final(unsigned char* md, void* mdCTX)
{
	return SHA256_NOASM_Final(md, (SHA256_CTX*)mdCTX);
}
int Hash_SHA256_WinCrypt_Final(unsigned char* md, void* mdCTX)
{
	DWORD dwHashLen = HASH_SHA_256_LEN;
	return CryptGetHashParam(((WinCrypt_CTX*)mdCTX)->hHash, HP_HASHVAL, md, &dwHashLen, 0);
}

int Hash_SHA384_WinCrypt_Final(unsigned char* md, void* mdCTX)
{
	DWORD dwHashLen = HASH_SHA_384_LEN;
	return CryptGetHashParam(((WinCrypt_CTX*)mdCTX)->hHash, HP_HASHVAL, md, &dwHashLen, 0);
}

int Hash_SHA512_Final(unsigned char* md, void* mdCTX)
{
	return SHA512_Final(md, (SHA512_CTX*)mdCTX);
}
int Hash_SHA512_NoAsm_Final(unsigned char* md, void* mdCTX)
{
	return SHA512_NOASM_Final(md, (SHA512_CTX*)mdCTX);
}
int Hash_SHA512_WinCrypt_Final(unsigned char* md, void* mdCTX)
{
	DWORD dwHashLen = HASH_SHA_512_LEN;
	return CryptGetHashParam(((WinCrypt_CTX*)mdCTX)->hHash, HP_HASHVAL, md, &dwHashLen, 0);
}

int Hash_SHA3_Final(unsigned char* md, void* mdCTX)
{
	return SHA3_Final(md, (KECCAK1600_CTX*)mdCTX);
}

int Hash_SHA3_NoAsm_Final(unsigned char* md, void* mdCTX)
{
	return SHA3_NOASM_Final(md, (KECCAK1600_CTX*)mdCTX);
}

int Hash_crc32_Final(unsigned char* md, void* mdCTX)
{
	return crc32_Final(md, (unsigned int*)mdCTX);
}

int Hash_crc32c_Final(unsigned char* md, void* mdCTX)
{
	return crc32c_Final(md, (unsigned int*)mdCTX);
}

int Hash_XXH32_Final(unsigned char* md, void* mdCTX)
{
	return XXH32_Final(md, (XXH32_state_t*)mdCTX);
}

int Hash_XXH64_Final(unsigned char* md, void* mdCTX)
{
	return XXH64_Final(md, (XXH64_state_t*)mdCTX);
}

int Hash_XXH3_Final(unsigned char* md, void* mdCTX)
{
	return XXH3_Final(md, (XXH3_state_t*)mdCTX);
}

int Hash_XXH128_Final(unsigned char* md, void* mdCTX)
{
	return XXH128_Final(md, (XXH3_state_t*)mdCTX);
}

int Hash_MD2_Final(unsigned char* md, void* mdCTX)
{
	return MD2_Final(md, (MD2_CTX*)mdCTX);
}

int Hash_MD4_Final(unsigned char* md, void* mdCTX)
{
	return MD4_Final(md, (MD4_CTX*)mdCTX);
}

int Hash_None_Final(unsigned char* md, void* mdCTX)
{
	UNREFERENCED_PARAMETER(mdCTX);
	memcpy(md, szNoneBuf, HASH_NONE_LEN);
	return 1;
}


#if _MSC_VER > 1300
#include "..\shared\xxhash-sse2.h"

int Hash_XXH3_SSE2_Init(void* mdCTX)
{
	return XXH3_SSE2_Init((XXH3_state_t*)mdCTX);
}

int Hash_XXH128_SSE2_Init(void* mdCTX)
{
	return XXH128_SSE2_Init((XXH3_state_t*)mdCTX);
}


int Hash_XXH3_SSE2_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH3_SSE2_Update((XXH3_state_t*)mdCTX, data, len);
}

int Hash_XXH128_SSE2_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH128_SSE2_Update((XXH3_state_t*)mdCTX, data, len);
}


int Hash_XXH3_SSE2_Final(unsigned char* md, void* mdCTX)
{
	return XXH3_SSE2_Final(md, (XXH3_state_t*)mdCTX);
}

int Hash_XXH128_SSE2_Final(unsigned char* md, void* mdCTX)
{
	return XXH128_SSE2_Final(md, (XXH3_state_t*)mdCTX);
}
#endif


#if _MSC_VER > 1600
#include "..\shared\xxhash-avx2.h"
#include "..\shared\xxhash-avx512.h"

int Hash_XXH3_AVX2_Init(void* mdCTX)
{
	return XXH3_AVX2_Init((XXH3_state_t*)mdCTX);
}

int Hash_XXH3_AVX512_Init(void* mdCTX)
{
	return XXH3_AVX512_Init((XXH3_state_t*)mdCTX);
}

int Hash_XXH128_AVX2_Init(void* mdCTX)
{
	return XXH128_AVX2_Init((XXH3_state_t*)mdCTX);
}

int Hash_XXH128_AVX512_Init(void* mdCTX)
{
	return XXH128_AVX512_Init((XXH3_state_t*)mdCTX);
}


int Hash_XXH3_AVX2_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH3_AVX2_Update((XXH3_state_t*)mdCTX, data, len);
}

int Hash_XXH3_AVX512_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH3_AVX512_Update((XXH3_state_t*)mdCTX, data, len);
}

int Hash_XXH128_AVX2_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH128_AVX2_Update((XXH3_state_t*)mdCTX, data, len);
}

int Hash_XXH128_AVX512_Update(void* mdCTX, const void* data, size_t len)
{
	return XXH128_AVX512_Update((XXH3_state_t*)mdCTX, data, len);
}


int Hash_XXH3_AVX2_Final(unsigned char* md, void* mdCTX)
{
	return XXH3_AVX2_Final(md, (XXH3_state_t*)mdCTX);
}

int Hash_XXH3_AVX512_Final(unsigned char* md, void* mdCTX)
{
	return XXH3_AVX512_Final(md, (XXH3_state_t*)mdCTX);
}

int Hash_XXH128_AVX2_Final(unsigned char* md, void* mdCTX)
{
	return XXH128_AVX2_Final(md, (XXH3_state_t*)mdCTX);
}

int Hash_XXH128_AVX512_Final(unsigned char* md, void* mdCTX)
{
	return XXH128_AVX512_Final(md, (XXH3_state_t*)mdCTX);
}
#endif


int (*Hash_Init[MAX_HASH_TYPE + 1])(void*) = {
	Hash_MD5_Init,
	Hash_SHA1_Init,
	Hash_SHA256_192_Init,
	Hash_SHA224_Init,
	Hash_SHA256_Init,
	Hash_SHA384_Init,
	Hash_SHA512_Init,
	Hash_SHA512_224_Init,
	Hash_SHA512_256_Init,
	Hash_SHA3_224_Init,
	Hash_SHA3_256_Init,
	Hash_SHA3_384_Init,
	Hash_SHA3_512_Init,
	Hash_crc32_Init,
	Hash_crc32c_Init,
	Hash_XXH32_Init,
	Hash_XXH64_Init,
	Hash_XXH3_Init,
	Hash_XXH128_Init,
	Hash_MD2_Init,
	Hash_MD4_Init,
	Hash_None_Init
};

int (*Hash_Update[MAX_HASH_TYPE + 1])(void*, const void*, size_t) = {
	Hash_MD5_Update,
	Hash_SHA1_Update,
	Hash_SHA256_Update,
	Hash_SHA256_Update,
	Hash_SHA256_Update,
	Hash_SHA512_Update,
	Hash_SHA512_Update,
	Hash_SHA512_Update,
	Hash_SHA512_Update,
	Hash_SHA3_Update,
	Hash_SHA3_Update,
	Hash_SHA3_Update,
	Hash_SHA3_Update,
	Hash_crc32_Update,
	Hash_crc32c_Update,
	Hash_XXH32_Update,
	Hash_XXH64_Update,
	Hash_XXH3_Update,
	Hash_XXH128_Update,
	Hash_MD2_Update,
	Hash_MD4_Update,
	Hash_None_Update
};

int (*Hash_Final[MAX_HASH_TYPE + 1])(unsigned char*, void*) = {
	Hash_MD5_Final,
	Hash_SHA1_Final,
	Hash_SHA256_Final,
	Hash_SHA256_Final,
	Hash_SHA256_Final,
	Hash_SHA512_Final,
	Hash_SHA512_Final,
	Hash_SHA512_Final,
	Hash_SHA512_Final,
	Hash_SHA3_Final,
	Hash_SHA3_Final,
	Hash_SHA3_Final,
	Hash_SHA3_Final,
	Hash_crc32_Final,
	Hash_crc32c_Final,
	Hash_XXH32_Final,
	Hash_XXH64_Final,
	Hash_XXH3_Final,
	Hash_XXH128_Final,
	Hash_MD2_Final,
	Hash_MD4_Final,
	Hash_None_Final
};

extern const DWORD cdwHashLen[MAX_HASH_TYPE + 1] = {
	HASH_MD5_LEN,
	HASH_SHA1_LEN,
	HASH_SHA_256_192_LEN,
	HASH_SHA_224_LEN,
	HASH_SHA_256_LEN,
	HASH_SHA_384_LEN,
	HASH_SHA_512_LEN,
	HASH_SHA_224_LEN,
	HASH_SHA_256_LEN,
	HASH_SHA_224_LEN,
	HASH_SHA_256_LEN,
	HASH_SHA_384_LEN,
	HASH_SHA_512_LEN,
	HASH_CRC32_LEN,
	HASH_CRC32C_LEN,
	HASH_XXH32_LEN,
	HASH_XXH64_LEN,
	HASH_XXH3_LEN,
	HASH_XXH128_LEN,
	HASH_MD2_LEN,
	HASH_MD4_LEN,
	HASH_NONE_LEN
};

extern const char* cpHashNameA[MAX_HASH_TYPE + 1] = {
	HASH_MD5_NAME,
	HASH_SHA1_NAME,
	HASH_SHA_256_192_NAME,
	HASH_SHA_224_NAME,
	HASH_SHA_256_NAME,
	HASH_SHA_384_NAME,
	HASH_SHA_512_NAME,
	HASH_SHA_512_224_NAME,
	HASH_SHA_512_256_NAME,
	HASH_SHA3_224_NAME,
	HASH_SHA3_256_NAME,
	HASH_SHA3_384_NAME,
	HASH_SHA3_512_NAME,
	HASH_CRC32_NAME,
	HASH_CRC32C_NAME,
	HASH_XXH32_NAME,
	HASH_XXH64_NAME,
	HASH_XXH3_NAME,
	HASH_XXH128_NAME,
	HASH_MD2_NAME,
	HASH_MD4_NAME,
	HASH_NONE_NAME
};

extern const WCHAR* cpHashNameW[MAX_HASH_TYPE + 1] = {
	HASH_MD5_NAMEW,
	HASH_SHA1_NAMEW,
	HASH_SHA_256_192_NAMEW,
	HASH_SHA_224_NAMEW,
	HASH_SHA_256_NAMEW,
	HASH_SHA_384_NAMEW,
	HASH_SHA_512_NAMEW,
	HASH_SHA_512_224_NAMEW,
	HASH_SHA_512_256_NAMEW,
	HASH_SHA3_224_NAMEW,
	HASH_SHA3_256_NAMEW,
	HASH_SHA3_384_NAMEW,
	HASH_SHA3_512_NAMEW,
	HASH_CRC32_NAMEW,
	HASH_CRC32C_NAMEW,
	HASH_XXH32_NAMEW,
	HASH_XXH64_NAMEW,
	HASH_XXH3_NAMEW,
	HASH_XXH128_NAMEW,
	HASH_MD2_NAMEW,
	HASH_MD4_NAMEW,
	HASH_NONE_NAMEW
};

extern const size_t cnHashNameLen[MAX_HASH_TYPE + 1] = {
	sizeof(HASH_MD5_NAME) - 1,
	sizeof(HASH_SHA1_NAME) - 1,
	sizeof(HASH_SHA_256_192_NAME) - 1,
	sizeof(HASH_SHA_224_NAME) - 1,
	sizeof(HASH_SHA_256_NAME) - 1,
	sizeof(HASH_SHA_384_NAME) - 1,
	sizeof(HASH_SHA_512_NAME) - 1,
	sizeof(HASH_SHA_512_224_NAME) - 1,
	sizeof(HASH_SHA_512_256_NAME) - 1,
	sizeof(HASH_SHA3_224_NAME) - 1,
	sizeof(HASH_SHA3_256_NAME) - 1,
	sizeof(HASH_SHA3_384_NAME) - 1,
	sizeof(HASH_SHA3_512_NAME) - 1,
	sizeof(HASH_CRC32_NAME) - 1,
	sizeof(HASH_CRC32C_NAME) - 1,
	sizeof(HASH_XXH32_NAME) - 1,
	sizeof(HASH_XXH64_NAME) - 1,
	sizeof(HASH_XXH3_NAME) - 1,
	sizeof(HASH_XXH128_NAME) - 1,
	sizeof(HASH_MD2_NAME) - 1,
	sizeof(HASH_MD4_NAME) - 1,
	sizeof(HASH_NONE_NAME) - 1
};

extern const char* cpHashListNameA[MAX_HASH_TYPE + 1] = {
	HASH_MD5_LISTNAME,
	HASH_SHA1_LISTNAME,
	HASH_SHA_256_192_LISTNAME,
	HASH_SHA_224_LISTNAME,
	HASH_SHA_256_LISTNAME,
	HASH_SHA_384_LISTNAME,
	HASH_SHA_512_LISTNAME,
	HASH_SHA_512_224_LISTNAME,
	HASH_SHA_512_256_LISTNAME,
	HASH_SHA3_224_LISTNAME,
	HASH_SHA3_256_LISTNAME,
	HASH_SHA3_384_LISTNAME,
	HASH_SHA3_512_LISTNAME,
	HASH_CRC32_LISTNAME,
	HASH_CRC32C_LISTNAME,
	HASH_XXH32_LISTNAME,
	HASH_XXH64_LISTNAME,
	HASH_XXH3_LISTNAME,
	HASH_XXH128_LISTNAME,
	HASH_MD2_LISTNAME,
	HASH_MD4_LISTNAME,
	HASH_NONE_LISTNAME
};

extern const WCHAR* cpHashListNameW[MAX_HASH_TYPE + 1] = {
	HASH_MD5_LISTNAMEW,
	HASH_SHA1_LISTNAMEW,
	HASH_SHA_256_192_LISTNAMEW,
	HASH_SHA_224_LISTNAMEW,
	HASH_SHA_256_LISTNAMEW,
	HASH_SHA_384_LISTNAMEW,
	HASH_SHA_512_LISTNAMEW,
	HASH_SHA_512_224_LISTNAMEW,
	HASH_SHA_512_256_LISTNAMEW,
	HASH_SHA3_224_LISTNAMEW,
	HASH_SHA3_256_LISTNAMEW,
	HASH_SHA3_384_LISTNAMEW,
	HASH_SHA3_512_LISTNAMEW,
	HASH_CRC32_LISTNAMEW,
	HASH_CRC32C_LISTNAMEW,
	HASH_XXH32_LISTNAMEW,
	HASH_XXH64_LISTNAMEW,
	HASH_XXH3_LISTNAMEW,
	HASH_XXH128_LISTNAMEW,
	HASH_MD2_LISTNAMEW,
	HASH_MD4_LISTNAMEW,
	HASH_NONE_LISTNAMEW
};

extern const size_t cnHashListNameLen[MAX_HASH_TYPE + 1] = {
	sizeof(HASH_MD5_LISTNAME) - 1,
	sizeof(HASH_SHA1_LISTNAME) - 1,
	sizeof(HASH_SHA_256_192_LISTNAME) - 1,
	sizeof(HASH_SHA_224_LISTNAME) - 1,
	sizeof(HASH_SHA_256_LISTNAME) - 1,
	sizeof(HASH_SHA_384_LISTNAME) - 1,
	sizeof(HASH_SHA_512_LISTNAME) - 1,
	sizeof(HASH_SHA_512_224_LISTNAME) - 1,
	sizeof(HASH_SHA_512_256_LISTNAME) - 1,
	sizeof(HASH_SHA3_224_LISTNAME) - 1,
	sizeof(HASH_SHA3_256_LISTNAME) - 1,
	sizeof(HASH_SHA3_384_LISTNAME) - 1,
	sizeof(HASH_SHA3_512_LISTNAME) - 1,
	sizeof(HASH_CRC32_LISTNAME) - 1,
	sizeof(HASH_CRC32C_LISTNAME) - 1,
	sizeof(HASH_XXH32_LISTNAME) - 1,
	sizeof(HASH_XXH64_LISTNAME) - 1,
	sizeof(HASH_XXH3_LISTNAME) - 1,
	sizeof(HASH_XXH128_LISTNAME) - 1,
	sizeof(HASH_MD2_LISTNAME) - 1,
	sizeof(HASH_MD4_LISTNAME) - 1,
	sizeof(HASH_NONE_LISTNAME) - 1
};

extern const size_t nHashExtLen[MAX_HASH_TYPE + 1] = {
	sizeof("md5") - 1,
	sizeof("sha1") - 1,
	sizeof("sha256_192") - 1,
	sizeof("sha224") - 1,
	sizeof("sha256") - 1,
	sizeof("sha384") - 1,
	sizeof("sha512") - 1,
	sizeof("sha512_224") - 1,
	sizeof("sha512_256") - 1,
	sizeof("sha3_224") - 1,
	sizeof("sha3_256") - 1,
	sizeof("sha3_384") - 1,
	sizeof("sha3_512") - 1,
	sizeof("crc32") - 1,
	sizeof("crc32c") - 1,
	sizeof("xxh32") - 1,
	sizeof("xxh64") - 1,
	sizeof("xxh3") - 1,
	sizeof("xxh128") - 1,
	sizeof("md2") - 1,
	sizeof("md4") - 1,
	sizeof("none") - 1
};

extern const TCHAR* cpHashExt[MAX_HASH_TYPE + 1] = {
	_T(".md5"),
	_T(".sha1"),
	_T(".sha256_192"),
	_T(".sha224"),
	_T(".sha256"),
	_T(".sha384"),
	_T(".sha512"),
	_T(".sha512_224"),
	_T(".sha512_256"),
	_T(".sha3_224"),
	_T(".sha3_256"),
	_T(".sha3_384"),
	_T(".sha3_512"),
	_T(".crc32"),
	_T(".crc32c"),
	_T(".xxh32"),
	_T(".xxh64"),
	_T(".xxh3"),
	_T(".xxh128"),
	_T(".md2"),
	_T(".md4"),
	_T(".none")
};

extern const BOOL nHashByteSwap[MAX_HASH_TYPE + 1] = {
	HASH_MD5_BYTE_SWAP,
	HASH_SHA1_BYTE_SWAP,
	HASH_SHA_256_192_BYTE_SWAP,
	HASH_SHA_224_BYTE_SWAP,
	HASH_SHA_256_BYTE_SWAP,
	HASH_SHA_384_BYTE_SWAP,
	HASH_SHA_512_BYTE_SWAP,
	HASH_SHA_512_224_BYTE_SWAP,
	HASH_SHA_512_256_BYTE_SWAP,
	HASH_SHA3_224_BYTE_SWAP,
	HASH_SHA3_256_BYTE_SWAP,
	HASH_SHA3_384_BYTE_SWAP,
	HASH_SHA3_512_BYTE_SWAP,
	HASH_CRC32_BYTE_SWAP,
	HASH_CRC32C_BYTE_SWAP,
	HASH_XXH32_BYTE_SWAP,
	HASH_XXH64_BYTE_SWAP,
	HASH_XXH3_BYTE_SWAP,
	HASH_XXH128_BYTE_SWAP,
	HASH_MD2_BYTE_SWAP,
	HASH_MD4_BYTE_SWAP,
	HASH_NONE_BYTE_SWAP
};

unsigned __int64 nHashBenchSize[MAX_HASH_TYPE + 1] = 
{
	HASH_MD5_BENCH_SIZE,
	HASH_SHA1_BENCH_SIZE,
	HASH_SHA_256_192_BENCH_SIZE,
	HASH_SHA_224_BENCH_SIZE,
	HASH_SHA_256_BENCH_SIZE,
	HASH_SHA_384_BENCH_SIZE,
	HASH_SHA_512_BENCH_SIZE,
	HASH_SHA_512_224_BENCH_SIZE,
	HASH_SHA_512_256_BENCH_SIZE,
	HASH_SHA3_224_BENCH_SIZE,
	HASH_SHA3_256_BENCH_SIZE,
	HASH_SHA3_384_BENCH_SIZE,
	HASH_SHA3_512_BENCH_SIZE,
	HASH_CRC32_BENCH_SIZE,
	HASH_CRC32C_BENCH_SIZE,
	HASH_XXH32_BENCH_SIZE,
	HASH_XXH64_BENCH_SIZE,
	HASH_XXH3_BENCH_SIZE,
	HASH_XXH128_BENCH_SIZE,
	HASH_MD2_BENCH_SIZE,
	HASH_MD4_BENCH_SIZE,
	HASH_NONE_BENCH_SIZE
};
extern const size_t nHashColorType[MAX_HASH_TYPE + 1] = 
{
	HASH_MD5_COLOR,
	HASH_SHA1_COLOR,
	HASH_SHA_256_192_COLOR,
	HASH_SHA_224_COLOR,
	HASH_SHA_256_COLOR,
	HASH_SHA_384_COLOR,
	HASH_SHA_512_COLOR,
	HASH_SHA_512_224_COLOR,
	HASH_SHA_512_256_COLOR,
	HASH_SHA3_224_COLOR,
	HASH_SHA3_256_COLOR,
	HASH_SHA3_384_COLOR,
	HASH_SHA3_512_COLOR,
	HASH_CRC32_HASH_COLOR,
	HASH_CRC32C_HASH_COLOR,
	HASH_XXH32_HASH_COLOR,
	HASH_XXH64_COLOR,
	HASH_XXH3_COLOR,
	XXH128_HASH_COLOR,
	HASH_MD2_COLOR,
	HASH_MD4_COLOR,
	HASH_NONE_COLOR
};
extern const char* cpRtfColorTag[HASH_MAX_COLOR] =
{
	HASH_RTF_COLOR1,
	HASH_RTF_COLOR2,
	HASH_RTF_COLOR3,
	HASH_RTF_COLOR4,
	HASH_RTF_COLOR5,
	HASH_RTF_COLOR6,
	HASH_RTF_COLOR7,
	HASH_RTF_COLOR8,
	HASH_RTF_COLOR9,
	HASH_RTF_COLOR10,
	HASH_RTF_COLOR11,
	HASH_RTF_COLOR12,
	HASH_RTF_COLOR13,
	HASH_RTF_COLOR14,
	HASH_RTF_COLOR15,
	HASH_RTF_COLOR16,
	HASH_RTF_COLOR17,
	HASH_RTF_COLOR18,
	HASH_RTF_COLOR19,
	HASH_RTF_COLOR20,
	HASH_RTF_COLOR21,
	HASH_RTF_COLOR22
};
extern const char* cpHtmlColorTag[HASH_MAX_COLOR] =
{
	HASH_HTML_COLOR1,
	HASH_HTML_COLOR2,
	HASH_HTML_COLOR3,
	HASH_HTML_COLOR4,
	HASH_HTML_COLOR5,
	HASH_HTML_COLOR6,
	HASH_HTML_COLOR7,
	HASH_HTML_COLOR8,
	HASH_HTML_COLOR9,
	HASH_HTML_COLOR10,
	HASH_HTML_COLOR11,
	HASH_HTML_COLOR12,
	HASH_HTML_COLOR13,
	HASH_HTML_COLOR14,
	HASH_HTML_COLOR15,
	HASH_HTML_COLOR16,
	HASH_HTML_COLOR17,
	HASH_HTML_COLOR18,
	HASH_HTML_COLOR19,
	HASH_HTML_COLOR20,
	HASH_HTML_COLOR21,
	HASH_HTML_COLOR22
};


DWORD HashThread_GetHashLenToType(DWORD dwHashLen)
{
	DWORD dwHashType = HASH_MD5_TYPE;

	switch (dwHashLen)
	{
	case HASH_CRC32_LEN:
		dwHashType = HASH_CRC32_TYPE;
		break;
	case HASH_XXH64_LEN:
		dwHashType = HASH_XXH64_TYPE;
		break;
	case HASH_SHA1_LEN:
		dwHashType = HASH_SHA1_TYPE;
		break;
	case HASH_SHA_256_192_LEN:
		dwHashType = HASH_SHA_256_192_TYPE;
		break;
	case HASH_SHA_224_LEN:
		dwHashType = HASH_SHA_224_TYPE;
		break;
	case HASH_SHA_256_LEN:
		dwHashType = HASH_SHA_256_TYPE;
		break;
	case HASH_SHA_384_LEN:
		dwHashType = HASH_SHA_384_TYPE;
		break;
	case HASH_SHA_512_LEN:
		dwHashType = HASH_SHA_512_TYPE;
		break;
	}
	return dwHashType;
}

BOOL HashThread_SetHashType(HashThread* ptagHashThread, DWORD dwHashType)
{
	IF_LIKELY(dwHashType < MAX_HASH_TYPE) {
		ptagHashThread->dwHashType = dwHashType;
	}
	else
	{
		dwHashType = 0;
		ptagHashThread->dwHashType = 0;
	}

	ptagHashThread->dwHashLen = HashThread_GetHashLen(dwHashType);
	ptagHashThread->cpHashName = HashThread_GetHashName(dwHashType);
	ptagHashThread->cpHashExt = HashThread_GetHashExt(dwHashType);
	ptagHashThread->nHashExtLen = HashThread_GetHashExtLen(dwHashType);
	SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_HASHFILE_HASHCHANGE, dwHashType);
	return TRUE;
}

BOOL HashThread_Startup(HashThread *ptagHashThread)
{
#if _MSC_VER < 1400
	OSVERSIONINFOEX osVerInfo = { 0 };
#endif
	DWORD dwRet = HashThread_CreateBuffer(ptagHashThread, FILE_DEF_COUNTSIZE);

	IF_UNLIKELY(dwRet == 0 || dwRet == (DWORD)-1) {
		return -1;
	}

	HashThread_ClearFileRecode(ptagHashThread);
	ptagHashThread->tagMultiFile->dwHashFileCharCode = HASHFILE_CHARCODE_UTF8;
	ptagHashThread->tagMultiFile->dwHashFileBom = 0;

	if (dwRet > 0)
	{
		memset(ptagHashThread->tagMultiFile->ptagFileRecode, 0, sizeof(tagHashThread_FileRecode));
		ptagHashThread->tagMultiFile->ptagFileRecode->dwHashFileHashLine = HASHFILE_NOLINE;
	}

	free(ptagHashThread->pFileBuffer);
	if (ptagHashThread->dwFileBufferSize == 0) {
		ptagHashThread->dwFileBufferSize = FILE_BUFFER_SIZE;
	}

	byBuf = (BYTE*)_AlignedMalloc(0x201400, 64);
	IF_UNLIKELY(byBuf == NULL) {
		return -1;
	}

	ptagHashThread->mdCTX = byBuf + 0x1000;
	ptagHashThread->pFileBuffer = byBuf + 0x1400;
	szNoneBuf = byBuf + 0x101400;
	ptagHashThread->dwFileBufferAllocateSize = 0x100000;
	return ptagHashThread->tagMultiFile != NULL ? FILE_DEF_COUNTSIZE : -1;
}

BOOL HashThread_Shutdown(HashThread* ptagHashThread)
{
	_AlignedFree(byBuf);
	byBuf = NULL;
	ptagHashThread->mdCTX = NULL;
	szNoneBuf = NULL;
	ptagHashThread->pFileBuffer = NULL;
	CloseHandle(ptagHashThread->hFile);
	ptagHashThread->hFile = NULL;
	return HashThread_ClearBuffer_Core(ptagHashThread, TRUE);
}

BOOL HashThread_SetHashFunc(BOOL nIsNoAsm)
{
	if (!nIsNoAsm)
	{
		Hash_Update[HASH_MD5_TYPE] = Hash_MD5_Update;					// MD5
		Hash_Update[HASH_SHA1_TYPE] = Hash_SHA1_Update;					// SHA-1
		Hash_Update[HASH_SHA_256_192_TYPE] = Hash_SHA256_Update;		// SHA-256/192
		Hash_Update[HASH_SHA_224_TYPE] = Hash_SHA256_Update;			// SHA-224
		Hash_Update[HASH_SHA_256_TYPE] = Hash_SHA256_Update;			// SHA-256
		Hash_Update[HASH_SHA_384_TYPE] = Hash_SHA512_Update;			// SHA-384
		Hash_Update[HASH_SHA_512_TYPE] = Hash_SHA512_Update;			// SHA-512
		Hash_Update[HASH_SHA_512_224_TYPE] = Hash_SHA512_Update;		// SHA-512/224
		Hash_Update[HASH_SHA_512_256_TYPE] = Hash_SHA512_Update;		// SHA-512/256
		Hash_Update[HASH_SHA3_224_TYPE] = Hash_SHA3_Update;				// SHA3-224
		Hash_Update[HASH_SHA3_256_TYPE] = Hash_SHA3_Update;				// SHA3-256
		Hash_Update[HASH_SHA3_384_TYPE] = Hash_SHA3_Update;				// SHA3-384
		Hash_Update[HASH_SHA3_512_TYPE] = Hash_SHA3_Update;				// SHA3-512

		Hash_Final[HASH_MD5_TYPE] = Hash_MD5_Final;						// MD5
		Hash_Final[HASH_SHA1_TYPE] = Hash_SHA1_Final;					// SHA-1
		Hash_Final[HASH_SHA_256_192_TYPE] = Hash_SHA256_Final;			// SHA-256/192
		Hash_Final[HASH_SHA_224_TYPE] = Hash_SHA256_Final;				// SHA-224
		Hash_Final[HASH_SHA_256_TYPE] = Hash_SHA256_Final;				// SHA-256
		Hash_Final[HASH_SHA_384_TYPE] = Hash_SHA512_Final;				// SHA-384
		Hash_Final[HASH_SHA_512_TYPE] = Hash_SHA512_Final;				// SHA-512
		Hash_Final[HASH_SHA_512_224_TYPE] = Hash_SHA512_Final;			// SHA-512/224
		Hash_Final[HASH_SHA_512_256_TYPE] = Hash_SHA512_Final;			// SHA-512/256
		Hash_Final[HASH_SHA3_224_TYPE] = Hash_SHA3_Final;				// SHA3-224
		Hash_Final[HASH_SHA3_256_TYPE] = Hash_SHA3_Final;				// SHA3-256
		Hash_Final[HASH_SHA3_384_TYPE] = Hash_SHA3_Final;				// SHA3-384
		Hash_Final[HASH_SHA3_512_TYPE] = Hash_SHA3_Final;				// SHA3-512

		if (CPUID_IS_SHAEXT)
		{
			nHashBenchSize[HASH_SHA1_TYPE] = HASH_SHA1_BENCH_SIZE;
			nHashBenchSize[HASH_SHA_224_TYPE] = HASH_SHA_224_BENCH_SIZE;
			nHashBenchSize[HASH_SHA_256_TYPE] = HASH_SHA_256_BENCH_SIZE;
			nHashBenchSize[HASH_SHA_256_192_TYPE] = HASH_SHA_256_192_BENCH_SIZE;
		}
		else
		{
			nHashBenchSize[HASH_SHA1_TYPE] = HASH_SHA1_BENCH_SIZE / 2;
			nHashBenchSize[HASH_SHA_224_TYPE] = HASH_SHA_224_BENCH_SIZE / 2;
			nHashBenchSize[HASH_SHA_256_TYPE] = HASH_SHA_256_BENCH_SIZE / 2;
			nHashBenchSize[HASH_SHA_256_192_TYPE] = HASH_SHA_256_192_BENCH_SIZE / 2;
		}
		nHashBenchSize[HASH_SHA_384_TYPE] = HASH_SHA_384_BENCH_SIZE;
		nHashBenchSize[HASH_SHA_512_TYPE] = HASH_SHA_512_BENCH_SIZE;
		nHashBenchSize[HASH_SHA3_224_TYPE] = HASH_SHA3_224_BENCH_SIZE;
		nHashBenchSize[HASH_SHA3_256_TYPE] = HASH_SHA3_256_BENCH_SIZE;
		nHashBenchSize[HASH_SHA3_384_TYPE] = HASH_SHA3_384_BENCH_SIZE;
		nHashBenchSize[HASH_SHA3_512_TYPE] = HASH_SHA3_512_BENCH_SIZE;

#if _MSC_VER > 1600
		if (CPUID_IS_AVX512)
		{
			Hash_Init[HASH_XXH3_TYPE] = Hash_XXH3_AVX512_Init;
			Hash_Init[HASH_XXH128_TYPE] = Hash_XXH128_AVX512_Init;

			Hash_Update[HASH_XXH3_TYPE] = Hash_XXH3_AVX512_Update;
			Hash_Update[HASH_XXH128_TYPE] = Hash_XXH128_AVX512_Update;

			Hash_Final[HASH_XXH3_TYPE] = Hash_XXH3_AVX512_Final;
			Hash_Final[HASH_XXH128_TYPE] = Hash_XXH128_AVX512_Final;

			nHashBenchSize[HASH_XXH3_TYPE] = HASH_XXH3_BENCH_SIZE * 2;
			nHashBenchSize[HASH_XXH128_TYPE] = HASH_XXH128_BENCH_SIZE * 2;
		}
		else if (CPUID_IS_AVX2)
		{
			Hash_Init[HASH_XXH3_TYPE] = Hash_XXH3_AVX2_Init;
			Hash_Init[HASH_XXH128_TYPE] = Hash_XXH128_AVX2_Init;

			Hash_Update[HASH_XXH3_TYPE] = Hash_XXH3_AVX2_Update;
			Hash_Update[HASH_XXH128_TYPE] = Hash_XXH128_AVX2_Update;

			Hash_Final[HASH_XXH3_TYPE] = Hash_XXH3_AVX2_Final;
			Hash_Final[HASH_XXH128_TYPE] = Hash_XXH128_AVX2_Final;

			nHashBenchSize[HASH_XXH3_TYPE] = HASH_XXH3_BENCH_SIZE * 2;
			nHashBenchSize[HASH_XXH128_TYPE] = HASH_XXH128_BENCH_SIZE * 2;
		}
		else
#endif

#if _MSC_VER > 1300
		if (CPUID_IS_SSE2)
		{
			Hash_Init[HASH_XXH3_TYPE] = Hash_XXH3_SSE2_Init;
			Hash_Init[HASH_XXH128_TYPE] = Hash_XXH128_SSE2_Init;

			Hash_Update[HASH_XXH3_TYPE] = Hash_XXH3_SSE2_Update;
			Hash_Update[HASH_XXH128_TYPE] = Hash_XXH128_SSE2_Update;

			Hash_Final[HASH_XXH3_TYPE] = Hash_XXH3_SSE2_Final;
			Hash_Final[HASH_XXH128_TYPE] = Hash_XXH128_SSE2_Final;

			nHashBenchSize[HASH_XXH3_TYPE] = HASH_XXH3_BENCH_SIZE;
			nHashBenchSize[HASH_XXH128_TYPE] = HASH_XXH128_BENCH_SIZE;
		}
		else
		{
			Hash_Init[HASH_XXH3_TYPE] = Hash_XXH3_Init;
			Hash_Init[HASH_XXH128_TYPE] = Hash_XXH128_Init;

			Hash_Update[HASH_XXH3_TYPE] = Hash_XXH3_Update;
			Hash_Update[HASH_XXH128_TYPE] = Hash_XXH128_Update;

			Hash_Final[HASH_XXH3_TYPE] = Hash_XXH3_Final;
			Hash_Final[HASH_XXH128_TYPE] = Hash_XXH128_Final;

			nHashBenchSize[HASH_XXH3_TYPE] = HASH_XXH3_BENCH_SIZE / 2;
			nHashBenchSize[HASH_XXH128_TYPE] = HASH_XXH128_BENCH_SIZE / 2;
		}
#endif

		if (CPUID_IS_SSE4_2)
		{
			Hash_Update[HASH_CRC32C_TYPE] = Hash_crc32c_SSE4_2_Update;
			nHashBenchSize[HASH_CRC32C_TYPE] = HASH_CRC32C_BENCH_SIZE;
		}
		else
		{
			Hash_Update[HASH_CRC32C_TYPE] = Hash_crc32c_Update;
#ifdef _M_X64
			nHashBenchSize[HASH_CRC32C_TYPE] = HASH_CRC32C_BENCH_SIZE / 8;
#else
			nHashBenchSize[HASH_CRC32C_TYPE] = HASH_CRC32C_BENCH_SIZE / 4;
#endif
		}
	}
	else
	{
		Hash_Update[HASH_MD5_TYPE] = Hash_MD5_NoAsm_Update;				// MD5
		Hash_Update[HASH_SHA1_TYPE] = Hash_SHA1_NoAsm_Update;			// SHA-1
		Hash_Update[HASH_SHA_256_192_TYPE] = Hash_SHA256_NoAsm_Update;	// SHA-256/192
		Hash_Update[HASH_SHA_224_TYPE] = Hash_SHA256_NoAsm_Update;		// SHA-224
		Hash_Update[HASH_SHA_256_TYPE] = Hash_SHA256_NoAsm_Update;		// SHA-256
		Hash_Update[HASH_SHA_384_TYPE] = Hash_SHA512_NoAsm_Update;		// SHA-384
		Hash_Update[HASH_SHA_512_TYPE] = Hash_SHA512_NoAsm_Update;		// SHA-512
		Hash_Update[HASH_SHA_512_224_TYPE] = Hash_SHA512_NoAsm_Update;	// SHA-512/224
		Hash_Update[HASH_SHA_512_256_TYPE] = Hash_SHA512_NoAsm_Update;	// SHA-512/256
		Hash_Update[HASH_SHA3_224_TYPE] = Hash_SHA3_NoAsm_Update;		// SHA3-224
		Hash_Update[HASH_SHA3_256_TYPE] = Hash_SHA3_NoAsm_Update;		// SHA3-256
		Hash_Update[HASH_SHA3_384_TYPE] = Hash_SHA3_NoAsm_Update;		// SHA3-384
		Hash_Update[HASH_SHA3_512_TYPE] = Hash_SHA3_NoAsm_Update;		// SHA3-512

		Hash_Final[HASH_MD5_TYPE] = Hash_MD5_NoAsm_Final;				// MD5
		Hash_Final[HASH_SHA1_TYPE] = Hash_SHA1_NoAsm_Final;				// SHA-1
		Hash_Final[HASH_SHA_256_192_TYPE] = Hash_SHA256_NoAsm_Final;	// SHA-256/192
		Hash_Final[HASH_SHA_224_TYPE] = Hash_SHA256_NoAsm_Final;		// SHA-224
		Hash_Final[HASH_SHA_256_TYPE] = Hash_SHA256_NoAsm_Final;		// SHA-256
		Hash_Final[HASH_SHA_384_TYPE] = Hash_SHA512_NoAsm_Final;		// SHA-384
		Hash_Final[HASH_SHA_512_TYPE] = Hash_SHA512_NoAsm_Final;		// SHA-512
		Hash_Final[HASH_SHA_512_224_TYPE] = Hash_SHA512_NoAsm_Final;	// SHA-512/224
		Hash_Final[HASH_SHA_512_256_TYPE] = Hash_SHA512_NoAsm_Final;	// SHA-512/256
		Hash_Final[HASH_SHA3_224_TYPE] = Hash_SHA3_NoAsm_Final;			// SHA3-224
		Hash_Final[HASH_SHA3_256_TYPE] = Hash_SHA3_NoAsm_Final;			// SHA3-256
		Hash_Final[HASH_SHA3_384_TYPE] = Hash_SHA3_NoAsm_Final;			// SHA3-384
		Hash_Final[HASH_SHA3_512_TYPE] = Hash_SHA3_NoAsm_Final;			// SHA3-512

		nHashBenchSize[HASH_SHA1_TYPE] = HASH_SHA1_BENCH_SIZE / 4;
		nHashBenchSize[HASH_SHA_224_TYPE] = HASH_SHA_224_BENCH_SIZE / 4;
		nHashBenchSize[HASH_SHA_256_TYPE] = HASH_SHA_256_BENCH_SIZE / 4;
		nHashBenchSize[HASH_SHA_256_192_TYPE] = HASH_SHA_256_192_BENCH_SIZE / 4;

#ifdef _M_X64
		nHashBenchSize[HASH_SHA_384_TYPE] = HASH_SHA_384_BENCH_SIZE;
		nHashBenchSize[HASH_SHA_512_TYPE] = HASH_SHA_512_BENCH_SIZE;
		nHashBenchSize[HASH_SHA3_224_TYPE] = HASH_SHA3_224_BENCH_SIZE;
		nHashBenchSize[HASH_SHA3_256_TYPE] = HASH_SHA3_256_BENCH_SIZE;
		nHashBenchSize[HASH_SHA3_384_TYPE] = HASH_SHA3_384_BENCH_SIZE;
		nHashBenchSize[HASH_SHA3_512_TYPE] = HASH_SHA3_512_BENCH_SIZE;
#else
		nHashBenchSize[HASH_SHA_384_TYPE] = HASH_SHA_384_BENCH_SIZE / 4;
		nHashBenchSize[HASH_SHA_512_TYPE] = HASH_SHA_512_BENCH_SIZE / 4;
		nHashBenchSize[HASH_SHA3_224_TYPE] = HASH_SHA3_224_BENCH_SIZE / 4;
		nHashBenchSize[HASH_SHA3_256_TYPE] = HASH_SHA3_256_BENCH_SIZE / 4;
		nHashBenchSize[HASH_SHA3_384_TYPE] = HASH_SHA3_384_BENCH_SIZE / 4;
		nHashBenchSize[HASH_SHA3_512_TYPE] = HASH_SHA3_512_BENCH_SIZE / 4;
#endif

#if _MSC_VER > 1300
		Hash_Init[HASH_XXH3_TYPE] = Hash_XXH3_Init;
		Hash_Init[HASH_XXH128_TYPE] = Hash_XXH128_Init;

		Hash_Update[HASH_XXH3_TYPE] = Hash_XXH3_Update;
		Hash_Update[HASH_XXH128_TYPE] = Hash_XXH128_Update;

		Hash_Final[HASH_XXH3_TYPE] = Hash_XXH3_Final;
		Hash_Final[HASH_XXH128_TYPE] = Hash_XXH128_Final;

		nHashBenchSize[HASH_XXH3_TYPE] = HASH_XXH3_BENCH_SIZE / 2;
		nHashBenchSize[HASH_XXH128_TYPE] = HASH_XXH128_BENCH_SIZE / 2;
#endif

		Hash_Init[HASH_CRC32C_TYPE] = Hash_crc32c_Init;
		Hash_Update[HASH_CRC32C_TYPE] = Hash_crc32c_Update;
		Hash_Final[HASH_CRC32C_TYPE] = Hash_crc32c_Final;
#ifdef _M_X64
		nHashBenchSize[HASH_CRC32C_TYPE] = HASH_CRC32C_BENCH_SIZE / 8;
#else
		nHashBenchSize[HASH_CRC32C_TYPE] = HASH_CRC32C_BENCH_SIZE / 4;
#endif
	}


#ifdef _DEBUG
	if (1)
	{
		int i = 0;

		for (; i < MAX_HASH_TYPE; i++) {
			nHashBenchSize[i] /= 8;
		}
	}
#endif
	return TRUE;
}

DWORD HashThread_GetHashLen(DWORD dwHashType)
{
	return cdwHashLen[dwHashType];
}

const TCHAR* HashThread_GetHashName(DWORD dwHashType)
{
#ifdef UNICODE
	return cpHashNameW[dwHashType];
#else
	return cpHashNameA[dwHashType];
#endif
}

size_t HashThread_GetHashNameLen(DWORD dwHashType)
{
	return cnHashNameLen[dwHashType];
}

const TCHAR* HashThread_GetHashListName(DWORD dwHashType)
{
#ifdef UNICODE
	return cpHashListNameW[dwHashType];
#else
	return cpHashListNameA[dwHashType];
#endif
}

size_t HashThread_GetHashListNameLen(DWORD dwHashType)
{
	return cnHashListNameLen[dwHashType];
}

const TCHAR* HashThread_GetHashExt(DWORD dwHashType)
{
	return cpHashExt[dwHashType];
}

size_t HashThread_GetHashExtLen(DWORD dwHashType)
{
	return nHashExtLen[dwHashType];
}

BOOL HashThread_GetByteSwap(DWORD dwHashType)
{
	return nHashByteSwap[dwHashType];
}

BOOL HashThread_GetString(TCHAR* pOutTChar, const BYTE* cpInBinary, DWORD dwHashType, BOOL bIsUpper, TCHAR* pOutBase64TChar)
{
	const DWORD dwLength = HashThread_GetHashLen(dwHashType);
	const BOOL bIsSwap = HashThread_GetByteSwap(dwHashType);

	IF_LIKELY(!bIsSwap)
	{
		BinaryToTChar(pOutTChar, cpInBinary, dwLength, bIsUpper);
		if (pOutBase64TChar) {
			BinaryToBase64ToTChar(cpInBinary, dwLength, pOutBase64TChar);
		}
	}
	else
	{
		unsigned __int64 r[32] = { 0 };
		unsigned long* e = (unsigned long*)r;

		switch (dwLength)
		{
		case 1:		// 8bit
			memcpy(&e[0], cpInBinary, 1);
			break;
		case 2:		// 16bit
			memcpy(&e[0], cpInBinary, 2);
			e[0] = BSwap16((unsigned short)(e[0] & 0xFFFF));
			break;
		case 4:		// 32bit
			memcpy(&e[0], cpInBinary, 4);
			e[0] = BSwap32(e[0]);
			break;
		case 8:		// 64bit
#if defined(_M_X64)
			memcpy(&r[0], cpInBinary, 8);
			r[0] = BSwap64(r[0]);
#else
			memcpy(&e[2], cpInBinary, 8);
			e[1] = BSwap32(e[2]);
			e[0] = BSwap32(e[3]);
#endif
			break;
		case 16:	// 128bit
#if defined(_M_X64)
			memcpy(&r[2], cpInBinary, 16);
			r[1] = BSwap64(r[2]);
			r[0] = BSwap64(r[3]);
#else
			memcpy(&e[4], cpInBinary, 16);
			e[3] = BSwap32(e[4]);
			e[2] = BSwap32(e[5]);
			e[1] = BSwap32(e[6]);
			e[0] = BSwap32(e[7]);
#endif
			break;
		case 20:	// 160bit
			memcpy(&e[6], cpInBinary, 24);
			e[4] = BSwap32(e[6]);
			e[3] = BSwap32(e[7]);
			e[2] = BSwap32(e[8]);
			e[1] = BSwap32(e[9]);
			e[0] = BSwap32(e[10]);
			break;
		case 24:	// 192bit
#if defined(_M_X64)
			memcpy(&r[3], cpInBinary, 24);
			r[2] = BSwap64(r[3]);
			r[1] = BSwap64(r[4]);
			r[0] = BSwap64(r[5]);
#else
			memcpy(&e[6], cpInBinary, 24);
			e[5] = BSwap32(e[6]);
			e[4] = BSwap32(e[7]);
			e[3] = BSwap32(e[8]);
			e[2] = BSwap32(e[9]);
			e[1] = BSwap32(e[10]);
			e[0] = BSwap32(e[11]);
#endif
			break;
		case 28:	// 224bit
			memcpy(&e[8], cpInBinary, 32);
			e[6] = BSwap32(e[8]);
			e[5] = BSwap32(e[9]);
			e[4] = BSwap32(e[10]);
			e[3] = BSwap32(e[11]);
			e[2] = BSwap32(e[12]);
			e[1] = BSwap32(e[13]);
			e[0] = BSwap32(e[14]);
			break;
		case 32:	// 256bit
#if defined(_M_X64)
			memcpy(&r[4], cpInBinary, 32);
			r[3] = BSwap64(r[4]);
			r[2] = BSwap64(r[5]);
			r[1] = BSwap64(r[6]);
			r[0] = BSwap64(r[7]);
#else
			memcpy(&e[8], cpInBinary, 32);
			e[7] = BSwap32(e[8]);
			e[6] = BSwap32(e[9]);
			e[5] = BSwap32(e[10]);
			e[4] = BSwap32(e[11]);
			e[3] = BSwap32(e[12]);
			e[2] = BSwap32(e[13]);
			e[1] = BSwap32(e[14]);
			e[0] = BSwap32(e[15]);
#endif
			break;
		case 48:	// 384bit
#if defined(_M_X64)
			memcpy(&r[6], cpInBinary, 48);
			r[5] = BSwap64(r[6]);
			r[4] = BSwap64(r[7]);
			r[3] = BSwap64(r[8]);
			r[2] = BSwap64(r[9]);
			r[1] = BSwap64(r[10]);
			r[0] = BSwap64(r[11]);
#else
			memcpy(&e[12], cpInBinary, 48);
			e[11] = BSwap32(e[12]);
			e[10] = BSwap32(e[13]);
			e[9] = BSwap32(e[14]);
			e[8] = BSwap32(e[15]);
			e[7] = BSwap32(e[16]);
			e[6] = BSwap32(e[17]);
			e[5] = BSwap32(e[18]);
			e[4] = BSwap32(e[19]);
			e[3] = BSwap32(e[20]);
			e[2] = BSwap32(e[21]);
			e[1] = BSwap32(e[22]);
			e[0] = BSwap32(e[23]);
#endif
			break;
		case 64:	// 512bit
#if defined(_M_X64)
			memcpy(&r[8], cpInBinary, 64);
			r[7] = BSwap64(r[8]);
			r[6] = BSwap64(r[9]);
			r[5] = BSwap64(r[10]);
			r[4] = BSwap64(r[11]);
			r[3] = BSwap64(r[12]);
			r[2] = BSwap64(r[13]);
			r[1] = BSwap64(r[14]);
			r[0] = BSwap64(r[15]);
#else
			memcpy(&e[16], cpInBinary, 64);
			e[15] = BSwap32(e[16]);
			e[14] = BSwap32(e[17]);
			e[13] = BSwap32(e[18]);
			e[12] = BSwap32(e[19]);
			e[11] = BSwap32(e[20]);
			e[10] = BSwap32(e[21]);
			e[9] = BSwap32(e[22]);
			e[8] = BSwap32(e[23]);
			e[7] = BSwap32(e[24]);
			e[6] = BSwap32(e[25]);
			e[5] = BSwap32(e[26]);
			e[4] = BSwap32(e[27]);
			e[3] = BSwap32(e[28]);
			e[2] = BSwap32(e[29]);
			e[1] = BSwap32(e[30]);
			e[0] = BSwap32(e[31]);
#endif
			break;
		case 128:	// 1024bit
#if defined(_M_X64)
			memcpy(&r[16], cpInBinary, 128);
			r[15] = BSwap64(r[16]);
			r[14] = BSwap64(r[17]);
			r[13] = BSwap64(r[18]);
			r[12] = BSwap64(r[19]);
			r[11] = BSwap64(r[20]);
			r[10] = BSwap64(r[21]);
			r[9] = BSwap64(r[22]);
			r[8] = BSwap64(r[23]);
			r[7] = BSwap64(r[24]);
			r[6] = BSwap64(r[25]);
			r[5] = BSwap64(r[26]);
			r[4] = BSwap64(r[27]);
			r[3] = BSwap64(r[28]);
			r[2] = BSwap64(r[29]);
			r[1] = BSwap64(r[30]);
			r[0] = BSwap64(r[31]);
#else
			memcpy(&e[32], cpInBinary, 128);
			e[31] = BSwap32(e[32]);
			e[30] = BSwap32(e[33]);
			e[29] = BSwap32(e[34]);
			e[28] = BSwap32(e[35]);
			e[27] = BSwap32(e[36]);
			e[26] = BSwap32(e[37]);
			e[25] = BSwap32(e[38]);
			e[24] = BSwap32(e[39]);
			e[23] = BSwap32(e[40]);
			e[22] = BSwap32(e[41]);
			e[21] = BSwap32(e[42]);
			e[20] = BSwap32(e[43]);
			e[19] = BSwap32(e[44]);
			e[18] = BSwap32(e[45]);
			e[17] = BSwap32(e[46]);
			e[16] = BSwap32(e[47]);
			e[15] = BSwap32(e[48]);
			e[14] = BSwap32(e[49]);
			e[13] = BSwap32(e[50]);
			e[12] = BSwap32(e[51]);
			e[11] = BSwap32(e[52]);
			e[10] = BSwap32(e[53]);
			e[9] = BSwap32(e[54]);
			e[8] = BSwap32(e[55]);
			e[7] = BSwap32(e[56]);
			e[6] = BSwap32(e[57]);
			e[5] = BSwap32(e[58]);
			e[4] = BSwap32(e[59]);
			e[3] = BSwap32(e[60]);
			e[2] = BSwap32(e[61]);
			e[1] = BSwap32(e[62]);
			e[0] = BSwap32(e[63]);
#endif
			break;
		}
		BinaryToTChar(pOutTChar, (BYTE*)r, dwLength, bIsUpper);
		if (pOutBase64TChar) {
			BinaryToBase64ToTChar(r, dwLength, pOutBase64TChar);
		}
	}
	return TRUE;
}

unsigned __stdcall HashThread_MultiFile_Thread(void* lpThread)
{
	HashThread* ptagHashThread = (HashThread*)lpThread;
	XXH3_state_s* mdCTX = (XXH3_state_s*)ptagHashThread->mdCTX;
	tagHashThread_FileRecode* ptagHashFile_Recode = NULL;
	const DWORD dwFileBufferSize = ptagHashThread->dwFileBufferSize;
	const DWORD dwHashType = ptagHashThread->dwHashType;
	const DWORD dwHashLen = HashThread_GetHashLen(dwHashType);
	DWORD dwCreateFileAttributes = 0;
	DWORD dwRead = 0;
	DWORD dwLasterror = 0;
	DWORD dwFileCount = 0;
	DWORD dwFileCurrentCount = 0;
	BOOL nRet = FALSE;
	BOOL nIsFileSkip = FALSE;
	int (*Hash_Init_Func)(void*) = Hash_Init[dwHashType];
	int (*Hash_Update_Func)(void*, const void*, size_t) = Hash_Update[dwHashType];
	int (*Hash_Final_Func)(unsigned char*, void*) = Hash_Final[dwHashType];
	BYTE* pFileBuffer = ptagHashThread->pFileBuffer;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	if (ptagHashThread->nFileNoCache == FALSE) {
		dwCreateFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN;
	}
	else {
		dwCreateFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING;
	}

	IF_UNLIKELY(ptagHashThread->tagMultiFile->dwFileCount == 0)
	{
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_ERROR, 0);
		ptagHashThread->nSuspendMessage = APP_PRGRESS_EXIT;
		IF_LIKELY(ptagHashThread->hHashThreadHandle)
		{
			ptagHashThread->hHashThreadHandle = NULL;
			_endthreadex(0);
		}
		return 0;
	}


	// 構造体の初期化
	ptagHashThread->nSuspendMessage = 0;
	ptagHashThread->llFileSize = 0;
	ptagHashThread->tagMultiFile->llFileAllCurrentSize = 0;

	// マルチファイルに関する初期化

	IF_UNLIKELY(ptagHashThread->tagMultiFile->dwFileCurrentCount > 0)
	{
		DWORD dwCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;

		ptagHashThread->tagMultiFile->llFileAllSize = 0;
		dwFileCount = HashThread_GetCountItem(ptagHashThread);

		for (; dwCount < dwFileCount; dwCount++) {
			ptagHashThread->tagMultiFile->llFileAllSize += HashThread_GetFileRecode_Core(ptagHashThread, dwCount)->llFileSize;
		}
	}
	else {
		ptagHashThread->tagMultiFile->llFileAllSize = ptagHashThread->tagMultiFile->llFileAllSizeBackup;
	}

	dwFileCount = HashThread_GetCountItem(ptagHashThread);
	dwFileCurrentCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;

	IF_UNLIKELY(dwFileCurrentCount >= dwFileCount)
	{
		if (dwFileCurrentCount != dwFileCount) {
			ptagHashThread->tagMultiFile->dwFileCurrentCount = 0;
		}
		else {
			ptagHashThread->tagMultiFile->dwFileCurrentCount--;
		}
		dwFileCurrentCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;
	}
	SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_INIT, ptagHashThread->tagMultiFile->dwFileCurrentCount);

	IF_UNLIKELY(ptagHashThread->nIsFileNoCheck == 0)
	{
		WIN32_FIND_DATA fdFindData = { 0 };
		HANDLE hFind = NULL;

		// マルチファイルに関する初期化
		ptagHashThread->tagMultiFile->dwFileCount = 0;
		ptagHashThread->tagMultiFile->llFileAllSize = 0;
		dwFileCount = 0;

		// ファイルのサイズ取得
		hFind = FindFirstFile(ptagHashThread->pFilePath, &fdFindData);
		IF_LIKELY(hFind != INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);

			if (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				HashThread_Folder_Init(ptagHashThread, ptagHashThread->pFilePath, FALSE, TRUE);
			}
			else {
				HashThread_MultiFile_Init(ptagHashThread, ptagHashThread->pFilePath, FALSE);
			}
		}
	}

	ptagHashThread->dwStartTime = timeGetTime();
	PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_START, 0);
	ptagHashThread->dwEndTime = ptagHashThread->dwStartTime;

	WHILE_LIKELY(ptagHashThread->nSuspendMessage == 0)
	{
		HANDLE hFile = NULL;

		nIsFileSkip = FALSE;
		ptagHashFile_Recode = ptagHashThread->tagMultiFile->ptagFileRecode + dwFileCurrentCount;
		ptagHashFile_Recode->dwFileHashType	= dwHashType;
		ptagHashThread->llFileSize = ptagHashFile_Recode->llFileSize;
		ptagHashThread->llFileReadSize = 0;
		memset(ptagHashFile_Recode->pFileHashByte, 0, HASH_LEN);
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_CHANGE, dwFileCurrentCount);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Thread(): [%d] %s\r\n"), dwFileCurrentCount, ptagHashFile_Recode->szFileName);
		OutputDebugString(szDebugText);

		if (ptagHashFile_Recode->dwFileLastError != (DWORD)-1)
		{
			STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Thread(): LastError: %d\r\n"), ptagHashFile_Recode->dwFileLastError);
			OutputDebugString(szDebugText);
		}
#endif

		hFile = CreateFile(ptagHashFile_Recode->szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, dwCreateFileAttributes, NULL);
		IF_UNLIKELY(hFile == INVALID_HANDLE_VALUE)
		{
			dwLasterror = GetLastError();
			hFile = NULL;
			goto FILE_EXIT2;
		}

		Hash_Init_Func(mdCTX);

		WHILE_LIKELY((nRet = ReadFile(hFile, pFileBuffer, dwFileBufferSize, &dwRead, NULL)) != 0 && dwRead > 0)
		{
			IF_UNLIKELY(ptagHashThread->nSuspendMessage & (APP_PRGRESS_EXIT | APP_PRGRESS_CANCEL))
			{
				dwLasterror = (DWORD)-1;
				nRet = FALSE;
				goto FILE_EXIT;
			}

			//ハッシュ計算
			Hash_Update_Func(mdCTX, pFileBuffer, dwRead);

			ptagHashThread->llFileReadSize += dwRead;
		}

		IF_UNLIKELY(ptagHashThread->llFileReadSize != ptagHashThread->llFileSize)
		{
			ptagHashFile_Recode->llFileSize = ptagHashThread->llFileReadSize;
			ptagHashThread->tagMultiFile->llFileAllSize -= ptagHashThread->llFileSize - ptagHashThread->llFileReadSize;
			ptagHashThread->llFileSize = ptagHashThread->llFileReadSize;
		}

		// ReadFile() の失敗処理
		IF_UNLIKELY(nRet == FALSE)
		{
			dwLasterror = GetLastError();
			ptagHashFile_Recode->dwFileLastError = dwLasterror;

#ifdef _DEBUG
			STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Thread(): %s\r\nLastError: %d\r\n"), _T("ReadFile()"), dwLasterror);
			OutputDebugString(szDebugText);
#endif
			goto FILE_EXIT;
		}

		Hash_Final_Func(ptagHashFile_Recode->pFileHashByte, mdCTX);


FILE_EXIT:
		CloseHandle(hFile);

FILE_EXIT2:
		hFile = NULL;

		IF_UNLIKELY(dwLasterror != 0)
		{
			ptagHashThread->llFileSize = 0;
			ptagHashFile_Recode->dwFileLastError = dwLasterror;
			ptagHashThread->tagMultiFile->llFileAllSize -= ptagHashFile_Recode->llFileSize;
		}
		else
		{
			ptagHashFile_Recode->dwFileLastError = 0;
			ptagHashThread->tagMultiFile->llFileAllCurrentSize += ptagHashThread->llFileSize;

			if (ptagHashFile_Recode->dwHashFileHashLine != HASHFILE_NOLINE && ptagHashFile_Recode->dwFileHashType == ptagHashFile_Recode->dwHashFileHashType)
			{
				ptagHashFile_Recode->nHashFileCmp = memcmp(ptagHashFile_Recode->pFileHashByte, ptagHashFile_Recode->nHashFileHashByte, dwHashLen);
				PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_HASHCOMP, ptagHashFile_Recode->nHashFileCmp);
			}
		}

#ifdef _DEBUG
		if (ptagHashFile_Recode->dwFileLastError == 0)
		{
			TCHAR szHashBuf[HASH_LOADSTRING];

			BinaryToTCharToLower(szHashBuf, ptagHashFile_Recode->pFileHashByte, dwHashLen);
			STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Thread(): %s\r\n"), szHashBuf);
			OutputDebugString(szDebugText);
		}
#endif

		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_EXIT, dwFileCurrentCount);

		IF_UNLIKELY((dwFileCurrentCount + 1) >= dwFileCount) {
			break;
		}
		dwLasterror = 0;
		ptagHashThread->tagMultiFile->dwFileCurrentCount++;
		dwFileCurrentCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;
	}

	ptagHashThread->dwEndTime = timeGetTime();
	ptagHashThread->llFileReadSize = 0;
	ptagHashThread->llFileSize = 0;
	memcpy(ptagHashThread->bHashByte, ptagHashFile_Recode->pFileHashByte, HASH_LEN);

	IF_LIKELY(nRet != FALSE)
	{
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_END, ptagHashThread->tagMultiFile->dwFileCurrentCount);
		dwLasterror = 0;
	}
	else
	{
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_LASTERROR, dwLasterror);
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_ERROR, dwLasterror);
	}

	ptagHashThread->nSuspendMessage = APP_PRGRESS_EXIT;

	IF_LIKELY(ptagHashThread->hHashThreadHandle)
	{
		ptagHashThread->hHashThreadHandle = NULL;
		ptagHashThread->nThreadID = NULL;
		_endthreadex(dwLasterror);
	}
	return dwLasterror;
}

/*
unsigned __stdcall HashThread_MultiFile_Mmap_Thread(void* lpThread)
{
	HashThread* ptagHashThread = (HashThread*)lpThread;
	XXH3_state_s* mdCTX = (XXH3_state_s*)ptagHashThread->mdCTX;
	tagHashThread_FileRecode* ptagHashFile_Recode = NULL;
	const DWORD dwHashType = ptagHashThread->dwHashType;
	const DWORD dwHashLen = HashThread_GetHashLen(dwHashType);
	DWORD dwCreateFileAttributes = 0;
	DWORD dwLasterror = 0;
	DWORD dwFileCount = 0;
	DWORD dwFileCurrentCount = 0;
	BOOL nRet = FALSE;
	BOOL nIsFileSkip = FALSE;
	int (*Hash_Init_Func)(void*) = Hash_Init[dwHashType];
	int (*Hash_Update_Func)(void*, const void*, size_t) = Hash_Update[dwHashType];
	int (*Hash_Final_Func)(unsigned char*, void*) = Hash_Final[dwHashType];
	BYTE* pFileBuffer = ptagHashThread->pFileBuffer;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	if (ptagHashThread->nFileNoCache == FALSE) {
		dwCreateFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN;
	}
	else {
		dwCreateFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING;
	}

	IF_UNLIKELY(ptagHashThread->tagMultiFile->dwFileCount == 0)
	{
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_ERROR, 0);
		ptagHashThread->nSuspendMessage = APP_PRGRESS_EXIT;
		IF_LIKELY(ptagHashThread->hHashThreadHandle)
		{
			ptagHashThread->hHashThreadHandle = NULL;
			_endthreadex(0);
		}
		return 0;
	}


	// 構造体の初期化
	ptagHashThread->nSuspendMessage = 0;
	ptagHashThread->llFileSize = 0;
	ptagHashThread->tagMultiFile->llFileAllCurrentSize = 0;

	// マルチファイルに関する初期化

	IF_UNLIKELY(ptagHashThread->tagMultiFile->dwFileCurrentCount > 0)
	{
		DWORD dwCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;

		ptagHashThread->tagMultiFile->llFileAllSize = 0;
		dwFileCount = HashThread_GetCountItem(ptagHashThread);

		for (; dwCount < dwFileCount; dwCount++) {
			ptagHashThread->tagMultiFile->llFileAllSize += HashThread_GetFileRecode_Core(ptagHashThread, dwCount)->llFileSize;
		}
	}
	else {
		ptagHashThread->tagMultiFile->llFileAllSize = ptagHashThread->tagMultiFile->llFileAllSizeBackup;
	}

	dwFileCount = HashThread_GetCountItem(ptagHashThread);
	dwFileCurrentCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;

	IF_UNLIKELY(dwFileCurrentCount >= dwFileCount)
	{
		if (dwFileCurrentCount != dwFileCount) {
			ptagHashThread->tagMultiFile->dwFileCurrentCount = 0;
		}
		else {
			ptagHashThread->tagMultiFile->dwFileCurrentCount--;
		}
		dwFileCurrentCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;
	}
	SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_INIT, ptagHashThread->tagMultiFile->dwFileCurrentCount);

	IF_UNLIKELY(ptagHashThread->nIsFileNoCheck == 0)
	{
		WIN32_FIND_DATA fdFindData = { 0 };
		HANDLE hFind = NULL;

		// マルチファイルに関する初期化
		ptagHashThread->tagMultiFile->dwFileCount = 0;
		ptagHashThread->tagMultiFile->llFileAllSize = 0;
		dwFileCount = 0;

		// ファイルのサイズ取得
		hFind = FindFirstFile(ptagHashThread->pFilePath, &fdFindData);
		IF_LIKELY(hFind != INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);

			if (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				HashThread_Folder_Init(ptagHashThread, ptagHashThread->pFilePath, FALSE, TRUE);
			}
			else {
				HashThread_MultiFile_Init(ptagHashThread, ptagHashThread->pFilePath, FALSE);
			}
		}
	}

	ptagHashThread->dwStartTime = timeGetTime();
	PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_START, 0);
	ptagHashThread->dwEndTime = ptagHashThread->dwStartTime;

	WHILE_LIKELY(ptagHashThread->nSuspendMessage == 0)
	{
		HANDLE hFile = NULL;
		HANDLE hMap = NULL;
		unsigned __int64 nFileSize = 0;
		unsigned __int64 nLoopSize = 0;
		unsigned __int64 nReadSize = 0;

		ptagHashFile_Recode = ptagHashThread->tagMultiFile->ptagFileRecode + dwFileCurrentCount;
		nFileSize = ptagHashFile_Recode->llFileSize;
		nLoopSize = ptagHashFile_Recode->llFileSize / 0x100000 * 0x100000;
		nRet = TRUE;
		nIsFileSkip = FALSE;
		ptagHashFile_Recode->dwFileHashType = dwHashType;
		ptagHashThread->llFileSize = nFileSize;
		ptagHashThread->llFileReadSize = 0;
		memset(ptagHashFile_Recode->pFileHashByte, 0, HASH_LEN);
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_CHANGE, dwFileCurrentCount);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Thread(): [%d] %s\r\n"), dwFileCurrentCount, ptagHashFile_Recode->szFileName);
		OutputDebugString(szDebugText);

		if (ptagHashFile_Recode->dwFileLastError != (DWORD)-1)
		{
			STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Thread(): LastError: %d\r\n"), ptagHashFile_Recode->dwFileLastError);
			OutputDebugString(szDebugText);
		}
#endif

		hFile = CreateFile(ptagHashFile_Recode->szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, dwCreateFileAttributes, NULL);
		IF_UNLIKELY(hFile == INVALID_HANDLE_VALUE)
		{
			dwLasterror = GetLastError();
			hFile = NULL;
			goto FILE_EXIT;
		}

		if (nFileSize == 0)
		{
			Hash_Init_Func(mdCTX);
			Hash_Update_Func(mdCTX, pFileBuffer, 0);
			Hash_Final_Func(ptagHashFile_Recode->pFileHashByte, mdCTX);
			goto FILE_EXIT;
		}

		hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		IF_UNLIKELY(!hMap)
		{
			dwLasterror = GetLastError();
			nRet = FALSE;
			goto FILE_EXIT;
		}

		Hash_Init_Func(mdCTX);

		WHILE_LIKELY(nReadSize < nLoopSize)
		{
			IF_UNLIKELY(ptagHashThread->nSuspendMessage & (APP_PRGRESS_EXIT | APP_PRGRESS_CANCEL))
			{
				dwLasterror = (DWORD)-1;
				nRet = FALSE;
				goto FILE_EXIT;
			}

			pFileBuffer = (BYTE*)MapViewOfFile(hMap, FILE_MAP_READ, nReadSize >> 32, nReadSize & 0xFFFFFFFF, 0x100000);
			IF_UNLIKELY(!pFileBuffer)
			{
				dwLasterror = GetLastError();
				nRet = FALSE;
				goto FILE_EXIT;
			}

			//ハッシュ計算
			Hash_Update_Func(mdCTX, pFileBuffer, 0x100000);
			UnmapViewOfFile(hMap);

			ptagHashThread->llFileReadSize += 0x100000;
			nReadSize += 0x100000;
		}

		if (nReadSize < nFileSize)
		{
			const size_t n = nFileSize % 0x100000;

			pFileBuffer = (BYTE*)MapViewOfFile(hMap, FILE_MAP_READ, nReadSize >> 32, nReadSize & 0xFFFFFFFF, n);
			IF_UNLIKELY(!pFileBuffer)
			{
				dwLasterror = GetLastError();
				nRet = FALSE;
				goto FILE_EXIT;
			}

			Hash_Update_Func(mdCTX, pFileBuffer, n);
			UnmapViewOfFile(hMap);
			ptagHashThread->llFileReadSize += n;
		}

		IF_UNLIKELY(ptagHashThread->llFileReadSize != ptagHashThread->llFileSize)
		{
			ptagHashFile_Recode->llFileSize = ptagHashThread->llFileReadSize;
			ptagHashThread->tagMultiFile->llFileAllSize -= ptagHashThread->llFileSize - ptagHashThread->llFileReadSize;
			ptagHashThread->llFileSize = ptagHashThread->llFileReadSize;
		}

		// ReadFile() の失敗処理
		IF_UNLIKELY(nRet == FALSE)
		{
			dwLasterror = GetLastError();
			ptagHashFile_Recode->dwFileLastError = dwLasterror;

#ifdef _DEBUG
			STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Thread(): %s\r\nLastError: %d\r\n"), _T("ReadFile()"), dwLasterror);
			OutputDebugString(szDebugText);
#endif
			goto FILE_EXIT;
		}

		Hash_Final_Func(ptagHashFile_Recode->pFileHashByte, mdCTX);


	FILE_EXIT:
		CloseHandle(hMap);
		CloseHandle(hFile);
		hFile = NULL;

		IF_UNLIKELY(dwLasterror != 0)
		{
			ptagHashThread->llFileSize = 0;
			ptagHashFile_Recode->dwFileLastError = dwLasterror;
			ptagHashThread->tagMultiFile->llFileAllSize -= ptagHashFile_Recode->llFileSize;
		}
		else
		{
			ptagHashFile_Recode->dwFileLastError = 0;
			ptagHashThread->tagMultiFile->llFileAllCurrentSize += ptagHashThread->llFileSize;

			if (ptagHashFile_Recode->dwHashFileHashLine != HASHFILE_NOLINE && ptagHashFile_Recode->dwFileHashType == ptagHashFile_Recode->dwHashFileHashType)
			{
				ptagHashFile_Recode->nHashFileCmp = memcmp(ptagHashFile_Recode->pFileHashByte, ptagHashFile_Recode->nHashFileHashByte, dwHashLen);
				PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_HASHCOMP, ptagHashFile_Recode->nHashFileCmp);
			}
		}

#ifdef _DEBUG
		if (ptagHashFile_Recode->dwFileLastError == 0)
		{
			TCHAR szHashBuf[HASH_LOADSTRING];

			BinaryToTChar(szHashBuf, ptagHashFile_Recode->pFileHashByte, dwHashLen);
			STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Thread(): %s\r\n"), szHashBuf);
			OutputDebugString(szDebugText);
		}
#endif

		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_EXIT, dwFileCurrentCount);

		IF_UNLIKELY((dwFileCurrentCount + 1) >= dwFileCount) {
			break;
		}
		dwLasterror = 0;
		ptagHashThread->tagMultiFile->dwFileCurrentCount++;
		dwFileCurrentCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;
	}

	ptagHashThread->dwEndTime = timeGetTime();
	ptagHashThread->llFileReadSize = 0;
	ptagHashThread->llFileSize = 0;
	memcpy(ptagHashThread->bHashByte, ptagHashFile_Recode->pFileHashByte, HASH_LEN);

	IF_UNLIKELY(ptagHashThread->nSuspendMessage & APP_PRGRESS_EXIT)
	{
		HashThread_ClearBuffer(ptagHashThread);

#ifdef _DEBUG
		OutputDebugString(_T("HashThread: HashThread_MultiFile_Thread(): 緊急終了\r\n"));
#endif

	}

	IF_LIKELY(nRet != FALSE)
	{
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_END, ptagHashThread->tagMultiFile->dwFileCurrentCount);
		dwLasterror = 0;
	}
	else
	{
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_LASTERROR, dwLasterror);
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_ERROR, dwLasterror);
	}

	ptagHashThread->nSuspendMessage = APP_PRGRESS_EXIT;

	IF_LIKELY(ptagHashThread->hHashThreadHandle)
	{
		ptagHashThread->hHashThreadHandle = NULL;
		ptagHashThread->nThreadID = NULL;
		_endthreadex(dwLasterror);
	}
	return dwLasterror;
}
*/

unsigned __stdcall HashThread_MultiFile_Bench_Thread(void* lpThread)
{
	HashThread* ptagHashThread = (HashThread*)lpThread;
	XXH3_state_s* mdCTX = (XXH3_state_s*)ptagHashThread->mdCTX;
	tagHashThread_FileRecode* ptagHashFile_Recode = NULL;
	const DWORD dwFileBufferSize = ptagHashThread->dwFileBufferSize;
	const DWORD dwHashType = ptagHashThread->dwHashType;
	const DWORD dwHashLen = HashThread_GetHashLen(dwHashType);
	DWORD dwLasterror = 0;
	DWORD dwFileCount = 0;
	DWORD dwFileCurrentCount = 0;
	BOOL nRet = FALSE;
	int (*Hash_Init_Func)(void*) = NULL;
	int (*Hash_Update_Func)(void*, const void*, size_t) = NULL;
	int (*Hash_Final_Func)(unsigned char*, void*) = NULL;
	BYTE* pFileBuffer = ptagHashThread->pFileBuffer;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	Hash_Init_Func = Hash_Init[dwHashType];
	Hash_Update_Func = Hash_Update[dwHashType];
	Hash_Final_Func = Hash_Final[dwHashType];

	// 構造体の初期化
	ptagHashThread->nSuspendMessage = 0;

	dwFileCount = 1;
	SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_INIT, ptagHashThread->tagMultiFile->dwFileCurrentCount);
	ptagHashThread->dwStartTime = timeGetTime();
	PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_START, 0);
	ptagHashThread->dwEndTime = ptagHashThread->dwStartTime;

	WHILE_LIKELY(ptagHashThread->nSuspendMessage == 0)
	{
		ptagHashFile_Recode = ptagHashThread->tagMultiFile->ptagFileRecode + dwFileCurrentCount;
		ptagHashThread->llFileSize = ptagHashFile_Recode->llFileSize;
		ptagHashThread->llFileReadSize = 0;
		memset(ptagHashFile_Recode->pFileHashByte, 0, HASH_LEN);
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_CHANGE, dwFileCurrentCount);

		Hash_Init_Func(mdCTX);

		WHILE_LIKELY(ptagHashThread->llFileReadSize < ptagHashThread->tagMultiFile->llFileAllSize)
		{
			IF_UNLIKELY(ptagHashThread->nSuspendMessage & (APP_PRGRESS_EXIT | APP_PRGRESS_CANCEL))
			{
				dwLasterror = (DWORD)-1;
				nRet = FALSE;
				goto FILE_EXIT;
			}

			//ハッシュ計算
			Hash_Update_Func(mdCTX, pFileBuffer, dwFileBufferSize);

			ptagHashThread->llFileReadSize += dwFileBufferSize;
		}

		nRet = Hash_Final_Func(ptagHashFile_Recode->pFileHashByte, mdCTX);

FILE_EXIT:

		IF_UNLIKELY(dwLasterror != 0)
		{
			ptagHashThread->llFileSize = 0;
			ptagHashFile_Recode->dwFileLastError = dwLasterror;
			ptagHashThread->tagMultiFile->llFileAllSize -= ptagHashFile_Recode->llFileSize;
		}
		else
		{
			ptagHashFile_Recode->dwFileLastError = 0;
			ptagHashThread->tagMultiFile->llFileAllCurrentSize += ptagHashThread->llFileSize;

			if (ptagHashFile_Recode->dwHashFileHashLine != HASHFILE_NOLINE && ptagHashFile_Recode->dwFileHashType == ptagHashFile_Recode->dwHashFileHashType)
			{
				ptagHashFile_Recode->nHashFileCmp = memcmp(ptagHashFile_Recode->pFileHashByte, ptagHashFile_Recode->nHashFileHashByte, dwHashLen);
				PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_HASHCOMP, ptagHashFile_Recode->nHashFileCmp);
			}
		}

#ifdef _DEBUG
		if (ptagHashFile_Recode->dwFileLastError == 0)
		{
			TCHAR szHashBuf[HASH_LOADSTRING];

			BinaryToTCharToLower(szHashBuf, ptagHashFile_Recode->pFileHashByte, dwHashLen);
			STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_MultiFile_Bench_Thread(): %s\r\n"), szHashBuf);
			OutputDebugString(szDebugText);
		}
#endif

		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FILE_EXIT, dwFileCurrentCount);

		IF_UNLIKELY((dwFileCurrentCount + 1) >= dwFileCount) {
			break;
		}
		dwLasterror = 0;
		ptagHashThread->tagMultiFile->dwFileCurrentCount++;
		dwFileCurrentCount = ptagHashThread->tagMultiFile->dwFileCurrentCount;
	}

	ptagHashThread->dwEndTime = timeGetTime();
	ptagHashThread->llFileReadSize = 0;
	ptagHashThread->llFileSize = 0;
	memcpy(ptagHashThread->bHashByte, ptagHashFile_Recode->pFileHashByte, HASH_LEN);

	IF_UNLIKELY(ptagHashThread->nSuspendMessage & APP_PRGRESS_EXIT)
	{
		HashThread_ClearBuffer(ptagHashThread);

#ifdef _DEBUG
		OutputDebugString(_T("HashThread: HashThread_MultiFile_Thread(): 緊急終了\r\n"));
#endif

	}

	IF_LIKELY(nRet != FALSE)
	{
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_END, ptagHashThread->tagMultiFile->dwFileCurrentCount);
		dwLasterror = 0;
	}
	else
	{
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_LASTERROR, dwLasterror);
		PostMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_ERROR, dwLasterror);
	}

	ptagHashThread->nSuspendMessage = APP_PRGRESS_EXIT;

	IF_UNLIKELY(ptagHashThread->hHashThreadHandle)
	{
		ptagHashThread->hHashThreadHandle = NULL;
		ptagHashThread->nThreadID = NULL;
		_endthreadex(dwLasterror);
	}
	return dwLasterror;
} 

tagHashThread_FileRecode* HashThread_GetFileRecode_Core(const HashThread* cptagHashThread, DWORD dwItem)
{
	IF_UNLIKELY(dwItem > FILE_MAX_COUNTSIZE) {
		dwItem = 0;
	}
	return cptagHashThread->tagMultiFile->ptagFileRecode + dwItem;
}

tagHashThread_FileRecode* HashThread_GetFileRecode(const HashThread* cptagHashThread, DWORD dwItem)
{
	DWORD dwMaxItem;

	IF_UNLIKELY(cptagHashThread == NULL || cptagHashThread->tagMultiFile == NULL) {
		return FALSE;
	}
	dwMaxItem = HashThread_GetCountItem_Core(cptagHashThread);
	if (dwItem > dwMaxItem) {
		dwItem = dwMaxItem;
	}
	return HashThread_GetFileRecode_Core(cptagHashThread, dwItem);
}

tagHashThread_FileRecode* HashThread_GetFileCurrentFileRecode_Core(const HashThread* cptagHashThread)
{
	DWORD dwItem;
	dwItem = HashThread_GetCurrentItem_Core(cptagHashThread);
	return HashThread_GetFileRecode_Core(cptagHashThread, dwItem);
}

tagHashThread_FileRecode* HashThread_GetFileCurrentFileRecode(const HashThread* cptagHashThread)
{
	IF_UNLIKELY(cptagHashThread == NULL || cptagHashThread->tagMultiFile == NULL) {
		return FALSE;
	}
	return HashThread_GetFileCurrentFileRecode_Core(cptagHashThread);
}

tagHashThread_FileRecode* HashThread_GetFileCountFileRecode_Core(const HashThread* cptagHashThread)
{
	DWORD dwItem;
	dwItem = HashThread_GetCountItem_Core(cptagHashThread);
	return HashThread_GetFileRecode_Core(cptagHashThread, dwItem);
}

tagHashThread_FileRecode* HashThread_GetFileCountFileRecode(const HashThread* cptagHashThread)
{
	IF_UNLIKELY(cptagHashThread == NULL || cptagHashThread->tagMultiFile == NULL) {
		return FALSE;
	}
	return HashThread_GetFileCountFileRecode_Core(cptagHashThread);
}

DWORD HashThread_GetCurrentItem_Core(const HashThread* cptagHashThread)
{
	return cptagHashThread->tagMultiFile->dwFileCurrentCount;
}

DWORD HashThread_GetCurrentItem(const HashThread* cptagHashThread)
{
	IF_UNLIKELY(cptagHashThread == NULL || cptagHashThread->tagMultiFile == NULL) {
		return (DWORD)-1;
	}
	return HashThread_GetCurrentItem_Core(cptagHashThread);
}

DWORD HashThread_GetCountItem_Core(const HashThread* cptagHashThread)
{
	return cptagHashThread->tagMultiFile->dwFileCount;
}

DWORD HashThread_GetCountItem(const HashThread* cptagHashThread)
{
	IF_UNLIKELY(cptagHashThread == NULL || cptagHashThread->tagMultiFile == NULL) {
		return (DWORD)-1;
	}
	return HashThread_GetCountItem_Core(cptagHashThread);
}

BOOL HashThread_AddFileRecode(HashThread* ptagHashThread, const TCHAR* cpInFileName, WIN32_FIND_DATA* pfdFind, DWORD dwHashLine)
{
	TCHAR szBuf[MAX_PATH_SIZE] = _T("");
	TCHAR* pFileName = NULL;
	size_t nFileNameLength = 0;
	tagHashThread_FileRecode* ptagHashFile_Recode = NULL;
	DWORD dwFileCount = 0;
	DWORD dwMaxBufSize = 0;
	int nRet = TRUE;
#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	dwFileCount = HashThread_GetCountItem_Core(ptagHashThread);
	dwMaxBufSize = ptagHashThread->tagMultiFile->dwMaxBufSize;

	IF_UNLIKELY(dwFileCount > FILE_MAX_COUNTSIZE) {
		return FALSE;
	}

	IF_UNLIKELY((dwFileCount + 1) > dwMaxBufSize)
	{
		DWORD dwRet = HashThread_CreateBuffer(ptagHashThread, dwFileCount + 1);

		IF_UNLIKELY(dwRet == (DWORD)-1)
		{

#ifdef _DEBUG
			STPRINTF_FUNC(szDebugText, _T("HashThraed: %s の関数を終了します。\r\n"), 	_T("HashThread_AddFileRecode"));
			OutputDebugString(szDebugText);
#endif

			return FALSE;
		}
	}

	ptagHashFile_Recode = HashThread_GetFileRecode_Core(ptagHashThread, dwFileCount);

	if (cpInFileName != NULL) {
		pFileName = (TCHAR*)cpInFileName;
	}
	else
	{
		IF_UNLIKELY(pfdFind == NULL) {
			return FALSE;
		}

		if (_tfullpath(szBuf, pfdFind->cFileName, MAX_PATH_SIZE) != NULL) {
			*(szBuf + MAX_PATH_SIZE - 1) = '\0';
		}
		pFileName = szBuf;
	}

	nFileNameLength = _tcslen(pFileName);
	IF_UNLIKELY(nFileNameLength >= MAX_PATH_SIZE) {
		return FALSE;
	}

	qtcscpy(ptagHashFile_Recode->szFileName, pFileName);
	ptagHashFile_Recode->nFileNameLength = (DWORD)nFileNameLength;
	ptagHashFile_Recode->dwFileLastError = (DWORD)-1;
	ptagHashFile_Recode->dwFileHashType = ptagHashThread->dwHashType;
	ptagHashFile_Recode->dwHashFileID = (DWORD)-1;
	ptagHashFile_Recode->dwHashFileHashLine = dwHashLine;
	ptagHashFile_Recode->nHashFileCmp = (DWORD)-1;

	if (pfdFind != NULL)
	{
		ptagHashFile_Recode->llFileSize = pfdFind->nFileSizeLow;
		ptagHashFile_Recode->llFileSize += (__int64)pfdFind->nFileSizeHigh << 32;
		ptagHashFile_Recode->dwFileAttributes = pfdFind->dwFileAttributes;
		memcpy(&ptagHashFile_Recode->ftFileLastWriteTime, &pfdFind->ftLastWriteTime, sizeof(FILETIME));
		ptagHashThread->tagMultiFile->llFileAllSize += ptagHashFile_Recode->llFileSize;
	}
	else
	{
		ptagHashFile_Recode->llFileSize = 0;
		ptagHashFile_Recode->dwFileAttributes = 0;
		ptagHashFile_Recode->ftFileLastWriteTime.dwLowDateTime = 0;
		ptagHashFile_Recode->ftFileLastWriteTime.dwHighDateTime = 0;
		memset(&ptagHashFile_Recode->pFileHashByte, 0, sizeof(HASH_LEN));
	}

	ptagHashThread->tagMultiFile->llFileAllSizeBackup = ptagHashThread->tagMultiFile->llFileAllSize;
	ptagHashFile_Recode->nNextFile = ptagHashThread->tagMultiFile->dwFileCount + 1;

#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText, _T("HashThraed: HashThread_AddFileRecode(): %d番目のファイルが追加されました。\r\n"), ptagHashThread->tagMultiFile->dwFileCount);
	OutputDebugString(szDebugText);

	STPRINTF_FUNC(szDebugText, _T("%s\r\n"), ptagHashFile_Recode->szFileName);
	OutputDebugString(szDebugText);
#endif

	return nRet;
}

BOOL HashThread_ClearFileRecode(HashThread* ptagHashThread, BOOL nCurrentFolderClear)
{
	tagHashThread_FileRecode* ptagHashFile_Recode = NULL;

	ptagHashThread->tagMultiFile->dwFileCount = 0;
	ptagHashThread->tagMultiFile->dwFileCurrentCount = 0;
	if (nCurrentFolderClear) {
		*ptagHashThread->tagMultiFile->szCurrentFolder = '\0';
	}
	ptagHashThread->tagMultiFile->llFileAllSize = 0;
	ptagHashThread->tagMultiFile->llFileAllCurrentSize = 0;
	ptagHashThread->tagMultiFile->llFileAllSizeBackup = 0;

	ptagHashFile_Recode = ptagHashThread->tagMultiFile->ptagFileRecode;
	ptagHashFile_Recode->szFileName[0] = '\0';
	ptagHashFile_Recode->nFileNameLength = 0;
	ptagHashFile_Recode->llFileSize = 0;
	ptagHashFile_Recode->dwFileAttributes = (DWORD)-1;
	ptagHashFile_Recode->ftFileLastWriteTime.dwLowDateTime = 0;
	ptagHashFile_Recode->ftFileLastWriteTime.dwHighDateTime = 0;
	memset(ptagHashFile_Recode->pFileHashByte, NULL, HASH_LEN);
	ptagHashFile_Recode->dwFileLastError = (DWORD)-1;
	ptagHashFile_Recode->dwHashFileID = (DWORD)-1;
	ptagHashFile_Recode->dwHashFileHashLine = (DWORD)-1;
	ptagHashFile_Recode->nIsHashOldFile = FALSE;
	memset(ptagHashFile_Recode->nHashFileHashByte, NULL, HASH_LEN);
	ptagHashFile_Recode->nHashFileCmp = (DWORD)-1;

	HashThread_SetHashType(ptagHashThread, ptagHashThread->dwHashType);
	return TRUE;
}


BOOL HashThread_ReNameFileRecode(HashThread* ptagHashThread, DWORD dwItem, const TCHAR* cpInFileName)
{
	tagHashThread_FileRecode* ptagHashFile_Recode = NULL;
	TCHAR* pInFileName = NULL;
	TCHAR* pTmp = NULL;
	int nRet = 0;

	ptagHashFile_Recode = HashThread_GetFileRecode_Core(ptagHashThread, dwItem);

	pInFileName = TCharToCopyToPadding(ptagHashFile_Recode->szFileName, _MAX_FNAME);
	IF_UNLIKELY(pInFileName == NULL) {
		return FALSE;
	}
	pTmp = GetTCharToFileName(pInFileName);
	qtcscpy(pTmp, cpInFileName);

	IF_UNLIKELY(_tcsicmp(ptagHashFile_Recode->szFileName, pInFileName) == 0)
	{
		free(pInFileName);
		return FALSE;
	}

	nRet = MoveFile(ptagHashFile_Recode->szFileName, pInFileName);
	IF_UNLIKELY(nRet == FALSE)
	{
		free(pInFileName);
		return FALSE;
	}

	if (ptagHashFile_Recode->nIsHashOldFile)
	{
		const DWORD dwFileHashType = ptagHashFile_Recode->dwFileHashType;
		TCHAR* pFileName = TCharToCopy2ToPadding(ptagHashFile_Recode->szFileName, cpHashExt[dwFileHashType],  20);
		TCHAR* pHashFileName = TCharToCopy2ToPadding(pInFileName, cpHashExt[dwFileHashType], 20);

		IF_LIKELY(pFileName != NULL && pHashFileName != NULL)
		{
			nRet = MoveFile(pFileName, pHashFileName);
			IF_UNLIKELY(nRet == FALSE) {
				ptagHashFile_Recode->nIsHashOldFile = FALSE;
			}
		}
		free(pFileName);
		free(pHashFileName);
	}

	pTmp = qtcscpy(ptagHashFile_Recode->szFileName, pInFileName);
	ptagHashFile_Recode->nFileNameLength = pTmp - ptagHashFile_Recode->szFileName;
	free(pInFileName);
	return TRUE;
}


DWORD HashThread_MultiFile_Init(HashThread* ptagHashThread, const TCHAR* cpInFileName, BOOL IsAddFile)
{
	TCHAR szFile[MAX_PATH_SIZE];
	WIN32_FIND_DATA fdFindData = { 0 };
	HANDLE hFind = NULL;
	DWORD dwRet = 0;
	DWORD dwFileCount = 0;

	dwFileCount = ptagHashThread->tagMultiFile->dwFileCount;

	IF_UNLIKELY(dwFileCount >= FILE_MAX_COUNTSIZE)
	{
		ptagHashThread->nIsFileNoCheck = 1;
		return FILE_MAX_COUNTSIZE;
	}

	if (IsAddFile == 0 || dwFileCount == 0)
	{
		ptagHashThread->tagMultiFile->dwFileCount = 0;
		ptagHashThread->tagMultiFile->llFileAllSize = 0;
		ptagHashThread->tagMultiFile->llFileAllCurrentSize = 0;
	}

	if (ExpandEnvironmentStrings(cpInFileName, szFile, MAX_PATH_SIZE) == 0) {
		qtcscpy(szFile, cpInFileName);
	}

	IF_LIKELY(dwFileCount > 0)
	{
		const tagHashThread_FileRecode* cptagHashFile_Recode;

		dwRet = HashThread_GetCountItem_Core(ptagHashThread);
		dwRet--;
		cptagHashFile_Recode = HashThread_GetFileRecode_Core(ptagHashThread, dwRet);

		IF_UNLIKELY(cptagHashFile_Recode == NULL) {
			goto FILECHK_END;
		}

		IF_UNLIKELY(_tcscmp(szFile, cptagHashFile_Recode->szFileName) == 0 && cptagHashFile_Recode->nIsHashOldFile)
		{
			ptagHashThread->dwHashThreadLastError = (DWORD)-2;

#ifdef _DEBUG
			OutputDebugString(_T("HashThraed: HashThread_MultiFile_Init(): 前回登録したファイルと重複しています。\r\n"));
#endif
			return dwFileCount;
		}
	}

FILECHK_END:

	if (*ptagHashThread->tagMultiFile->szCurrentFolder == '\0')
	{
		qtcscpy(ptagHashThread->tagMultiFile->szCurrentFolder, szFile);
		GetTCharToFolderName(ptagHashThread->tagMultiFile->szCurrentFolder);//PathRemoveFileSpec
	}

	hFind = FindFirstFile(szFile, &fdFindData);
	IF_UNLIKELY(hFind == INVALID_HANDLE_VALUE) {
		return dwFileCount;
	}

	if (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		dwRet = HashThread_Folder_Init(ptagHashThread, szFile, IsAddFile, TRUE);
		FindClose(hFind);
		return dwRet;
	}

	dwRet = HashThread_AddFileRecode(ptagHashThread, cpInFileName, &fdFindData, HASHFILE_NOLINE);
	FindClose(hFind);

	IF_UNLIKELY(dwRet == (DWORD)-1) {
		return dwFileCount;
	}

	// MD5チェックツール (1x) 形式のHashファイルが存在したら読み込みます。
	HashThread_GetOldHashFile(ptagHashThread, szFile);

	ptagHashThread->tagMultiFile->dwFileCount++;
	dwFileCount++;

	if (dwFileCount > 0) {
		HashThread_GetFileRecode_Core(ptagHashThread, dwFileCount - 1)->nNextFile = (UINT)-1;
	}
	else {
		HashThread_GetFileRecode_Core(ptagHashThread, 0)->nNextFile = (UINT)-1;
	}
	ptagHashThread->nIsFileNoCheck = 1;
	ptagHashThread->dwHashThreadLastError = 0;

	return dwFileCount;
}

DWORD HashThread_Folder_Init(HashThread* ptagHashThread, const TCHAR* cpInFileName, BOOL nIsAddFile, BOOL nIsSubFolder)
{
	TCHAR szFolder[MAX_PATH_SIZE];
	TCHAR* pFolder = NULL;
	const TCHAR* pTemp = NULL;
	DWORD dwRet = 0;
	size_t nRet = 0;
#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	IF_UNLIKELY(ptagHashThread->tagMultiFile->dwFileCount >= FILE_MAX_COUNTSIZE)
	{
		ptagHashThread->nIsFileNoCheck = 1;
		return FILE_MAX_COUNTSIZE;
	}

	dwRet = ExpandEnvironmentStrings(cpInFileName, szFolder, MAX_PATH_SIZE);
	pFolder = dwRet ? (TCHAR*)cpInFileName : szFolder;
	if (*(pFolder + dwRet - 1) == '\\')
	{
		dwRet--;
		*(pFolder + dwRet) = _T('\0');
	}
	pTemp = qtcscpy(ptagHashThread->tagMultiFile->szCurrentFolder, pFolder);
	nRet = pTemp - ptagHashThread->tagMultiFile->szCurrentFolder;

#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText,
		_T("HashThraed: フォルダーの一覧を取得します。\r\n%s\r\n"), szFolder);
	OutputDebugString(szDebugText);
#endif

	SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FOLDER_INIT, 0);
	dwRet = HashThread_Folder_Open(ptagHashThread, pFolder, nRet, nIsAddFile, nIsSubFolder);
	SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_FOLDER_INIT_EXIT, dwRet);

	return dwRet;
}

DWORD HashThread_Folder_Open(HashThread* ptagHashThread, TCHAR* cpInFileName, size_t nInFileNameLen, BOOL nIsAddFile, BOOL nIsSubFolder)
{
	HANDLE hFind = NULL;
	WIN32_FIND_DATA* fdFindData;
	TCHAR* pExt = NULL;
	DWORD dwRet = 0;
	DWORD dwHashType = 0;
	DWORD dwHashLen = 0;
	DWORD dwFileCount = 0;
	const TCHAR* cpHashExt1;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	dwHashType = ptagHashThread->dwHashType;
	dwHashLen = HashThread_GetHashLen(dwHashType);
	dwFileCount = ptagHashThread->tagMultiFile->dwFileCount;
	cpHashExt1 = HashThread_GetHashExt(dwHashType);

	if (nIsAddFile == 0 || dwFileCount == 0)
	{
		HashThread_ClearFileRecode(ptagHashThread, FALSE);
		ptagHashThread->nIsFileNoCheck = 1;
	}

	fdFindData = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));
	IF_UNLIKELY(fdFindData == NULL)
	{

#ifdef _DEBUG
		OutputDebugString(_T("HashThraed: malloc() に失敗しました。\r\n"));
#endif

		goto FILE_EXIT;
	}

	SetCurrentDirectory(cpInFileName);
	qtcscpy(cpInFileName + nInFileNameLen, _T("\\*"));

	if (nIsSubFolder == 0) {
		goto FILE_FIND;
	}

	hFind = FindFirstFile(cpInFileName, fdFindData);
	IF_UNLIKELY(hFind == INVALID_HANDLE_VALUE) {
		goto FILE_FIND;
	}

	do
	{
		size_t nRet = 0;

		if (~fdFindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY || fdFindData->dwFileAttributes & ~ptagHashThread->dwFileAttributeMask || _tcscmp(fdFindData->cFileName, _T(".")) == 0 || _tcscmp(fdFindData->cFileName, _T("..")) == 0) {
			continue;
		}

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("HashThraed: %s\r\n→%s のサブフォルダーが見つかりました。\r\n"), cpInFileName, fdFindData->cFileName);
		OutputDebugString(szDebugText);
#endif

		nRet = qtcscpy(cpInFileName + nInFileNameLen + 1, fdFindData->cFileName) - cpInFileName;
		dwRet = HashThread_Folder_Open(ptagHashThread, cpInFileName, nRet, TRUE, TRUE);
		dwFileCount = ptagHashThread->tagMultiFile->dwFileCount;

		IF_UNLIKELY(dwRet == (DWORD)-1)
		{
			free(fdFindData);
			return (DWORD)-1;
		}
		SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_SUBFOLDER_OPEN, dwFileCount);
	} while (FindNextFile(hFind, fdFindData));
	FindClose(hFind);

	*(cpInFileName + nInFileNameLen) = '\0';
	SetCurrentDirectory(cpInFileName);
	qtcscpy(cpInFileName + nInFileNameLen, _T("\\*"));

FILE_FIND:

	// 各ファイル名とファイルのサイズ取得
	hFind = FindFirstFile(cpInFileName, fdFindData);
	IF_UNLIKELY(hFind == INVALID_HANDLE_VALUE) {
		goto FILE_EXIT;
	}

	do
	{
		if (fdFindData->dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | ~ptagHashThread->dwFileAttributeMask))
		{

#ifdef _DEBUG

			if (~fdFindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				static const TCHAR* cszFileAttibute[] = {
					_T("無"),			//00000000
					_T("読"),			//00000001
					_T("隠"),			//00000010
					_T("読隠"),			//00000011
					_T("シ"),			//00000100
					_T("読シ"),			//00000101
					_T("隠シ"),			//00000110
					_T("隠読シ"),		//00000111
					_T("無"),			//00001000
					_T("読"),			//00001001
					_T("隠"),			//00001010
					_T("読隠"),			//00001011
					_T("シ"),			//00001100
					_T("読シ"),			//00001101
					_T("隠シ"),			//00001110
					_T("隠読シ"),		//00001111
					_T("デ"),			//00010000
					_T("読デ"),			//00010001
					_T("隠デ"),			//00010010
					_T("読隠デ"),		//00010011
					_T("シデ"),			//00010100
					_T("読シデ"),		//00010101
					_T("隠シデ"),		//00010110
					_T("隠読シデ"),		//00010111
					_T("デ"),			//00011000
					_T("読デ"),			//00011001
					_T("隠デ"),			//00011010
					_T("読隠デ"),		//00011011
					_T("シデ"),			//00011100
					_T("読シデ"),		//00011101
					_T("隠シデ"),		//00011110
					_T("隠読シデ"),		//00011111
					_T("ア"),			//00100000
					_T("読ア"),			//00100001
					_T("隠ア"),			//00100010
					_T("読隠ア"),		//00100011
					_T("シア"),			//00100100
					_T("読シア"),		//00100101
					_T("隠シア"),		//00100110
					_T("隠読シア"),		//00100111
					_T("ア"),			//00101000
					_T("読ア"),			//00101001
					_T("隠ア"),			//00101010
					_T("読隠ア"),		//00101011
					_T("シア"),			//00101100
					_T("読シア"),		//00101101
					_T("隠シア"),		//00101110
					_T("隠読シア"),		//00101111
					_T("デア"),			//00110000
					_T("読デア"),		//00110001
					_T("隠デア"),		//00110010
					_T("読隠デア"),		//00110011
					_T("シデア"),		//00110100
					_T("読シデア"),		//00110101
					_T("隠シデア"),		//00110110
					_T("隠読シデア"),	//00110111
					_T("デア"),			//00111000
					_T("読デア"),		//00111001
					_T("隠デア"),		//00111010
					_T("読隠デア"),		//00111011
					_T("シデア"),		//00111100
					_T("読シデア"),		//00111101
					_T("隠シデア"),		//00111110
					_T("隠読シデア"),	//00111111
				};

				OutputDebugString(_T("HashThraed: HashThread_Folder_Open(): 以下のファイルは除外します。\r\n"));
				STPRINTF_FUNC(szDebugText,
					_T("HashThraed: HashThread_Folder_Open(): [%s]\t%s\\%s\r\n"),
					cszFileAttibute[fdFindData->dwFileAttributes & 0x3f],
					ptagHashThread->tagMultiFile->szCurrentFolder,
					fdFindData->cFileName);
				OutputDebugString(szDebugText);
			}

#endif
			continue;
		}

		qtcscpy(cpInFileName + nInFileNameLen + 1, fdFindData->cFileName);
		pExt = GetTCharToExtension2(fdFindData->cFileName, TRUE);

		if (pExt != NULL && !(_tcsicmp(pExt, cpHashExt1) == 0 && fdFindData->nFileSizeLow == dwHashLen * 2))
		{
			dwRet = HashThread_AddFileRecode(ptagHashThread, cpInFileName, fdFindData, HASHFILE_NOLINE);
			IF_UNLIKELY(dwRet == (DWORD)-1)
			{
				free(fdFindData);
				return dwFileCount;
			}

			if (_tcsicmp(pExt, cpHashExt1) == 0)
			{
				dwRet = HashThread_HashFile_Init(ptagHashThread, cpInFileName, TRUE, NULL);

				IF_UNLIKELY(dwRet == (DWORD)-1)
				{
					free(fdFindData);
					return (DWORD)-1;
				}
				else if (dwRet > 0)
				{
					ptagHashThread->tagMultiFile->dwFileCount--;
					dwFileCount--;
				}
			}
			else
			{
				// MD5チェックツール (1x) 形式のHashファイルが存在したら読み込みます。
				HashThread_GetOldHashFile(ptagHashThread, cpInFileName);
			}
			ptagHashThread->tagMultiFile->dwFileCount++;
			dwFileCount++;
		}
	} while (FindNextFile(hFind, fdFindData));
	FindClose(hFind);
	*(cpInFileName + nInFileNameLen) = '\0';

	if (dwFileCount > 0) {
		HashThread_GetFileRecode_Core(ptagHashThread, dwFileCount - 1)->nNextFile = (UINT)-1;
	}
	else {
		HashThread_GetFileRecode_Core(ptagHashThread, 0)->nNextFile = (UINT)-1;
	}
	ptagHashThread->nIsFileNoCheck = 1;
	ptagHashThread->dwHashThreadLastError = 0;

FILE_EXIT:
	free(fdFindData);
	return dwFileCount;
}

DWORD HashThread_GetHashFileType(HashThread* ptagHashThread, const TCHAR* cpInFileName)
{
	FILE* pInFilePointer = NULL;
	DWORD dwRet = 0;

	if (ptagHashThread->pInFilePointer != NULL) {
		pInFilePointer = ptagHashThread->pInFilePointer;
	}
	else if (cpInFileName != NULL)
	{
#ifndef _NODLL
		errno_t error;

		error = _tfopen_s(&pInFilePointer, cpInFileName, _T("rb"));
		IF_UNLIKELY(pInFilePointer == NULL || error)
#else
		IF_UNLIKELY((pInFilePointer = _tfopen(cpInFileName, _T("rb"))) == NULL)
#endif
		{
			return (DWORD)-1;
		}

		if (ptagHashThread->tagMultiFile->dwHashFileCharCode == HASHFILE_CHARCODE_UNICODE) {
			fseek(pInFilePointer, ptagHashThread->tagMultiFile->dwHashFileBom, SEEK_SET);
		}
	}
	else {
		return (DWORD)-1;
	}
	dwRet = HashThread_GetHashFileType(ptagHashThread, cpInFileName, pInFilePointer);

	if (ptagHashThread->pInFilePointer == NULL) {
		fclose(pInFilePointer);
	}
	return dwRet;
}

DWORD HashThread_GetHashFileType(HashThread* ptagHashThread, const TCHAR* cpInFileName, FILE* pInFilePointer)
{
	const TCHAR* pExt;
	char szExtBuffer[12] = { 0 };
	const char* cpExt = szExtBuffer;
	DWORD dwHashType = (DWORD)-1;

	if (cpInFileName == NULL) {
		goto FILETYPE_CHK;
	}

	pExt = GetTCharToExtension2(cpInFileName, TRUE);
	IF_UNLIKELY(pExt == NULL || _tcslen(pExt) > 11)
	{
		IF_UNLIKELY(ptagHashThread->pInFilePointer != NULL) {
			goto FILETYPE_CHK;
		}
		return dwHashType;
	}

	TCharToCharConv2(pExt, szExtBuffer);
	CharToLower(szExtBuffer);

	switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
	{
	case MAKE_CHAR_TO_INT_4CHAR('.', 'm', 'd', '2'):
		cpExt += 4;
		switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
		{
		case MAKE_CHAR_TO_INT_4CHAR('\0', '\0', '\0', '\0'):
			dwHashType = HASH_MD2_TYPE;
			break;
		}
		break;
	case MAKE_CHAR_TO_INT_4CHAR('.', 'm', 'd', '4'):
		cpExt += 4;
		switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
		{
		case MAKE_CHAR_TO_INT_4CHAR('\0', '\0', '\0', '\0'):
			dwHashType = HASH_MD4_TYPE;
			break;
		}
		break;
	case MAKE_CHAR_TO_INT_4CHAR('.', 'm', 'd', '5'):
		cpExt += 4;
		switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
		{
		case MAKE_CHAR_TO_INT_4CHAR('\0', '\0', '\0', '\0'):
			dwHashType = HASH_MD5_TYPE;
			break;
		}
		break;
	case MAKE_CHAR_TO_INT_4CHAR('.', 's', 'h', 'a'):
		cpExt += 4;
		switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
		{
		case MAKE_CHAR_TO_INT_4CHAR('1', '\0', '\0', '\0'):
			dwHashType = HASH_SHA1_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('2', '2', '4', '\0'):
			dwHashType = HASH_SHA_224_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('2', '5', '6', '\0'):
			dwHashType = HASH_SHA_256_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('2', '5', '6', '_'):
			cpExt += 4;
			switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
			{
			case MAKE_CHAR_TO_INT_4CHAR('1', '9', '2', '\0'):
				dwHashType = HASH_SHA_256_192_TYPE;
				break;
			}
			break;
		case MAKE_CHAR_TO_INT_4CHAR('3', '8', '4', '\0'):
			dwHashType = HASH_SHA_384_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('5', '1', '2', '\0'):
			dwHashType = HASH_SHA_512_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('5', '1', '2', '_'):
			cpExt += 4;
			switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
			{
			case MAKE_CHAR_TO_INT_4CHAR('2', '2', '4', '\0'):
				dwHashType = HASH_SHA_512_224_TYPE;
				break;
			case MAKE_CHAR_TO_INT_4CHAR('2', '5', '6', '\0'):
				dwHashType = HASH_SHA_512_256_TYPE;
				break;
			}
			break;
		case MAKE_CHAR_TO_INT_4CHAR('3', '_', '2', '2'):
			cpExt += 4;
			switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
			{
			case MAKE_CHAR_TO_INT_4CHAR('4', '\0', '\0', '\0'):
				dwHashType = HASH_SHA3_224_TYPE;
				break;
			}
			break;
		case MAKE_CHAR_TO_INT_4CHAR('3', '_', '2', '5'):
			cpExt += 4;
			switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
			{
			case MAKE_CHAR_TO_INT_4CHAR('6', '\0', '\0', '\0'):
				dwHashType = HASH_SHA3_256_TYPE;
				break;
			}
			break;
		case MAKE_CHAR_TO_INT_4CHAR('3', '_', '3', '8'):
			cpExt += 4;
			switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
			{
			case MAKE_CHAR_TO_INT_4CHAR('4', '\0', '\0', '\0'):
				dwHashType = HASH_SHA3_384_TYPE;
				break;
			}
			break;
		case MAKE_CHAR_TO_INT_4CHAR('3', '_', '5', '1'):
			cpExt += 4;
			switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
			{
			case MAKE_CHAR_TO_INT_4CHAR('2', '\0', '\0', '\0'):
				dwHashType = HASH_SHA3_512_TYPE;
				break;
			}
			break;
		}
		break;
	case MAKE_CHAR_TO_INT_4CHAR('.', 'c', 'r', 'c'):
		cpExt += 4;
		switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
		{
		case MAKE_CHAR_TO_INT_4CHAR('3', '2', '\0', '\0'):
			dwHashType = HASH_CRC32_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('3', '2', 'c', '\0'):
			dwHashType = HASH_CRC32C_TYPE;
			break;
		}
		break;
	case MAKE_CHAR_TO_INT_4CHAR('.', 'x', 'x', 'h'):
		cpExt += 4;
		switch (*(CHAR_TO_4CHAR_TYPE*)cpExt)
		{
		case MAKE_CHAR_TO_INT_4CHAR('3', '2', '\0', '\0'):
			dwHashType = HASH_XXH32_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('6', '4', '\0', '\0'):
			dwHashType = HASH_XXH64_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('3', '\0', '\0', '\0'):
			dwHashType = HASH_XXH3_TYPE;
			break;
		case MAKE_CHAR_TO_INT_4CHAR('1', '2', '8', '\0'):
			dwHashType = HASH_XXH128_TYPE;
			break;
		}
		break;
	case MAKE_CHAR_TO_INT_4CHAR('.', 't', 'x', 't'):
	FILETYPE_CHK:
		if (ptagHashThread != NULL && ptagHashThread->tagMultiFile != NULL && dwHashType == -1)
		{
			while (fgets((char*)byBuf, sizeof(byBuf), pInFilePointer) != NULL)
			{
				size_t nRet = 0;
				size_t nBufLen = 0;

				tagHashThread_FileRecode* ptagHashFile_Recode = HashThread_GetFileCountFileRecode_Core(ptagHashThread);

				if (ptagHashThread->tagMultiFile->dwHashFileCharCode == HASHFILE_CHARCODE_UNICODE)
				{
					nBufLen = wcslen((WCHAR*)byBuf);
					nRet = WCharToHashFileLine((WCHAR*)byBuf, nBufLen, NULL, ptagHashFile_Recode->nHashFileHashByte, 64);
				}
				else
				{
					nBufLen = strlen((char*)byBuf);
					nRet = CharToHashFileLine((char*)byBuf, nBufLen, NULL, ptagHashFile_Recode->nHashFileHashByte, 64);
				}

				if (nRet == 0) {
					break;
				}

				dwHashType = HashThread_GetHashLenToType((DWORD)nRet);
				break;
			}
		}
		break;
	}
	return dwHashType;
}

DWORD HashThread_HashFile_Init(HashThread* ptagHashThread, const TCHAR* cpInFileName, BOOL nIsAddFile, const TCHAR* cpInOption)
{
	TCHAR* pBuf = NULL;
	TCHAR* pFile = NULL;
	BYTE* pbyBuf = NULL;
	TCHAR* pPathBuf = NULL;
	const TCHAR* pOpenOption = _T("rb");
	DWORD dwHashType = 0;
	const TCHAR* cpHashExt1;
	BOOL  nIsAddMode = FALSE;
	DWORD dwCharCode = 0;
	DWORD dwLineCount = 0;
	DWORD dwRet = 0;
	DWORD dwFileCount = 0;
	DWORD dwHashLen = 0;
	TCHAR szNoOption[] = _T("");

#ifndef _NODLL
	errno_t error;
#endif
#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	dwHashType = ptagHashThread->dwHashType;
	cpHashExt1 = HashThread_GetHashExt(dwHashType);
	dwFileCount = ptagHashThread->tagMultiFile->dwFileCount;
	dwHashLen = ptagHashThread->dwHashLen;

	IF_UNLIKELY(dwFileCount >= FILE_MAX_COUNTSIZE)
	{
		ptagHashThread->nIsFileNoCheck = 1;
		return FILE_MAX_COUNTSIZE;
	}

	// オプションの解析
	if (cpInOption == NULL)
	{
		cpInOption = szNoOption;
		goto OPTION_SKIP;
	}

	while (*cpInOption != NULL)
	{
		switch (*cpInOption)
		{
		case 'a':
			nIsAddMode = TRUE;
#ifdef _DEBUG
			OutputDebugString(_T("HashThread: MD5File_Init(): 追記モード(計算済み)としてオープンします。\r\n"));
#endif
			break;
		}
		cpInOption++;
	}
OPTION_SKIP:

	pBuf = (TCHAR*)malloc(MAX_PATH_SIZE * 21 * sizeof(TCHAR));
	IF_UNLIKELY(pBuf == NULL)
	{
		SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_HASHFILE_INIT_EXIT, -1);
		return dwFileCount;
	}

	*pBuf = '\0';
	pFile = MAX_PATH_SIZE * 9 + pBuf;
	pbyBuf = MAX_PATH_SIZE * 3 + (BYTE*)pFile;
	*pbyBuf = 0;
	pPathBuf = MAX_PATH_SIZE * 3 + (TCHAR*)pbyBuf;

	TCharToEnvString2(cpInFileName, pFile, MAX_PATH_SIZE);
	dwRet = HashThread_FileNameCheck(pFile);
	IF_UNLIKELY(!dwRet)
	{
		free(pBuf);
		return dwFileCount;
	}

#ifndef _NODLL
	error = _tfopen_s(&(ptagHashThread->pInFilePointer), pFile, pOpenOption);

	IF_UNLIKELY(ptagHashThread->pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((ptagHashThread->pInFilePointer = _tfopen(pFile, pOpenOption)) == NULL)
#endif
	{
		free(pBuf);
		SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_HASHFILE_INIT_EXIT, -1);
		return dwFileCount;
	}


	dwCharCode = HashThread_GetCharCode(ptagHashThread);
	IF_UNLIKELY(dwCharCode == (DWORD)-1)
	{
		free(pBuf);
		return dwFileCount;
	}
	ptagHashThread->nIsFileNoCheck = 1;

	if (nIsAddFile == 0 || dwFileCount == 0)
	{
		ptagHashThread->tagMultiFile->dwFileCount = 0;
		ptagHashThread->tagMultiFile->llFileAllSize = 0;
		ptagHashThread->tagMultiFile->llFileAllCurrentSize = 0;
	}

	if (dwFileCount == 0)
	{
		dwRet = HashThread_GetHashFileType(ptagHashThread, pFile);
		if (dwRet != (DWORD)-1)
		{
			HashThread_SetHashType(ptagHashThread, dwRet);
			dwHashType = dwRet;
			cpHashExt1 = HashThread_GetHashExt(dwHashType);
			dwFileCount = ptagHashThread->tagMultiFile->dwFileCount;
			dwHashLen = ptagHashThread->dwHashLen;
		}
		fseek(ptagHashThread->pInFilePointer, ptagHashThread->tagMultiFile->dwHashFileBom, SEEK_SET);
	}

	const DWORD dwHashFileType = ptagHashThread->dwHashType;
	SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_HASHFILE_INIT, dwFileCount);

	qtcscpy(ptagHashThread->tagMultiFile->szCurrentFolder, pFile);
	GetTCharToFolderName(ptagHashThread->tagMultiFile->szCurrentFolder);//PathRemoveFileSpec
	SetCurrentDirectory(ptagHashThread->tagMultiFile->szCurrentFolder);
	dwLineCount = dwFileCount;

	// 1行ずつ解析していく。
	while (fgets((char*)pbyBuf, MAX_PATH_SIZE * 2, ptagHashThread->pInFilePointer) != NULL)
	{
		WIN32_FIND_DATA fdFindData = { 0 };
		HANDLE hFind = NULL;
		TCHAR* pFilePath = NULL;
		tagHashThread_FileRecode* ptagHashFile_Recode;
		size_t nRet = 0;
		size_t nBufLen = 0;

		dwLineCount++;

		IF_UNLIKELY(dwFileCount > FILE_MAX_COUNTSIZE)
		{
			free(pBuf);
			return FALSE;
		}

		if ((dwFileCount + 1) > ptagHashThread->tagMultiFile->dwMaxBufSize)
		{
			dwRet = HashThread_CreateBuffer(ptagHashThread, dwFileCount + 1);

			IF_UNLIKELY(dwRet == (DWORD)-1)
			{

#ifdef _DEBUG
				STPRINTF_FUNC(szDebugText, _T("HashThraed: %s の関数を終了します。\r\n"), _T("HashThread_HashFile_Init"));
				OutputDebugString(szDebugText);
#endif

				free(pBuf);
				return FALSE;
			}
		}

		ptagHashFile_Recode = HashThread_GetFileCountFileRecode_Core(ptagHashThread);
		IF_UNLIKELY(ptagHashFile_Recode == NULL) {
			continue;
		}

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("HashThraed: HashThraed_HashFile_Init(): %d行目の処理開始します。\r\n"), dwLineCount);
		OutputDebugString(szDebugText);
#endif

		switch (dwCharCode)
		{
		case HASHFILE_CHARCODE_UNICODE:
			pBuf = WCharToTCharConv2((WCHAR*)pbyBuf, pBuf);
			break;
		case HASHFILE_CHARCODE_UTF8:
			pBuf = Utf8CharToTCharConv2((char*)pbyBuf, pBuf);
			break;
		default:
			pBuf = CharToTCharConv2((char*)pbyBuf, pBuf);
		}
		nBufLen = _tcslen(pBuf);
		*(pBuf + ((MAX_PATH_SIZE * 2) + 1)) = '\0';

		nBufLen = TCharToDeleteReturnCode(pBuf, nBufLen);
		IF_UNLIKELY(nBufLen < (size_t)4 * 2) {
			continue;
		}

		ptagHashFile_Recode->llFileSize = 0;
		ptagHashFile_Recode->dwHashFileHashLine = HASHFILE_NOLINE;

		if (nBufLen / 2 == dwHashLen) {
			ptagHashFile_Recode->nIsHashOldFile = TRUE;
		}
		else {
			ptagHashFile_Recode->nIsHashOldFile = FALSE;
		}

		nRet = TCharToHashFileLine(pBuf, nBufLen, pPathBuf, ptagHashFile_Recode->nHashFileHashByte, dwHashLen);
		if (nRet == 0) {
			continue;
		}

		if (nRet != dwHashLen)
		{
			dwHashType = HashThread_GetHashLenToType((DWORD)nRet);
			if (dwFileCount == 0)
			{
				HashThread_SetHashType(ptagHashThread, dwHashType);
				cpHashExt1 = HashThread_GetHashExt(dwHashType);
				dwHashLen = ptagHashThread->dwHashLen;
			}
		}
		ptagHashFile_Recode->dwHashFileHashType = dwHashType;

		if (*pPathBuf != '\0')
		{
			dwRet = HashThread_FileNameCheck(pPathBuf);
			if (!dwRet)
			{
				memset(ptagHashFile_Recode->nHashFileHashByte, 0, HASH_LEN);
				continue;
			}
			TCharToEnvString2(pPathBuf, pBuf, MAX_PATH_SIZE);

			if ((_istalpha(*pBuf) && *(pBuf + 1) == ':' && *(pBuf + 2) == '\\') || TCHAR_COMP_TO_INT_2CHAR(pBuf, '\\', '\\')) {
				;// 絶対パスであれば何もしない
			}
			else
			{
				qtcscpy(pPathBuf, pBuf);
				if (_tfullpath(pBuf, pPathBuf, MAX_PATH_SIZE) == NULL) {
					continue;
				}
			}
		}
		else
		{
			TCHAR* pExt;

			qtcscpy(pBuf, pFile);
			pExt = GetTCharToExtension2(pBuf, TRUE);
			if (pExt != NULL && _tcsicmp(pExt, HashThread_GetHashExt(dwHashType)) == 0) {
				*pExt = '\0';
			}
		}
		pFilePath = pBuf;

		if (dwFileCount > 0)
		{
			const tagHashThread_FileRecode* cptagHashFile_Recode;

			dwRet = HashThread_GetCountItem_Core(ptagHashThread);
			dwRet--;
			cptagHashFile_Recode = HashThread_GetFileRecode_Core(ptagHashThread, dwRet);
			IF_UNLIKELY(cptagHashFile_Recode == NULL) {
				goto FILECHK_END;
			}

			IF_UNLIKELY(_tcscmp(pFilePath, cptagHashFile_Recode->szFileName) == 0 && cptagHashFile_Recode->nIsHashOldFile)
			{
				free(pBuf);
				fclose(ptagHashThread->pInFilePointer);
				ptagHashThread->dwHashThreadLastError = (DWORD)-2;

#ifdef _DEBUG
				OutputDebugString(_T("HashThraed: HashThread_MultiFile_Init(): 前回登録したファイルと重複しています。\r\n"));
#endif

				return dwFileCount;
			}
		}
FILECHK_END:


		hFind = FindFirstFile(pBuf, &fdFindData);
		IF_LIKELY(hFind != INVALID_HANDLE_VALUE)
		{
			dwRet = HashThread_AddFileRecode(ptagHashThread, pFilePath, &fdFindData, dwHashFileType == dwHashType ? dwLineCount : -1);
			FindClose(hFind);
		}
		else {
			dwRet = HashThread_AddFileRecode(ptagHashThread, pFilePath, NULL, dwHashFileType == dwHashType ? dwLineCount : -1);
		}

		IF_UNLIKELY(dwRet == (DWORD)-1)
		{
			free(pBuf);
			fclose(ptagHashThread->pInFilePointer);
			ptagHashThread->pInFilePointer = NULL;
			return dwFileCount;
		}

		ptagHashFile_Recode = HashThread_GetFileCountFileRecode_Core(ptagHashThread);

		if (nIsAddMode)
		{
			memcpy(ptagHashFile_Recode->pFileHashByte, ptagHashFile_Recode->nHashFileHashByte, dwHashLen);
			ptagHashFile_Recode->dwFileLastError = 0;
			ptagHashFile_Recode->nHashFileCmp = 0;
		}

		ptagHashFile_Recode->dwHashFileHashLine = dwLineCount;
		/*
		dwRet = HashThread_GetHashFileType(ptagHashThread, ptagHashFile_Recode->szFileName);
		if (dwRet != (DWORD)-1)
		{
			if (_tcsicmp(cpInFileName, ptagHashFile_Recode->szFileName) != 0)
			{
				FILE* pInFilePointer = ptagHashThread->pInFilePointer;

				dwRet = HashThread_HashFile_Init(ptagHashThread, ptagHashFile_Recode->szFileName, TRUE, cpInOption);
				ptagHashThread->pInFilePointer = pInFilePointer;
				if (dwRet == (DWORD)-1)
				{
					free(pBuf);
					fclose(ptagHashThread->pInFilePointer);
					ptagHashThread->pInFilePointer = NULL;
					return (DWORD)-1;
				}
			}
			else {
				dwLineCount--;
			}
			ptagHashThread->tagMultiFile->dwFileCount--;
			dwFileCount--;
		}
		*/
		dwFileCount++;
		ptagHashThread->tagMultiFile->dwFileCount++;
	}
	free(pBuf);
	fclose(ptagHashThread->pInFilePointer);
	ptagHashThread->pInFilePointer = NULL;

	if (nIsAddMode != FALSE) {
		ptagHashThread->tagMultiFile->dwFileCurrentCount = dwFileCount - 1;
	}

	if (ptagHashThread->tagMultiFile->dwFileCount > 0) {
		(ptagHashThread->tagMultiFile->ptagFileRecode + dwFileCount - 1)->nNextFile = (UINT)-1;
	}
	else {
		ptagHashThread->tagMultiFile->ptagFileRecode->nNextFile = (UINT)-1;
	}
	SendMessage(ptagHashThread->hWnd, ptagHashThread->nMessageID, APP_MESSAGE_HASHFILE_INIT_EXIT, dwFileCount);
	ptagHashThread->dwHashThreadLastError = 0;
	return dwFileCount;
}

DWORD HashThread_GetOldHashFile(const HashThread* cptagHashThread, const TCHAR* cpInFileName)
{
	WIN32_FIND_DATA fdFindData = { 0 };
	HANDLE hFind = NULL;
	TCHAR* pTemp = NULL;
	DWORD dwRet = 0;
	DWORD dwHashLen = 0;
	size_t nFileNameLength = 0;
	const TCHAR* cpHashExt1;
	tagHashThread_FileRecode* ptagHashFile_Recode;
	FILE* pInFilePointer = NULL;
#ifndef _NODLL
	errno_t error;
#endif
#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	dwRet = HashThread_GetCountItem_Core(cptagHashThread);
	ptagHashFile_Recode = HashThread_GetFileRecode_Core(cptagHashThread, dwRet);

	IF_UNLIKELY(ptagHashFile_Recode == NULL) {
		return (DWORD)-1;
	}

	dwHashLen = cptagHashThread->dwHashLen;
	cpHashExt1 = cptagHashThread->cpHashExt;
	nFileNameLength = ptagHashFile_Recode->nFileNameLength;

	IF_UNLIKELY((nFileNameLength + cptagHashThread->nHashExtLen + 1) >= MAX_PATH_SIZE) {
		return FALSE;
	}

	pTemp = (TCHAR*)cpInFileName + nFileNameLength;
	qtcscpy(pTemp, cpHashExt1);

	hFind = FindFirstFile(cpInFileName, &fdFindData);
	IF_UNLIKELY(hFind == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	FindClose(hFind);

	IF_UNLIKELY(fdFindData.nFileSizeLow < dwHashLen * 2)
	{
		*pTemp = '\0';
		return FALSE;
	}

#ifdef _DEBUG
	OutputDebugString(_T("HashThraed: HashThraed_GetOldHashFile(): MD5チェックツール (1x) 形式の HASHファイルが見つかりました。\r\n"));
	STPRINTF_FUNC(szDebugText, _T("%s\r\n"), cpInFileName);
	OutputDebugString(szDebugText);
#endif

#ifndef _NODLL
	error = _tfopen_s(&pInFilePointer, cpInFileName, _T("r"));

	IF_UNLIKELY(pInFilePointer == NULL || error)
#else
	IF_UNLIKELY((pInFilePointer = _tfopen(cpInFileName, _T("r"))) == NULL)
#endif
	{
#ifdef _DEBUG
		OutputDebugString(_T("HashThraed: HashThraed_GetOldHashFile(): _tfopen() に失敗しました。\r\n"));
#endif
		return FALSE;
	}
	else
	{
		TCHAR szHash[HASH_LOADSTRING];
		size_t nHashLen = 0;

		IF_UNLIKELY(_fgetts(szHash, HASH_LOADSTRING, pInFilePointer) == NULL)
		{
			fclose(pInFilePointer);
			return FALSE;
		}

		nHashLen = _tcslen(szHash);

		IF_UNLIKELY(nHashLen < (size_t)dwHashLen * 2)
		{
			fclose(pInFilePointer);
			return FALSE;
		}

		if (HashThread_HashToHash(ptagHashFile_Recode->nHashFileHashByte, szHash, dwHashLen))
		{
			ptagHashFile_Recode->nIsHashOldFile = TRUE;
			ptagHashFile_Recode->dwHashFileHashLine = 1;
			ptagHashFile_Recode->dwHashFileHashType = cptagHashThread->dwHashType;
			dwRet = TRUE;
#ifdef _DEBUG
			OutputDebugString(_T("HashThraed: HashThraed_GetOldHashFile(): HASH の読み込みが完了しました。\r\n"));
#endif
		}
		fclose(pInFilePointer);
	}
	*pTemp = '\0';
	return dwRet;
}

DWORD HashThread_GetCharCode(const HashThread* cptagHashThread)
{
	DWORD dwCharCode = 0;
	DWORD dwHitCharCode[3] = { 1, 0, 0 };
	size_t dwSize = 0;
	tagHashThread_MultiFile* ptagHashFile_MultiFile;


	ptagHashFile_MultiFile = cptagHashThread->tagMultiFile;
	dwCharCode = HASHFILE_CHARCODE_ANSI;

	dwSize = fread(byBuf, 1, sizeof(byBuf), cptagHashThread->pInFilePointer);
	byBuf[dwSize - 1] = '\0';


	// 文字コードの検出
	const BYTE byUncodeBom[] = { 0xFF, 0xFE };
	const BYTE byUtf8Bom[] = { 0xEF, 0xBB, 0xBF };

	if (memcmp(byBuf, byUncodeBom, sizeof(byUncodeBom)) == 0)
	{
		// UTF-16 Little Endian
		dwHitCharCode[1] += 2;
		ptagHashFile_MultiFile->dwHashFileBom = 2;
	}
	else if (memcmp(byBuf, byUtf8Bom, sizeof(byUtf8Bom)) == 0)
	{
		// UTF-8 BOM
		dwHitCharCode[2] += 2;
		ptagHashFile_MultiFile->dwHashFileBom = 3;
	}
	else
	{
		unsigned char* b = byBuf;
		ptagHashFile_MultiFile->dwHashFileBom = 0;
		size_t ui = 0;

		while (ui < dwSize)
		{
			if (IS_UTF8_3(*b))
			{
				if (IS_UTF8_TAIL(*(b + 1)) && IS_UTF8_TAIL(*(b + 2))) {
					dwHitCharCode[2]++;// UTF-8 3byte
				}
				ui += 3;
				b += 3;
			}
			else if (IS_UTF8_2(*b))
			{
				if (IS_UTF8_TAIL(*(b + 1))) {
					dwHitCharCode[2]++;// UTF-8 2byte
				}
				ui += 2;
				b += 2;
			}
			else if (_ismbblead(*b))
			{
				dwHitCharCode[0]++;// Shift-JIS
				ui += 2;
				b += 2;
			}
			else
			{
				ui++;
				b++;
			}
		}
	}
	fseek(cptagHashThread->pInFilePointer, cptagHashThread->tagMultiFile->dwHashFileBom, SEEK_SET);

	if (dwHitCharCode[1] >= max(dwHitCharCode[0], dwHitCharCode[2])) {
		dwCharCode = HASHFILE_CHARCODE_UNICODE;
	}

	if (dwHitCharCode[2] >= max(dwHitCharCode[0], dwHitCharCode[1])) {
		dwCharCode = HASHFILE_CHARCODE_UTF8;
	}
	ptagHashFile_MultiFile->dwHashFileCharCode = dwCharCode;

	return dwCharCode;
}

DWORD HashThread_CreateBuffer(HashThread* ptagHashThread, DWORD dwFileCount)
{
	tagHashThread_MultiFile* ptagHashFile_MultiFile = NULL;
	DWORD ___dwBufLen = 0;
	DWORD ___dwBufSize = 0;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
	const TCHAR* pMalloc = _T("");
#endif

	IF_UNLIKELY(ptagHashThread->tagMultiFile == NULL)
	{
		ptagHashThread->tagMultiFile = HashThread_CreateMultiFileBuffer(dwFileCount);
		IF_UNLIKELY(ptagHashThread->tagMultiFile == NULL)
		{
#ifdef _DEBUG
			pMalloc = _T("HashThread_CreateMultiFileBuffer()");
#endif
			goto MALLOC_FAIL;
		}

		ptagHashFile_MultiFile = ptagHashThread->tagMultiFile;
	}
	ptagHashFile_MultiFile = ptagHashThread->tagMultiFile;
	___dwBufSize = ptagHashFile_MultiFile->dwMaxBufSize;
	___dwBufLen = ptagHashFile_MultiFile->dwFileCount;

	IF_UNLIKELY(___dwBufSize > FILE_MAX_COUNTSIZE)
	{
#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("HashThraed: HashThraed_CreateBuffer(): FILE_MAX_COUNTSIZE (%d) の上限が超えてます。\r\n"), FILE_MAX_COUNTSIZE);
		OutputDebugString(szDebugText);
#endif
		return (DWORD)-1;
	}

	IF_UNLIKELY(dwFileCount > ptagHashFile_MultiFile->dwMaxBufSize)
	{
		tagHashThread_FileRecode* ___OLD_ptagHashFile_Recode = NULL;
		tagHashThread_FileRecode* ___NEW_ptagHashFile_Recode = NULL;

		IF_UNLIKELY(___dwBufSize == 0)
		{
			___dwBufSize = FILE_DEF_COUNTSIZE;
			ptagHashFile_MultiFile->ptagFileRecode;
		}

		// 大きめにバッファのサイズを計算する
		while (___dwBufSize < dwFileCount)
		{
			if (___dwBufSize < 4096) {
				___dwBufSize *= FILE_DEF_COUNTSIZE;// dwBufSize = dwBufSize << 4 同等
			}
			else {
				___dwBufSize *= 2;
			}
		}

		___dwBufLen = ptagHashFile_MultiFile->dwFileCount;
		___OLD_ptagHashFile_Recode = ptagHashFile_MultiFile->ptagFileRecode;
		___NEW_ptagHashFile_Recode = (tagHashThread_FileRecode*)realloc(___OLD_ptagHashFile_Recode, ___dwBufSize * sizeof(tagHashThread_FileRecode));

		IF_UNLIKELY(___NEW_ptagHashFile_Recode == NULL)
		{
#ifdef _DEBUG
			pMalloc = _T("HashThread_CreateFileRecodeBuffer()");
#endif
			goto MALLOC_FAIL;
		}
		ptagHashFile_MultiFile->ptagFileRecode = ___NEW_ptagHashFile_Recode;
		ptagHashFile_MultiFile->dwMaxBufSize = ___dwBufSize;
	}
	return ___dwBufSize;

MALLOC_FAIL:
#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText, _T("HashThraed: HashThraed_CreateBuffer(): バッファ確保に失敗しました。(%s)\r\n"), pMalloc);
	OutputDebugString(szDebugText);
#endif
	return (DWORD)-1;
}

tagHashThread_MultiFile* HashThread_CreateMultiFileBuffer(DWORD dwBufSize)
{
	tagHashThread_MultiFile* ___ptagHashFile_MultiFile = NULL;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
	const TCHAR* pText = _T("");
#endif

	// メモリー確保
	___ptagHashFile_MultiFile = (tagHashThread_MultiFile*)malloc(sizeof(tagHashThread_MultiFile));
	IF_UNLIKELY(___ptagHashFile_MultiFile == NULL)
	{
#ifdef _DEBUG
		pText = _T("___ptagHashFile_MultiFile");
#endif
		goto MALLOC_FAIL;
	}

	memset(___ptagHashFile_MultiFile, 0, sizeof(tagHashThread_MultiFile));
	___ptagHashFile_MultiFile->dwStructSize = sizeof(tagHashThread_MultiFile);
	___ptagHashFile_MultiFile->dwMaxBufSize = dwBufSize;
	___ptagHashFile_MultiFile->ptagFileRecode = HashThread_CreateFileRecodeBuffer(dwBufSize);
	return ___ptagHashFile_MultiFile;

MALLOC_FAIL:
	free(___ptagHashFile_MultiFile);

#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText, _T("HashThraed: HashThraed_CreateMultiFileBuffer(): バッファ確保に失敗しました。(%s)\r\n"), pText);
	OutputDebugString(szDebugText);
#endif

	return NULL;
}

tagHashThread_FileRecode* HashThread_CreateFileRecodeBuffer(DWORD dwBufSize)
{

	tagHashThread_FileRecode* ___ptagHashFile_Recode = NULL;
	const size_t ___BufSize = dwBufSize * sizeof(tagHashThread_FileRecode);

#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	___ptagHashFile_Recode = (tagHashThread_FileRecode*)malloc(___BufSize);
	IF_UNLIKELY(___ptagHashFile_Recode == NULL) {
		return NULL;
	}

#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText, _T("HashThraed: HashThraed_CreateFileRecodeBuffer(): バッファが %u個確保できました。(0x%08p, %lluByte)\r\n"), dwBufSize, &___ptagHashFile_Recode, (unsigned __int64)___BufSize);
	OutputDebugString(szDebugText);
#endif

	return ___ptagHashFile_Recode;
}

BOOL HashThread_ClearBuffer(HashThread* ptagHashThread)
{
	tagHashThread_MultiFile* ptagHashFile_MultiFile = ptagHashThread->tagMultiFile;
	BOOL nRet = HashThread_ClearBuffer_Core(ptagHashThread, FALSE);

	ptagHashFile_MultiFile->dwFileCurrentCount = 0;
	ptagHashFile_MultiFile->dwFileCount = 0;
	ptagHashFile_MultiFile->dwMaxBufSize = 0;

#ifdef _DEBUG
	OutputDebugString(_T("HashThread: HashThread_ClearBuffer(): バッファのクリアしました。\r\n"));
#endif
	return nRet;
}

BOOL HashThread_ClearBuffer_Core(HashThread* ptagHashThread, BOOL nIsMultiFileClear)
{
	IF_LIKELY(ptagHashThread->tagMultiFile)
	{
		tagHashThread_MultiFile* ptagHashFile_MultiFile = ptagHashThread->tagMultiFile;

		if (ptagHashFile_MultiFile->ptagFileRecode)
		{
			free(ptagHashFile_MultiFile->ptagFileRecode);
			ptagHashFile_MultiFile->ptagFileRecode = NULL;
		}

		if (nIsMultiFileClear != FALSE)
		{
			free(ptagHashFile_MultiFile);
			ptagHashFile_MultiFile = NULL;
		}
	}

#ifdef _DEBUG
	OutputDebugString(_T("HashThread: HashThread_ClearBuffer_Core(): バッファのクリアしました。\r\n"));
#endif

	return TRUE;
}

BOOL HashThread_DeleteItemBuffer(HashThread* ptagHashThread, DWORD dwItem)
{
	tagHashThread_MultiFile* ptagHashFile_MultiFile = NULL;
	unsigned __int64 ___llFileAllSize = 0;
	DWORD ___dwBufLen = 0;

#ifdef _DEBUG
	DWORD dwTime1;
	DWORD dwTime2;
	TCHAR szDebugText[1024];
#endif

	ptagHashFile_MultiFile = ptagHashThread->tagMultiFile;

	IF_UNLIKELY(dwItem > ptagHashFile_MultiFile->dwFileCount)
	{
		return FALSE;
	}

	if (ptagHashFile_MultiFile->dwFileCount == 1)
	{
		ptagHashFile_MultiFile->dwFileCurrentCount = 0;
		ptagHashFile_MultiFile->dwFileCount = 0;
		ptagHashFile_MultiFile->llFileAllSize = 0;
		ptagHashFile_MultiFile->llFileAllCurrentSize = 0;
		return FALSE;
	}

	___dwBufLen = ptagHashFile_MultiFile->dwFileCount - dwItem;
	___llFileAllSize = (ptagHashFile_MultiFile->ptagFileRecode + dwItem)->llFileSize;

#ifdef _DEBUG
	dwTime1 = timeGetTime();
#endif

	memmove(ptagHashFile_MultiFile->ptagFileRecode + dwItem, ptagHashFile_MultiFile->ptagFileRecode + dwItem + 1, ___dwBufLen * sizeof(tagHashThread_FileRecode));


#ifdef _DEBUG
	dwTime2 = timeGetTime();
	STPRINTF_FUNC(szDebugText, _T("HashThraed: HashThraed_DeleteItemBuffer(): バッファの項目を 1個削除しました。項目(%d)メモリームーブ(%d個) %dミリ秒\r\n"), dwItem, ___dwBufLen, dwTime2 - dwTime1);
	OutputDebugString(szDebugText);
#endif

	ptagHashFile_MultiFile->llFileAllSize -= ___llFileAllSize;
	IF_UNLIKELY(ptagHashFile_MultiFile->llFileAllSize < 0) {
		ptagHashFile_MultiFile->llFileAllSize = 0;
	}
	ptagHashFile_MultiFile->llFileAllSizeBackup = ptagHashFile_MultiFile->llFileAllSize;
	ptagHashFile_MultiFile->dwFileCount--;
	return TRUE;
}

BOOL HashThread_SwapItemBuffer(const HashThread* cptagHashThread, DWORD dwItem1, DWORD dwItem2)
{
	tagHashThread_FileRecode tagHashFile_Swap = { 0 };
	tagHashThread_FileRecode* ptagHashFile_Item1 = NULL;
	tagHashThread_FileRecode* ptagHashFile_Item2 = NULL;
	DWORD dwFileCount = 0;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	dwFileCount = cptagHashThread->tagMultiFile->dwFileCount;
	ptagHashFile_Item1 = HASHTHREAD_FILERECODE(cptagHashThread, dwItem1);
	ptagHashFile_Item2 = HASHTHREAD_FILERECODE(cptagHashThread, dwItem2);

	IF_UNLIKELY(dwItem1 > dwFileCount || dwItem2 > dwFileCount) {
		return FALSE;
	}

	memcpy(&tagHashFile_Swap, ptagHashFile_Item1, sizeof(tagHashThread_FileRecode));
	memcpy(ptagHashFile_Item1, ptagHashFile_Item2, sizeof(tagHashThread_FileRecode));
	memcpy(ptagHashFile_Item2, &tagHashFile_Swap, sizeof(tagHashThread_FileRecode));

#ifdef _DEBUG
	STPRINTF_FUNC(szDebugText, _T("HashThraed: HashThraed_SwapItemBuffer(): バッファの項目を入れ替えました。%d←→%d\r\n"), dwItem1, dwItem2);
	OutputDebugString(szDebugText);
#endif

	return TRUE;
}

BOOL HashThread_CreateHashFile(const HashThread* cptagHashThread, const TCHAR* cpInFileName, const TCHAR* cpInOption)
{
	TCHAR* pBuf = NULL;
	TCHAR* pFilePath = NULL;
	TCHAR* pCurrentFolder = NULL;
	DWORD dwCharCode = 0;
	DWORD dwHashType = 0;
	DWORD dwHashLen = 0;
	size_t	szLen = 0;
	int nIsOldHashFile = FALSE;
	int nIsHashUpper = FALSE;
	int nFileWriteTime = FALSE;
	const TCHAR* pHashExt;

#ifdef _DEBUG
	TCHAR szDebugText[1024];
	const TCHAR* szCharCodeText[] = { _T("Shift_JIS"), _T("Unicode"), _T("UTF-8") };
#endif

	dwHashType = cptagHashThread->dwHashType;
	dwHashLen = cptagHashThread->dwHashLen;
	pHashExt = cptagHashThread->cpHashExt;

	pBuf = (TCHAR*)malloc((MAX_PATH_SIZE * 5) * sizeof(TCHAR));
	IF_UNLIKELY(pBuf == NULL) {
		return 0;
	}

	*pBuf = '\0';
	pFilePath = MAX_PATH_SIZE + (TCHAR*)pBuf;
	*pFilePath = '\0';
	pCurrentFolder = MAX_PATH_SIZE * 3 + pFilePath;
	*pCurrentFolder = '\0';
	dwCharCode = cptagHashThread->tagMultiFile->dwHashFileCharCode;

	// オプションの解析
	if (cpInOption != NULL)
	{
		while (*cpInOption != NULL)
		{
			switch(*cpInOption)
			{
			case 'c':
				cpInOption++;
				dwCharCode = *cpInOption - '0';
				dwCharCode &= 0x2;

#ifdef _DEBUG
				STPRINTF_FUNC(szDebugText,
					_T("HashThread: HashThread_CreateHashFile(): %sの文字コード出力が選択されました。\r\n"),
					szCharCodeText[dwCharCode]);
				OutputDebugString(szDebugText);
#endif
				break;
			case 'l':
				nIsHashUpper = FALSE;
#ifdef _DEBUG
				OutputDebugString(_T("HashThread: HashThread_CreateHashFile(): ハッシュの小文字出力が選択されました。\r\n"));
#endif
				break;
			case 'u':
				nIsHashUpper = TRUE;
#ifdef _DEBUG
				OutputDebugString(_T("HashThread: HashThread_CreateHashFile(): ハッシュの大文字出力が選択されました。\r\n"));
#endif
				break;
			case 'o':
				nIsOldHashFile = TRUE;
#ifdef _DEBUG
				OutputDebugString(_T("HashThread: HashThread_CreateHashFile(): MD5チェックツール (1x) 形式の Hashファイル出力が選択されました。\r\n"));
#endif
				break;
			case 'F':
				nFileWriteTime = TRUE;
#ifdef _DEBUG
				OutputDebugString(_T("HashThread: HashThread_CreateHashFile(): ハッシュファイルの更新日時を更新します。\r\n"));
#endif
				break;
			}
			cpInOption++;
		}
	}

	if (nIsOldHashFile)
	{
		DWORD dwCount = 0;
		DWORD dwFileCount = cptagHashThread->tagMultiFile->dwFileCount;

		for (; dwCount < dwFileCount; dwCount++)
		{
			tagHashThread_FileRecode* ptagHashFile_Recode = HashThread_GetFileRecode_Core(cptagHashThread, dwCount);
			TCHAR* pTmp = NULL;
			HANDLE hFile = NULL;
			DWORD dwWrite = 0;
			DWORD dwRet = 0;

			if (ptagHashFile_Recode->dwFileLastError != 0 && (ptagHashFile_Recode->nFileNameLength + cptagHashThread->nHashExtLen) >= MAX_PATH_SIZE) {
				continue;
			}

			pTmp = qtcscpy(pFilePath, ptagHashFile_Recode->szFileName);
			qtcscpy(pTmp, pHashExt);

			hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			IF_UNLIKELY(hFile == INVALID_HANDLE_VALUE)
			{

#ifdef _DEBUG
				OutputDebugString(_T("HashThread: HashThread_CreateHashFile(): CreateFile() のエラーが発生しました。\r\n"));
#endif

				continue;
			}

			ptagHashFile_Recode->dwHashFileHashType = dwHashType;
			ptagHashFile_Recode->dwHashFileHashLine = 0;
			ptagHashFile_Recode->nIsHashOldFile = TRUE;
			ptagHashFile_Recode->nHashFileCmp = 0;
			memcpy(ptagHashFile_Recode->nHashFileHashByte, ptagHashFile_Recode->pFileHashByte, HASH_LEN);

			/*
			if (ptagHashFile_Recode->dwHashFileHashLine != (DWORD)-1) {
				memcpy(ptagHashFile_Recode->pFileHashByte, ptagHashFile_Recode->nHashFileHashByte, HASH_LEN);
			}
			else {
				memcpy(ptagHashFile_Recode->nHashFileHashByte, ptagHashFile_Recode->pFileHashByte, HASH_LEN);
			}
			*/

			if (dwCharCode != HASHFILE_CHARCODE_UNICODE)
			{
				BinaryToChar((char*)pBuf, ptagHashFile_Recode->pFileHashByte, dwHashLen, nIsHashUpper);
				dwRet = WriteFile(hFile, pBuf, dwHashLen * 2, &dwWrite, FALSE);
			}
			else
			{
				const BYTE byUncodeBom[] = { 0xFF, 0xFE };
				dwRet = WriteFile(hFile, byUncodeBom, sizeof(byUncodeBom), &dwWrite, FALSE);

				IF_UNLIKELY(dwRet == 0)
				{
					CloseHandle(hFile);
					DeleteFile(pFilePath);
					continue;
				}

				BinaryToWChar((WCHAR*)pBuf, ptagHashFile_Recode->pFileHashByte, dwHashLen, nIsHashUpper);
				dwRet = WriteFile(hFile, pBuf, dwHashLen * 2 * sizeof(WCHAR), &dwWrite, FALSE);
			}

			IF_UNLIKELY(dwRet == 0)
			{
				CloseHandle(hFile);
				DeleteFile(pFilePath);
				continue;
			}
			CloseHandle(hFile);

			if (nFileWriteTime) {
				HashThread_FileWriteTime(pFilePath, &ptagHashFile_Recode->ftFileLastWriteTime);
			}
		}
	}
	else
	{
		HANDLE hFile = NULL;
		DWORD dwCount = 0;
		DWORD dwFileCount = 0;
		DWORD dwWrite = 0;
		DWORD dwRet = 0;
		char* (*TCharToCharConv_Func[])(const TCHAR*, char*) = { TCharToCharConv2, NULL, TCharToUtf8CharConv2, NULL };

		hFile = CreateFile(cpInFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		IF_UNLIKELY(hFile == INVALID_HANDLE_VALUE)
		{
			free(pBuf);

#ifdef _DEBUG
			OutputDebugString(_T("HashThread: HashThread_CreateHashFile(): CreateFile() のエラーが発生しました。\r\n"));
#endif
			return -1;
		}

		if (dwCharCode == HASHFILE_CHARCODE_UNICODE)
		{
			const BYTE byUncodeBom[] = { 0xFF, 0xFE };
			dwRet = WriteFile(hFile, byUncodeBom, sizeof(byUncodeBom), &dwWrite, FALSE);

			IF_UNLIKELY(dwRet == 0)
			{
				CloseHandle(hFile);
				free(pBuf);
				return -1;
			}
		}
		GetCurrentDirectory(MAX_PATH_SIZE, pCurrentFolder);

		dwFileCount = cptagHashThread->tagMultiFile->dwFileCount;
		for (; dwCount < dwFileCount; dwCount++)
		{
			tagHashThread_FileRecode* ptagHashFile_Recode = HashThread_GetFileRecode(cptagHashThread, dwCount);
			TCHAR* pOutFilePath = pFilePath;

			IF_UNLIKELY(ptagHashFile_Recode->dwFileLastError != 0) {
				continue;
			}

			ptagHashFile_Recode->dwHashFileHashType = dwHashType;
			ptagHashFile_Recode->dwHashFileHashLine = dwCount;
			ptagHashFile_Recode->nIsHashOldFile = FALSE;
			ptagHashFile_Recode->nHashFileCmp = 0;
			memcpy(ptagHashFile_Recode->nHashFileHashByte, ptagHashFile_Recode->pFileHashByte, HASH_LEN);

			if (!PathRelativePathTo(pFilePath, pCurrentFolder, FILE_ATTRIBUTE_DIRECTORY, ptagHashFile_Recode->szFileName, FILE_ATTRIBUTE_ARCHIVE)) {
				pOutFilePath = ptagHashFile_Recode->szFileName;
			}

			if (dwCharCode != HASHFILE_CHARCODE_UNICODE)
			{
				char* b = (char*)byBuf;

				BinaryToChar(b, ptagHashFile_Recode->pFileHashByte, dwHashLen, nIsHashUpper);
				b += (size_t)dwHashLen * 2;
				*b++ = ' ';
				*b++ = ' ';
				TCharToCharConv_Func[dwCharCode](pOutFilePath, b);
				b += strlen(b);
				*b++ = '\r';
				*b++ = '\n';
				*b = '\0';
				szLen = (size_t)((BYTE*)b - (BYTE*)byBuf); //szLen = strlen((char*)byBuf);
				dwRet = WriteFile(hFile, byBuf, (DWORD)szLen, &dwWrite, FALSE);
			}
			else
			{
				WCHAR* p = (WCHAR*)pBuf;

				BinaryToWChar(p, ptagHashFile_Recode->pFileHashByte, dwHashLen, nIsHashUpper);
				p += (size_t)dwHashLen * 2;
				*p++ = ' ';
				*p++ = ' ';
				TCharToWCharConv2(pOutFilePath, p);
				p += wcslen(p);
				*p++ = '\r';
				*p++ = '\n';
				*p = '\0';
				szLen = (size_t)((BYTE*)p - (BYTE*)pBuf); //szLen = wcslen((WCHAR*)pBuf);
				dwRet = WriteFile(hFile, pBuf, (DWORD)szLen, &dwWrite, FALSE);
			}

			if (dwRet == 0) {
				break;
			}
		}
		SetEndOfFile(hFile);
		CloseHandle(hFile);

		if (nFileWriteTime)
		{
			tagHashThread_FileRecode* ptagHashFile_Recode = HashThread_GetFileRecode(cptagHashThread, dwFileCount);
			HashThread_FileWriteTime(pFilePath, &ptagHashFile_Recode->ftFileLastWriteTime);
		}
	}
	free(pBuf);
	return 0;
}

BOOL HashThread_FileWriteTime(const TCHAR* cpInFileName, const FILETIME* cpInFileWriteTime)
{
	HANDLE hFile = NULL;
	DWORD dwLasterror = 0;
#ifdef _DEBUG
	TCHAR szDebugText[1024];
#endif

	hFile = CreateFile(cpInFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	IF_UNLIKELY(hFile == INVALID_HANDLE_VALUE)
	{
		dwLasterror = GetLastError();

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_FileWriteTime(): %s\r\nLastError: %d\r\n"), _T("CreateFile()"), dwLasterror);
		OutputDebugString(szDebugText);
#endif

		return FALSE;
	}

	IF_UNLIKELY(!SetFileTime(hFile, cpInFileWriteTime, NULL, cpInFileWriteTime))
	{
		dwLasterror = GetLastError();
		CloseHandle(hFile);

#ifdef _DEBUG
		STPRINTF_FUNC(szDebugText, _T("HashThread: HashThread_FileWriteTime(): %s\r\nLastError: %d\r\n"), _T("SetFileTime()"), dwLasterror);
		OutputDebugString(szDebugText);
#endif

		return FALSE;
	}

	CloseHandle(hFile);
	return TRUE;
}

BOOL HashThread_FileNameCheck(TCHAR* pInFileName)
{
	static WCHAR szBufWChar[MAX_PATH_SIZE] = { 0 };
	static WCHAR szOutWChar[MAX_PATH_SIZE] = { 0 };
	WCHAR* pBufWChar = szBufWChar;
	WCHAR* pOutWChar = szOutWChar;
	WCHAR* pSrc = szBufWChar;
	WCHAR* pDst = szOutWChar;
	int nEnvironmentCount = 0;
	BOOL bIsEnvironment = 0;


#ifdef _UNICODE
	qtcscpy(pBufWChar, pInFileName);
#else
	size_t nLen;
	size_t nFileSize;

	// マルチバイト特有の不具合防止に一度 Unicode に変換します。
	nFileSize = strlen(pInFileName);
	pBufWChar = CharToWCharConv2(pInFileName, pBufWChar);
	nLen = wcslen(pBufWChar);

	IF_UNLIKELY(!(nLen > 1)) {
		return 0;
	}
#endif

	// "ファイル名" に指定された『"』の削除
	if (*pSrc == '\"') {
		pSrc++;
	}

	if (WCHAR_COMP_TO_INT_4CHAR(pSrc, '\\', '\\', '?', '\\'))
	{
		pSrc += 4;
		WCHAR_COPY_TO_INT_4CHAR(pDst, '\\', '\\', '?', '\\');
		pDst += 4;
	}

	while (*pSrc != NULL)
	{
		if (*pSrc < 32)
		{
			pSrc++;
			continue;
		}

		switch (*pSrc)
		{
		case '\"':
		case '<':
		case '>':
		case '|':
		case '?':
			pSrc++;
			break;
		case '%':
			if (nEnvironmentCount == 0) {
				nEnvironmentCount++;
			}
			else
			{
				nEnvironmentCount = 0;
				bIsEnvironment++;
			}
			*pDst++ = *pSrc++;
			break;
		default:
			*pDst++ = *pSrc++;
		}
	}
	*pDst = '\0';


#ifdef _UNICODE
	qtcscpy(pInFileName, pOutWChar);
#else
	WCharToCharConv2(pOutWChar, pInFileName);
	nLen = strlen(pInFileName);
	IF_UNLIKELY(!(nLen > 1)) {
		return 0;
	}
#endif

	return bIsEnvironment + 1;
}


BOOL HashThread_DigitCheck(const TCHAR* cpDigit, DWORD dwStringLen)
{
	BOOL nRet = 0;
	size_t dwCount;
	TCHAR* pSrc;

	if (dwStringLen == -1) {
		dwStringLen = (DWORD)_tcslen(cpDigit);
	}

	pSrc = (TCHAR*)cpDigit;
	for (dwCount = 0; dwCount < (size_t)dwStringLen; dwCount++, pSrc++)
	{
		if (_istxdigit(*pSrc)) {
			nRet++;
		}
	}
	return nRet;
}

char* HashThread_HashToString_no_sprintf(char* pOutChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen)
{
	const char* cpHexLU[2] = { "0123456789abcdef", "0123456789ABCDEF" };
	const char* cpHex = cpHexLU[nIsHashUpper & 1];
	char* p;
	DWORD* b;

	p = (char*)pOutChar;
	b = (DWORD*)cpInHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		unsigned char nByte;

		nByte = (unsigned char)(*b);
		*p++ = cpHex[nByte >> 4];
		*p++ = cpHex[nByte & 0x0F];
		nByte = (unsigned char)(*b >> 8);
		*p++ = cpHex[nByte >> 4];
		*p++ = cpHex[nByte & 0x0F];
		nByte = (unsigned char)(*b >> 16);
		*p++ = cpHex[nByte >> 4];
		*p++ = cpHex[nByte & 0x0F];
		nByte = (unsigned char)(*b >> 24);
		*p++ = cpHex[nByte >> 4];
		*p++ = cpHex[nByte & 0x0F];
		b++;
	}
	*p = '\0';
	return pOutChar;
}

WCHAR* HashThread_HashToString_no_sprintf(WCHAR* pOutWChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen)
{
	const char* cpHexLU[2] = { "0123456789abcdef", "0123456789ABCDEF" };
	const char* cpHex = cpHexLU[nIsHashUpper & 1];
	WCHAR* p;
	DWORD* b;

	p = (WCHAR*)pOutWChar;
	b = (DWORD*)cpInHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		unsigned char nByte;

		nByte = (unsigned char)(*b);
		*p++ = cpHex[nByte >> 4];
		*p++ = cpHex[nByte & 0x0F];
		nByte = (unsigned char)(*b >> 8);
		*p++ = cpHex[nByte >> 4];
		*p++ = cpHex[nByte & 0x0F];
		nByte = (unsigned char)(*b >> 16);
		*p++ = cpHex[nByte >> 4];
		*p++ = cpHex[nByte & 0x0F];
		nByte = (unsigned char)(*b >> 24);
		*p++ = cpHex[nByte >> 4];
		*p++ = cpHex[nByte & 0x0F];
		b++;
	}
	*p = '\0';
	return pOutWChar;
}

BYTE* HashThread_HashToHash_no_scanf(BYTE* pOutHashByte, const char* cpInChar, const DWORD dwHashLen)
{
	char* p;
	DWORD* b;

	p = (char*)cpInChar;
	b = (DWORD*)pOutHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		char* endptr;
		DWORD dwHashByte;

		dwHashByte = strtoul(p, &endptr, 16);
		*b++ = BSwap32(dwHashByte);
		p += 8;
	}
	return pOutHashByte;
}

BYTE* HashThread_HashToHash_no_scanf(BYTE* pOutHashByte, const WCHAR* cpInWChar, const DWORD dwHashLen)
{
	WCHAR* p;
	DWORD* b;

	p = (WCHAR*)cpInWChar;
	b = (DWORD*)pOutHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		WCHAR* endptr;
		DWORD dwHashByte;

		dwHashByte = wcstoul(p, &endptr, 16);
		*b++ = BSwap32(dwHashByte);
		p += 8;
	}
	return pOutHashByte;
}

static const unsigned char AToHexTable[] =
{
	/* ASCII table */
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,
	 0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

BYTE* HashThread_HashToHash_no_strtoul(BYTE* pOutHashByte, const char* cpInChar, const DWORD dwHashLen)
{
	unsigned char* p;
	volatile DWORD* b;

	p = (unsigned char*)cpInChar;
	b = (DWORD*)pOutHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		DWORD dwHashByte;

		dwHashByte = AToHexTable[p[1]];
		dwHashByte += AToHexTable[p[0]] << 4;
		dwHashByte += AToHexTable[p[3]] << 8;
		dwHashByte += AToHexTable[p[2]] << 12;
		dwHashByte += AToHexTable[p[5]] << 16;
		dwHashByte += AToHexTable[p[4]] << 20;
		dwHashByte += AToHexTable[p[7]] << 24;
		dwHashByte += AToHexTable[p[6]] << 28;
		*b++ = dwHashByte;
		p += 8;
	}
	return pOutHashByte;
}

BYTE* HashThread_HashToHash_no_strtoul(BYTE* pOutHashByte, const WCHAR* cpInWChar, const DWORD dwHashLen)
{
	WCHAR* p;
	volatile DWORD* b;

	p = (WCHAR*)cpInWChar;
	b = (DWORD*)pOutHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		DWORD dwHashByte;

		dwHashByte  = AToHexTable[p[1]];
		dwHashByte += AToHexTable[p[0]] << 4;
		dwHashByte += AToHexTable[p[3]] << 8;
		dwHashByte += AToHexTable[p[2]] << 12;
		dwHashByte += AToHexTable[p[5]] << 16;
		dwHashByte += AToHexTable[p[4]] << 20;
		dwHashByte += AToHexTable[p[7]] << 24;
		dwHashByte += AToHexTable[p[6]] << 28;
		*b++ = dwHashByte;
		p += 8;
	}
	return pOutHashByte;
}

char* HashThread_HashToString_Default(char* pOutChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen)
{
	const char* cpFormatLU[] = { "%08x", "%08X" };
	const char* cpFormat = cpFormatLU[nIsHashUpper & 1];
	char* p;
	DWORD* b;

	p = (char*)pOutChar;
	b = (DWORD*)cpInHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		DWORD dwHashByte;
		char szBuf[10];

		dwHashByte = *b;
		dwHashByte = BSwap32(dwHashByte);

#ifndef _NODLL
		sprintf_s(szBuf, cpFormat, dwHashByte);
#else
		sprintf(szBuf, cpFormat, dwHashByte);
#endif

		memcpy(p, szBuf, 8);
		p += 8;
		b++;
	}
	*p = '\0';
	return pOutChar;
}

WCHAR* HashThread_HashToString_Default(WCHAR* pOutWChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen)
{
	const WCHAR* cpFormatLU[] = { L"%08x", L"%08X" };
	const WCHAR* cpFormat = cpFormatLU[nIsHashUpper & 1];
	WCHAR* p;
	DWORD* b;

	p = (WCHAR*)pOutWChar;
	b = (DWORD*)cpInHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		DWORD dwHashByte;
		WCHAR szBuf[10];

		dwHashByte = *b;
		dwHashByte = BSwap32(dwHashByte);

#ifndef _NODLL
		swprintf_s(szBuf, cpFormat, dwHashByte);
#else
		swprintf(szBuf, cpFormat, dwHashByte);
#endif

		memcpy(p, szBuf, 8 * sizeof(WCHAR));
		p += 8;
		b++;
	}
	*p = '\0';
	return pOutWChar;
}

BYTE* HashThread_HashToHash_Default(BYTE* pOutHashByte, const char* cpInChar, const DWORD dwHashLen)
{
	char szBuf[10] = { 0 };
	char* p;
	DWORD* b;

	p = (char*)cpInChar;
	b = (DWORD*)pOutHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		DWORD dwHashByte;

		memcpy(szBuf, p, 8);

#ifndef _NODLL
		sscanf_s(szBuf, "%08x", &dwHashByte);
#else
		sscanf(szBuf, "%08x", &dwHashByte);
#endif

		*b++ = BSwap32(dwHashByte);
		p += 8;
	}
	return pOutHashByte;
}

BYTE* HashThread_HashToHash_Default(BYTE* pOutHashByte, const WCHAR* cpInWChar, const DWORD dwHashLen)
{
	WCHAR szBuf[10] = { 0 };
	WCHAR* p;
	DWORD* b;

	p = (WCHAR*)cpInWChar;
	b = (DWORD*)pOutHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount -= 4)
	{
		DWORD dwHashByte;

		memcpy(szBuf, p, 8 * sizeof(WCHAR));

#ifndef _NODLL
		swscanf_s(szBuf, L"%8x", &dwHashByte);
#else
		swscanf(szBuf, L"%8x", &dwHashByte);
#endif

		*b++ = BSwap32(dwHashByte);
		p += 8;
	}
	return pOutHashByte;
}

char* HashThread_HashToString_Old(char* pOutChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen)
{
	const char* cpFormat = nIsHashUpper != 0 ? "%02X" : "%02x";
	char* p;
	BYTE* b;

	p = (char*)pOutChar;
	b = (BYTE*)cpInHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount--)
	{
		char szBuf[2 + 1];

#ifndef _NODLL
		sprintf_s(szBuf, cpFormat, *b++);
#else
		sprintf(szBuf, cpFormat, *b++);
#endif

		memcpy(p, szBuf, 2);
		p += 2;
	}
	*p = '\0';
	return pOutChar;
}

WCHAR* HashThread_HashToString_Old(WCHAR* pOutWChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen)
{
	const WCHAR* cpFormat = nIsHashUpper != 0 ? L"%02X" : L"%02x";
	WCHAR* p;
	BYTE* b;

	p = (WCHAR*)pOutWChar;
	b = (BYTE*)cpInHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount--)
	{
		WCHAR szBuf[2 + 1];

#ifndef _NODLL
		swprintf_s(szBuf, cpFormat, *b++);
#else
		swprintf(szBuf, cpFormat, *b++);
#endif

		memcpy(p, szBuf, 2 * sizeof(WCHAR));
		p += 2;
	}
	*p = '\0';
	return pOutWChar;
}

BYTE* HashThread_HashToHash_Old(BYTE* pOutHashByte, const char* cpInChar, const DWORD dwHashLen)
{
	char szBuf[4] = { 0 };
	char* p;
	BYTE* b;

	p = (char*)cpInChar;
	b = (BYTE*)pOutHashByte;

	for (DWORD dwCount = dwHashLen; dwCount> 0; dwCount--)
	{
		DWORD dwHashByte;

		memcpy(szBuf, p, 2);

#ifndef _NODLL
		sscanf_s(szBuf, "%02x", &dwHashByte);
#else
		sscanf(szBuf, "%02x", &dwHashByte);
#endif

		*b++ = (BYTE)dwHashByte;
		p += 2;
	}
	return pOutHashByte;
}

BYTE* HashThread_HashToHash_Old(BYTE* pOutHashByte, const WCHAR* cpInWChar, const DWORD dwHashLen)
{
	WCHAR szBuf[4] = { 0 };
	WCHAR* p;
	BYTE* b;

	p = (WCHAR*)cpInWChar;
	b = (BYTE*)pOutHashByte;

	for (DWORD dwCount = dwHashLen; dwCount > 0; dwCount--)
	{
		DWORD dwHashByte;

		memcpy(szBuf, p, 2 * sizeof(WCHAR));

#ifndef _NODLL
		swscanf_s(szBuf, L"%02x", &dwHashByte);
#else
		swscanf(szBuf, L"%02x", &dwHashByte);
#endif

		*b++ = (BYTE)dwHashByte;
		p += 2;
	}
	return pOutHashByte;
}
