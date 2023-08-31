/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * MD4 low level APIs are deprecated for public use, but still ok for
 * internal use.
 */
#include <string.h>
#include "md4.h"
#include "cpuid.h"

#define DATA_ORDER_IS_LITTLE_ENDIAN

#define HASH_LONG				MD4_LONG
#define HASH_CTX				MD4_CTX
#define HASH_CBLOCK				MD4_CBLOCK
#define HASH_UPDATE				MD4_Update
#define HASH_TRANSFORM			MD4_Transform
#define HASH_FINAL				MD4_Final
#define HASH_MAKE_STRING(c,s)   do {	\
		unsigned long ll;				\
		ll=(c)->A; (void)HOST_l2c(ll,(s));		\
		ll=(c)->B; (void)HOST_l2c(ll,(s));		\
		ll=(c)->C; (void)HOST_l2c(ll,(s));		\
		ll=(c)->D; (void)HOST_l2c(ll,(s));		\
		} while (0)
#define HASH_BLOCK_DATA_ORDER	md4_block_data_order

#include "md32_common.h"

/*-
#define F(x,y,z)		(((x) & (y))  |  ((~(x)) & (z)))
#define G(x,y,z)		(((x) & (y))  |  ((x) & ((z))) | ((y) & ((z))))
*/

/*
 * As pointed out by Wei Dai, the above can be simplified to the code
 * below.  Wei attributes these optimizations to Peter Gutmann's SHS code,
 * and he attributes it to Rich Schroeppel.
 */
#define F(b,c,d)		((((c) ^ (d)) & (b)) ^ (d))
#define G(b,c,d)		(((b) & (c)) | ((b) & (d)) | ((c) & (d)))
#define H(b,c,d)		((b) ^ (c) ^ (d))

#define R0(a,b,c,d,k,s,t) { \
		a+=((k)+(t)+F((b),(c),(d))); \
		a=ROTATE(a,s); };

#define R1(a,b,c,d,k,s,t) { \
		a+=((k)+(t)+G((b),(c),(d))); \
		a=ROTATE(a,s); };

#define R2(a,b,c,d,k,s,t) { \
		a+=((k)+(t)+H((b),(c),(d))); \
		a=ROTATE(a,s); };

/*
 * Implemented from RFC1186 The MD4 Message-Digest Algorithm
 */

#define INIT_DATA_A (unsigned long)0x67452301L
#define INIT_DATA_B (unsigned long)0xefcdab89L
#define INIT_DATA_C (unsigned long)0x98badcfeL
#define INIT_DATA_D (unsigned long)0x10325476L

int MD4_Init(MD4_CTX* c)
{
	memset(c, 0, sizeof(*c));
	c->A = INIT_DATA_A;
	c->B = INIT_DATA_B;
	c->C = INIT_DATA_C;
	c->D = INIT_DATA_D;
	return 1;
}

#ifndef md4_block_data_order
# ifdef X
#  undef X
# endif
void md4_block_data_order(MD4_CTX* c, const void* data_, size_t num)
{
	const unsigned char* data = (const unsigned char*)data_;
	register unsigned MD32_REG_T A;
	register unsigned MD32_REG_T B;
	register unsigned MD32_REG_T C;
	register unsigned MD32_REG_T D;
	register unsigned MD32_REG_T l;
# ifndef MD32_XARRAY
	/* See comment in crypto/sha/sha_local.h for details. */
	unsigned MD32_REG_T XX0;
	unsigned MD32_REG_T XX1;
	unsigned MD32_REG_T XX2;
	unsigned MD32_REG_T XX3;
	unsigned MD32_REG_T XX4;
	unsigned MD32_REG_T XX5;
	unsigned MD32_REG_T XX6;
	unsigned MD32_REG_T XX7;
	unsigned MD32_REG_T XX8;
	unsigned MD32_REG_T XX9;
	unsigned MD32_REG_T XX10;
	unsigned MD32_REG_T XX11;
	unsigned MD32_REG_T XX12;
	unsigned MD32_REG_T XX13;
	unsigned MD32_REG_T XX14;
	unsigned MD32_REG_T XX15;
#  define X(i)   XX##i
# else
	MD4_LONG XX[MD4_LBLOCK];
#  define X(i)   XX[i]
# endif

	A = c->A;
	B = c->B;
	C = c->C;
	D = c->D;

	for (; num--;)
	{
		(void)HOST_c2l(data, l);
		X(0) = l;
		(void)HOST_c2l(data, l);
		X(1) = l;
		/* Round 0 */
		R0(A, B, C, D, X(0),	3,	0);
		(void)HOST_c2l(data, l);
		X(2) = l;
		R0(D, A, B, C, X(1),	7,	0);
		(void)HOST_c2l(data, l);
		X(3) = l;
		R0(C, D, A, B, X(2),	11,	0);
		(void)HOST_c2l(data, l);
		X(4) = l;
		R0(B, C, D, A, X(3),	19,	0);
		(void)HOST_c2l(data, l);
		X(5) = l;
		R0(A, B, C, D, X(4),	3,	0);
		(void)HOST_c2l(data, l);
		X(6) = l;
		R0(D, A, B, C, X(5),	7,	0);
		(void)HOST_c2l(data, l);
		X(7) = l;
		R0(C, D, A, B, X(6),	11,	0);
		(void)HOST_c2l(data, l);
		X(8) = l;
		R0(B, C, D, A, X(7),	19,	0);
		(void)HOST_c2l(data, l);
		X(9) = l;
		R0(A, B, C, D, X(8),	3,	0);
		(void)HOST_c2l(data, l);
		X(10) = l;
		R0(D, A, B, C, X(9),	7,	0);
		(void)HOST_c2l(data, l);
		X(11) = l;
		R0(C, D, A, B, X(10),	11,	0);
		(void)HOST_c2l(data, l);
		X(12) = l;
		R0(B, C, D, A, X(11),	19,	0);
		(void)HOST_c2l(data, l);
		X(13) = l;
		R0(A, B, C, D, X(12),	3,	0);
		(void)HOST_c2l(data, l);
		X(14) = l;
		R0(D, A, B, C, X(13),	7,	0);
		(void)HOST_c2l(data, l);
		X(15) = l;
		R0(C, D, A, B, X(14),	11,	0);
		R0(B, C, D, A, X(15),	19,	0);
		/* Round 1 */
		R1(A, B, C, D, X(0),	3,	0x5A827999);
		R1(D, A, B, C, X(4),	5,	0x5A827999);
		R1(C, D, A, B, X(8),	9,	0x5A827999);
		R1(B, C, D, A, X(12),	13,	0x5A827999);
		R1(A, B, C, D, X(1),	3,	0x5A827999);
		R1(D, A, B, C, X(5),	5,	0x5A827999);
		R1(C, D, A, B, X(9),	9,	0x5A827999);
		R1(B, C, D, A, X(13),	13,	0x5A827999);
		R1(A, B, C, D, X(2),	3,	0x5A827999);
		R1(D, A, B, C, X(6),	5,	0x5A827999);
		R1(C, D, A, B, X(10),	9,	0x5A827999);
		R1(B, C, D, A, X(14),	13,	0x5A827999);
		R1(A, B, C, D, X(3),	3,	0x5A827999);
		R1(D, A, B, C, X(7),	5,	0x5A827999);
		R1(C, D, A, B, X(11),	9,	0x5A827999);
		R1(B, C, D, A, X(15),	13,	0x5A827999);
		/* Round 2 */
		R2(A, B, C, D, X(0),	3,	0x6ED9EBA1);
		R2(D, A, B, C, X(8),	9,	0x6ED9EBA1);
		R2(C, D, A, B, X(4),	11,	0x6ED9EBA1);
		R2(B, C, D, A, X(12),	15,	0x6ED9EBA1);
		R2(A, B, C, D, X(2),	3,	0x6ED9EBA1);
		R2(D, A, B, C, X(10),	9,	0x6ED9EBA1);
		R2(C, D, A, B, X(6),	11,	0x6ED9EBA1);
		R2(B, C, D, A, X(14),	15,	0x6ED9EBA1);
		R2(A, B, C, D, X(1),	3,	0x6ED9EBA1);
		R2(D, A, B, C, X(9),	9,	0x6ED9EBA1);
		R2(C, D, A, B, X(5),	11,	0x6ED9EBA1);
		R2(B, C, D, A, X(13),	15,	0x6ED9EBA1);
		R2(A, B, C, D, X(3),	3,	0x6ED9EBA1);
		R2(D, A, B, C, X(11),	9,	0x6ED9EBA1);
		R2(C, D, A, B, X(7),	11,	0x6ED9EBA1);
		R2(B, C, D, A, X(15),	15,	0x6ED9EBA1);

		A = c->A += A;
		B = c->B += B;
		C = c->C += C;
		D = c->D += D;
	}
	return;
}
#endif
