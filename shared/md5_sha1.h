#if _MSC_VER > 1000
#pragma once
#endif

/*
 * Copyright 2015-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef __MD5_SHA1_H_B42FED59_3475_4C8D_B0F8_5AF6206D4C5C__
#define __MD5_SHA1_H_B42FED59_3475_4C8D_B0F8_5AF6206D4C5C__

#include "md5.h"
#include "sha1.h"

#define MD5_SHA1_DIGEST_LENGTH	(MD5_DIGEST_LENGTH + SHA_DIGEST_LENGTH)
#define MD5_SHA1_CBLOCK			MD5_CBLOCK

typedef struct md5_sha1_st {
	MD5_CTX md5;
	SHA_CTX sha1;
} MD5_SHA1_CTX;

#ifdef __cplusplus
extern "C" {
#endif

int MD5_SHA1_Init(MD5_SHA1_CTX* mctx);
int MD5_SHA1_Update(MD5_SHA1_CTX* mctx, const void* data, size_t count);
int MD5_SHA1_Final(unsigned char* md, MD5_SHA1_CTX* mctx);
int MD5_SHA1_Ctrl(MD5_SHA1_CTX* mctx, int cmd, int mslen, void* ms);

#ifdef __cplusplus
}
#endif
#endif
