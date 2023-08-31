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

#ifndef __XXHASH_CTX_H_36A2C202_4161_4B21_B2F7_0C0C68B4D73F__
#define __XXHASH_CTX_H_36A2C202_4161_4B21_B2F7_0C0C68B4D73F__

typedef unsigned long XXH32_hash_t;
typedef unsigned __int64 XXH64_hash_t;

 /*!
  * @internal
  * @brief Structure for XXH32 streaming API.
  *
  * @note This is only defined when @ref XXH_STATIC_LINKING_ONLY,
  * @ref XXH_INLINE_ALL, or @ref XXH_IMPLEMENTATION is defined. Otherwise it is
  * an opaque type. This allows fields to safely be changed.
  *
  * Typedef'd to @ref XXH32_state_t.
  * Do not access the members of this struct directly.
  * @see XXH64_state_s, XXH3_state_s
  */

typedef struct XXH32_state_t {
	XXH32_hash_t total_len_32; /*!< Total length hashed, modulo 2^32 */
	XXH32_hash_t large_len;    /*!< Whether the hash is >= 16 (handles @ref total_len_32 overflow) */
	XXH32_hash_t v[4];         /*!< Accumulator lanes */
	XXH32_hash_t mem32[4];     /*!< Internal buffer for partial reads. Treated as unsigned char[16]. */
	XXH32_hash_t memsize;      /*!< Amount of data in @ref mem32 */
	XXH32_hash_t reserved;     /*!< Reserved field. Do not read nor write to it. */
} XXH32_state_t;   /* typedef'd to XXH32_state_t */

/*!
 * @internal
 * @brief Structure for XXH64 streaming API.
 *
 * @note This is only defined when @ref XXH_STATIC_LINKING_ONLY,
 * @ref XXH_INLINE_ALL, or @ref XXH_IMPLEMENTATION is defined. Otherwise it is
 * an opaque type. This allows fields to safely be changed.
 *
 * Typedef'd to @ref XXH64_state_t.
 * Do not access the members of this struct directly.
 * @see XXH32_state_s, XXH3_state_s
 */
typedef struct XXH64_state_s {
	XXH64_hash_t total_len;    /*!< Total length hashed. This is always 64-bit. */
	XXH64_hash_t v[4];         /*!< Accumulator lanes */
	XXH64_hash_t mem64[4];     /*!< Internal buffer for partial reads. Treated as unsigned char[32]. */
	XXH32_hash_t memsize;      /*!< Amount of data in @ref mem64 */
	XXH32_hash_t reserved32;   /*!< Reserved field, needed for padding anyways*/
	XXH64_hash_t reserved64;   /*!< Reserved field. Do not read or write to it. */
} XXH64_state_t;   /* typedef'd to XXH64_state_t */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) /* >= C11 */
#  include <stdalign.h>
#  define XXH_ALIGN(n)      alignas(n)
#elif defined(__cplusplus) && (__cplusplus >= 201103L) /* >= C++11 */
/* In C++ alignas() is a keyword */
#  define XXH_ALIGN(n)      alignas(n)
#elif defined(__GNUC__)
#  define XXH_ALIGN(n)      __attribute__ ((aligned(n)))
#elif defined(_MSC_VER) && _MSC_VER > 1300
#  define XXH_ALIGN(n)      __declspec(align(n))
#else
#  define XXH_ALIGN(n)   /* disabled */
#endif

/* Old GCC versions only accept the attribute after the type in structures. */
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L))   /* C11+ */ \
	&& ! (defined(__cplusplus) && (__cplusplus >= 201103L)) /* >= C++11 */ \
	&& defined(__GNUC__)
#   define XXH_ALIGN_MEMBER(align, type) type XXH_ALIGN(align)
#else
#   define XXH_ALIGN_MEMBER(align, type) XXH_ALIGN(align) type
#endif

/*!
 * @brief The size of the internal XXH3 buffer.
 *
 * This is the optimal update size for incremental hashing.
 *
 * @see XXH3_64b_update(), XXH3_128b_update().
 */
#define XXH3_INTERNALBUFFER_SIZE 256

 /*!
  * @internal
  * @brief Default size of the secret buffer (and @ref XXH3_kSecret).
  *
  * This is the size used in @ref XXH3_kSecret and the seeded functions.
  *
  * Not to be confused with @ref XXH3_SECRET_SIZE_MIN.
  */
#define XXH3_SECRET_DEFAULT_SIZE 192

  /*!
   * @internal
   * @brief Structure for XXH3 streaming API.
   *
   * @note This is only defined when @ref XXH_STATIC_LINKING_ONLY,
   * @ref XXH_INLINE_ALL, or @ref XXH_IMPLEMENTATION is defined.
   * Otherwise it is an opaque type.
   * Never use this definition in combination with dynamic library.
   * This allows fields to safely be changed in the future.
   *
   * @note ** This structure has a strict alignment requirement of 64 bytes!! **
   * Do not allocate this with `malloc()` or `new`,
   * it will not be sufficiently aligned.
   * Use @ref XXH3_createState() and @ref XXH3_freeState(), or stack allocation.
   *
   * Typedef'd to @ref XXH3_state_t.
   * Do never access the members of this struct directly.
   *
   * @see XXH3_INITSTATE() for stack initialization.
   * @see XXH3_createState(), XXH3_freeState().
   * @see XXH32_state_s, XXH64_state_s
   */
typedef struct XXH3_state_s {
	XXH_ALIGN_MEMBER(64, XXH64_hash_t acc[8]);
	/*!< The 8 accumulators. See @ref XXH32_state_s::v and @ref XXH64_state_s::v */
	XXH_ALIGN_MEMBER(64, unsigned char customSecret[XXH3_SECRET_DEFAULT_SIZE]);
	/*!< Used to store a custom secret generated from a seed. */
	XXH_ALIGN_MEMBER(64, unsigned char buffer[XXH3_INTERNALBUFFER_SIZE]);
	/*!< The internal buffer. @see XXH32_state_s::mem32 */
	XXH32_hash_t bufferedSize;
	/*!< The amount of memory in @ref buffer, @see XXH32_state_s::memsize */
	XXH32_hash_t useSeed;
	/*!< Reserved field. Needed for padding on 64-bit. */
	size_t nbStripesSoFar;
	/*!< Number or stripes processed. */
	XXH64_hash_t totalLen;
	/*!< Total length hashed. 64-bit even on 32-bit targets. */
	size_t nbStripesPerBlock;
	/*!< Number of stripes per block. */
	size_t secretLimit;
	/*!< Size of @ref customSecret or @ref extSecret */
	XXH64_hash_t seed;
	/*!< Seed for _withSeed variants. Must be zero otherwise, @see XXH3_INITSTATE() */
	XXH64_hash_t reserved64;
	/*!< Reserved field. */
	const unsigned char* extSecret;
	/*!< Reference to an external secret for the _withSecret variants, NULL
	 *   for other variants. */
	 /* note: there may be some padding at the end due to alignment on 64 bytes */
} XXH3_state_t; /* typedef'd to XXH3_state_t */

#endif
