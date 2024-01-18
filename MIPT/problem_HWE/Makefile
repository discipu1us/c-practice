NODEBUG ?= 1
ifeq ($(NODEBUG), 1)
    CFLAGS = -std=gnu99 -Wall -O2 -DNDEBUG
else
    CFLAGS = -std=gnu99 -Wall -fsanitize=undefined -fsanitize=address -fsanitize=leak -O2
endif

LDLIBS = -lm

all: prime bench test

sieve.o: sieve.c sieve.h

str2i.o: str2i.c str2i.h

naive.o: naive.c naive.h

%.o: %.c
	cc $(CFLAGS) -o $@ -c $<

prime: str2i.o sieve.o prime.c
	cc $(CFLAGS) str2i.o sieve.o prime.c $(LDLIBS) -o prime

bench: sieve.o naive.o prime_bench.c simple-bench.h
	cc $(CFLAGS) sieve.o naive.o prime_bench.c $(LDLIBS) -o pb

test: prime
	./prime 1 > prime.test 
	for i in $$(seq 2 1 1000) ; do ./prime $$i >> prime.test ; done 
	./prime abracadabra 2> prime.test2 || true 
	./prime 3000000000 2>> prime.test2 || true
	./prime 11 73 2>> prime.test2 || true 
	for i in $$(seq -100 10 0) ; do ./prime $$i 2>> prime.test2 || true ; done 
	for i in $$(seq 1 1 1000) ; do echo $$i > test.input ; ./prime < test.input >> prime.test3; done 
	cmp prime.test prime.test3
	rm test.input

clean:
	rm -f prime pb prime.test prime.test2 prime.test3 *.o
