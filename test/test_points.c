#define KISS64_IMPLEMENTATION
#include <stdlib.h>
#include <stdio.h>
#include "../rng/kiss64.h"

const int N = 10000;

double frand() {
  return (next() >> 11) * (1.0 / (1L << 53));
}

int main(int argc, char** argv) {

  FILE* f = fopen("/dev/random", "r");
  char s[seed_length];
  fread(s, 1, seed_length, f);
  fclose(f);
  seed((uint64_t *) s);

  printf("%d\n", N);
  for (int i = 0; i < N; i++) {
    printf("%lf %lf\n", frand(), frand());
  }

  return 0;
}
