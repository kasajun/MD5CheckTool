#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __CRC32C_H_267A182D_0A1C_426d_B3CA_F99E82A712E5__
#define __CRC32C_H_267A182D_0A1C_426d_B3CA_F99E82A712E5__

#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

	int crc32c_Init(unsigned int* c);
	int crc32c_Update(unsigned int* c, const void* data, size_t len);
	int crc32c_Final(unsigned char* md, unsigned int* c);
	int crc32c_SSE4_2_Update(unsigned int* c, const void* data, size_t len);

#ifdef  __cplusplus
}

#endif
#endif
