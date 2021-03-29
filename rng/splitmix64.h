/* Written in 2015 by Sebastiano Vigna (vigna@acm.org) To the extent possible
 * under law, the author has dedicated all copyright and related and
 * neighboring rights to this software to the public domain worldwide. This
 * software is distributed without any warranty.  See
 *
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * This is a fixed-increment version of Java 8's SplittableRandom generator
 * See http://dx.doi.org/10.1145/2714064.2660195 and
 * http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html
 * It is a very fast generator passing BigCrush, and it can be useful if
 * for some reason you absolutely want 64 bits of state; otherwise, we
 * rather suggest to use a xoroshiro128+ (for moderately parallel
 * computations) or xorshift1024* (for massively parallel computations)
 * generator.
 *
 * Extracted from https://github.com/svaarala/duktape/blob/master/misc/splitmix64.c
 * */

#ifndef SPLITMIX64_H
#define SPLITMIX64_H
#include <stdint.h>

void seed(uint64_t seed);

uint64_t next();

#endif
#ifdef SPLITMIX64_IMPL

uint64_t x;

void seed(uint64_t seed) {
  x = seed;
}

uint64_t next() {
  x += UINT64_C(0x9E3779B97F4A7C15);
  const uint64_t a = (x ^ (x >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  const uint64_t b = (a ^ (a >> 27)) * UINT64_C(0x94D049BB133111EB);
  return b ^ (b >> 31);
}
#endif
