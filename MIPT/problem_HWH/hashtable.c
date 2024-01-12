#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

typedef struct node_t {
  char *token;
  unsigned long freq;
  struct node_t *next;
} Node;

struct hashtable_t {
  unsigned size;
  unsigned long (*hash_func)(const char *str);
  Node **dict;
};

/* wrappers for dynamic allocation */

static void *calloc_wrap(size_t count, size_t size) {
    void *ptr;
    if ( (ptr = calloc(count, size)) == NULL ) {
        fprintf(stderr, "error: failed to allocate memory...\n");
        exit(EXIT_FAILURE);
    }
    return (ptr);
}

void *malloc_wrap(size_t size) {
    void *ptr;
    if ( (ptr = malloc(size)) == NULL ) {
        fprintf(stderr, "error: failed to allocate memory...\n");
        exit(EXIT_FAILURE);
    }
    return (ptr);
}

/* functions for struct node_t aka Node */

static int is_member(Node *top, const char *token) {
  if(!top) return 0;
  do {
    if (!strcmp(top->token, token)) return 1;
    top = top->next;
  } while(top);
  return 0;
}

static void compare_and_count(Node *top, const char *token) {
  if(!top) return;
  do {
    if (!strcmp(top->token, token)) {
      top->freq += 1;
      return;
    }
    top = top->next;
  } while(top);
  return;
}

static void compare_and_print(Node *top, const char *token) {
  if(!top) return;
  do {
    if (!strcmp(top->token, token)) {
      printf("%lu ", top->freq);
      return;
    }
    top = top->next;
  } while(top);
  return;
}

#if 0
static void print_list(Node *top) {
  if(!top) return;
  do {
    printf("%s\n", top->token);
    top = top->next;
  } while (top);
}
#endif

static Node *add(Node *top, const char *token) {
  Node *new;
  if(!top) {
    top = malloc_wrap(sizeof(Node));
    top->next = NULL;
    top->freq = 0;
    top->token = malloc_wrap((strlen(token) + 1) * sizeof(char));
    top->token = strcpy(top->token, token);
    return top;
  }
  if(is_member(top, token)) return top;
  new = malloc_wrap(sizeof(Node));
  new->next = top;
  new->freq = 0;
  new->token = malloc_wrap((strlen(token) + 1) * sizeof(char));
  new->token = strcpy(new->token, token);
  return new;
}

static void free_list(Node *top) {
  Node *tmp;  
  if(!top) return; //nothing to free
  do {
    tmp = top;
    top = top->next;
    free(tmp->token);
    free(tmp);
  } while (top);
}

/* Hash functions for strings */

#if 0
//sdbm algo
static unsigned long sdbm(const char *str) {
  unsigned long hash = 0;
  int c;
  while ((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;
  return hash;
}
#endif

/* Functions for struct hashtable_t aka Hashtable */

Hashtable *init(Hashtable *table, unsigned size,
  unsigned long (*hash)(const char *str)) {
  table = malloc_wrap(sizeof(Hashtable));
  table->size = size;
  table->hash_func = hash;
  table->dict = calloc_wrap(size, sizeof(Node*));
  return table;
}

static unsigned hash(Hashtable *table, const char *str) {
  return (unsigned)(table->hash_func(str) % table->size);
}

void add2table(Hashtable *table, const char *token) {
  unsigned index;
  if(!table) return;
  index = hash(table, token);
  table->dict[index] = add(table->dict[index], token);
}

void count_words(Hashtable *table, const char *token) {
  unsigned index;
  if(!table) return;
  index = hash(table, token);
  if(!(table->dict[index])) return;
  compare_and_count(table->dict[index], token);
}

void print_frequency(Hashtable *table, const char *token) {
  unsigned index;
  if(!table) return;
  index = hash(table, token);
  if(!(table->dict[index])) return;
  compare_and_print(table->dict[index], token);
}

void free_table(Hashtable *table) {
  unsigned i;
  if(!table) return;
  for(i = 0; i < table->size; i++)
    free_list(table->dict[i]);
  free(table->dict);
  free(table);
  table = NULL;
}
