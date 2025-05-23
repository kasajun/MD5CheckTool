#if _MSC_VER > 1000
#pragma once
#endif

/*
 * xxHash - Extremely Fast Hash algorithm
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
 *   - xxHash homepage: https://www.xxhash.com
 *   - xxHash source repository: https://github.com/Cyan4973/xxHash
 */

#ifndef __XXHASH_H_36A2C202_4161_4B21_B2F7_0C0C68B4D73F__
#define __XXHASH_H_36A2C202_4161_4B21_B2F7_0C0C68B4D73F__

#include "xxhash-0.8.2-ctx.h"

#ifdef __cplusplus
extern "C" {
#endif

	int XXH32_Init(XXH32_state_t* c);
	int XXH32_Update(XXH32_state_t* c, const void* data, size_t len);
	int XXH32_Final(unsigned char* md, XXH32_state_t* c);

	int XXH64_Init(XXH64_state_t* c);
	int XXH64_Update(XXH64_state_t* c, const void* data, size_t len);
	int XXH64_Final(unsigned char* md, XXH64_state_t* c);

	int XXH3_Init(XXH3_state_t* c);
	int XXH3_Update(XXH3_state_t* c, const void* data, size_t len);
	int XXH3_Final(unsigned char* md, XXH3_state_t* c);

	int XXH128_Init(XXH3_state_t* c);
	int XXH128_Update(XXH3_state_t* c, const void* data, size_t len);
	int XXH128_Final(unsigned char* md, XXH3_state_t* c);

#ifdef __cplusplus
}
#endif
#endif
