#include "sieve.h"
#include "str2i.h"

int main(int argc, char **argv) {

    int r;
    int32_t input;
    uint32_t num, size;
    struct sieve_t s;

    if(argc > 2) {
        fprintf(stderr, "usage: %s [n]\n"
                        "    n: number between 1 and %u\n", 
                        argv[0], INT32_MAX);
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
    s = init_sieve(size);
    fill_sieve(&s);
    fprintf(stdout, "%llu\n", find_prime(&s, num));
    free(s.mod1);
    free(s.mod5);

    return 0;
}
