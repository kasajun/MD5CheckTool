#if _MSC_VER > 1000
#pragma once
#endif

/*
 * Copyright 2017-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef __SHA3_NOASM_H_707F6D0E_37BA_417f_9C94_63C0225B1F7C__
#define __SHA3_NOASM_H_707F6D0E_37BA_417f_9C94_63C0225B1F7C__

#include "sha3_ctx.h"

#ifdef  __cplusplus
extern "C" {
#endif

void SHA3_NOASM_Reset(KECCAK1600_CTX* ctx);
int SHA3_NOASM_Init(KECCAK1600_CTX* ctx, unsigned char pad, size_t bitlen);
int KECCAK_KMAC_NOASM_Init(KECCAK1600_CTX* ctx, unsigned char pad, size_t bitlen);
int SHA3_224_NOASM_Init(KECCAK1600_CTX* ctx);
int SHA3_256_NOASM_Init(KECCAK1600_CTX* ctx);
int SHA3_384__NOASMInit(KECCAK1600_CTX* ctx);
int SHA3_512_NOASM_Init(KECCAK1600_CTX* ctx);
int SHA3_NOASM_Update(KECCAK1600_CTX* ctx, const void* _inp, size_t len);
int SHA3_NOASM_Final(unsigned char* md, KECCAK1600_CTX* ctx);

#define SHA3_224_NOASM_Update SHA3_NOASM_Update
#define SHA3_224_NOASM_Final SHA3_NOASM_Final

#define SHA3_256_NOASM_Update SHA3_NOASM_Update
#define SHA3_256_NOASM_Final SHA3_NOASM_Final

#define SHA3_384_NOASM_Update SHA3_NOASM_Update
#define SHA3_384_NOASM_Final SHA3_NOASM_Final

#define SHA3_512_NOASM_Update SHA3_NOASM_Update
#define SHA3_512_NOASM_Final SHA3_NOASM_Final

#ifdef  __cplusplus
}
#endif
#endif
