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

const int N = 10000;
const int N2 = 20000;
const double pi = 3.14159265359;

double frand(){
  return (next() >> 11) * (2.0/(1L << 53)) - 1.0;
}

void naive_fft(double* signal, double* spectrum){
  for(int i = 0; i < N; i++){
    spectrum[2*i] = 0.0;
    spectrum[2*i+1] = 0.0;
    for(int j = 0; j < N; j++){
      spectrum[2*i] += signal[j] * cos(2*pi/N * i * j);
      spectrum[2*i+1] += signal[j] * sin(2*pi/N * i * j);
    }
  }
}

int main(int argc, char** argv){

  FILE* f = fopen("/dev/random", "r");
  char s[seed_length];
  fread(s, 1, seed_length, f);
  fclose(f);
  seed((uint64_t*) s);

  double data[N], spectrum[N2];

  for(int i = 0; i < N; i++){
    data[i] = frand();
  }

  naive_fft(data, spectrum);

  printf("%d\n", N);
  for(int i = 0; i < N; i++){
    printf("%lf %lf\n", spectrum[2*i], spectrum[2*i+1]);
  }

  return 0;
}

