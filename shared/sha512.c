/*
 * Copyright 2004-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*-
* IMPLEMENTATION NOTES.
*
* As you might have noticed 32-bit hash algorithms:
*
* - permit SHA_LONG to be wider than 32-bit
* - optimized versions implement two transform functions: one operating
*   on [aligned] data in host byte order and one - on data in input
*   stream byte order;
* - share common byte-order neutral collector and padding function
*   implementations, ../md32_common.h;
*
* Neither of the above applies to this SHA-512 implementations. Reasons
* [in reverse order] are:
*
* - it's the only 64-bit hash algorithm for the moment of this writing,
*   there is no need for common collector/padding implementation [yet];
* - by supporting only one transform function [which operates on
*   *aligned* data in input stream byte order, big-endian in this case]
*   we minimize burden of maintenance in two ways: a) collector/padding
*   function is simpler; b) only one transform function to stare at;
* - SHA_LONG64 is required to be exactly 64-bit in order to be able to
*   apply a number of optimizations to mitigate potential performance
*   penalties caused by previous design decision;
*
* Caveat lector.
*
* Implementation relies on the fact that "long long" is 64-bit on
* both 32- and 64-bit platforms. If some compiler vendor comes up
* with 128-bit long long, adjustment to sha.h would be required.
* As this implementation relies on 64-bit integer type, it's totally
* inappropriate for platforms which don't support it, most notably
* 16-bit platforms.
*/
#include <string.h>
#include "sha512.h"
#include "cpuid.h"
#include "c20.h"


#define SHA512_BLOCK_CAN_MANAGE_UNALIGNED_DATA

int SHA512_224_Init(SHA512_CTX* c)
{
	c->h[0] = U64(0x8c3d37c819544da2);
	c->h[1] = U64(0x73e1996689dcd4d6);
	c->h[2] = U64(0x1dfab7ae32ff9c82);
	c->h[3] = U64(0x679dd514582f9fcf);
	c->h[4] = U64(0x0f6d2b697bd44da8);
	c->h[5] = U64(0x77e36f7304c48942);
	c->h[6] = U64(0x3f9d85a86a1d36c8);
	c->h[7] = U64(0x1112e6ad91d692a1);

	c->Nl = 0;
	c->Nh = 0;
	c->num = 0;
	c->md_len = SHA224_DIGEST_LENGTH;
	return 1;
}

int SHA512_256_Init(SHA512_CTX* c)
{
	c->h[0] = U64(0x22312194fc2bf72c);
	c->h[1] = U64(0x9f555fa3c84c64c2);
	c->h[2] = U64(0x2393b86b6f53b151);
	c->h[3] = U64(0x963877195940eabd);
	c->h[4] = U64(0x96283ee2a88effe3);
	c->h[5] = U64(0xbe5e1e2553863992);
	c->h[6] = U64(0x2b0199fc2c85b8aa);
	c->h[7] = U64(0x0eb72ddc81c52ca2);

	c->Nl = 0;
	c->Nh = 0;
	c->num = 0;
	c->md_len = SHA256_DIGEST_LENGTH;
	return 1;
}

int SHA384_Init(SHA512_CTX* c)
{
	c->h[0] = U64(0xcbbb9d5dc1059ed8);
	c->h[1] = U64(0x629a292a367cd507);
	c->h[2] = U64(0x9159015a3070dd17);
	c->h[3] = U64(0x152fecd8f70e5939);
	c->h[4] = U64(0x67332667ffc00b31);
	c->h[5] = U64(0x8eb44a8768581511);
	c->h[6] = U64(0xdb0c2e0d64f98fa7);
	c->h[7] = U64(0x47b5481dbefa4fa4);

	c->Nl = 0;
	c->Nh = 0;
	c->num = 0;
	c->md_len = SHA384_DIGEST_LENGTH;
	return 1;
}

int SHA512_Init(SHA512_CTX* c)
{
	c->h[0] = U64(0x6a09e667f3bcc908);
	c->h[1] = U64(0xbb67ae8584caa73b);
	c->h[2] = U64(0x3c6ef372fe94f82b);
	c->h[3] = U64(0xa54ff53a5f1d36f1);
	c->h[4] = U64(0x510e527fade682d1);
	c->h[5] = U64(0x9b05688c2b3e6c1f);
	c->h[6] = U64(0x1f83d9abfb41bd6b);
	c->h[7] = U64(0x5be0cd19137e2179);

	c->Nl = 0;
	c->Nh = 0;
	c->num = 0;
	c->md_len = SHA512_DIGEST_LENGTH;
	return 1;
}

void sha512_block_data_order(SHA512_CTX* ctx, const void* in, size_t num);

int SHA512_Final(unsigned char* md, SHA512_CTX* c)
{
	unsigned char* p = (unsigned char*)c->u.p;
	size_t n = c->num;

	p[n] = 0x80;                /* There always is a room for one */
	n++;
	if (n > (sizeof(c->u) - 16))
	{
		memset(p + n, 0, sizeof(c->u) - n);
		n = 0;
		sha512_block_data_order(c, p, 1);
	}

	memset(p + n, 0, sizeof(c->u) - 16 - n);
	p[sizeof(c->u) - 1] = (unsigned char)(c->Nl);
	p[sizeof(c->u) - 2] = (unsigned char)(c->Nl >> 8);
	p[sizeof(c->u) - 3] = (unsigned char)(c->Nl >> 16);
	p[sizeof(c->u) - 4] = (unsigned char)(c->Nl >> 24);
	p[sizeof(c->u) - 5] = (unsigned char)(c->Nl >> 32);
	p[sizeof(c->u) - 6] = (unsigned char)(c->Nl >> 40);
	p[sizeof(c->u) - 7] = (unsigned char)(c->Nl >> 48);
	p[sizeof(c->u) - 8] = (unsigned char)(c->Nl >> 56);
	p[sizeof(c->u) - 9] = (unsigned char)(c->Nh);
	p[sizeof(c->u) - 10] = (unsigned char)(c->Nh >> 8);
	p[sizeof(c->u) - 11] = (unsigned char)(c->Nh >> 16);
	p[sizeof(c->u) - 12] = (unsigned char)(c->Nh >> 24);
	p[sizeof(c->u) - 13] = (unsigned char)(c->Nh >> 32);
	p[sizeof(c->u) - 14] = (unsigned char)(c->Nh >> 40);
	p[sizeof(c->u) - 15] = (unsigned char)(c->Nh >> 48);
	p[sizeof(c->u) - 16] = (unsigned char)(c->Nh >> 56);

	sha512_block_data_order(c, p, 1);

	/*
	if (md == 0) {
		return 0;
	}
	*/

	switch (c->md_len)
	{
		/* Let compiler decide if it's appropriate to unroll... */
	case SHA224_DIGEST_LENGTH:
		for (n = 0; n < SHA224_DIGEST_LENGTH / 8; n++)
		{
			SHA_LONG64 t = c->h[n];

			*(md++) = (unsigned char)(t >> 56);
			*(md++) = (unsigned char)(t >> 48);
			*(md++) = (unsigned char)(t >> 40);
			*(md++) = (unsigned char)(t >> 32);
			*(md++) = (unsigned char)(t >> 24);
			*(md++) = (unsigned char)(t >> 16);
			*(md++) = (unsigned char)(t >> 8);
			*(md++) = (unsigned char)(t);
		}
		/*
		 * For 224 bits, there are four bytes left over that have to be
		 * processed separately.
		 */
		{
			SHA_LONG64 t = c->h[SHA224_DIGEST_LENGTH / 8];

			*(md++) = (unsigned char)(t >> 56);
			*(md++) = (unsigned char)(t >> 48);
			*(md++) = (unsigned char)(t >> 40);
			*(md++) = (unsigned char)(t >> 32);
		}
		break;
	case SHA256_DIGEST_LENGTH:
		for (n = 0; n < SHA256_DIGEST_LENGTH / 8; n++)
		{
			SHA_LONG64 t = c->h[n];

			*(md++) = (unsigned char)(t >> 56);
			*(md++) = (unsigned char)(t >> 48);
			*(md++) = (unsigned char)(t >> 40);
			*(md++) = (unsigned char)(t >> 32);
			*(md++) = (unsigned char)(t >> 24);
			*(md++) = (unsigned char)(t >> 16);
			*(md++) = (unsigned char)(t >> 8);
			*(md++) = (unsigned char)(t);
		}
		break;
	case SHA384_DIGEST_LENGTH:
		for (n = 0; n < SHA384_DIGEST_LENGTH / 8; n++)
		{
			SHA_LONG64 t = c->h[n];

			*(md++) = (unsigned char)(t >> 56);
			*(md++) = (unsigned char)(t >> 48);
			*(md++) = (unsigned char)(t >> 40);
			*(md++) = (unsigned char)(t >> 32);
			*(md++) = (unsigned char)(t >> 24);
			*(md++) = (unsigned char)(t >> 16);
			*(md++) = (unsigned char)(t >> 8);
			*(md++) = (unsigned char)(t);
		}
		break;
	case SHA512_DIGEST_LENGTH:
		for (n = 0; n < SHA512_DIGEST_LENGTH / 8; n++)
		{
			SHA_LONG64 t = c->h[n];

			*(md++) = (unsigned char)(t >> 56);
			*(md++) = (unsigned char)(t >> 48);
			*(md++) = (unsigned char)(t >> 40);
			*(md++) = (unsigned char)(t >> 32);
			*(md++) = (unsigned char)(t >> 24);
			*(md++) = (unsigned char)(t >> 16);
			*(md++) = (unsigned char)(t >> 8);
			*(md++) = (unsigned char)(t);
		}
		break;
		/* ... as well as make sure md_len is not abused. */
	default:
		return 0;
	}

	return 1;
}

int SHA512_Update(SHA512_CTX* c, const void* _data, size_t len)
{
	SHA_LONG64 l;
	unsigned char* p = c->u.p;
	const unsigned char* data = (const unsigned char*)_data;

	/*
	if (len == 0) {
		return 1;
	}
	*/

	l = (c->Nl + (((SHA_LONG64) len) << 3)) & U64(0xffffffffffffffff);
	if (l < c->Nl) {
		c->Nh++;
	}
	if (sizeof(size_t) >= 8) { // warning C4127
		c->Nh += (((SHA_LONG64) len) >> 61);
	}
	c->Nl = l;

	if (c->num != 0)
	{
		size_t n = sizeof(c->u) - c->num;

		if (len < n)
		{
			memcpy(p + c->num, data, len);
			c->num += (unsigned int)len;
			return 1;
		}
		else
		{
			memcpy(p + c->num, data, n);
			c->num = 0;
			len -= n, data += n;
			sha512_block_data_order(c, p, 1);
		}
	}

	if (len >= sizeof(c->u))
	{
		sha512_block_data_order(c, data, len / sizeof(c->u)),
		data += len, len %= sizeof(c->u), data -= len;
	}

	if (len != 0) {
		memcpy(p, data, len), c->num = (int)len;
	}

	return 1;
}

void SHA512_Transform(SHA512_CTX* c, const unsigned char* data)
{
	sha512_block_data_order(c, data, 1);
}

unsigned char* SHA384(const unsigned char* d, size_t n, unsigned char* md)
{
	SHA512_CTX c;
	static unsigned char m[SHA384_DIGEST_LENGTH];

	IF_UNLIKELY(md == NULL) {
		md = m;
	}
	SHA384_Init(&c);
	SHA512_Update(&c, d, n);
	SHA512_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));
	return md;
}

unsigned char* SHA512(const unsigned char* d, size_t n, unsigned char* md)
{
	SHA512_CTX c;
	static unsigned char m[SHA512_DIGEST_LENGTH];

	IF_UNLIKELY(md == NULL) {
		md = m;
	}
	SHA512_Init(&c);
	SHA512_Update(&c, d, n);
	SHA512_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));
	return md;
}

unsigned char* SHA512_224(const unsigned char* d, size_t n, unsigned char* md)
{
	SHA512_CTX c;
	static unsigned char m[SHA512_DIGEST_LENGTH];

	IF_UNLIKELY(md == NULL) {
		md = m;
	}
	SHA512_224_Init(&c);
	SHA512_Update(&c, d, n);
	SHA512_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));
	return md;
}

unsigned char* SHA512_256(const unsigned char* d, size_t n, unsigned char* md)
{
	SHA512_CTX c;
	static unsigned char m[SHA512_DIGEST_LENGTH];

	IF_UNLIKELY(md == NULL) {
		md = m;
	}
	SHA512_256_Init(&c);
	SHA512_Update(&c, d, n);
	SHA512_Final(md, &c);
	OPENSSL_cleanse(&c, sizeof(c));
	return md;
}
