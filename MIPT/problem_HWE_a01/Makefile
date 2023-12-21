CFLAGS = -std=c99 -Wall -Werror -O2

all: prime bench test

prime: str2i.c sieve.c prime.c
	cc $(CFLAGS) str2i.c sieve.c prime.c -o prime

bench: sieve.c naive.c prime_bench.c
	cc $(CFLAGS) sieve.c naive.c prime_bench.c -o pb

test: prime
	./prime 1 > prime.test 
	for i in $$(seq 2 1 1000) ; do ./prime $$i >> prime.test ; done 
	./prime abracadabra 2> prime.test2 || true 
	./prime 3000000000 2>> prime.test2 || true
	./prime 11 73 2>> prime.test2 || true 
	for i in $$(seq -100 10 0) ; do ./prime $$i 2>> prime.test2 || true ; done 

clean:
	rm -f prime pb prime.test prime.test2
