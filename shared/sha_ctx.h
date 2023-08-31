#if _MSC_VER > 1000
#pragma once
#endif

/*
 * Copyright 1995-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef __SHA_CTX_H_2E594BDB_59EF_4BC6_BDCE_E0CE5A51435C__
#define __SHA_CTX_H_2E594BDB_59EF_4BC6_BDCE_E0CE5A51435C__

#include <stdlib.h>

#define SHA_LONG					unsigned int
#define SHA_LONG64					unsigned __int64
#define SHA_LBLOCK					16
#define SHA_CBLOCK					(SHA_LBLOCK * 4)
#define SHA256_CBLOCK				SHA_CBLOCK
#define SHA512_CBLOCK				(SHA_LBLOCK * 8)
#define SHA_LAST_BLOCK				(SHA_CBLOCK - 8)
#define U64(C)						C##UI64

#define SHA_DIGEST_LENGTH			20
#define SHA1_DIGEST_LENGTH			SHA_DIGEST_LENGTH
#define SHA256_192_DIGEST_LENGTH	24
#define SHA224_DIGEST_LENGTH		28
#define SHA256_DIGEST_LENGTH		32
#define SHA384_DIGEST_LENGTH		48
#define SHA512_DIGEST_LENGTH		64


typedef struct SHAstate_st {
	SHA_LONG h0, h1, h2, h3, h4;
	SHA_LONG Nl, Nh;
	SHA_LONG data[SHA_LBLOCK];
	unsigned int num;
} SHA_CTX;

typedef struct SHA256state_st {
	SHA_LONG h[8];
	SHA_LONG Nl, Nh;
	SHA_LONG data[SHA_LBLOCK];
	unsigned int num, md_len;
} SHA256_CTX;

typedef struct SHA512state_st {
	SHA_LONG64 h[8];
	SHA_LONG64 Nl, Nh;
	union {
		SHA_LONG64 d[SHA_LBLOCK];
		unsigned char p[SHA512_CBLOCK];
	} u;
	unsigned int num, md_len;
} SHA512_CTX;

#endif
