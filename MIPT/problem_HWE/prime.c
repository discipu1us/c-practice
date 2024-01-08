#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "sieve.h"
#include "str2i.h"

int main(int argc, char **argv) {

    int r;
    int input;
    unsigned num, size;
    struct sieve_t *s = NULL;

    /* test if unsigned long > unsigned */
    _Static_assert(sizeof(unsigned long) > sizeof(unsigned), 
            "expected unsigned long to be bigger than unsigned");

    if(argc > 2) {
        fprintf(stderr, "usage: %s [n]\n"
                        "    n: number between 1 and %u\n", 
                        argv[0], UINT_MAX);
        exit(EXIT_FAILURE);
    }
    if(1 == argc) {
        if((r = fscanf(stdin, "%d", &input)) != 1) {
            fprintf(stderr, "error: incorrect input\n");
            exit(EXIT_FAILURE);
        };
        if(input <= 0) {
            fprintf(stderr, "error: incorrect input\n");
            exit(EXIT_FAILURE);
        }
        num = input;
    }
    else 
        num = str2i(argv[1]); 
    size = sieve_bound(num);
    s = init_sieve(s, size);
    fill_sieve(s);
    fprintf(stdout, "%lu\n", find_prime(s, num));
    free_sieve(s);

    return 0;
}
