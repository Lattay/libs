/* Written in 2020 by Théo Cavignac based on Adler-32 Wikipedia page
 * https://en.wikipedia.org/wiki/Adler-32
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * 
 * See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdlib.h>
#include <stdint.h>

// expect a null terminated string
uint32_t hash(const char* content);

// use a length parameter to determine the length of the buffer
uint32_t hashn(const uint8_t* content, size_t length);

#ifdef ADLER_32_IMPLEMENTATION

static const uint16_t ADLER_MOD = 0xFFF1;

uint32_t hash(const char* content) {
  uint16_t a = 1, b = 0;
  while (*content != 0) {
    a = (a + *(content++)) % ADLER_MOD;
    b = (b + a) % ADLER_MOD;
  }
  return (b << 16) | a;
}

uint32_t hashn(const uint8_t* content, size_t length) {
  uint16_t a = 1, b = 0;
  for (int i = 0; i < length; i++) {
    a = (a + content[i]) % ADLER_MOD;
    b = (b + a) % ADLER_MOD;
  }
  return (b << 16) | a;
}

#endif
