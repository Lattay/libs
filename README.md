A list of interesting and usefull FOSS libraries taken from the web.

Data structures
- [hashtable](./structures/hashtable.h) hashtable implementation ([source](http://www.pomakis.com)) modified to fit the single file header model
  and my tastes in terms of code format (but it is unmodified in term of algorithm)

RNG
- [xoshiro256+](./rng/xoshiro256.h) fast floating point generator ([source](http://vigna.di.unimi.it/xorshift/))
- [xoshiro256\*\*](./rng/xoshiro256starstar.h) fast general purpose generator ([source](http://vigna.di.unimi.it/xorshift/))

Hash algorithms
- [adler_32](./hash/adler_32.h) very fast 32 bit hash algorithm
- [lch32](./hash/lch32.h) my own 32bit digest hash algorithm, taimed at very short hashed data
