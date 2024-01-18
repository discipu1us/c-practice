#ifndef HASH_T
#define HASH_T

typedef struct hashtable_t Hashtable;

void *malloc_wrap(size_t size);
Hashtable *init(Hashtable *table, unsigned size,
  unsigned long (*hash)(const char *str), double llf);
void rehash(Hashtable *table, double mult);
void add2table(Hashtable *table, const char *token);
void count_words(Hashtable *table, const char *token);
void print_frequency(Hashtable *table, const char *token);
double get_load(Hashtable *table);
void print_table(Hashtable *table);
void free_table(Hashtable *table);

#endif
