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

#ifndef __SHA3_CTX_H_12F8B048_C4E3_49CC_A00D_C392FE49DF61__
#define __SHA3_CTX_H_12F8B048_C4E3_49CC_A00D_C392FE49DF61__

#include <stdlib.h>

#define KECCAK1600_WIDTH			1600
#define SHA3_MDSIZE(bitlen)			(bitlen / 8)
#define KMAC_MDSIZE(bitlen)			(2 * (bitlen / 8))
#define SHA3_BLOCKSIZE(bitlen)		((KECCAK1600_WIDTH - bitlen * 2) / 8)

typedef size_t(sha3_absorb_fn)(void* vctx, const void* inp, size_t len);
typedef int(sha3_final_fn)(unsigned char* md, void* vctx);
typedef int (sha3_squeeze_fn)(void* vctx, unsigned char* out, size_t outlen);

typedef struct prov_sha3_meth_st
{
	sha3_absorb_fn* absorb;
	sha3_final_fn* final;
	sha3_squeeze_fn* squeeze;
} PROV_SHA3_METHOD;

#define XOF_STATE_INIT				0
#define XOF_STATE_ABSORB			1
#define XOF_STATE_FINAL				2
#define XOF_STATE_SQUEEZE			3

typedef struct keccak_st {
	unsigned __int64 A[5][5];
	unsigned char buf[KECCAK1600_WIDTH / 8 - 32];
	size_t block_size;			/* cached ctx->digest->block_size */
	size_t md_size;				/* output length, variable in XOF */
	size_t bufsz;				/* used bytes in below buffer */
	unsigned char pad;
	PROV_SHA3_METHOD meth;
	int xof_state;
} KECCAK1600_CTX;

#endif
