/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include "md5_noasm.h"
#include "cpuid.h"

#define DATA_ORDER_IS_LITTLE_ENDIAN

#define HASH_LONG				MD5_LONG
#define HASH_CTX				MD5_CTX
#define HASH_CBLOCK				MD5_CBLOCK
#define HASH_UPDATE				MD5_NOASM_Update
#define HASH_TRANSFORM			MD5_NOASM_Transform
#define HASH_FINAL				MD5_NOASM_Final
#define HASH_MAKE_STRING(c,s)	do {	\
		unsigned long ll;				\
		ll=(c)->A; (void)HOST_l2c(ll,(s));		\
		ll=(c)->B; (void)HOST_l2c(ll,(s));		\
		ll=(c)->C; (void)HOST_l2c(ll,(s));		\
		ll=(c)->D; (void)HOST_l2c(ll,(s));		\
		} while (0)
#define HASH_BLOCK_DATA_ORDER	md5_noasm_block_data_order

#include "md32_common.h"

 /*-
 #define F(x,y,z)		(((x) & (y))  |  ((~(x)) & (z)))
 #define G(x,y,z)		(((x) & (z))  |  ((y) & (~(z))))
 */

 /*
  * As pointed out by Wei Dai, the above can be simplified to the code
  * below.  Wei attributes these optimizations to Peter Gutmann's
  * SHS code, and he attributes it to Rich Schroeppel.
  */
#define F(b,c,d)		((((c) ^ (d)) & (b)) ^ (d))
#define G(b,c,d)		((((b) ^ (c)) & (d)) ^ (c))
#define H(b,c,d)		((b) ^ (c) ^ (d))
#define I(b,c,d)		(((~(d)) | (b)) ^ (c))

#define R0(a,b,c,d,k,s,t) { \
		a+=((k)+(t)+F((b),(c),(d))); \
		a=ROTATE(a,s); \
		a+=b; };

#define R1(a,b,c,d,k,s,t) { \
		a+=((k)+(t)+G((b),(c),(d))); \
		a=ROTATE(a,s); \
		a+=b; };

#define R2(a,b,c,d,k,s,t) { \
		a+=((k)+(t)+H((b),(c),(d))); \
		a=ROTATE(a,s); \
		a+=b; };

#define R3(a,b,c,d,k,s,t) { \
		a+=((k)+(t)+I((b),(c),(d))); \
		a=ROTATE(a,s); \
		a+=b; };

 /*
  * Implemented from RFC1321 The MD5 Message-Digest Algorithm
  */

#define INIT_DATA_A (unsigned long)0x67452301
#define INIT_DATA_B (unsigned long)0xefcdab89
#define INIT_DATA_C (unsigned long)0x98badcfe
#define INIT_DATA_D (unsigned long)0x10325476

int MD5_NOASM_Init(MD5_CTX* c)
{
	memset(c, 0, sizeof(*c));
	c->A = INIT_DATA_A;
	c->B = INIT_DATA_B;
	c->C = INIT_DATA_C;
	c->D = INIT_DATA_D;
	return 1;
}

unsigned char* MD5_NOASM(const unsigned char* d, size_t n, unsigned char* md)
{
	MD5_CTX c;
	static unsigned char m[MD5_DIGEST_LENGTH];

	if (md == NULL) {
		md = m;
	}

	MD5_NOASM_Init(&c);

#ifndef CHARSET_EBCDIC
    MD5_NOASM_Update(&c, d, n);
#else
	{
		char temp[1024];
		unsigned long chunk;

		while (n > 0)
		{
			chunk = (n > sizeof(temp)) ? sizeof(temp) : n;
			ebcdic2ascii(temp, d, chunk);
			MD5_NOASM_Update(&c, temp, chunk);
			n -= chunk;
			d += chunk;
		}
	}
#endif
	MD5_NOASM_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c)); /* security consideration */
	return md;
}


#ifdef X
# undef X
#endif
void md5_noasm_block_data_order(MD5_CTX* c, const void* data_, size_t num)
{
	const unsigned char* data = (const unsigned char*)data_;
	register unsigned MD32_REG_T A;
	register unsigned MD32_REG_T B;
	register unsigned MD32_REG_T C;
	register unsigned MD32_REG_T D;
	register unsigned MD32_REG_T l;
#ifndef MD32_XARRAY
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
# define X(i)   XX##i
#else
	MD5_LONG XX[MD5_LBLOCK];
# define X(i)   XX[i]
#endif

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
		R0(A, B, C, D, X(0),	7,	0xd76aa478);
		(void)HOST_c2l(data, l);
		X(2) = l;
		R0(D, A, B, C, X(1),	12,	0xe8c7b756);
		(void)HOST_c2l(data, l);
		X(3) = l;
		R0(C, D, A, B, X(2),	17,	0x242070db);
		(void)HOST_c2l(data, l);
		X(4) = l;
		R0(B, C, D, A, X(3),	22,	0xc1bdceee);
		(void)HOST_c2l(data, l);
		X(5) = l;
		R0(A, B, C, D, X(4),	7,	0xf57c0faf);
		(void)HOST_c2l(data, l);
		X(6) = l;
		R0(D, A, B, C, X(5),	12,	0x4787c62a);
		(void)HOST_c2l(data, l);
		X(7) = l;
		R0(C, D, A, B, X(6),	17,	0xa8304613);
		(void)HOST_c2l(data, l);
		X(8) = l;
		R0(B, C, D, A, X(7),	22,	0xfd469501);
		(void)HOST_c2l(data, l);
		X(9) = l;
		R0(A, B, C, D, X(8),	7,	0x698098d8);
		(void)HOST_c2l(data, l);
		X(10) = l;
		R0(D, A, B, C, X(9),	12,	0x8b44f7af);
		(void)HOST_c2l(data, l);
		X(11) = l;
		R0(C, D, A, B, X(10),	17,	0xffff5bb1);
		(void)HOST_c2l(data, l);
		X(12) = l;
		R0(B, C, D, A, X(11),	22,	0x895cd7be);
		(void)HOST_c2l(data, l);
		X(13) = l;
		R0(A, B, C, D, X(12),	7,	0x6b901122);
		(void)HOST_c2l(data, l);
		X(14) = l;
		R0(D, A, B, C, X(13),	12,	0xfd987193);
		(void)HOST_c2l(data, l);
		X(15) = l;
		R0(C, D, A, B, X(14),	17,	0xa679438e);
		R0(B, C, D, A, X(15),	22,	0x49b40821);
		/* Round 1 */
		R1(A, B, C, D, X(1),	5,	0xf61e2562);
		R1(D, A, B, C, X(6),	9,	0xc040b340);
		R1(C, D, A, B, X(11),	14,	0x265e5a51);
		R1(B, C, D, A, X(0),	20,	0xe9b6c7aa);
		R1(A, B, C, D, X(5),	5,	0xd62f105d);
		R1(D, A, B, C, X(10),	9,	0x02441453);
		R1(C, D, A, B, X(15),	14,	0xd8a1e681);
		R1(B, C, D, A, X(4),	20,	0xe7d3fbc8);
		R1(A, B, C, D, X(9),	5,	0x21e1cde6);
		R1(D, A, B, C, X(14),	9,	0xc33707d6);
		R1(C, D, A, B, X(3),	14,	0xf4d50d87);
		R1(B, C, D, A, X(8),	20,	0x455a14ed);
		R1(A, B, C, D, X(13),	5,	0xa9e3e905);
		R1(D, A, B, C, X(2),	9,	0xfcefa3f8);
		R1(C, D, A, B, X(7),	14,	0x676f02d9);
		R1(B, C, D, A, X(12),	20,	0x8d2a4c8a);
		/* Round 2 */
		R2(A, B, C, D, X(5),	4,	0xfffa3942);
		R2(D, A, B, C, X(8),	11,	0x8771f681);
		R2(C, D, A, B, X(11),	16,	0x6d9d6122);
		R2(B, C, D, A, X(14),	23,	0xfde5380c);
		R2(A, B, C, D, X(1),	4,	0xa4beea44);
		R2(D, A, B, C, X(4),	11,	0x4bdecfa9);
		R2(C, D, A, B, X(7),	16,	0xf6bb4b60);
		R2(B, C, D, A, X(10),	23,	0xbebfbc70);
		R2(A, B, C, D, X(13),	4,	0x289b7ec6);
		R2(D, A, B, C, X(0),	11,	0xeaa127fa);
		R2(C, D, A, B, X(3),	16,	0xd4ef3085);
		R2(B, C, D, A, X(6),	23,	0x04881d05);
		R2(A, B, C, D, X(9),	4,	0xd9d4d039);
		R2(D, A, B, C, X(12),	11,	0xe6db99e5);
		R2(C, D, A, B, X(15),	16,	0x1fa27cf8);
		R2(B, C, D, A, X(2),	23,	0xc4ac5665);
		/* Round 3 */
		R3(A, B, C, D, X(0),	6,	0xf4292244);
		R3(D, A, B, C, X(7),	10,	0x432aff97);
		R3(C, D, A, B, X(14),	15,	0xab9423a7);
		R3(B, C, D, A, X(5),	21,	0xfc93a039);
		R3(A, B, C, D, X(12),	6,	0x655b59c3);
		R3(D, A, B, C, X(3),	10,	0x8f0ccc92);
		R3(C, D, A, B, X(10),	15,	0xffeff47d);
		R3(B, C, D, A, X(1),	21,	0x85845dd1);
		R3(A, B, C, D, X(8),	6,	0x6fa87e4f);
		R3(D, A, B, C, X(15),	10,	0xfe2ce6e0);
		R3(C, D, A, B, X(6),	15,	0xa3014314);
		R3(B, C, D, A, X(13),	21,	0x4e0811a1);
		R3(A, B, C, D, X(4),	6,	0xf7537e82);
		R3(D, A, B, C, X(11),	10,	0xbd3af235);
		R3(C, D, A, B, X(2),	15,	0x2ad7d2bb);
		R3(B, C, D, A, X(9),	21,	0xeb86d391);

		A = c->A += A;
		B = c->B += B;
		C = c->C += C;
		D = c->D += D;
	}
	return;
}
