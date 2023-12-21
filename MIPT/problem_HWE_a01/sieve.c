/*
   Collection of functions for struct sieve_t
*/

#include "sieve.h"

/* additional functions */

uint32_t sieve_bound(uint32_t num) {
    double dnum, dres;
    uint64_t bound;
    if (num < 20) return 2u;
    dnum = num;
    dres = dnum * (log(dnum) + log(log(dnum)));
    bound = (((uint64_t) round(dres)) / Q6 + 1) / CHAR_BIT + 1;
    assert(bound <= UINT32_MAX);
    return (uint32_t) bound;
}

struct sieve_t init_sieve(uint32_t size) {
    unsigned char *mod1 = calloc(size, sizeof(unsigned char)); 
    unsigned char *mod2 = calloc(size, sizeof(unsigned char)); 
    struct sieve_t res = { size, mod1, mod2 };
    assert ((size > 1) && (mod1 != NULL) && (mod2 != NULL));
    return res;
}

void set_bit(unsigned char *arr, uint64_t n) {
    // compute byte index and bit index to update given n
    uint32_t byte_index, bit_index;
    assert(CHAR_BIT > 0 && n != 0 && (n / CHAR_BIT) <= UINT32_MAX);
    byte_index = n / CHAR_BIT;
    bit_index = n % CHAR_BIT;
    // update
    arr[byte_index] = arr[byte_index] | (1 << bit_index);
}

/* base functions */

int is_prime(struct sieve_t *sv, uint64_t n) {
    uint64_t series_index; 
    uint32_t byte_index; 
    int bit_index;
    if(2 == n || 3 == n)
        return 1;
    if(1 == n % Q6) {
        series_index = n / Q6;
        byte_index = series_index / CHAR_BIT;
        bit_index = series_index % CHAR_BIT;
        return ((sv->mod1[byte_index] >> bit_index) & 1) ? 0 : 1;
    }
    if(5 == n % Q6) {
        series_index = n / Q6;
        byte_index = series_index / CHAR_BIT;
        bit_index = series_index % CHAR_BIT;
        return ((sv->mod5[byte_index] >> bit_index) & 1) ? 0 : 1;
    }
    return 0;
}

void fill_sieve(struct sieve_t *sv) {
    uint64_t i, j, start, u_bound;
    uint64_t m1, m5, m1_max, m5_max;
    sv->mod1[0] = 1;
    u_bound = (uint64_t)sv->n * CHAR_BIT;
    m1_max = (uint64_t)sv->n * CHAR_BIT * Q6 + 1;
    m5_max = (uint64_t)sv->n * CHAR_BIT * Q6 + 5;
    for(i = 1; i < sv->n; i++) {
        // Starting with 5 and 7
        m5 = Q6 * i - 1;
        m1 = Q6 * i + 1;
        //printf("%llu %llu\n", m1 ,m5);
        if (is_prime(sv, m5)) {
            if((start = (m5 * (m1 - Q6)) / Q6 + m5) > m5_max) break;
            // Fill mod5 with m5 multiples
            for(j = start; j < u_bound; j+=m5)
                set_bit(sv->mod5, j);
            if((start = (m5 * m5) / Q6) > m1_max) break;
            // Fill mod1 with m5 multiples
            for(j = start; j < u_bound; j+=m5)
                set_bit(sv->mod1, j);
        }
        if (is_prime(sv, m1)) {
            if((start = (m1 * m1) / Q6) > m1_max) break;
            // Fill mod1 with m1 multiples
            for(j = start; j < u_bound; j+=m1)
                set_bit(sv->mod1, j);
            if((start = (m1 * m5) / Q6 + m1) > m5_max) break;
            // Fill mod5 with m1 multiples
            for(j = start; j < u_bound; j+=m1)
                set_bit(sv->mod5, j);
        }
    }
}

uint64_t find_prime(struct sieve_t *sv, uint32_t n) {
    uint64_t i, num;
    uint32_t counter = 2;
    assert(n <= INT32_MAX && n >= 1);
    if(1 == n) return 2;
    if(2 == n) return 3;
    for(i = 0;; i++) {
        num = Q6 * i + 1;
        if(is_prime(sv, num))
            counter++;
        if(counter == n)
            return num;
        num = Q6 * i + 5;
        if(is_prime(sv, num))
            counter++;
        if(counter == n)
            return num;
    }
}
