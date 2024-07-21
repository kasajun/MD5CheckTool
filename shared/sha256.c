/*
 * Copyright 2004-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include "sha256.h"
#include "cpuid.h"


#define DATA_ORDER_IS_BIG_ENDIAN

#define HASH_LONG				SHA_LONG
#define HASH_CTX				SHA256_CTX
#define HASH_CBLOCK				SHA_CBLOCK

/*
* Note that FIPS180-2 discusses "Truncation of the Hash Function Output."
* default: case below covers for it. It's not clear however if it's
* permitted to truncate to amount of bytes not divisible by 4. I bet not,
* but if it is, then default: case shall be extended. For reference.
* Idea behind separate cases for pre-defined lengths is to let the
* compiler decide if it's appropriate to unroll small loops.
*/
#define HASH_MAKE_STRING(c, s) do { \
	unsigned long ll; \
	unsigned int nn; \
	switch ((c)->md_len) \
	{ \
	case SHA256_192_DIGEST_LENGTH: \
		for (nn = 0; nn < SHA256_192_DIGEST_LENGTH / 4; nn++) \
		{ \
			ll = (c)->h[nn]; \
			(void)HOST_l2c(ll, (s)); \
		}  \
		break; \
	case SHA224_DIGEST_LENGTH: \
		for (nn = 0; nn < SHA224_DIGEST_LENGTH / 4; nn++) \
		{ \
			ll = (c)->h[nn]; \
			(void)HOST_l2c(ll, (s)); \
		}  \
		break; \
	case SHA256_DIGEST_LENGTH: \
		for (nn = 0; nn < SHA256_DIGEST_LENGTH / 4; nn++) \
		{ \
			ll = (c)->h[nn]; \
			(void)HOST_l2c(ll, (s)); \
		} \
		break; \
	default: \
		if ((c)->md_len > SHA256_DIGEST_LENGTH) { \
			return 0; \
		} \
		for (nn = 0; nn < (c)->md_len / 4; nn++) \
		{ \
			ll = (c)->h[nn]; \
			(void)HOST_l2c(ll, (s)); \
		} \
		break; \
	} \
} while (0)

#define HASH_UPDATE				SHA256_Update
#define HASH_TRANSFORM			SHA256_Transform
#define HASH_FINAL				SHA256_Final
#define HASH_BLOCK_DATA_ORDER	sha256_block_data_order

void sha256_block_data_order(SHA256_CTX* ctx, const void* in, size_t num);

#include "md32_common.h"

int SHA224_Init(SHA256_CTX* c)
{
	memset(c, 0, sizeof(*c));
	c->h[0] = 0xc1059ed8;
	c->h[1] = 0x367cd507;
	c->h[2] = 0x3070dd17;
	c->h[3] = 0xf70e5939;
	c->h[4] = 0xffc00b31;
	c->h[5] = 0x68581511;
	c->h[6] = 0x64f98fa7;
	c->h[7] = 0xbefa4fa4;
	c->md_len = SHA224_DIGEST_LENGTH;
	return 1;
}

int SHA256_Init(SHA256_CTX* c)
{
	memset(c, 0, sizeof(*c));
	c->h[0] = 0x6a09e667;
	c->h[1] = 0xbb67ae85;
	c->h[2] = 0x3c6ef372;
	c->h[3] = 0xa54ff53a;
	c->h[4] = 0x510e527f;
	c->h[5] = 0x9b05688c;
	c->h[6] = 0x1f83d9ab;
	c->h[7] = 0x5be0cd19;
	c->md_len = SHA256_DIGEST_LENGTH;
	return 1;
}

int SHA256_192_Init(SHA256_CTX* c)
{
	memset(c, 0, sizeof(*c));
	c->h[0] = 0x6a09e667;
	c->h[1] = 0xbb67ae85;
	c->h[2] = 0x3c6ef372;
	c->h[3] = 0xa54ff53a;
	c->h[4] = 0x510e527f;
	c->h[5] = 0x9b05688c;
	c->h[6] = 0x1f83d9ab;
	c->h[7] = 0x5be0cd19;
	c->md_len = SHA256_192_DIGEST_LENGTH;
	return 1;
}

unsigned char* SHA224(const unsigned char* d, size_t n, unsigned char* md)
{
	SHA256_CTX c;
	static unsigned char m[SHA224_DIGEST_LENGTH];

	if (md == NULL) {
		md = m;
	}
	SHA224_Init(&c);
	SHA256_Update(&c, d, n);
	SHA256_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));
	return md;
}

unsigned char* SHA256(const unsigned char* d, size_t n, unsigned char* md)
{
	SHA256_CTX c;
	static unsigned char m[SHA256_DIGEST_LENGTH];

	if (md == NULL) {
		md = m;
	}
	SHA256_Init(&c);
	SHA256_Update(&c, d, n);
	SHA256_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));
	return md;
}

unsigned char* SHA256_192(const unsigned char* d, size_t n, unsigned char* md)
{
	SHA256_CTX c;
	static unsigned char m[SHA256_192_DIGEST_LENGTH];

	if (md == NULL) {
		md = m;
	}
	SHA256_192_Init(&c);
	SHA256_Update(&c, d, n);
	SHA256_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));
	return md;
}
