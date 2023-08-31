#include "cpuid.h"

unsigned int OPENSSL_ia32cap_P[4];
unsigned __int64 OPENSSL_ia32_cpuid(unsigned int*);

void OPENSSL_cpuid_setup1(void)
{
    static int trigger = 0;
    unsigned __int64 nRet;

    if (trigger) {
        return;
    }

    trigger = 1;
    nRet = OPENSSL_ia32_cpuid(OPENSSL_ia32cap_P);

    /*
     * |(1<<10) sets a reserved bit to signal that variable
     * was initialized already... This is to avoid interference
     * with cpuid snippets in ELF .init segment.
     */
    OPENSSL_ia32cap_P[0] = (unsigned int)nRet | (1 << 10);
    OPENSSL_ia32cap_P[1] = (unsigned int)(nRet >> 32);
	return;
}
