/* crc32.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995-2022 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 *
 * This interleaved implementation of a CRC makes use of pipelined multiple
 * arithmetic-logic units, commonly found in modern CPU cores. It is due to
 * Kadatch and Jenkins (2010). See doc/crc-doc.1.0.pdf in this distribution.
 */

#include <string.h>
#include "crc32.h"
#define N 5

#if defined(_M_X64) && _MSC_VER > 1600
#  define W 8
#  define z_size_t	size_t
#  define z_word_t size_t
#else
#  define W 4
#  define z_size_t	unsigned int
#  define z_word_t unsigned int
#endif

#ifndef z_crc_t
#define z_crc_t unsigned int
#endif

/* CRC polynomial. */
#define POLY 0xedb88320         /* p(x) reflected, with x^32 implied */

#include "crc32_table.h"


#ifdef W
static unsigned int crc_word(size_t data)
{
	int k;
	for (k = 0; k < W; k++) {
		data = (data >> 8) ^ crc_table[data & 0xff];
	}
	return (unsigned int)data;
}
#endif

/* ========================================================================= */
unsigned int crc32(unsigned int crc, const unsigned char* buf, size_t len)
{
	/* Return initial CRC, if requested. */

	/* Pre-condition the CRC */
	crc ^= 0xffffffff;

#ifdef W

	/* If provided enough bytes, do a braided CRC calculation. */
	if (len >= N * W + W - 1)
	{
		z_size_t blks;
		z_word_t const* words;
		int k;
		z_crc_t crc0;
		z_word_t word0;
		z_crc_t crc1;
		z_word_t word1;
		z_crc_t crc2;
		z_word_t word2;
		z_crc_t crc3;
		z_word_t word3;
		z_crc_t crc4;
		z_word_t word4;

		/* Compute the CRC up to a size_t boundary. */
		/*
		while (len && ((size_t)buf & (W - 1)) != 0)
		{
			len--;
			crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
		}
		*/

		/* Compute the CRC on as many N size_t blocks as are available. */
		blks = len / (N * W);
		len -= blks * N * W;
		words = (z_word_t const*)buf;

		/* Do endian check at execution time instead of compile time, since ARM
		   processors can change the endianess at execution time. If the
		   compiler knows what the endianess will be, it can optimize out the
		   check and the unused branch. */

		/* Initialize the CRC for each braid. */
		crc0 = crc;
		crc1 = 0;
		crc2 = 0;
		crc3 = 0;
		crc4 = 0;

		/*
		  Process the first blks-1 blocks, computing the CRCs on each braid
		  independently.
		 */
		while (--blks)
		{
			/* Load the word for each braid into registers. */
			word0 = crc0 ^ words[0];
			word1 = crc1 ^ words[1];
			word2 = crc2 ^ words[2];
			word3 = crc3 ^ words[3];
			word4 = crc4 ^ words[4];
			words += N;

			/* Compute and update the CRC for each word. The loop should
			   get unrolled. */
			crc0 = crc_braid_table[0][word0 & 0xff];
			crc1 = crc_braid_table[0][word1 & 0xff];
			crc2 = crc_braid_table[0][word2 & 0xff];
			crc3 = crc_braid_table[0][word3 & 0xff];
			crc4 = crc_braid_table[0][word4 & 0xff];

			for (k = 1; k < W; k++)
			{
				crc0 ^= crc_braid_table[k][(word0 >> (k << 3)) & 0xff];
				crc1 ^= crc_braid_table[k][(word1 >> (k << 3)) & 0xff];
				crc2 ^= crc_braid_table[k][(word2 >> (k << 3)) & 0xff];
				crc3 ^= crc_braid_table[k][(word3 >> (k << 3)) & 0xff];
				crc4 ^= crc_braid_table[k][(word4 >> (k << 3)) & 0xff];
			}
		}

		/*
		  Process the last block, combining the CRCs of the N braids at the
		  same time.
		 */
		crc = crc_word(crc0 ^ words[0]);
		crc = crc_word(crc1 ^ words[1] ^ crc);
		crc = crc_word(crc2 ^ words[2] ^ crc);
		crc = crc_word(crc3 ^ words[3] ^ crc);
		crc = crc_word(crc4 ^ words[4] ^ crc);
		words += N;

		/*
		  Update the pointer to the remaining bytes to process.
		 */
		buf = (unsigned char const*)words;
	}

#endif /* W */

	/* Complete the computation of the CRC on any remaining bytes. */
	while (len >= 8)
	{
		len -= 8;
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
	}

	while (len)
	{
		len--;
		crc = (crc >> 8) ^ crc_table[(crc ^ *buf++) & 0xff];
	}

	/* Return the CRC, post-conditioned. */
	return crc ^ 0xffffffff;
}

int crc32_Init(unsigned int* c)
{
	memset(c, 0, sizeof(unsigned int));
	return 1;
}

int crc32_Update(unsigned int* c, const void* data, size_t len)
{
	*(unsigned int*)c = crc32(*(unsigned int*)c, (unsigned char*)data, len);
	return 1;
}

int crc32_Final(unsigned char* md, unsigned int* c)
{
	memcpy(md, c, sizeof(unsigned int));
	return 1;
}
