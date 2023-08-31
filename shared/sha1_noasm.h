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

#ifndef __SHA1_NOASM_H_518650A9_26AA_414d_9467_0DD0F4E05291__
#define __SHA1_NOASM_H_518650A9_26AA_414d_9467_0DD0F4E05291__

#include "sha_ctx.h"

#ifdef  __cplusplus
extern "C" {
#endif

int SHA1_NOASM_Init(SHA_CTX* c);
int SHA1_NOASM_Update(SHA_CTX* c, const void* data, size_t len);
int SHA1_NOASM_Final(unsigned char* md, SHA_CTX* c);
unsigned char* SHA1_NOASM(const unsigned char* d, size_t n, unsigned char* md);
void SHA1_NOASM_Transform(SHA_CTX* c, const unsigned char* data);
void sha1_noasm_block_data_order(SHA_CTX* c, const void* p, size_t num);

#ifdef  __cplusplus
}
#endif
#endif
