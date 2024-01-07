#ifndef HASH_T
#define HASH_T

typedef struct node_t Node;
typedef struct hashtable_t Hashtable;

void *malloc_wrap(size_t size);

Node *add(Node *top, const char *data);
void print_list(Node *top);
void free_list(Node *top);

Hashtable *init(Hashtable *table, unsigned long size);
void add2table(Hashtable *table, const char *token);
void count_words(Hashtable *table, const char *token);
void print_frequency(Hashtable *table, const char *token);
void free_table(Hashtable *table);

#endif
