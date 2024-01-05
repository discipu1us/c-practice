/*
   struct sieve_t definition and collection of functions for using it
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "sieve.h"

struct sieve_t {
    unsigned n; 
    unsigned char *mod1;
    unsigned char *mod5;
};

/* additional functions */

void *calloc_wrap(size_t count, size_t size) {
    void *ptr;
    if ( (ptr = calloc(count, size)) == NULL ) {
        fprintf(stderr, "error: failed to allocate memory...\n");
        exit(EXIT_FAILURE);
    }
    return (ptr);
}

unsigned sieve_bound(unsigned num) {
    double dnum, dres;
    unsigned long bound;
    if (num < 20) return 2u;
    dnum = num;
    dres = dnum * (log(dnum) + log(log(dnum)));
    bound = (((unsigned long) round(dres)) / 6 + 1) / CHAR_BIT + 1;
    assert(bound <= UINT_MAX);
    return (unsigned) bound;
}

struct sieve_t init_sieve(unsigned size) {
    unsigned char *mod1 = calloc_wrap(size, sizeof(unsigned char)); 
    unsigned char *mod2 = calloc_wrap(size, sizeof(unsigned char)); 
    struct sieve_t res = { size, mod1, mod2 };
    assert ((size > 1) && (mod1 != NULL) && (mod2 != NULL));
    return res;
}

void set_bit(unsigned char *arr, unsigned long n) {
    // compute byte index and bit index to update given n
    unsigned byte_index, bit_index;
    assert(CHAR_BIT > 0 && n != 0 && (n / CHAR_BIT) <= UINT32_MAX);
    byte_index = n / CHAR_BIT;
    bit_index = n % CHAR_BIT;
    // update
    arr[byte_index] = arr[byte_index] | (1 << bit_index);
}

/* base functions */

int is_prime(struct sieve_t *sv, unsigned long n) {
    unsigned long series_index; 
    unsigned byte_index; 
    int bit_index;
    assert(sv != NULL);
    if(2 == n || 3 == n)
        return 1;
    if(1 == n % 6) {
        series_index = n / 6;
        byte_index = series_index / CHAR_BIT;
        bit_index = series_index % CHAR_BIT;
        return ((sv->mod1[byte_index] >> bit_index) & 1) ? 0 : 1;
    }
    if(5 == n % 6) {
        series_index = n / 6;
        byte_index = series_index / CHAR_BIT;
        bit_index = series_index % CHAR_BIT;
        return ((sv->mod5[byte_index] >> bit_index) & 1) ? 0 : 1;
    }
    return 0;
}

void fill_sieve(struct sieve_t *sv) {
    unsigned long i, j, start, u_bound;
    unsigned long m1, m5, m1_max, m5_max;
    sv->mod1[0] = 1;
    u_bound = (unsigned long)sv->n * CHAR_BIT;
    m1_max = (unsigned long)sv->n * CHAR_BIT * 6 + 1;
    m5_max = (unsigned long)sv->n * CHAR_BIT * 6 + 5;
    for(i = 1; i < sv->n; i++) {
        // Starting with 5 and 7
        m5 = 6 * i - 1;
        m1 = 6 * i + 1;
        //printf("%llu %llu\n", m1 ,m5);
        if (is_prime(sv, m5)) {
            if((start = (m5 * (m1 - 6)) / 6 + m5) > m5_max) break;
            // Fill mod5 with m5 multiples
            for(j = start; j < u_bound; j+=m5)
                set_bit(sv->mod5, j);
            if((start = (m5 * m5) / 6) > m1_max) break;
            // Fill mod1 with m5 multiples
            for(j = start; j < u_bound; j+=m5)
                set_bit(sv->mod1, j);
        }
        if (is_prime(sv, m1)) {
            if((start = (m1 * m1) / 6) > m1_max) break;
            // Fill mod1 with m1 multiples
            for(j = start; j < u_bound; j+=m1)
                set_bit(sv->mod1, j);
            if((start = (m1 * m5) / 6 + m1) > m5_max) break;
            // Fill mod5 with m1 multiples
            for(j = start; j < u_bound; j+=m1)
                set_bit(sv->mod5, j);
        }
    }
}

unsigned long find_prime(struct sieve_t *sv, unsigned n) {
    unsigned long i, num;
    unsigned counter = 2;
    assert(n <= UINT_MAX && n >= 1);
    assert(sv != NULL);
    if(1 == n) return 2;
    if(2 == n) return 3;
    for(i = 0;; i++) {
        num = 6 * i + 1;
        if(is_prime(sv, num))
            counter++;
        if(counter == n)
            return num;
        num = 6 * i + 5;
        if(is_prime(sv, num))
            counter++;
        if(counter == n)
            return num;
    }
}

void free_sieve(struct sieve_t *sv) {
    if(!sv) {
        free(sv->mod1);
        free(sc->mod5);
    };
}
