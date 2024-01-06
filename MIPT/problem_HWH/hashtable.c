#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

struct node_t {
  char *token;
  struct node_t *next;
};

struct hashtable_t {
  unsigned long size;
  Node **dict;
};

/* wrappers for dynamic allocation */

void *calloc_wrap(size_t count, size_t size) {
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

int is_member(Node *top, const char *token) {
  if(!top) return 0;
  do {
    if (!strcmp(top->token, token)) return 1;
    top = top->next;
  } while(top);
  return 0;
}

void print_list(Node *top) {
  if(!top) return;
  do {
    printf("%s\n", top->token);
    top = top->next;
  } while (top);
}

Node *add(Node *top, const char *token) {
  Node *new;
  if(!top) {
    top = malloc_wrap(sizeof(Node));
    top->next = NULL;
    top->token = malloc_wrap((strlen(token) + 1) * sizeof(char));
    top->token = strcpy(top->token, token);
    return top;
  }
  if(is_member(top, token)) return top;
  new = malloc_wrap(sizeof(Node));
  new->next = top;
  new->token = malloc_wrap((strlen(token) + 1) * sizeof(char));
  new->token = strcpy(new->token, token);
  return new;
}

void free_list(Node *top) {
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

//sdbm algo
static unsigned long sdbm(const char *str) {
  unsigned long hash = 0;
  int c;
  while ((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;
  return hash;
}

//djb2 algo
static unsigned long djb2(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

static unsigned long hash(const char *str, unsigned long m) {
  return djb2(str) % m;
}

/* Functions for struct hashtable_t aka Hashtable */

Hashtable *init(Hashtable *table, unsigned long size) {
  table = malloc_wrap(sizeof(Hashtable));
  table->size = size;
  table->dict = calloc_wrap(size, sizeof(Node*));
  return table;
}

void add2table(Hashtable *table, const char *token) {
  unsigned long index;
  if(!table) return;
  index = hash(token, table->size);
  (table->dict)[index] = add((table->dict)[index], token);
  print_list((table->dict)[index]);
}
