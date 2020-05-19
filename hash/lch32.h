/* Written in 2020 by Théo Cavignac
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * 
 * See <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * LCH32: Lattay's Clunky Hash 
 * LCH32 aims at being a reasonable hash algorithm for very short data.
 * As far as tests goes I have to say I am pretty happy with it. It seems to
 * keep a consistent 0.5% of collisions in same content of a given size from
 * 2 bytes to 4096 bytes.
 * It is inspired by XoShiRo algorithm. */

#include <stdlib.h>
#include <stdint.h>

// expect a null terminated string
uint32_t hash(const char* content);

// use a length parameter to determine the length of the buffer
uint32_t hashn(const uint8_t* content, size_t length);

#ifdef LCH32_IMPLEMENTATION

static const uint8_t ash_magic[5] = {0x96, 0x47, 0xe2, 0xbc, 0x8d} ;

static inline uint32_t rotl(const uint32_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

uint32_t hash(const char* content){
  size_t length = 0;
  while(*(content + length) != 0) length++;
  return hashn(content, length);
}

uint32_t hashn(const uint8_t* content, size_t length){
  uint32_t buf[3] = {0x51b73064, 0x9f4a5705, 0x7b049943};
  uint32_t digest = 0x698a3c57;

  for(int i = 0; i < length; i+=2){
    uint8_t a = content[i];
    uint8_t b = (i + 1 < length) ? content[i + 1] : content[i % length];
    uint16_t block1 = a | (b << 8);
    uint32_t block2 = rotl(block1, 5);

    uint64_t t = buf[2] + buf[0];
    buf[1] ^= block1 | (block2 << 16);

    buf[2] ^= buf[0];
    digest ^= buf[1];
    buf[1] ^= buf[2];
    buf[0] ^= digest;

    buf[0] ^= t;


    digest = rotl(digest, 23);
  }
  return digest;
}

#endif
