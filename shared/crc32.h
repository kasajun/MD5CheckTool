#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CRC32_H_267A182D_0A1C_426d_B3CA_F99E82A712E5__
#define __CRC32_H_267A182D_0A1C_426d_B3CA_F99E82A712E5__

#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

unsigned int crc32(unsigned int crc, const unsigned char* buf, size_t len);

int crc32_Init(unsigned int* c);
int crc32_Update(unsigned int* c, const void* data, size_t len);
int crc32_Final(unsigned char* md, unsigned int* c);

#ifdef  __cplusplus
}
#endif
#endif
