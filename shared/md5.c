/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include "md5.h"
#include "cpuid.h"

#define DATA_ORDER_IS_LITTLE_ENDIAN

#define HASH_LONG				MD5_LONG
#define HASH_CTX				MD5_CTX
#define HASH_CBLOCK				MD5_CBLOCK
#define HASH_UPDATE				MD5_Update
#define HASH_TRANSFORM			MD5_Transform
#define HASH_FINAL				MD5_Final
#define HASH_MAKE_STRING(c,s)	do {	\
		unsigned long ll;				\
		ll=(c)->A; (void)HOST_l2c(ll,(s));		\
		ll=(c)->B; (void)HOST_l2c(ll,(s));		\
		ll=(c)->C; (void)HOST_l2c(ll,(s));		\
		ll=(c)->D; (void)HOST_l2c(ll,(s));		\
		} while (0)
#define HASH_BLOCK_DATA_ORDER	md5_block_data_order

#include "md32_common.h"

 /*
  * Implemented from RFC1321 The MD5 Message-Digest Algorithm
  */

#define INIT_DATA_A (unsigned long)0x67452301L
#define INIT_DATA_B (unsigned long)0xefcdab89L
#define INIT_DATA_C (unsigned long)0x98badcfeL
#define INIT_DATA_D (unsigned long)0x10325476L

int MD5_Init(MD5_CTX* c)
{
	memset(c, 0, sizeof(*c));
	c->A = INIT_DATA_A;
	c->B = INIT_DATA_B;
	c->C = INIT_DATA_C;
	c->D = INIT_DATA_D;
	return 1;
}

unsigned char* MD5(const unsigned char* d, size_t n, unsigned char* md)
{
	MD5_CTX c;
	static unsigned char m[MD5_DIGEST_LENGTH];

	IF_UNLIKELY(md == NULL) {
		md = m;
	}

	MD5_Init(&c);

#ifndef CHARSET_EBCDIC
    MD5_Update(&c, d, n);
#else
	{
		char temp[1024];
		unsigned long chunk;

		while (n > 0)
		{
			chunk = (n > sizeof(temp)) ? sizeof(temp) : n;
			ebcdic2ascii(temp, d, chunk);
			MD5_Update(&c, temp, chunk);
			n -= chunk;
			d += chunk;
		}
	}
#endif
	MD5_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c)); /* security consideration */
	return md;
}

