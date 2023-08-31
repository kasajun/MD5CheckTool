#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CPUID_H_A5814456_D164_44bf_A932_8518A754B629__
#define __CPUID_H_A5814456_D164_44bf_A932_8518A754B629__

#include <stddef.h>

//
// CPUID 情報
//
// https://www.wdic.org/w/SCI/cpuid%20%28x86%29
// https://www.wdic.org/w/SCI/SSE
//

#define CPUID_IS_MMX			(OPENSSL_ia32cap_P[0] & 0x00800000)
#define CPUID_IS_SSE			(OPENSSL_ia32cap_P[0] & 0x02000000)
#define CPUID_IS_SSE2			(OPENSSL_ia32cap_P[0] & 0x04000000)
#define CPUID_IS_SSE3			(OPENSSL_ia32cap_P[1] & 0x00000001)
#define CPUID_IS_SSSE3			(OPENSSL_ia32cap_P[1] & 0x00000200)
#define CPUID_IS_SSE4_1			(OPENSSL_ia32cap_P[1] & 0x00080000)
#define CPUID_IS_SSE4_2			(OPENSSL_ia32cap_P[1] & 0x00100000)
#define CPUID_IS_AVX			(OPENSSL_ia32cap_P[1] & 0x10000000)
#define CPUID_IS_AVX2			(OPENSSL_ia32cap_P[2] & 0x00000020)		// ((OPENSSL_ia32cap_P[1] & 0x50000000) == 0x50000000)
#define CPUID_IS_AVX512			(OPENSSL_ia32cap_P[2] & 0x00010000)
#define CPUID_IS_SHAEXT			(OPENSSL_ia32cap_P[2] & 0x20000000)


#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int OPENSSL_ia32cap_P[4];
/*
OPENSSL_ia32cap_P[0] = cpuid(1)->EDX | 0x40000400
OPENSSL_ia32cap_P[1] = cpuid(1)->ECX & 0xFFFFF7FF
OPENSSL_ia32cap_P[2] = cpuid(7)->EBX
OPENSSL_ia32cap_P[3] = cpuid(7)->ECX

※Core i3 12100
OPENSSL_ia32cap_P[0] = 0xFFEBFFFF;
OPENSSL_ia32cap_P[1] = 0x7FFAF3BF;
OPENSSL_ia32cap_P[2] = 0x239C27EB;
OPENSSL_ia32cap_P[3] = 0x98C007AC;

※Core i5 3550
OPENSSL_ia32cap_P[0] = 0x478BFFFF;
OPENSSL_ia32cap_P[1] = 0x469A2203;
OPENSSL_ia32cap_P[2] = 0x00000001;
OPENSSL_ia32cap_P[3] = 0x00000000;
*/

/*
// Windows 95 で実行時はSSEを使わないようにビットをクリアしてください。
#include "cpuid.h"
#include "os.h"

OPENSSL_cpuid_setup1();

if (IsWin95()) {
	OPENSSL_ia32cap_P[1] &= ~(0x200);
}

*/

void OPENSSL_cpuid_setup1(void);
void OPENSSL_cleanse(void* ptr, size_t len);
int CRYPTO_memcmp(const void* in_a, const void* in_b, size_t len);
unsigned int OPENSSL_rdtsc(void);
size_t OPENSSL_instrument_bus(unsigned int* out, size_t cnt);
size_t OPENSSL_instrument_bus2(unsigned int* out, size_t cnt, size_t max);

#ifdef __cplusplus
}
#endif
#endif
