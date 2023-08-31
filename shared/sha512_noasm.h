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

#ifndef __SHA512_NOASM_H_31430EF6_305B_4a62_B0C0_D6D11B79F818__
#define __SHA512_NOASM_H_31430EF6_305B_4a62_B0C0_D6D11B79F818__

#include "sha_ctx.h"

#define SHA384_NOASM_Update		SHA512_NOASM_Update
#define SHA512_NOASM_224_Update	SHA512_NOASM_Update
#define SHA512_NOASM_256_Update	SHA512_NOASM_Update
#define SHA384_NOASM_Final		SHA512_NOASM_Final
#define SHA512_NOASM_224_Final	SHA512_NOASM_Final
#define SHA512_NOASM_256_Final	SHA512_NOASM_Final

#ifdef  __cplusplus
extern "C" {
#endif

int SHA384_NOASM_Init(SHA512_CTX* c);
unsigned char* SHA384_NOASM(const unsigned char* d, size_t n, unsigned char* md);

int SHA512_NOASM_Init(SHA512_CTX* c);
int SHA512_NOASM_Update(SHA512_CTX* c, const void* data, size_t len);
int SHA512_NOASM_Final(unsigned char* md, SHA512_CTX* c);
unsigned char* SHA512_NOASM(const unsigned char* d, size_t n, unsigned char* md);

int SHA512_224_NOASM_Init(SHA512_CTX* c);
unsigned char* SHA512_224_NOASM(const unsigned char* d, size_t n, unsigned char* md);

int SHA512_256_NOASM_Init(SHA512_CTX* c);
unsigned char* SHA512_256_NOASM(const unsigned char* d, size_t n, unsigned char* md);

void SHA512_NOASM_Transform(SHA512_CTX* c, const unsigned char* data);

#ifdef  __cplusplus
}
#endif
#endif
