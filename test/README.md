Some naive tests to evaluate the algorithms.
`test_*.c` compile to executables that produce textual results
`*.py` process the results.

- test\_autocorrel.c produce random values and compute autocorrelation
- test\_fft.c produce random values and compute (naive) Discrete Fourier Transform
- test\_points.c
- test\_hashes.c
- test\_hash\_random\_data.c produce a lot of short random strings and compute a 32bit hash for each of them
- test\_kiss.c
- autocorrel.py
- points.py process output from test\_points.c
- spectrum.py process output from test\_autocorrel.c
- find\_collisions.py find hash collisions from test\_hash\_random\_data.c outputs
- test\_hash.sh run test\_hash\_random\_data.c in parallel and run find\_collisions.py on the result.
