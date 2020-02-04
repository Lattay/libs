/* Written in 2020 by Théo Cavignac based on KISS64 algorithm from
 * this forum post:
 * https://www.thecodingforums.com/threads/64-bit-kiss-rngs.673657/
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * 
 * See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

const uint32_t seed_length = 5 * 8;
uint64_t next(void);
void seed(uint64_t seed[5]);

#ifdef KISS64_IMPLEMENTATION
static uint64_t s[5] = {
  1234567890987654321ULL,
  362436362436362436ULL,
  1066149217761810ULL,
  0,
  12345567890987654321ULL
};

void seed(uint64_t seed[5]){
  s[0] = seed[0];
  s[1] = seed[1];
  s[2] = seed[2];
  s[3] = seed[3];
  s[4] = seed[4];
}

uint64_t next(void) {
  s[3] = (s[0] << 58) + s[4];
  s[4] = (s[0] >> 6);
  s[0] += s[5];
  s[4] += s[0] < s[3];

  s[1] ^= (s[1] << 13);
  s[1] ^= (s[1] >> 17);
  s[1] ^= (s[1] <<43);
  s[2] = 6906969069LL * s[2] + 1234567;

  return s[0] + s[1] + s[2];
}
#endif
