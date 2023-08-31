/*
 * Copyright 2017-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include "sha3.h"
#include "c20.h"


size_t SHA3_absorb(unsigned __int64 A[5][5], const unsigned char* inp, size_t len, size_t r);
void SHA3_squeeze(unsigned __int64 A[5][5], unsigned char* out, size_t len, size_t r);

void SHA3_Reset(KECCAK1600_CTX* ctx)
{
	memset(ctx->A, 0, sizeof(ctx->A));
	ctx->bufsz = 0;
}

int SHA3_Init(KECCAK1600_CTX* ctx, unsigned char pad, size_t bitlen)
{
	size_t bsz = SHA3_BLOCKSIZE(bitlen);

	IF_LIKELY(bsz <= sizeof(ctx->buf))
	{
		SHA3_Reset(ctx);
		ctx->block_size = bsz;
		ctx->md_size = bitlen / 8;
		ctx->pad = pad;
		return 1;
	}
	return 0;
}

int KECCAK_KMAC_Init(KECCAK1600_CTX* ctx, unsigned char pad, size_t bitlen)
{
	int ret = SHA3_Init(ctx, pad, bitlen);

	IF_LIKELY(ret) {
		ctx->md_size *= 2;
	}
	return ret;
}

#define SHA3_INIT_FUNC(bitlen) \
int SHA3_ ## bitlen ## _Init(KECCAK1600_CTX* ctx) \
{ \
	return SHA3_Init(ctx, '\x06', bitlen); \
} \

SHA3_INIT_FUNC(224)
SHA3_INIT_FUNC(256)
SHA3_INIT_FUNC(384)
SHA3_INIT_FUNC(512)

int SHA3_Update(KECCAK1600_CTX* ctx, const void* _inp, size_t len)
{
	const unsigned char* inp = (const unsigned char*)_inp;
	size_t bsz = ctx->block_size;
	size_t num, rem;

	IF_UNLIKELY(len == 0) {
		return 1;
	}

	if ((num = ctx->bufsz) != 0)
	{	/* process intermediate buffer? */
		rem = bsz - num;

		if (len < rem)
		{
			memcpy(ctx->buf + num, inp, len);
			ctx->bufsz += len;
			return 1;
		}
		/*
		 * We have enough data to fill or overflow the intermediate
		 * buffer. So we append |rem| bytes and process the block,
		 * leaving the rest for later processing...
		 */
		memcpy(ctx->buf + num, inp, rem);
		inp += rem, len -= rem;
		(void)SHA3_absorb(ctx->A, ctx->buf, bsz, bsz);
		ctx->bufsz = 0;
		/* ctx->buf is processed, ctx->num is guaranteed to be zero */
	}

	if (len >= bsz) {
		rem = SHA3_absorb(ctx->A, inp, len, bsz);
	}
	else {
		rem = len;
	}

	if (rem)
	{
		memcpy(ctx->buf, inp + len - rem, rem);
		ctx->bufsz = rem;
	}

	return 1;
}

int SHA3_Final(unsigned char* md, KECCAK1600_CTX* ctx)
{
	size_t bsz = ctx->block_size;
	size_t num = ctx->bufsz;

	IF_UNLIKELY(ctx->md_size == 0) {
		return 1;
	}

	/*
	 * Pad the data with 10*1. Note that |num| can be |bsz - 1|
 	 * in which case both byte operations below are performed on
	 * same byte...
	 */
	memset(ctx->buf + num, 0, bsz - num);
	ctx->buf[num] = ctx->pad;
	ctx->buf[bsz - 1] |= 0x80;

	(void)SHA3_absorb(ctx->A, ctx->buf, bsz, bsz);
	SHA3_squeeze(ctx->A, md, ctx->md_size, bsz);

	return 1;
}
