## Overview

Program `prime` computes the $n$th prime number from $1$ up to $2147483647$ (`INT32_MAX`).
There are two ways you can run program:

- simple `./prime` will ask user to enter a number from `stdin`.
- alternatively you can call `prime` with an argument - `./prime n`.

To find $n$th prime program builds and fills a sieve that only holds primality
features of $6k + 1$ and $6k + 5$ numbers. Every primality feature stored in one bit.
So it saves memory and runs faster on big inputs than naive algorithm.

## Installation

Program can be installed using `makefile`.
There are few options available:

- `make prime` compiles only `prime` program.
- `make bench` compiles `pb` binary that measures run time of naive and sieve algorithms.
It also tests results of both algorithms against each over.
- `make test` compiles `prime` program and runs some tests.
Test script writes results to `prime.test` and `prime.test2` files.
- `make` or `make all` do all of the above.
- finally `make clean` removes all created files.

## Benchmark result

Running `./pb` on my old laptop I had the following resuls:

n:        sieve:        naive:
1         0.000022      0.000002
10        0.000002      0.000002
100       0.000037      0.000015
1000      0.000046      0.000575
10000     0.000591      0.012885
100000    0.003391      0.393543
1000000   0.050251     12.216195
10000000  0.868001    404.029482
