/*
 * Copyright 2017-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include "sha3_noasm.h"
#include "c20.h"


size_t SHA3_NOASM_absorb(unsigned __int64 A[5][5], const unsigned char* inp, size_t len, size_t r);
void SHA3_NOASM_squeeze(unsigned __int64 A[5][5], unsigned char* out, size_t len, size_t r);

void SHA3_NOASM_Reset(KECCAK1600_CTX* ctx)
{
	memset(ctx->A, 0, sizeof(ctx->A));
	ctx->bufsz = 0;
}

int SHA3_NOASM_Init(KECCAK1600_CTX* ctx, unsigned char pad, size_t bitlen)
{
	size_t bsz = SHA3_BLOCKSIZE(bitlen);

	IF_LIKELY(bsz <= sizeof(ctx->buf))
	{
		SHA3_NOASM_Reset(ctx);
		ctx->block_size = bsz;
		ctx->md_size = bitlen / 8;
		ctx->pad = pad;
		return 1;
	}
	return 0;
}

int KECCAK_KMAC_NOASM_Init(KECCAK1600_CTX* ctx, unsigned char pad, size_t bitlen)
{
	int ret = SHA3_NOASM_Init(ctx, pad, bitlen);

	IF_LIKELY(ret) {
		ctx->md_size *= 2;
	}
	return ret;
}

#define SHA3_NOASM_INIT_FUNC(bitlen) \
int SHA3_ ## bitlen ## _NOASM_Init(KECCAK1600_CTX* ctx) \
{ \
	return SHA3_NOASM_Init(ctx, '\x06', bitlen); \
} \

SHA3_NOASM_INIT_FUNC(224)
SHA3_NOASM_INIT_FUNC(256)
SHA3_NOASM_INIT_FUNC(384)
SHA3_NOASM_INIT_FUNC(512)

int SHA3_NOASM_Update(KECCAK1600_CTX* ctx, const void* _inp, size_t len)
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
		(void)SHA3_NOASM_absorb(ctx->A, ctx->buf, bsz, bsz);
		ctx->bufsz = 0;
		/* ctx->buf is processed, ctx->num is guaranteed to be zero */
	}

	if (len >= bsz) {
		rem = SHA3_NOASM_absorb(ctx->A, inp, len, bsz);
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

int SHA3_NOASM_Final(unsigned char* md, KECCAK1600_CTX* ctx)
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

	(void)SHA3_NOASM_absorb(ctx->A, ctx->buf, bsz, bsz);
	SHA3_NOASM_squeeze(ctx->A, md, ctx->md_size, bsz);

	return 1;
}

/*
 * Choose some sensible defaults
 */
#if !defined(KECCAK_REF) && !defined(KECCAK_1X) && !defined(KECCAK_1X_ALT) && \
	!defined(KECCAK_2X) && !defined(KECCAK_INPLACE)
# define KECCAK_2X	  /* default to KECCAK_2X variant */
#endif

#if defined(__i386) || defined(__i386__) || defined(_M_IX86) || \
	(defined(__x86_64) && !defined(__BMI__)) || defined(_M_X64) || \
	defined(__mips) || defined(__riscv) || defined(__s390__) || \
	defined(__EMSCRIPTEN__)
/*
 * These don't have "and with complement" instruction, so minimize amount
 * of "not"-s. Implemented only in the [default] KECCAK_2X variant.
 */
# define KECCAK_COMPLEMENTING_TRANSFORM
#endif

#if defined(__x86_64__) || defined(__aarch64__) || \
	defined(__mips64) || defined(__ia64) || \
	(defined(__VMS) && !defined(__vax))
/*
 * These are available even in ILP32 flavours, but even then they are
 * capable of performing 64-bit operations as efficiently as in *P64.
 * Since it's not given that we can use sizeof(void *), just shunt it.
 */
# define BIT_INTERLEAVE (0)
#else
# define BIT_INTERLEAVE (sizeof(void*) < 8)
#endif

#define ROL32(a, offset) (((a) << (offset)) | ((a) >> ((32 - (offset)) & 31)))

static unsigned __int64 ROL64(unsigned __int64 val, int offset)
{
	if (offset == 0) {
		return val;
	}
	else if (!BIT_INTERLEAVE) {
		return (val << offset) | (val >> (64-offset));
	}
	else
	{
		unsigned int hi = (unsigned int)(val >> 32), lo = (unsigned int)val;

		if (offset & 1)
		{
			unsigned int tmp = hi;

			offset >>= 1;
			hi = ROL32(lo, offset);
			lo = ROL32(tmp, offset + 1);
		}
		else
		{
			offset >>= 1;
			lo = ROL32(lo, offset);
			hi = ROL32(hi, offset);
		}

		return ((unsigned __int64)hi << 32) | lo;
	}
}

static const unsigned char rhotates[5][5] = {
	{  0,  1, 62, 28, 27 },
	{ 36, 44,  6, 55, 20 },
	{  3, 10, 43, 25, 39 },
	{ 41, 45, 15, 21,  8 },
	{ 18,  2, 61, 56, 14 }
};

static const unsigned __int64 iotas[] = {
	BIT_INTERLEAVE ? 0x0000000000000001ui64 : 0x0000000000000001ui64,
	BIT_INTERLEAVE ? 0x0000008900000000ui64 : 0x0000000000008082ui64,
	BIT_INTERLEAVE ? 0x8000008b00000000ui64 : 0x800000000000808aui64,
	BIT_INTERLEAVE ? 0x8000808000000000ui64 : 0x8000000080008000ui64,
	BIT_INTERLEAVE ? 0x0000008b00000001ui64 : 0x000000000000808bui64,
	BIT_INTERLEAVE ? 0x0000800000000001ui64 : 0x0000000080000001ui64,
	BIT_INTERLEAVE ? 0x8000808800000001ui64 : 0x8000000080008081ui64,
	BIT_INTERLEAVE ? 0x8000008200000001ui64 : 0x8000000000008009ui64,
	BIT_INTERLEAVE ? 0x0000000b00000000ui64 : 0x000000000000008aui64,
	BIT_INTERLEAVE ? 0x0000000a00000000ui64 : 0x0000000000000088ui64,
	BIT_INTERLEAVE ? 0x0000808200000001ui64 : 0x0000000080008009ui64,
	BIT_INTERLEAVE ? 0x0000800300000000ui64 : 0x000000008000000aui64,
	BIT_INTERLEAVE ? 0x0000808b00000001ui64 : 0x000000008000808bui64,
	BIT_INTERLEAVE ? 0x8000000b00000001ui64 : 0x800000000000008bui64,
	BIT_INTERLEAVE ? 0x8000008a00000001ui64 : 0x8000000000008089ui64,
	BIT_INTERLEAVE ? 0x8000008100000001ui64 : 0x8000000000008003ui64,
	BIT_INTERLEAVE ? 0x8000008100000000ui64 : 0x8000000000008002ui64,
	BIT_INTERLEAVE ? 0x8000000800000000ui64 : 0x8000000000000080ui64,
	BIT_INTERLEAVE ? 0x0000008300000000ui64 : 0x000000000000800aui64,
	BIT_INTERLEAVE ? 0x8000800300000000ui64 : 0x800000008000000aui64,
	BIT_INTERLEAVE ? 0x8000808800000001ui64 : 0x8000000080008081ui64,
	BIT_INTERLEAVE ? 0x8000008800000000ui64 : 0x8000000000008080ui64,
	BIT_INTERLEAVE ? 0x0000800000000001ui64 : 0x0000000080000001ui64,
	BIT_INTERLEAVE ? 0x8000808200000000ui64 : 0x8000000080008008ui64
};

#if defined(KECCAK_REF)
/*
 * This is straightforward or "maximum clarity" implementation aiming
 * to resemble section 3.2 of the FIPS PUB 202 "SHA-3 Standard:
 * Permutation-Based Hash and Extendible-Output Functions" as much as
 * possible. With one caveat. Because of the way C stores matrices,
 * references to A[x,y] in the specification are presented as A[y][x].
 * Implementation unrolls inner x-loops so that modulo 5 operations are
 * explicitly pre-computed.
 */
static void Theta(unsigned __int64 A[5][5])
{
	unsigned __int64 C[5], D[5];
	size_t y;

	C[0] = A[0][0];
	C[1] = A[0][1];
	C[2] = A[0][2];
	C[3] = A[0][3];
	C[4] = A[0][4];

	for (y = 1; y < 5; y++)
	{
		C[0] ^= A[y][0];
		C[1] ^= A[y][1];
		C[2] ^= A[y][2];
		C[3] ^= A[y][3];
		C[4] ^= A[y][4];
	}

	D[0] = ROL64(C[1], 1) ^ C[4];
	D[1] = ROL64(C[2], 1) ^ C[0];
	D[2] = ROL64(C[3], 1) ^ C[1];
	D[3] = ROL64(C[4], 1) ^ C[2];
	D[4] = ROL64(C[0], 1) ^ C[3];

	for (y = 0; y < 5; y++)
	{
		A[y][0] ^= D[0];
		A[y][1] ^= D[1];
		A[y][2] ^= D[2];
		A[y][3] ^= D[3];
		A[y][4] ^= D[4];
	}
}

static void Rho(unsigned __int64 A[5][5])
{
	size_t y;

	for (y = 0; y < 5; y++)
	{
		A[y][0] = ROL64(A[y][0], rhotates[y][0]);
		A[y][1] = ROL64(A[y][1], rhotates[y][1]);
		A[y][2] = ROL64(A[y][2], rhotates[y][2]);
		A[y][3] = ROL64(A[y][3], rhotates[y][3]);
		A[y][4] = ROL64(A[y][4], rhotates[y][4]);
	}
}

static void Pi(unsigned __int64 A[5][5])
{
	unsigned __int64 T[5][5];

	/*
	 * T = A
	 * A[y][x] = T[x][(3*y+x)%5]
	 */
	memcpy(T, A, sizeof(T));

	A[0][0] = T[0][0];
	A[0][1] = T[1][1];
	A[0][2] = T[2][2];
	A[0][3] = T[3][3];
	A[0][4] = T[4][4];

	A[1][0] = T[0][3];
	A[1][1] = T[1][4];
	A[1][2] = T[2][0];
	A[1][3] = T[3][1];
	A[1][4] = T[4][2];

	A[2][0] = T[0][1];
	A[2][1] = T[1][2];
	A[2][2] = T[2][3];
	A[2][3] = T[3][4];
	A[2][4] = T[4][0];

	A[3][0] = T[0][4];
	A[3][1] = T[1][0];
	A[3][2] = T[2][1];
	A[3][3] = T[3][2];
	A[3][4] = T[4][3];

	A[4][0] = T[0][2];
	A[4][1] = T[1][3];
	A[4][2] = T[2][4];
	A[4][3] = T[3][0];
	A[4][4] = T[4][1];
}

static void Chi(unsigned __int64 A[5][5])
{
	unsigned __int64 C[5];
	size_t y;

	for (y = 0; y < 5; y++)
	{
		C[0] = A[y][0] ^ (~A[y][1] & A[y][2]);
		C[1] = A[y][1] ^ (~A[y][2] & A[y][3]);
		C[2] = A[y][2] ^ (~A[y][3] & A[y][4]);
		C[3] = A[y][3] ^ (~A[y][4] & A[y][0]);
		C[4] = A[y][4] ^ (~A[y][0] & A[y][1]);

		A[y][0] = C[0];
		A[y][1] = C[1];
		A[y][2] = C[2];
		A[y][3] = C[3];
		A[y][4] = C[4];
	}
}

static void Iota(unsigned __int64 A[5][5], size_t i)
{
	//assert(i < (sizeof(iotas) / sizeof(iotas[0])));
	A[0][0] ^= iotas[i];
}

static void KeccakF1600(unsigned __int64 A[5][5])
{
	size_t i;

	for (i = 0; i < 24; i++)
	{
		Theta(A);
		Rho(A);
		Pi(A);
		Chi(A);
		Iota(A, i);
	}
}

#elif defined(KECCAK_1X)
/*
 * This implementation is optimization of above code featuring unroll
 * of even y-loops, their fusion and code motion. It also minimizes
 * temporary storage. Compiler would normally do all these things for
 * you, purpose of manual optimization is to provide "unobscured"
 * reference for assembly implementation [in case this approach is
 * chosen for implementation on some platform]. In the nutshell it's
 * equivalent of "plane-per-plane processing" approach discussed in
 * section 2.4 of "Keccak implementation overview".
 */
static void Round(unsigned __int64 A[5][5], size_t i)
{
	unsigned __int64 C[5], E[2];		/* registers */
	unsigned __int64 D[5], T[2][5];		/* memory	*/

	//assert(i < (sizeof(iotas) / sizeof(iotas[0])));

	C[0] = A[0][0] ^ A[1][0] ^ A[2][0] ^ A[3][0] ^ A[4][0];
	C[1] = A[0][1] ^ A[1][1] ^ A[2][1] ^ A[3][1] ^ A[4][1];
	C[2] = A[0][2] ^ A[1][2] ^ A[2][2] ^ A[3][2] ^ A[4][2];
	C[3] = A[0][3] ^ A[1][3] ^ A[2][3] ^ A[3][3] ^ A[4][3];
	C[4] = A[0][4] ^ A[1][4] ^ A[2][4] ^ A[3][4] ^ A[4][4];

#if defined(__arm__)
	D[1] = E[0] = ROL64(C[2], 1) ^ C[0];
	D[4] = E[1] = ROL64(C[0], 1) ^ C[3];
	D[0] = C[0] = ROL64(C[1], 1) ^ C[4];
	D[2] = C[1] = ROL64(C[3], 1) ^ C[1];
	D[3] = C[2] = ROL64(C[4], 1) ^ C[2];

	T[0][0] = A[3][0] ^ C[0]; /* borrow T[0][0] */
	T[0][1] = A[0][1] ^ E[0]; /* D[1] */
	T[0][2] = A[0][2] ^ C[1]; /* D[2] */
	T[0][3] = A[0][3] ^ C[2]; /* D[3] */
	T[0][4] = A[0][4] ^ E[1]; /* D[4] */

	C[3] = ROL64(A[3][3] ^ C[2], rhotates[3][3]);   /* D[3] */
	C[4] = ROL64(A[4][4] ^ E[1], rhotates[4][4]);   /* D[4] */
	C[0] =       A[0][0] ^ C[0]; /* rotate by 0 */  /* D[0] */
	C[2] = ROL64(A[2][2] ^ C[1], rhotates[2][2]);   /* D[2] */
	C[1] = ROL64(A[1][1] ^ E[0], rhotates[1][1]);   /* D[1] */
#else
	D[0] = ROL64(C[1], 1) ^ C[4];
	D[1] = ROL64(C[2], 1) ^ C[0];
	D[2] = ROL64(C[3], 1) ^ C[1];
	D[3] = ROL64(C[4], 1) ^ C[2];
	D[4] = ROL64(C[0], 1) ^ C[3];

	T[0][0] = A[3][0] ^ D[0]; /* borrow T[0][0] */
	T[0][1] = A[0][1] ^ D[1];
	T[0][2] = A[0][2] ^ D[2];
	T[0][3] = A[0][3] ^ D[3];
	T[0][4] = A[0][4] ^ D[4];

	C[0] =	     A[0][0] ^ D[0]; /* rotate by 0 */
	C[1] = ROL64(A[1][1] ^ D[1], rhotates[1][1]);
	C[2] = ROL64(A[2][2] ^ D[2], rhotates[2][2]);
	C[3] = ROL64(A[3][3] ^ D[3], rhotates[3][3]);
	C[4] = ROL64(A[4][4] ^ D[4], rhotates[4][4]);
#endif
	A[0][0] = C[0] ^ (~C[1] & C[2]) ^ iotas[i];
	A[0][1] = C[1] ^ (~C[2] & C[3]);
	A[0][2] = C[2] ^ (~C[3] & C[4]);
	A[0][3] = C[3] ^ (~C[4] & C[0]);
	A[0][4] = C[4] ^ (~C[0] & C[1]);

	T[1][0] = A[1][0] ^ (C[3] = D[0]);
	T[1][1] = A[2][1] ^ (C[4] = D[1]); /* borrow T[1][1] */
	T[1][2] = A[1][2] ^ (E[0] = D[2]);
	T[1][3] = A[1][3] ^ (E[1] = D[3]);
	T[1][4] = A[2][4] ^ (C[2] = D[4]); /* borrow T[1][4] */

	C[0] = ROL64(T[0][3],        rhotates[0][3]);
	C[1] = ROL64(A[1][4] ^ C[2], rhotates[1][4]);   /* D[4] */
	C[2] = ROL64(A[2][0] ^ C[3], rhotates[2][0]);   /* D[0] */
	C[3] = ROL64(A[3][1] ^ C[4], rhotates[3][1]);   /* D[1] */
	C[4] = ROL64(A[4][2] ^ E[0], rhotates[4][2]);   /* D[2] */

	A[1][0] = C[0] ^ (~C[1] & C[2]);
	A[1][1] = C[1] ^ (~C[2] & C[3]);
	A[1][2] = C[2] ^ (~C[3] & C[4]);
	A[1][3] = C[3] ^ (~C[4] & C[0]);
	A[1][4] = C[4] ^ (~C[0] & C[1]);

	C[0] = ROL64(T[0][1],        rhotates[0][1]);
	C[1] = ROL64(T[1][2],        rhotates[1][2]);
	C[2] = ROL64(A[2][3] ^ D[3], rhotates[2][3]);
	C[3] = ROL64(A[3][4] ^ D[4], rhotates[3][4]);
	C[4] = ROL64(A[4][0] ^ D[0], rhotates[4][0]);

	A[2][0] = C[0] ^ (~C[1] & C[2]);
	A[2][1] = C[1] ^ (~C[2] & C[3]);
	A[2][2] = C[2] ^ (~C[3] & C[4]);
	A[2][3] = C[3] ^ (~C[4] & C[0]);
	A[2][4] = C[4] ^ (~C[0] & C[1]);

	C[0] = ROL64(T[0][4],        rhotates[0][4]);
	C[1] = ROL64(T[1][0],        rhotates[1][0]);
	C[2] = ROL64(T[1][1],        rhotates[2][1]); /* originally A[2][1] */
	C[3] = ROL64(A[3][2] ^ D[2], rhotates[3][2]);
	C[4] = ROL64(A[4][3] ^ D[3], rhotates[4][3]);

	A[3][0] = C[0] ^ (~C[1] & C[2]);
	A[3][1] = C[1] ^ (~C[2] & C[3]);
	A[3][2] = C[2] ^ (~C[3] & C[4]);
	A[3][3] = C[3] ^ (~C[4] & C[0]);
	A[3][4] = C[4] ^ (~C[0] & C[1]);

	C[0] = ROL64(T[0][2],        rhotates[0][2]);
	C[1] = ROL64(T[1][3],        rhotates[1][3]);
	C[2] = ROL64(T[1][4],        rhotates[2][4]); /* originally A[2][4] */
	C[3] = ROL64(T[0][0],        rhotates[3][0]); /* originally A[3][0] */
	C[4] = ROL64(A[4][1] ^ D[1], rhotates[4][1]);

	A[4][0] = C[0] ^ (~C[1] & C[2]);
	A[4][1] = C[1] ^ (~C[2] & C[3]);
	A[4][2] = C[2] ^ (~C[3] & C[4]);
	A[4][3] = C[3] ^ (~C[4] & C[0]);
	A[4][4] = C[4] ^ (~C[0] & C[1]);
}

static void KeccakF1600(unsigned __int64 A[5][5])
{
	size_t i;

	for (i = 0; i < 24; i++) {
		Round(A, i);
	}
}

#elif defined(KECCAK_1X_ALT)
/*
 * This is variant of above KECCAK_1X that reduces requirement for
 * temporary storage even further, but at cost of more updates to A[][].
 * It's less suitable if A[][] is memory bound, but better if it's
 * register bound.
 */

static void Round(unsigned __int64 A[5][5], size_t i)
{
	unsigned __int64 C[5], D[5];

	//assert(i < (sizeof(iotas) / sizeof(iotas[0])));

	C[0] = A[0][0] ^ A[1][0] ^ A[2][0] ^ A[3][0] ^ A[4][0];
	C[1] = A[0][1] ^ A[1][1] ^ A[2][1] ^ A[3][1] ^ A[4][1];
	C[2] = A[0][2] ^ A[1][2] ^ A[2][2] ^ A[3][2] ^ A[4][2];
	C[3] = A[0][3] ^ A[1][3] ^ A[2][3] ^ A[3][3] ^ A[4][3];
	C[4] = A[0][4] ^ A[1][4] ^ A[2][4] ^ A[3][4] ^ A[4][4];

	D[1] = C[0] ^  ROL64(C[2], 1);
	D[2] = C[1] ^  ROL64(C[3], 1);
	D[3] = C[2] ^= ROL64(C[4], 1);
	D[4] = C[3] ^= ROL64(C[0], 1);
	D[0] = C[4] ^= ROL64(C[1], 1);

	A[0][1] ^= D[1];
	A[1][1] ^= D[1];
	A[2][1] ^= D[1];
	A[3][1] ^= D[1];
	A[4][1] ^= D[1];

	A[0][2] ^= D[2];
	A[1][2] ^= D[2];
	A[2][2] ^= D[2];
	A[3][2] ^= D[2];
	A[4][2] ^= D[2];

	A[0][3] ^= C[2];
	A[1][3] ^= C[2];
	A[2][3] ^= C[2];
	A[3][3] ^= C[2];
	A[4][3] ^= C[2];

	A[0][4] ^= C[3];
	A[1][4] ^= C[3];
	A[2][4] ^= C[3];
	A[3][4] ^= C[3];
	A[4][4] ^= C[3];

	A[0][0] ^= C[4];
	A[1][0] ^= C[4];
	A[2][0] ^= C[4];
	A[3][0] ^= C[4];
	A[4][0] ^= C[4];

	C[1] = A[0][1];
	C[2] = A[0][2];
	C[3] = A[0][3];
	C[4] = A[0][4];

	A[0][1] = ROL64(A[1][1], rhotates[1][1]);
	A[0][2] = ROL64(A[2][2], rhotates[2][2]);
	A[0][3] = ROL64(A[3][3], rhotates[3][3]);
	A[0][4] = ROL64(A[4][4], rhotates[4][4]);

	A[1][1] = ROL64(A[1][4], rhotates[1][4]);
	A[2][2] = ROL64(A[2][3], rhotates[2][3]);
	A[3][3] = ROL64(A[3][2], rhotates[3][2]);
	A[4][4] = ROL64(A[4][1], rhotates[4][1]);

	A[1][4] = ROL64(A[4][2], rhotates[4][2]);
	A[2][3] = ROL64(A[3][4], rhotates[3][4]);
	A[3][2] = ROL64(A[2][1], rhotates[2][1]);
	A[4][1] = ROL64(A[1][3], rhotates[1][3]);

	A[4][2] = ROL64(A[2][4], rhotates[2][4]);
	A[3][4] = ROL64(A[4][3], rhotates[4][3]);
	A[2][1] = ROL64(A[1][2], rhotates[1][2]);
	A[1][3] = ROL64(A[3][1], rhotates[3][1]);

	A[2][4] = ROL64(A[4][0], rhotates[4][0]);
	A[4][3] = ROL64(A[3][0], rhotates[3][0]);
	A[1][2] = ROL64(A[2][0], rhotates[2][0]);
	A[3][1] = ROL64(A[1][0], rhotates[1][0]);

	A[1][0] = ROL64(C[3],    rhotates[0][3]);
	A[2][0] = ROL64(C[1],    rhotates[0][1]);
	A[3][0] = ROL64(C[4],    rhotates[0][4]);
	A[4][0] = ROL64(C[2],    rhotates[0][2]);

	C[0] = A[0][0];
	C[1] = A[1][0];
	D[0] = A[0][1];
	D[1] = A[1][1];

	A[0][0] ^= (~A[0][1] & A[0][2]);
	A[1][0] ^= (~A[1][1] & A[1][2]);
	A[0][1] ^= (~A[0][2] & A[0][3]);
	A[1][1] ^= (~A[1][2] & A[1][3]);
	A[0][2] ^= (~A[0][3] & A[0][4]);
	A[1][2] ^= (~A[1][3] & A[1][4]);
	A[0][3] ^= (~A[0][4] & C[0]);
	A[1][3] ^= (~A[1][4] & C[1]);
	A[0][4] ^= (~C[0]    & D[0]);
	A[1][4] ^= (~C[1]    & D[1]);

	C[2] = A[2][0];
	C[3] = A[3][0];
	D[2] = A[2][1];
	D[3] = A[3][1];

	A[2][0] ^= (~A[2][1] & A[2][2]);
	A[3][0] ^= (~A[3][1] & A[3][2]);
	A[2][1] ^= (~A[2][2] & A[2][3]);
	A[3][1] ^= (~A[3][2] & A[3][3]);
	A[2][2] ^= (~A[2][3] & A[2][4]);
	A[3][2] ^= (~A[3][3] & A[3][4]);
	A[2][3] ^= (~A[2][4] & C[2]);
	A[3][3] ^= (~A[3][4] & C[3]);
	A[2][4] ^= (~C[2]	& D[2]);
	A[3][4] ^= (~C[3]	& D[3]);

	C[4] = A[4][0];
	D[4] = A[4][1];

	A[4][0] ^= (~A[4][1] & A[4][2]);
	A[4][1] ^= (~A[4][2] & A[4][3]);
	A[4][2] ^= (~A[4][3] & A[4][4]);
	A[4][3] ^= (~A[4][4] & C[4]);
	A[4][4] ^= (~C[4]    & D[4]);
	A[0][0] ^= iotas[i];
}

static void KeccakF1600(unsigned __int64 A[5][5])
{
	size_t i;

	for (i = 0; i < 24; i++) {
		Round(A, i);
	}
}

#elif defined(KECCAK_2X)
/*
 * This implementation is variant of KECCAK_1X above with outer-most
 * round loop unrolled twice. This allows to take temporary storage
 * out of round procedure and simplify references to it by alternating
 * it with actual data (see round loop below). Originally it was meant
 * rather as reference for an assembly implementation, but it seems to
 * play best with compilers [as well as provide best instruction per
 * processed byte ratio at minimal round unroll factor]...
 */
static void Round(unsigned __int64 R[5][5], unsigned __int64 A[5][5], size_t i)
{
	unsigned __int64 C[5], D[5];

	//assert(i < (sizeof(iotas) / sizeof(iotas[0])));

	C[0] = A[0][0] ^ A[1][0] ^ A[2][0] ^ A[3][0] ^ A[4][0];
	C[1] = A[0][1] ^ A[1][1] ^ A[2][1] ^ A[3][1] ^ A[4][1];
	C[2] = A[0][2] ^ A[1][2] ^ A[2][2] ^ A[3][2] ^ A[4][2];
	C[3] = A[0][3] ^ A[1][3] ^ A[2][3] ^ A[3][3] ^ A[4][3];
	C[4] = A[0][4] ^ A[1][4] ^ A[2][4] ^ A[3][4] ^ A[4][4];

	D[0] = ROL64(C[1], 1) ^ C[4];
	D[1] = ROL64(C[2], 1) ^ C[0];
	D[2] = ROL64(C[3], 1) ^ C[1];
	D[3] = ROL64(C[4], 1) ^ C[2];
	D[4] = ROL64(C[0], 1) ^ C[3];

	C[0] =	     A[0][0] ^ D[0]; /* rotate by 0 */
	C[1] = ROL64(A[1][1] ^ D[1], rhotates[1][1]);
	C[2] = ROL64(A[2][2] ^ D[2], rhotates[2][2]);
	C[3] = ROL64(A[3][3] ^ D[3], rhotates[3][3]);
	C[4] = ROL64(A[4][4] ^ D[4], rhotates[4][4]);

#ifdef KECCAK_COMPLEMENTING_TRANSFORM
	R[0][0] = C[0] ^ ( C[1] | C[2]) ^ iotas[i];
	R[0][1] = C[1] ^ (~C[2] | C[3]);
	R[0][2] = C[2] ^ ( C[3] & C[4]);
	R[0][3] = C[3] ^ ( C[4] | C[0]);
	R[0][4] = C[4] ^ ( C[0] & C[1]);
#else
	R[0][0] = C[0] ^ (~C[1] & C[2]) ^ iotas[i];
	R[0][1] = C[1] ^ (~C[2] & C[3]);
	R[0][2] = C[2] ^ (~C[3] & C[4]);
	R[0][3] = C[3] ^ (~C[4] & C[0]);
	R[0][4] = C[4] ^ (~C[0] & C[1]);
#endif

	C[0] = ROL64(A[0][3] ^ D[3], rhotates[0][3]);
	C[1] = ROL64(A[1][4] ^ D[4], rhotates[1][4]);
	C[2] = ROL64(A[2][0] ^ D[0], rhotates[2][0]);
	C[3] = ROL64(A[3][1] ^ D[1], rhotates[3][1]);
	C[4] = ROL64(A[4][2] ^ D[2], rhotates[4][2]);

#ifdef KECCAK_COMPLEMENTING_TRANSFORM
	R[1][0] = C[0] ^ (C[1] |  C[2]);
	R[1][1] = C[1] ^ (C[2] &  C[3]);
	R[1][2] = C[2] ^ (C[3] | ~C[4]);
	R[1][3] = C[3] ^ (C[4] |  C[0]);
	R[1][4] = C[4] ^ (C[0] &  C[1]);
#else
	R[1][0] = C[0] ^ (~C[1] & C[2]);
	R[1][1] = C[1] ^ (~C[2] & C[3]);
	R[1][2] = C[2] ^ (~C[3] & C[4]);
	R[1][3] = C[3] ^ (~C[4] & C[0]);
	R[1][4] = C[4] ^ (~C[0] & C[1]);
#endif

	C[0] = ROL64(A[0][1] ^ D[1], rhotates[0][1]);
	C[1] = ROL64(A[1][2] ^ D[2], rhotates[1][2]);
	C[2] = ROL64(A[2][3] ^ D[3], rhotates[2][3]);
	C[3] = ROL64(A[3][4] ^ D[4], rhotates[3][4]);
	C[4] = ROL64(A[4][0] ^ D[0], rhotates[4][0]);

#ifdef KECCAK_COMPLEMENTING_TRANSFORM
	R[2][0] =  C[0] ^ ( C[1] | C[2]);
	R[2][1] =  C[1] ^ ( C[2] & C[3]);
	R[2][2] =  C[2] ^ (~C[3] & C[4]);
	R[2][3] = ~C[3] ^ ( C[4] | C[0]);
	R[2][4] =  C[4] ^ ( C[0] & C[1]);
#else
	R[2][0] = C[0] ^ (~C[1] & C[2]);
	R[2][1] = C[1] ^ (~C[2] & C[3]);
	R[2][2] = C[2] ^ (~C[3] & C[4]);
	R[2][3] = C[3] ^ (~C[4] & C[0]);
	R[2][4] = C[4] ^ (~C[0] & C[1]);
#endif

	C[0] = ROL64(A[0][4] ^ D[4], rhotates[0][4]);
	C[1] = ROL64(A[1][0] ^ D[0], rhotates[1][0]);
	C[2] = ROL64(A[2][1] ^ D[1], rhotates[2][1]);
	C[3] = ROL64(A[3][2] ^ D[2], rhotates[3][2]);
	C[4] = ROL64(A[4][3] ^ D[3], rhotates[4][3]);

#ifdef KECCAK_COMPLEMENTING_TRANSFORM
	R[3][0] =  C[0] ^ ( C[1] & C[2]);
	R[3][1] =  C[1] ^ ( C[2] | C[3]);
	R[3][2] =  C[2] ^ (~C[3] | C[4]);
	R[3][3] = ~C[3] ^ ( C[4] & C[0]);
	R[3][4] =  C[4] ^ ( C[0] | C[1]);
#else
	R[3][0] = C[0] ^ (~C[1] & C[2]);
	R[3][1] = C[1] ^ (~C[2] & C[3]);
	R[3][2] = C[2] ^ (~C[3] & C[4]);
	R[3][3] = C[3] ^ (~C[4] & C[0]);
	R[3][4] = C[4] ^ (~C[0] & C[1]);
#endif

	C[0] = ROL64(A[0][2] ^ D[2], rhotates[0][2]);
	C[1] = ROL64(A[1][3] ^ D[3], rhotates[1][3]);
	C[2] = ROL64(A[2][4] ^ D[4], rhotates[2][4]);
	C[3] = ROL64(A[3][0] ^ D[0], rhotates[3][0]);
	C[4] = ROL64(A[4][1] ^ D[1], rhotates[4][1]);

#ifdef KECCAK_COMPLEMENTING_TRANSFORM
	R[4][0] =  C[0] ^ (~C[1] & C[2]);
	R[4][1] = ~C[1] ^ ( C[2] | C[3]);
	R[4][2] =  C[2] ^ ( C[3] & C[4]);
	R[4][3] =  C[3] ^ ( C[4] | C[0]);
	R[4][4] =  C[4] ^ ( C[0] & C[1]);
#else
	R[4][0] = C[0] ^ (~C[1] & C[2]);
	R[4][1] = C[1] ^ (~C[2] & C[3]);
	R[4][2] = C[2] ^ (~C[3] & C[4]);
	R[4][3] = C[3] ^ (~C[4] & C[0]);
	R[4][4] = C[4] ^ (~C[0] & C[1]);
#endif
}

static void KeccakF1600(unsigned __int64 A[5][5])
{
	unsigned __int64 T[5][5];
	size_t i;

#ifdef KECCAK_COMPLEMENTING_TRANSFORM
	A[0][1] = ~A[0][1];
	A[0][2] = ~A[0][2];
	A[1][3] = ~A[1][3];
	A[2][2] = ~A[2][2];
	A[3][2] = ~A[3][2];
	A[4][0] = ~A[4][0];
#endif

	for (i = 0; i < 24; i += 2)
	{
		Round(T, A, i);
		Round(A, T, i + 1);
	}

#ifdef KECCAK_COMPLEMENTING_TRANSFORM
	A[0][1] = ~A[0][1];
	A[0][2] = ~A[0][2];
	A[1][3] = ~A[1][3];
	A[2][2] = ~A[2][2];
	A[3][2] = ~A[3][2];
	A[4][0] = ~A[4][0];
#endif
}

#else   /* define KECCAK_INPLACE to compile this code path */
/*
 * This implementation is KECCAK_1X from above combined 4 times with
 * a twist that allows to omit temporary storage and perform in-place
 * processing. It's discussed in section 2.5 of "Keccak implementation
 * overview". It's likely to be best suited for processors with large
 * register bank... On the other hand processor with large register
 * bank can as well use KECCAK_1X_ALT, it would be as fast but much
 * more compact...
 */
static void FourRounds(unsigned __int64 A[5][5], size_t i)
{
	unsigned __int64 B[5], C[5], D[5];

	//assert(i <= (sizeof(iotas) / sizeof(iotas[0]) - 4));

	/* Round 4*n */
	C[0] = A[0][0] ^ A[1][0] ^ A[2][0] ^ A[3][0] ^ A[4][0];
	C[1] = A[0][1] ^ A[1][1] ^ A[2][1] ^ A[3][1] ^ A[4][1];
	C[2] = A[0][2] ^ A[1][2] ^ A[2][2] ^ A[3][2] ^ A[4][2];
	C[3] = A[0][3] ^ A[1][3] ^ A[2][3] ^ A[3][3] ^ A[4][3];
	C[4] = A[0][4] ^ A[1][4] ^ A[2][4] ^ A[3][4] ^ A[4][4];

	D[0] = ROL64(C[1], 1) ^ C[4];
	D[1] = ROL64(C[2], 1) ^ C[0];
	D[2] = ROL64(C[3], 1) ^ C[1];
	D[3] = ROL64(C[4], 1) ^ C[2];
	D[4] = ROL64(C[0], 1) ^ C[3];

	B[0] =       A[0][0] ^ D[0]; /* rotate by 0 */
	B[1] = ROL64(A[1][1] ^ D[1], rhotates[1][1]);
	B[2] = ROL64(A[2][2] ^ D[2], rhotates[2][2]);
	B[3] = ROL64(A[3][3] ^ D[3], rhotates[3][3]);
	B[4] = ROL64(A[4][4] ^ D[4], rhotates[4][4]);

	C[0] = A[0][0] = B[0] ^ (~B[1] & B[2]) ^ iotas[i];
	C[1] = A[1][1] = B[1] ^ (~B[2] & B[3]);
	C[2] = A[2][2] = B[2] ^ (~B[3] & B[4]);
	C[3] = A[3][3] = B[3] ^ (~B[4] & B[0]);
	C[4] = A[4][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[0][3] ^ D[3], rhotates[0][3]);
	B[1] = ROL64(A[1][4] ^ D[4], rhotates[1][4]);
	B[2] = ROL64(A[2][0] ^ D[0], rhotates[2][0]);
	B[3] = ROL64(A[3][1] ^ D[1], rhotates[3][1]);
	B[4] = ROL64(A[4][2] ^ D[2], rhotates[4][2]);

	C[0] ^= A[2][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[3][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[4][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[0][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[1][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[0][1] ^ D[1], rhotates[0][1]);
	B[1] = ROL64(A[1][2] ^ D[2], rhotates[1][2]);
	B[2] = ROL64(A[2][3] ^ D[3], rhotates[2][3]);
	B[3] = ROL64(A[3][4] ^ D[4], rhotates[3][4]);
	B[4] = ROL64(A[4][0] ^ D[0], rhotates[4][0]);

	C[0] ^= A[4][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[0][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[1][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[2][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[3][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[0][4] ^ D[4], rhotates[0][4]);
	B[1] = ROL64(A[1][0] ^ D[0], rhotates[1][0]);
	B[2] = ROL64(A[2][1] ^ D[1], rhotates[2][1]);
	B[3] = ROL64(A[3][2] ^ D[2], rhotates[3][2]);
	B[4] = ROL64(A[4][3] ^ D[3], rhotates[4][3]);

	C[0] ^= A[1][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[2][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[3][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[4][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[0][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[0][2] ^ D[2], rhotates[0][2]);
	B[1] = ROL64(A[1][3] ^ D[3], rhotates[1][3]);
	B[2] = ROL64(A[2][4] ^ D[4], rhotates[2][4]);
	B[3] = ROL64(A[3][0] ^ D[0], rhotates[3][0]);
	B[4] = ROL64(A[4][1] ^ D[1], rhotates[4][1]);

	C[0] ^= A[3][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[4][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[0][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[1][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[2][4] = B[4] ^ (~B[0] & B[1]);

	/* Round 4*n+1 */
	D[0] = ROL64(C[1], 1) ^ C[4];
	D[1] = ROL64(C[2], 1) ^ C[0];
	D[2] = ROL64(C[3], 1) ^ C[1];
	D[3] = ROL64(C[4], 1) ^ C[2];
	D[4] = ROL64(C[0], 1) ^ C[3];

	B[0] =	     A[0][0] ^ D[0]; /* rotate by 0 */
	B[1] = ROL64(A[3][1] ^ D[1], rhotates[1][1]);
	B[2] = ROL64(A[1][2] ^ D[2], rhotates[2][2]);
	B[3] = ROL64(A[4][3] ^ D[3], rhotates[3][3]);
	B[4] = ROL64(A[2][4] ^ D[4], rhotates[4][4]);

	C[0] = A[0][0] = B[0] ^ (~B[1] & B[2]) ^ iotas[i + 1];
	C[1] = A[3][1] = B[1] ^ (~B[2] & B[3]);
	C[2] = A[1][2] = B[2] ^ (~B[3] & B[4]);
	C[3] = A[4][3] = B[3] ^ (~B[4] & B[0]);
	C[4] = A[2][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[3][3] ^ D[3], rhotates[0][3]);
	B[1] = ROL64(A[1][4] ^ D[4], rhotates[1][4]);
	B[2] = ROL64(A[4][0] ^ D[0], rhotates[2][0]);
	B[3] = ROL64(A[2][1] ^ D[1], rhotates[3][1]);
	B[4] = ROL64(A[0][2] ^ D[2], rhotates[4][2]);

	C[0] ^= A[4][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[2][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[0][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[3][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[1][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[1][1] ^ D[1], rhotates[0][1]);
	B[1] = ROL64(A[4][2] ^ D[2], rhotates[1][2]);
	B[2] = ROL64(A[2][3] ^ D[3], rhotates[2][3]);
	B[3] = ROL64(A[0][4] ^ D[4], rhotates[3][4]);
	B[4] = ROL64(A[3][0] ^ D[0], rhotates[4][0]);

	C[0] ^= A[3][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[1][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[4][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[2][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[0][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[4][4] ^ D[4], rhotates[0][4]);
	B[1] = ROL64(A[2][0] ^ D[0], rhotates[1][0]);
	B[2] = ROL64(A[0][1] ^ D[1], rhotates[2][1]);
	B[3] = ROL64(A[3][2] ^ D[2], rhotates[3][2]);
	B[4] = ROL64(A[1][3] ^ D[3], rhotates[4][3]);

	C[0] ^= A[2][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[0][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[3][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[1][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[4][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[2][2] ^ D[2], rhotates[0][2]);
	B[1] = ROL64(A[0][3] ^ D[3], rhotates[1][3]);
	B[2] = ROL64(A[3][4] ^ D[4], rhotates[2][4]);
	B[3] = ROL64(A[1][0] ^ D[0], rhotates[3][0]);
	B[4] = ROL64(A[4][1] ^ D[1], rhotates[4][1]);

	C[0] ^= A[1][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[4][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[2][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[0][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[3][4] = B[4] ^ (~B[0] & B[1]);

	/* Round 4*n+2 */
	D[0] = ROL64(C[1], 1) ^ C[4];
	D[1] = ROL64(C[2], 1) ^ C[0];
	D[2] = ROL64(C[3], 1) ^ C[1];
	D[3] = ROL64(C[4], 1) ^ C[2];
	D[4] = ROL64(C[0], 1) ^ C[3];

	B[0] =	   A[0][0] ^ D[0]; /* rotate by 0 */
	B[1] = ROL64(A[2][1] ^ D[1], rhotates[1][1]);
	B[2] = ROL64(A[4][2] ^ D[2], rhotates[2][2]);
	B[3] = ROL64(A[1][3] ^ D[3], rhotates[3][3]);
	B[4] = ROL64(A[3][4] ^ D[4], rhotates[4][4]);

	C[0] = A[0][0] = B[0] ^ (~B[1] & B[2]) ^ iotas[i + 2];
	C[1] = A[2][1] = B[1] ^ (~B[2] & B[3]);
	C[2] = A[4][2] = B[2] ^ (~B[3] & B[4]);
	C[3] = A[1][3] = B[3] ^ (~B[4] & B[0]);
	C[4] = A[3][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[4][3] ^ D[3], rhotates[0][3]);
	B[1] = ROL64(A[1][4] ^ D[4], rhotates[1][4]);
	B[2] = ROL64(A[3][0] ^ D[0], rhotates[2][0]);
	B[3] = ROL64(A[0][1] ^ D[1], rhotates[3][1]);
	B[4] = ROL64(A[2][2] ^ D[2], rhotates[4][2]);

	C[0] ^= A[3][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[0][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[2][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[4][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[1][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[3][1] ^ D[1], rhotates[0][1]);
	B[1] = ROL64(A[0][2] ^ D[2], rhotates[1][2]);
	B[2] = ROL64(A[2][3] ^ D[3], rhotates[2][3]);
	B[3] = ROL64(A[4][4] ^ D[4], rhotates[3][4]);
	B[4] = ROL64(A[1][0] ^ D[0], rhotates[4][0]);

	C[0] ^= A[1][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[3][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[0][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[2][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[4][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[2][4] ^ D[4], rhotates[0][4]);
	B[1] = ROL64(A[4][0] ^ D[0], rhotates[1][0]);
	B[2] = ROL64(A[1][1] ^ D[1], rhotates[2][1]);
	B[3] = ROL64(A[3][2] ^ D[2], rhotates[3][2]);
	B[4] = ROL64(A[0][3] ^ D[3], rhotates[4][3]);

	C[0] ^= A[4][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[1][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[3][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[0][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[2][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[1][2] ^ D[2], rhotates[0][2]);
	B[1] = ROL64(A[3][3] ^ D[3], rhotates[1][3]);
	B[2] = ROL64(A[0][4] ^ D[4], rhotates[2][4]);
	B[3] = ROL64(A[2][0] ^ D[0], rhotates[3][0]);
	B[4] = ROL64(A[4][1] ^ D[1], rhotates[4][1]);

	C[0] ^= A[2][0] = B[0] ^ (~B[1] & B[2]);
	C[1] ^= A[4][1] = B[1] ^ (~B[2] & B[3]);
	C[2] ^= A[1][2] = B[2] ^ (~B[3] & B[4]);
	C[3] ^= A[3][3] = B[3] ^ (~B[4] & B[0]);
	C[4] ^= A[0][4] = B[4] ^ (~B[0] & B[1]);

	/* Round 4*n+3 */
	D[0] = ROL64(C[1], 1) ^ C[4];
	D[1] = ROL64(C[2], 1) ^ C[0];
	D[2] = ROL64(C[3], 1) ^ C[1];
	D[3] = ROL64(C[4], 1) ^ C[2];
	D[4] = ROL64(C[0], 1) ^ C[3];

	B[0] =	     A[0][0] ^ D[0]; /* rotate by 0 */
	B[1] = ROL64(A[0][1] ^ D[1], rhotates[1][1]);
	B[2] = ROL64(A[0][2] ^ D[2], rhotates[2][2]);
	B[3] = ROL64(A[0][3] ^ D[3], rhotates[3][3]);
	B[4] = ROL64(A[0][4] ^ D[4], rhotates[4][4]);

	/* C[0] = */ A[0][0] = B[0] ^ (~B[1] & B[2]) ^ iotas[i + 3];
	/* C[1] = */ A[0][1] = B[1] ^ (~B[2] & B[3]);
	/* C[2] = */ A[0][2] = B[2] ^ (~B[3] & B[4]);
	/* C[3] = */ A[0][3] = B[3] ^ (~B[4] & B[0]);
	/* C[4] = */ A[0][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[1][3] ^ D[3], rhotates[0][3]);
	B[1] = ROL64(A[1][4] ^ D[4], rhotates[1][4]);
	B[2] = ROL64(A[1][0] ^ D[0], rhotates[2][0]);
	B[3] = ROL64(A[1][1] ^ D[1], rhotates[3][1]);
	B[4] = ROL64(A[1][2] ^ D[2], rhotates[4][2]);

	/* C[0] ^= */ A[1][0] = B[0] ^ (~B[1] & B[2]);
	/* C[1] ^= */ A[1][1] = B[1] ^ (~B[2] & B[3]);
	/* C[2] ^= */ A[1][2] = B[2] ^ (~B[3] & B[4]);
	/* C[3] ^= */ A[1][3] = B[3] ^ (~B[4] & B[0]);
	/* C[4] ^= */ A[1][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[2][1] ^ D[1], rhotates[0][1]);
	B[1] = ROL64(A[2][2] ^ D[2], rhotates[1][2]);
	B[2] = ROL64(A[2][3] ^ D[3], rhotates[2][3]);
	B[3] = ROL64(A[2][4] ^ D[4], rhotates[3][4]);
	B[4] = ROL64(A[2][0] ^ D[0], rhotates[4][0]);

	/* C[0] ^= */ A[2][0] = B[0] ^ (~B[1] & B[2]);
	/* C[1] ^= */ A[2][1] = B[1] ^ (~B[2] & B[3]);
	/* C[2] ^= */ A[2][2] = B[2] ^ (~B[3] & B[4]);
	/* C[3] ^= */ A[2][3] = B[3] ^ (~B[4] & B[0]);
	/* C[4] ^= */ A[2][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[3][4] ^ D[4], rhotates[0][4]);
	B[1] = ROL64(A[3][0] ^ D[0], rhotates[1][0]);
	B[2] = ROL64(A[3][1] ^ D[1], rhotates[2][1]);
	B[3] = ROL64(A[3][2] ^ D[2], rhotates[3][2]);
	B[4] = ROL64(A[3][3] ^ D[3], rhotates[4][3]);

	/* C[0] ^= */ A[3][0] = B[0] ^ (~B[1] & B[2]);
	/* C[1] ^= */ A[3][1] = B[1] ^ (~B[2] & B[3]);
	/* C[2] ^= */ A[3][2] = B[2] ^ (~B[3] & B[4]);
	/* C[3] ^= */ A[3][3] = B[3] ^ (~B[4] & B[0]);
	/* C[4] ^= */ A[3][4] = B[4] ^ (~B[0] & B[1]);

	B[0] = ROL64(A[4][2] ^ D[2], rhotates[0][2]);
	B[1] = ROL64(A[4][3] ^ D[3], rhotates[1][3]);
	B[2] = ROL64(A[4][4] ^ D[4], rhotates[2][4]);
	B[3] = ROL64(A[4][0] ^ D[0], rhotates[3][0]);
	B[4] = ROL64(A[4][1] ^ D[1], rhotates[4][1]);

	/* C[0] ^= */ A[4][0] = B[0] ^ (~B[1] & B[2]);
	/* C[1] ^= */ A[4][1] = B[1] ^ (~B[2] & B[3]);
	/* C[2] ^= */ A[4][2] = B[2] ^ (~B[3] & B[4]);
	/* C[3] ^= */ A[4][3] = B[3] ^ (~B[4] & B[0]);
	/* C[4] ^= */ A[4][4] = B[4] ^ (~B[0] & B[1]);
}

static void KeccakF1600(unsigned __int64 A[5][5])
{
	size_t i;

	for (i = 0; i < 24; i += 4) {
		FourRounds(A, i);
	}
}

#endif

static unsigned __int64 BitInterleave(unsigned __int64 Ai)
{
	if (BIT_INTERLEAVE)
	{
		unsigned int hi = (unsigned int)(Ai >> 32), lo = (unsigned int)Ai;
		unsigned int t0, t1;

		t0 = lo & 0x55555555;
		t0 |= t0 >> 1;  t0 &= 0x33333333;
		t0 |= t0 >> 2;  t0 &= 0x0f0f0f0f;
		t0 |= t0 >> 4;  t0 &= 0x00ff00ff;
		t0 |= t0 >> 8;  t0 &= 0x0000ffff;

		t1 = hi & 0x55555555;
		t1 |= t1 >> 1;  t1 &= 0x33333333;
		t1 |= t1 >> 2;  t1 &= 0x0f0f0f0f;
		t1 |= t1 >> 4;  t1 &= 0x00ff00ff;
		t1 |= t1 >> 8;  t1 <<= 16;

		lo &= 0xaaaaaaaa;
		lo |= lo << 1;  lo &= 0xcccccccc;
		lo |= lo << 2;  lo &= 0xf0f0f0f0;
		lo |= lo << 4;  lo &= 0xff00ff00;
		lo |= lo << 8;  lo >>= 16;

		hi &= 0xaaaaaaaa;
		hi |= hi << 1;  hi &= 0xcccccccc;
		hi |= hi << 2;  hi &= 0xf0f0f0f0;
		hi |= hi << 4;  hi &= 0xff00ff00;
		hi |= hi << 8;  hi &= 0xffff0000;

		Ai = ((unsigned __int64)(hi | lo) << 32) | (t1 | t0);
	}

	return Ai;
}

static unsigned __int64 BitDeinterleave(unsigned __int64 Ai)
{
	if (BIT_INTERLEAVE)
	{
		unsigned int hi = (unsigned int)(Ai >> 32), lo = (unsigned int)Ai;
		unsigned int t0, t1;

		t0 = lo & 0x0000ffff;
		t0 |= t0 << 8;  t0 &= 0x00ff00ff;
		t0 |= t0 << 4;  t0 &= 0x0f0f0f0f;
		t0 |= t0 << 2;  t0 &= 0x33333333;
		t0 |= t0 << 1;  t0 &= 0x55555555;

		t1 = hi << 16;
		t1 |= t1 >> 8;  t1 &= 0xff00ff00;
		t1 |= t1 >> 4;  t1 &= 0xf0f0f0f0;
		t1 |= t1 >> 2;  t1 &= 0xcccccccc;
		t1 |= t1 >> 1;  t1 &= 0xaaaaaaaa;

		lo >>= 16;
		lo |= lo << 8;  lo &= 0x00ff00ff;
		lo |= lo << 4;  lo &= 0x0f0f0f0f;
		lo |= lo << 2;  lo &= 0x33333333;
		lo |= lo << 1;  lo &= 0x55555555;

		hi &= 0xffff0000;
		hi |= hi >> 8;  hi &= 0xff00ff00;
		hi |= hi >> 4;  hi &= 0xf0f0f0f0;
		hi |= hi >> 2;  hi &= 0xcccccccc;
		hi |= hi >> 1;  hi &= 0xaaaaaaaa;

		Ai = ((unsigned __int64)(hi | lo) << 32) | (t1 | t0);
	}

	return Ai;
}

/*
 * SHA3_absorb can be called multiple times, but at each invocation
 * largest multiple of |r| out of |len| bytes are processed. Then
 * remaining amount of bytes is returned. This is done to spare caller
 * trouble of calculating the largest multiple of |r|. |r| can be viewed
 * as blocksize. It is commonly (1600 - 256*n)/8, e.g. 168, 136, 104,
 * 72, but can also be (1600 - 448)/8 = 144. All this means that message
 * padding and intermediate sub-block buffering, byte- or bitwise, is
 * caller's responsibility.
 */
size_t SHA3_NOASM_absorb(unsigned __int64 A[5][5], const unsigned char* inp, size_t len, size_t r)
{
	unsigned __int64* A_flat = (unsigned __int64*)A;
	size_t i, w = r / 8;

	//assert(r < (25 * sizeof(A[0][0])) && (r % 8) == 0);

	while (len >= r)
	{
		for (i = 0; i < w; i++)
		{
			unsigned __int64 Ai = (unsigned __int64)inp[0]       | (unsigned __int64)inp[1] << 8  |
								  (unsigned __int64)inp[2] << 16 | (unsigned __int64)inp[3] << 24 |
								  (unsigned __int64)inp[4] << 32 | (unsigned __int64)inp[5] << 40 |
								  (unsigned __int64)inp[6] << 48 | (unsigned __int64)inp[7] << 56;
			inp += 8;

			A_flat[i] ^= BitInterleave(Ai);
		}
		KeccakF1600(A);
		len -= r;
	}

	return len;
}

/*
 * sha3_squeeze is called once at the end to generate |out| hash value
 * of |len| bytes.
 */
void SHA3_NOASM_squeeze(unsigned __int64 A[5][5], unsigned char* out, size_t len, size_t r)
{
	unsigned __int64* A_flat = (unsigned __int64*)A;
	size_t i, w = r / 8;

	//assert(r < (25 * sizeof(A[0][0])) && (r % 8) == 0);

	while (len != 0)
	{
		for (i = 0; i < w && len != 0; i++)
		{
			unsigned __int64 Ai = BitDeinterleave(A_flat[i]);

			if (len < 8)
			{
				for (i = 0; i < len; i++)
				{
					*out++ = (unsigned char)Ai;
					Ai >>= 8;
				}
				return;
			}

			out[0] = (unsigned char)(Ai);
			out[1] = (unsigned char)(Ai >> 8);
			out[2] = (unsigned char)(Ai >> 16);
			out[3] = (unsigned char)(Ai >> 24);
			out[4] = (unsigned char)(Ai >> 32);
			out[5] = (unsigned char)(Ai >> 40);
			out[6] = (unsigned char)(Ai >> 48);
			out[7] = (unsigned char)(Ai >> 56);
			out += 8;
			len -= 8;
		}

		if (len) {
			KeccakF1600(A);
		}
	}
}
