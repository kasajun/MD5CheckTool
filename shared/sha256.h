#if _MSC_VER > 1000
#pragma once
#endif

/*
 * Copyright 2004-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef __SHA256_H_DA0E1A40_3E49_407a_A2BF_15CD7932C03E__
#define __SHA256_H_DA0E1A40_3E49_407a_A2BF_15CD7932C03E__

#include "sha_ctx.h"

#define SHA224_Update		SHA256_Update
#define SHA224_Final		SHA256_Final
#define SHA256_192_Update	SHA256_Update
#define SHA256_192_Final	SHA256_Final

#ifdef  __cplusplus
extern "C" {
#endif

int SHA224_Init(SHA256_CTX* c);
unsigned char* SHA224(const unsigned char* d, size_t n, unsigned char* md);

int SHA256_Init(SHA256_CTX* c);
int SHA256_Update(SHA256_CTX* c, const void* data, size_t len);
int SHA256_Final(unsigned char* md, SHA256_CTX* c);
unsigned char* SHA256(const unsigned char* d, size_t n, unsigned char* md);

int SHA256_192_Init(SHA256_CTX* c);
unsigned char* SHA256_192(const unsigned char* d, size_t n, unsigned char* md);

void SHA256_Transform(SHA256_CTX* c, const unsigned char* data);

#ifdef  __cplusplus
}
#endif
#endif
