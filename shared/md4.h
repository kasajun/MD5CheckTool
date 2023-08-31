#if _MSC_VER > 1000
#pragma once
#endif

/*
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef __MD4_H_3585F3C2_43AE_4bdb_A4DD_9A1ABC3FEC47__
#define __MD4_H_3585F3C2_43AE_4bdb_A4DD_9A1ABC3FEC47__

#include "md32_ctx.h"

#ifdef __cplusplus
extern "C" {
#endif

int MD4_Init(MD4_CTX* c);
int MD4_Update(MD4_CTX* c, const void* data, size_t len);
int MD4_Final(unsigned char* md, MD4_CTX* c);
unsigned char* MD4(const unsigned char* d, size_t n, unsigned char* md);
void MD4_Transform(MD4_CTX* c, const unsigned char* b);
void md4_block_data_order(MD4_CTX* c, const void* p, size_t num);

#ifdef __cplusplus
}
#endif
#endif
