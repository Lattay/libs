# Libs

A list of interesting and usefull FOSS libraries taken from the web and often partially rewritten or at least reformatted.

All should fits the single-file header-only format for easy integration in C projects.

Check out the work of the inspirator of this projects: [Sean Barrett](https://github.com/nothings/stb)

## Data structures
- [hashtable](./structures/hashtable.h) hashtable implementation ([source](http://www.pomakis.com)) modified to fit the single file header model
  and my tastes in terms of code format (but it is unmodified in term of algorithm)
- [AVL trees](./structures/avl.h) generic AVL trees implementation ([source](https://github.com/etherealvisage/avl)) with same sort of modifications

## RNG

- [splitmix64](./rng/splitmix64.h) very fast RNG, with only 64b state, useful to seed xoshiro256 or when a there is no need for a huge period ([source](https://github.com/svaarala/duktape/blob/master/misc/splitmix64.c))
- [xoshiro256+](./rng/xoshiro256.h) fast floating point generator with huge period ([source](http://vigna.di.unimi.it/xorshift/))
- [xoshiro256\*\*](./rng/xoshiro256starstar.h) fast general purpose generator with huge period ([source](http://vigna.di.unimi.it/xorshift/))

## Noise generator
- [perlin](./rng/perlin.h) Perlin noise generator from 1 to 3D, adapted from a C++ lib ([C++ source](https://github.com/Reputeless/PerlinNoise))

## Hash algorithms
- 
- [crc32](./hash/crc32.h) a well know, very good hash algorithm
- [lch32](./hash/lch32.h) my own (clunky) 32bit digest hash algorithm, aimed at very short hashed data
- [adler_32](./hash/adler_32.h) very fast 32 bit hash algorithm, pretty bad at randomness and collision
