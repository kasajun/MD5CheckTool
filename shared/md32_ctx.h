#if _MSC_VER > 1000
#pragma once
#endif

/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef __MD32_CTX_H_DF34613D_E186_4095_8E30_20BA18F9555D__
#define __MD32_CTX_H_DF34613D_E186_4095_8E30_20BA18F9555D__

#include <stdlib.h>

#define MD2_INT						unsigned char
#define MD4_LONG					unsigned int
#define MD5_LONG					unsigned int
#define MDC2_BLOCK					8
#define MD2_BLOCK					16
#define MD4_CBLOCK					64
#define MD5_CBLOCK					64
#define MD4_LBLOCK					(MD4_CBLOCK / 4)
#define MD5_LBLOCK					(MD5_CBLOCK / 4)

#define MDC2_DIGEST_LENGTH			16
#define MD2_DIGEST_LENGTH			16
#define MD4_DIGEST_LENGTH			16
#define MD5_DIGEST_LENGTH			16


typedef unsigned char MDC2_cblock[8];
typedef struct mdc2_ctx_st {
    unsigned int num;
    unsigned char data[MDC2_BLOCK];
    MDC2_cblock h, hh;
    unsigned int pad_type;
} MDC2_CTX;

typedef struct MD2state_st {
	unsigned int num;
	unsigned char data[MD2_BLOCK];
	MD2_INT cksm[MD2_BLOCK];
	MD2_INT state[MD2_BLOCK];
} MD2_CTX;

typedef struct MD4state_st {
	MD4_LONG A, B, C, D;
	MD4_LONG Nl, Nh;
	MD4_LONG data[MD4_LBLOCK];
	unsigned int num;
} MD4_CTX;

typedef struct MD5state_st {
	MD5_LONG A, B, C, D;
	MD5_LONG Nl, Nh;
	MD5_LONG data[MD5_LBLOCK];
	unsigned int num;
} MD5_CTX;

#endif
