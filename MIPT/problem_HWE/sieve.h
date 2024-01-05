#ifndef SIEVE_H
#define SIEVE_H

struct sieve_t;

unsigned sieve_bound(unsigned num);
struct sieve_t *init_sieve(struct sieve_t *sv, unsigned size);
int is_prime(struct sieve_t *sv, unsigned long n);
unsigned long find_prime(struct sieve_t *sv, unsigned n);
void fill_sieve(struct sieve_t *sv);
void free_sieve(struct sieve_t *sv);

#endif
