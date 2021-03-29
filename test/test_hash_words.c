#include <stdlib.h>
#include <stdio.h>

#define ADLER_32 0
#define ADLER_32X 1
#define LCH32 2
#define CRC32 3

#ifndef USE_HASH
#define USE_HASH ADLER_32
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

const int M = 128;

int main(int argc, char** argv) {
  char* line = NULL;
  size_t len = 0;
  int read;

  while ((read = getline(&line, &len, stdin)) != -1) {
    line[read - 1] = 0;
    printf("%s %X\n", line, hash(line));
  }
  free(line);
  return 0;
}
