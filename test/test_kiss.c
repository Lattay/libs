#define KISS64_IMPLEMENTATION
#include <stdlib.h>
#include <stdio.h>
#include "../rng/kiss64.h"

void section(char* title){
  printf("\n%s\n", title);
}

void show_endianess(void){
  uint32_t n = 1;
  char* bytes = (char*) &n;
  if(bytes[0]){
    printf("Big endian\n");
  } else {
    printf("Little endian\n");
  }
}

int main(int argc, char** argv){

  FILE* f = fopen("/dev/random", "r");
  char s[seed_length];
  fread(s, 1, seed_length, f);
  fclose(f);
  seed((uint64_t*) s);

  section("Endianness:");
  show_endianess();

  section("64b unsigned integers:");
  for(int i = 0; i < 10; i++){
    printf("%lu\n", next());
  }

  section("32b unsigned ntegers:");
  for(int i = 0; i < 10; i++){
    printf("%u\n", (uint32_t) (next() >> 32));
  }

  section("[0, 1) float:");
  for(int i = 0; i < 10; i++){
    printf("%lf\n", (next() >> 11) * (1.0/(1L << 53)));
  }

  section("boolean:");
  for(int i = 0; i < 10; i++){
    printf("%d\n", (next() >> 63));
  }

  return 0;
}
