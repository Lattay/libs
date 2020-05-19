#include <stdlib.h>
#include <stdio.h>

#define ADLER_32 0
#define ADLER_32X 1
#define LCH32 2

#ifndef USE_HASH
#define USE_HASH LCH32
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

const char test_str1[50] = "abdTlhu7_-4e74)]=}+&~!:/;mpbgv  EA@mL.%++''(`09hj";
const char test_str2[50] = "cbdTlhu7_-4e74)]=}+&~!:/;mpbgv  EA@mL.%++''(`09hj";
const char test_str3[50] = "abdTlhu7_-4e74)]=}+&~!:/;mpbgv  EA@mL.%++''(`09hk";
const char test_str4[50] = "abdTlhu7_-4e74)]=}+&~!>/;mpbgv  EA@mL.%++''(`09hj";

const char test_str5[8] = "a#'po45";
const char test_str6[8] = "a#'po46";
const char test_str7[8] = "a3'po45";
const char test_str8[8] = "a#'`o45";

void test_string(const char* str, size_t n){
  printf("hashing %X, %s\n", hashn((uint8_t*)str, n), str);
  // printf("hash: %X\n", hash((uint8_t*)str));
}

int main(int argc, char** argv){
  test_string(test_str1, 49);
  test_string(test_str2, 49);
  test_string(test_str3, 49);
  test_string(test_str4, 49);

  printf("\nShorter messages:\n");
  test_string(test_str5, 7);
  test_string(test_str6, 7);
  test_string(test_str7, 7);
  test_string(test_str8, 7);
  return 0;
}

