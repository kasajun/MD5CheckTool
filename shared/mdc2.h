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

#ifndef __MDC2_H_B280FB6F_BDD6_4500_979C_1DABFA34DE16__
#define __MDC2_H_B280FB6F_BDD6_4500_979C_1DABFA34DE16__

#include "md32_ctx.h"

#ifdef __cplusplus
extern "C" {
#endif

int MDC2_Init(MDC2_CTX* c);
int MDC2_Update(MDC2_CTX* c, const void* data, size_t len);
int MDC2_Final(unsigned char* md, MDC2_CTX* c);
unsigned char* MDC2(const unsigned char* d, size_t n, unsigned char* md);
void MDC2_Transform(MDC2_CTX* c, const unsigned char* b);
void mdc2_block_data_order(MDC2_CTX* c, const void* p, size_t num);

#ifdef __cplusplus
}
#endif
#endif
