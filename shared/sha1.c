/*
 * Copyright 1995-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdlib.h>
#include <string.h>
#include "sha1.h"
#include "cpuid.h"

#define DATA_ORDER_IS_BIG_ENDIAN

#define HASH_LONG				SHA_LONG
#define HASH_CTX				SHA_CTX
#define HASH_CBLOCK				SHA_CBLOCK
#define HASH_UPDATE				SHA1_Update
#define HASH_TRANSFORM			SHA1_Transform
#define HASH_FINAL				SHA1_Final
#define HASH_MAKE_STRING(c, s) do {		\
	unsigned long ll;					\
	ll=(c)->h0; (void)HOST_l2c(ll,(s));	\
	ll=(c)->h1; (void)HOST_l2c(ll,(s));	\
	ll=(c)->h2; (void)HOST_l2c(ll,(s));	\
	ll=(c)->h3; (void)HOST_l2c(ll,(s));	\
	ll=(c)->h4; (void)HOST_l2c(ll,(s));	\
} while (0)
#define HASH_BLOCK_DATA_ORDER			sha1_block_data_order

#include "md32_common.h"

#define INIT_DATA_h0 0x67452301UL
#define INIT_DATA_h1 0xefcdab89UL
#define INIT_DATA_h2 0x98badcfeUL
#define INIT_DATA_h3 0x10325476UL
#define INIT_DATA_h4 0xc3d2e1f0UL

int SHA1_Init(SHA_CTX* c)
{
	memset(c, 0, sizeof(*c));
	c->h0 = INIT_DATA_h0;
	c->h1 = INIT_DATA_h1;
	c->h2 = INIT_DATA_h2;
	c->h3 = INIT_DATA_h3;
	c->h4 = INIT_DATA_h4;
	return 1;
}
