#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define KISS64 0
#define XOSHIROPLUS 1
#define XOSHIROSTAR 2

#ifndef USE_RNG
#define USE_RNG KISS64
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

const int N = 30000;
const int N0 = 10000;
const int N1 = 20000;
const double pi = 3.14159265359;

double frand(){
  return (next() >> 11) * (2.0/(1L << 53)) - 1.0;
}

void naive_autocorrel(double* signal, double* autocorrel){
  for(int i = 0; i < N1 - N0; i++){
    autocorrel[i] = 0.0;
    for(int j = N0; j < N1; j++){
      autocorrel[i] += signal[j - i] * signal[j];
    }
  }
}

int main(int argc, char** argv){

  FILE* f = fopen("/dev/random", "r");
  char s[seed_length];
  fread(s, 1, seed_length, f);
  fclose(f);
  seed((uint64_t*) s);

  double data[N], autocorrel[N1 - N0];

  for(int i = 0; i < N; i++){
    data[i] = frand();
  }

  naive_autocorrel(data, autocorrel);

  printf("%d\n", N1 - N0);
  for(int i = 0; i < N1 - N0; i++){
    printf("%lf\n", autocorrel[i]);
  }

  return 0;
}

