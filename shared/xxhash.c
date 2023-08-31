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

#define XXH_STATIC_LINKING_ONLY		/* access advanced declarations */
//#define XXH_IMPLEMENTATION		/* access definitions */
#define XXH_INLINE_ALL
#define XXH_VECTOR					0

#include "xxhash-0.8.3.h"


int XXH32_Init(XXH32_state_t* c)
{
	memset(c, 0, sizeof(XXH32_state_t));
	XXH32_reset((XXH32_state_t*)c, 0);
	return 1;
}

int XXH32_Update(XXH32_state_t* c, const void* data, size_t len)
{
	return XXH32_update((XXH32_state_t*)c, data, len);
}

int XXH32_Final(unsigned char* md, XXH32_state_t* c)
{
	*(XXH32_hash_t*)md = XXH32_digest((XXH32_state_t*)c);
	return 1;
}


int XXH64_Init(XXH64_state_t* c)
{
	memset(c, 0, sizeof(XXH64_state_t));
	XXH64_reset((XXH64_state_t*)c, 0);
	return 1;
}

int XXH64_Update(XXH64_state_t* c, const void* data, size_t len)
{
	return XXH64_update((XXH64_state_t*)c, data, len);
}

int XXH64_Final(unsigned char* md, XXH64_state_t* c)
{
	*(XXH64_hash_t*)md = XXH64_digest((XXH64_state_t*)c);
	return 1;
}


int XXH3_Init(XXH3_state_t* c)
{
	memset(c, 0, sizeof(XXH3_state_t));
	XXH3_64bits_reset((XXH3_state_t*)c);
	return 1;
}

int XXH3_Update(XXH3_state_t* c, const void* data, size_t len)
{
	return XXH3_64bits_update((XXH3_state_t*)c, data, len);
}

int XXH3_Final(unsigned char* md, XXH3_state_t* c)
{
	*(XXH64_hash_t*)md = XXH3_64bits_digest((XXH3_state_t*)c);
	return 1;
}


int XXH128_Init(XXH3_state_t* c)
{
	memset(c, 0, sizeof(XXH3_state_t));
	XXH3_64bits_reset((XXH3_state_t*)c);
	return 1;
}

int XXH128_Update(XXH3_state_t* c, const void* data, size_t len)
{
	return XXH3_128bits_update((XXH3_state_t*)c, data, len);
}

int XXH128_Final(unsigned char* md, XXH3_state_t* c)
{
	*(XXH128_hash_t*)md = XXH3_128bits_digest((XXH3_state_t*)c);
	return 1;
}


#if _MSC_VER == 1400 && defined(_DEBUG)
#if defined(_M_X64) || defined(_M_IA64) && defined(_WIN64)
unsigned __int64 _umul128(unsigned __int64 __a, unsigned __int64 __b, unsigned __int64 *__productHi)
{
  // TRANSITION, VSO-634761
  // The casts here help MSVC to avoid calls to the __allmul library function.
  const unsigned int __aLo = (unsigned int)(__a);
  const unsigned int __aHi = (unsigned int)(__a >> 32);
  const unsigned int __bLo = (unsigned int)(__b);
  const unsigned int __bHi = (unsigned int)(__b >> 32);

  const unsigned __int64 __b00 = (unsigned __int64)(__aLo) * __bLo;
  const unsigned __int64 __b01 = (unsigned __int64)(__aLo) * __bHi;
  const unsigned __int64 __b10 = (unsigned __int64)(__aHi) * __bLo;
  const unsigned __int64 __b11 = (unsigned __int64)(__aHi) * __bHi;

  const unsigned int __b00Lo = (unsigned int)(__b00);
  const unsigned int __b00Hi = (unsigned int)(__b00 >> 32);

  const unsigned __int64 __mid1 = __b10 + __b00Hi;
  const unsigned int __mid1Lo = (unsigned int)(__mid1);
  const unsigned int __mid1Hi = (unsigned int)(__mid1 >> 32);

  const unsigned __int64 __mid2 = __b01 + __mid1Lo;
  const unsigned int __mid2Lo = (unsigned int)(__mid2);
  const unsigned int __mid2Hi = (unsigned int)(__mid2 >> 32);

  const unsigned __int64 __pHi = __b11 + __mid1Hi + __mid2Hi;
  const unsigned __int64 __pLo = ((unsigned __int64)(__mid2Lo) << 32) | __b00Lo;

  *__productHi = __pHi;
  return __pLo;
}
#endif
#endif
