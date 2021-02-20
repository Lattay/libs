#include <stdlib.h>
#include <stdio.h>

#define KISS64 0
#define XOSHIROPLUS 1
#define XOSHIROSTAR 2

#define ADLER_32 0
#define ADLER_32X 1
#define LCH32 2
#define CRC32 3

#ifndef USE_RNG
#define USE_RNG KISS64
#endif

#ifndef USE_HASH
#define USE_HASH ADLER_32
#endif

#if USE_RNG == KISS64
#define KISS64_IMPLEMENTATION
#include "../rng/kiss64.h"
#endif

#if USE_RNG == XOSHIROPLUS
#define XOSHIRO_IMPLEMENTATION
#include "../rng/xoshiro256.h"
#endif
#if USE_RNG == XOSHIROSTAR
#define XOSHIRO_IMPLEMENTATION
#include "../rng/xoshiro256starstar.h"
#endif


#if USE_HASH == ADLER_32
#define ADLER_32_IMPLEMENTATION
#include "../hash/adler_32.h"
#endif

#if USE_HASH == ADLER_32X
#define ADLER_32X_IMPLEMENTATION
#include "../hash/adler_32x.h"
#endif

#if USE_HASH == LCH32
#define LCH32_IMPLEMENTATION
#include "../hash/lch32.h"
#endif

#if USE_HASH == CRC32
#define CRC32_IMPLEMENTATION
#include "../hash/crc32.h"
#endif

#ifndef DATA_LENGTH
#define DATA_LENGTH 32
#endif

const int N = 10000;
const int M = DATA_LENGTH;

uint8_t crand(){
  return (next() >> 24);
}

int main(int argc, char** argv){

  FILE* f = fopen("/dev/random", "r");
  char s[seed_length];
  fread(s, 1, seed_length, f);
  fclose(f);
  seed((uint64_t*) s);

  char data[M+1];
  char repr[2*M+1];

  for(int i = 0; i < N; i++){
    for(int j = 0; j < M; j++){
      unsigned char c = crand();
      data[j] = c;
      repr[2*j] = 'a' + (c & 0xF);
      repr[2*j+1] = 'a' + ((c >> 4) & 0xF);
    }
    data[M] = 0;
    repr[2*M] = 0;
    printf("%s %X\n", repr, hash(data));
  }
  return 0;
}

