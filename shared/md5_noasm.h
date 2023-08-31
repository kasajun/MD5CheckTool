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

#ifndef __MD5_NOASM_H_CAD41A9C_3F67_42d7_A912_483906EA4ECC__
#define __MD5_NOASM_H_CAD41A9C_3F67_42d7_A912_483906EA4ECC__

#include "md32_ctx.h"

#ifdef __cplusplus
extern "C" {
#endif

int MD5_NOASM_Init(MD5_CTX* c);
int MD5_NOASM_Update(MD5_CTX* c, const void* data, size_t len);
int MD5_NOASM_Final(unsigned char* md, MD5_CTX* c);
unsigned char* MD5_NOASM(const unsigned char* d, size_t n, unsigned char* md);
void MD5_NOASM_Transform(MD5_CTX* c, const unsigned char* b);
void md5_noasm_block_data_order(MD5_CTX* c, const void* p, size_t num);

#ifdef __cplusplus
}
#endif
#endif
