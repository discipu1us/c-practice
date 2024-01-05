#ifndef SIEVE_H
#define SIEVE_H

struct sieve_t;

extern unsigned sieve_bound(unsigned num);
extern struct sieve_t init_sieve(unsigned size);
extern void set_bit(unsigned char *arr, unsigned long n);
extern int is_prime(struct sieve_t *sv, unsigned long n);
extern unsigned long find_prime(struct sieve_t *sv, unsigned n);
extern void fill_sieve(struct sieve_t *sv);
extern void free_sieve(struct sieve_t *sv);

#endif
