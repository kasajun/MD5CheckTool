/*
 * XXH - Extremely Fast XXH algorithm
 * Copyright (C) 2012-2021 Yann Collet
 *
 * BSD 2-Clause License (https://www.opensource.org/licenses/bsd-license.php)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following disclaimer
 *      in the documentation and/or other materials provided with the
 *      distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You can contact the author at:
 *   - XXH homepage: https://www.xxhash.com
 *   - XXH source repository: https://github.com/Cyan4973/XXH
 */

 /*
  * xxhash.c instantiates functions defined in xxhash.h
  */

#if _MSC_VER > 1600

#define XXH_STATIC_LINKING_ONLY		/* access advanced declarations */
#define XXH_INLINE_ALL
#define XXH_VECTOR					2
#define XXH_DISPATCH_AVX2			1

#include "xxhash/xxhash-0.8.3-e626a72.h"


int XXH3_AVX2_Init(XXH3_state_t* c)
{
	memset(c, 0, sizeof(XXH3_state_t));
	XXH3_64bits_reset((XXH3_state_t*)c);
	return 1;
}

int XXH3_AVX2_Update(XXH3_state_t* c, const void* data, size_t len)
{
	return XXH3_64bits_update((XXH3_state_t*)c, data, len);
}

int XXH3_AVX2_Final(unsigned char* md, XXH3_state_t* c)
{
	*(XXH64_hash_t*)md = XXH3_64bits_digest((XXH3_state_t*)c);
	return 1;
}


int XXH128_AVX2_Init(XXH3_state_t* c)
{
	memset(c, 0, sizeof(XXH3_state_t));
	XXH3_64bits_reset((XXH3_state_t*)c);
	return 1;
}

int XXH128_AVX2_Update(XXH3_state_t* c, const void* data, size_t len)
{
	return XXH3_128bits_update((XXH3_state_t*)c, data, len);
}

int XXH128_AVX2_Final(unsigned char* md, XXH3_state_t* c)
{
	*(XXH128_hash_t*)md = XXH3_128bits_digest((XXH3_state_t*)c);
	return 1;
}

#endif
