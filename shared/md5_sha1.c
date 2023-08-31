/*
 * Copyright 2015-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * MD5 and SHA-1 low level APIs are deprecated for public use, but still ok for
 * internal use.
 */

#include <string.h>
#include "md5_sha1.h"
#include "cpuid.h"

#define EVP_CTRL_SSL3_MASTER_SECRET				0x1d


int MD5_SHA1_Init(MD5_SHA1_CTX *mctx)
{
	MD5_Init(&mctx->md5);
	return SHA1_Init(&mctx->sha1);
}

int MD5_SHA1_Update(MD5_SHA1_CTX *mctx, const void *data, size_t count)
{
	if (!MD5_Update(&mctx->md5, data, count)) {
		return 0;
	}
	return SHA1_Update(&mctx->sha1, data, count);
}

int MD5_SHA1_Final(unsigned char *md, MD5_SHA1_CTX *mctx)
{
	MD5_Final(md, &mctx->md5);
	return SHA1_Final(md + MD5_DIGEST_LENGTH, &mctx->sha1);
}

int MD5_SHA1_Ctrl(MD5_SHA1_CTX* mctx, int cmd, int mslen, void* ms)
{
	unsigned char padtmp[48];
	unsigned char md5tmp[MD5_DIGEST_LENGTH];
	unsigned char sha1tmp[SHA_DIGEST_LENGTH];

	if (cmd != EVP_CTRL_SSL3_MASTER_SECRET) {
		return -2;
	}

	if (mctx == NULL) {
		return 0;
	}

	/* SSLv3 client auth handling: see RFC-6101 5.6.8 */
	if (mslen != 48) {
		return 0;
	}

	/* At this point hash contains all handshake messages, update
	 * with master secret and pad_1.
	 */

	if (MD5_SHA1_Update(mctx, ms, mslen) <= 0) {
		return 0;
	}

	/* Set padtmp to pad_1 value */
	memset(padtmp, 0x36, sizeof(padtmp));

	if (!MD5_Update(&mctx->md5, padtmp, sizeof(padtmp))) {
		return 0;
	}

	if (!MD5_Final(md5tmp, &mctx->md5)) {
		return 0;
	}

	if (!SHA1_Update(&mctx->sha1, padtmp, 40)) {
		return 0;
	}

	if (!SHA1_Final(sha1tmp, &mctx->sha1)) {
		return 0;
	}

	/* Reinitialise context */

	if (!MD5_SHA1_Init(mctx)) {
		return 0;
	}

	if (MD5_SHA1_Update(mctx, ms, mslen) <= 0) {
		return 0;
	}

	/* Set padtmp to pad_2 value */
	memset(padtmp, 0x5c, sizeof(padtmp));

	if (!MD5_Update(&mctx->md5, padtmp, sizeof(padtmp))) {
		return 0;
	}

	if (!MD5_Update(&mctx->md5, md5tmp, sizeof(md5tmp))) {
		return 0;
	}

	if (!SHA1_Update(&mctx->sha1, padtmp, 40)) {
		return 0;
	}

	if (!SHA1_Update(&mctx->sha1, sha1tmp, sizeof(sha1tmp))) {
		return 0;
	}

	/* Now when ctx is finalised it will return the SSL v3 hash value */

	OPENSSL_cleanse(md5tmp, sizeof(md5tmp));
	OPENSSL_cleanse(sha1tmp, sizeof(sha1tmp));

	return 1;
}
