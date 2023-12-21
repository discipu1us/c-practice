/*
    Benchmarking and testing for naive and sieve algorithms for searching nth prime.
*/

#include "sieve.h"
#include "naive.h"
#include "simple-bench.c"

int main() {

    struct sieve_t s;
    uint32_t i, size;
    uint64_t res1, res2;
    double t;
    struct timespec t1,t2;
    char margin1 = 17;
    char margin2 = 14;

    printf("n:        sieve:        naive:\n");

    for(i = 1; i <= 10000000; i *= 10) {

        printf("%u", i);

        // sieve algo
        simple_gettime(&t1);
        size = sieve_bound(i);
        s = init_sieve(size);
        fill_sieve(&s);
        res1 = find_prime(&s, i);
        simple_gettime(&t2);
        t = diff(t1,t2);
        free(s.mod1);
        free(s.mod5);

        printf("%*lf", margin1--, t);

        // naive algo
        simple_gettime(&t1);
        res2 = find_prime2(i);
        simple_gettime(&t2);
        t = diff(t1,t2);

        printf("%*lf\n", margin2, t);

        // Testing results
        assert(res1 == res2);
    }

}
