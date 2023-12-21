#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>

#define Q6 6

struct sieve_t {
    uint32_t n; 
    unsigned char *mod1;
    unsigned char *mod5;
};

extern uint32_t sieve_bound(uint32_t num);
extern struct sieve_t init_sieve(uint32_t size);
extern void set_bit(unsigned char *arr, uint64_t n);
extern int is_prime(struct sieve_t *sv, uint64_t n);
extern uint64_t find_prime(struct sieve_t *sv, uint32_t n);
extern void fill_sieve(struct sieve_t *sv);
