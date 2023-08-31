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

#ifndef __MD2_H_4DE9DBC1_BB38_4828_8366_F45EE7B2DA71__
#define __MD2_H_4DE9DBC1_BB38_4828_8366_F45EE7B2DA71__

#include "md32_ctx.h"

#ifdef __cplusplus
extern "C" {
#endif

int MD2_Init(MD2_CTX* c);
int MD2_Update(MD2_CTX* c, const void* data, size_t len);
int MD2_Final(unsigned char* md, MD2_CTX* c);
unsigned char* MD2(const unsigned char* d, size_t n, unsigned char* md);
void MD2_Transform(MD2_CTX* c, const unsigned char* b);
void md2_block(MD2_CTX* c, const unsigned char* d);

#ifdef __cplusplus
}
#endif
#endif
