/*
 * Adapted in 2021 by Théo Cavignac from preious work of Ryo Suzuki
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * 
 * See <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * Original license of siv::PerlinNoise written in C++ and largely at the
 * origin of this code (see https://github.com/Reputeless/PerlinNoise for the
 * original files):
 *
 * siv::PerlinNoise
 * Perlin noise library for modern C++
 * 
 * Copyright (C) 2013-2020 Ryo Suzuki <reputeless@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* This is an implementation of Perlin noise for up to 3D in the form of a
 * single file header only C99 program.
 *
 * To use it include it in your project.
 * In one and only one of the file including this header, before the include
 * directive, add #define PERLIN_IMPL
 */

#ifndef PERLIN_H
#define PERLIN_H
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef float val_t;
#define vfloor floorf

typedef uint8_t* perlin_state;

perlin_state new_state(uint64_t seed);
void free_state(perlin_state s);

void reseed(uint64_t seed);

/* basic noise functions */
val_t noise_1D(val_t x, uint32_t octaves);
val_t noise_2D(val_t x, val_t y, uint32_t octaves);
val_t noise_3D(val_t x, val_t y, val_t z, uint32_t octaves);

/* nn stands for "not normed": octaves are accumulated but the result is
 * not normed */
val_t noise_1D_nn(val_t x, uint32_t octaves);
val_t noise_2D_nn(val_t x, val_t y, uint32_t octaves);
val_t noise_3D_nn(val_t x, val_t y, val_t z, uint32_t octaves);

/* "raw" noise: only one octave */
val_t noise_1D_raw(val_t x);
val_t noise_2D_raw(val_t x, val_t y);
val_t noise_3D_raw(val_t x, val_t y, val_t z);

val_t noise_3D_cstm(val_t x, val_t y, val_t z, perlin_state s);

#endif
#ifdef PERLIN_IMPL

/* Fast and simple random number generator from 
 * https://github.com/Lattay/libs/blob/master/rng/splitmix64.h
 *
 * Originally written in 2015 by Sebastiano Vigna (vigna@acm.org) and
 * under CC-0 license.
 * 
 * This RNG is not the best when it comes to period, but this particular
 * application never ask for more than 256 numbers anyway.
 */

/* RNG state */
static uint64_t rng_state;

static inline void seed(uint64_t s) {
  rng_state = s;
}

static uint64_t next(void) {
  rng_state += UINT64_C(0x9E3779B97F4A7C15);
  const uint64_t a =
      (rng_state ^ (rng_state >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  const uint64_t b = (a ^ (a >> 27)) * UINT64_C(0x94D049BB133111EB);
  return b ^ (b >> 31);
}

/* 
 * Perlin noise implementation
 */

/* Perlin table */
static uint8_t state[512];

static inline val_t fade(val_t t) {
  /* Cheap sigmoid (basically a smooth version of round between 0 and 1)
   * 6 t^5 - 15 t^4 + 10 t^3 */
  return t * t * t * (t * (t * 6 - 15) + 10);
}

static inline val_t lerp(val_t t, val_t a, val_t b) {
  /* linear interpolation: [0, 1] -> [a, b] */
  return a + t * (b - a);
}

static inline val_t grad(uint8_t hash, val_t x, val_t y, val_t z) {
  const uint8_t h = hash & 15;
  const val_t u = h < 8 ? x : y;
  const val_t v = h < 4 ? y : h == 12 || h == 14 ? x : z;
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

static inline val_t inv_weight(uint32_t octaves) {
  if (octaves == 0) return 1;
  return 0.5 / (1.0 - pow(0.5, octaves)); /* 1/(\sum_{k=0}^{octaves - 1} 0.5^k) */
}

/* Fisher-Yates/Knuth shuffle for */
static inline void shuffle256(uint8_t array[256]) {
  for (uint8_t i = 255; i > 0; --i) {
    /* shift because lower bits are not as random as the others */
    uint8_t j = (next() >> 8) % i;  /* j in [0, i - 1] */
    /* swap array[i] and array[j] */
    uint8_t v = array[i];
    array[i] = array[j];
    array[j] = v;
  }
}

static inline int32_t ifloor(val_t f) {
  return vfloor(f);
}

static inline void seed_state(uint64_t sd, perlin_state s) {
  seed(sd);
  for (size_t i = 0; i < 256; ++i) {
    s[i] = i;
  }

  shuffle256(s);

  for (size_t i = 0; i < 256; ++i) {
    s[256 + i] = s[i];
  }
}

/* rebuild the Perlin's table from a 32b seed */
void reseed(uint64_t s) {
  seed_state(s, state);
}

perlin_state new_state(uint64_t seed) {
  perlin_state s = malloc(512);
  seed_state(seed, s);
  return s;
}

void free_state(perlin_state s) {
  free(s);
}

val_t noise_3D_raw(val_t x, val_t y, val_t z) {
  return noise_3D_cstm(x, y, z, state);
}

val_t noise_3D_cstm(val_t x, val_t y, val_t z, perlin_state s) {
  const int fx = floor(x);
  const int fy = floor(y);
  const int fz = floor(z);
  const uint8_t X = fx;
  const uint8_t Y = fy;
  const uint8_t Z = fz;

  x -= fx;
  y -= fy;
  z -= fz;

  /* map x from [-inf, +inf] to [0, 1] by translation
   * then "fade" it to get u */
  const val_t u = fade(x);
  const val_t v = fade(y);
  const val_t w = fade(z);

  const uint32_t A = s[X] + Y;
  const uint32_t AA = s[A] + Z;
  const uint32_t AB = s[A + 1] + Z;
  const uint32_t B = s[X + 1] + Y;
  const uint32_t BA = s[B] + Z;
  const uint32_t BB = s[B + 1] + Z;

  return lerp(w,
              lerp(v,
                   lerp(u, grad(s[AA], x, y, z), grad(s[BA], x - 1, y, z)),
                   lerp(u, grad(s[AB], x, y - 1, z),
                        grad(s[BB], x - 1, y - 1, z))
              ), lerp(v,
                      lerp(u, grad(s[AA + 1], x, y, z - 1),
                           grad(s[BA + 1], x - 1, y, z - 1)), lerp(u,
                                                                   grad(s
                                                                        [AB +
                                                                         1], x,
                                                                        y - 1,
                                                                        z - 1),
                                                                   grad(s
                                                                        [BB +
                                                                         1],
                                                                        x - 1,
                                                                        y - 1,
                                                                        z - 1))
              )
      );
}

val_t noise_1D_raw(val_t x) {
  return noise_3D_raw(x, 0, 0);
}

val_t noise_2D_raw(val_t x, val_t y) {
  return noise_3D_raw(x, y, 0);
}

val_t noise_1D_nn(val_t x, uint32_t octaves) {
  val_t result = 0;
  val_t amp = 1;

  for (uint32_t i = 0; i < octaves; ++i) {
    result += noise_1D_raw(x) * amp;
    x *= 2;
    amp /= 2;
  }

  return result;
}

val_t noise_2D_nn(val_t x, val_t y, uint32_t octaves) {
  val_t result = 0;
  val_t amp = 1;

  for (uint32_t i = 0; i < octaves; ++i) {
    result += noise_2D_raw(x, y) * amp;
    x *= 2;
    y *= 2;
    amp /= 2;
  }

  return result;
}

val_t noise_3D_nn(val_t x, val_t y, val_t z, uint32_t octaves) {
  val_t result = 0;
  val_t amp = 1;

  for (int32_t i = 0; i < octaves; ++i) {
    result += noise_3D_raw(x, y, z) * amp;
    x *= 2;
    y *= 2;
    z *= 2;
    amp /= 2;
  }

  return result;
}

val_t noise_1D(val_t x, uint32_t octaves) {
  return noise_1D_nn(x, octaves) * inv_weight(octaves);
}

val_t noise_2D(val_t x, val_t y, uint32_t octaves) {
  return noise_2D_nn(x, y, octaves) * inv_weight(octaves);
}

val_t noise_3D(val_t x, val_t y, val_t z, uint32_t octaves) {
  return noise_3D_nn(x, y, z, octaves) * inv_weight(octaves);
}

#endif
