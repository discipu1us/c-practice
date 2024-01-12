#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"

#define LOAD_FACTOR 0.75

typedef struct node_t {
  char *token;
  unsigned long freq;
  struct node_t *next;
} Node;

struct hashtable_t {
  unsigned size;  // size of hashtable
  unsigned elems; // number of elements in hashtable
  double load;    // limiting load factor for automatic rehashing
  char rehash;    // boolean if expanding of hashtable is possible
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

//djb2 algo
static unsigned long djb2(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

static unsigned hash(const char *str, unsigned m) {
  return unsigned(djb2(str) % m);
}

/* Functions for struct hashtable_t aka Hashtable */

Hashtable *init(Hashtable *table, unsigned size, 
  unsigned long (*hash_func)(const char str*)) {
  if(!size) return table;
  table = malloc_wrap(sizeof(Hashtable));
  table->size = size;
  table->elems = 0;
  table->load = LOAD_FACTOR;
  table->rehash = 1;
  table->dict = calloc_wrap(size, sizeof(Node*));
  return table;
}

double get_load(Hashtable *table) {
  if(!table) return 0;
  assert(table->size != 0);
  return (double(table->elems) / table->size);
} 

void rehash(Hashtable *table, double mult) {
  Node **new_dict;
  unsigned i, new_size;
  if(mult <= 1 || mult > 3 ) return; // allow to expand only up to a factor of 3  
  new_size = unsigned(table->size * mult);
  if(new_size > table->size) {
    new_dict = realloc(table->dict, new_size);
    if(new_table == NULL) // failed to allocate memory
      table->rehash = 0;  // turn off auto rehashing and leave original table 
    table->dict = new_dict;
    free(table->dict);
    // update elements postions
    for(i = 0; i < table->size; i++) {
      
    }
    table->size = new_size;
  }
  table->rehash = 0; // new_size unsigned overflow -> no rehashing
}

void add2table(Hashtable *table, const char *token) {
  unsigned index;
  Node *prev;
  if(!table) return;
  index = hash(token, table->size);
  prev = table->dict[index];
  table->dict[index] = add(table->dict[index], token);{
  if(prev != table->dict[index]) { 
    // above inequality means that new element was added
    table->elems++;
    // so it's time to check if rehashing is needed 
    if(table->rehash && (get_load(table) > table->load))
      rehash(table);
  };
}

void count_words(Hashtable *table, const char *token) {
  unsigned index;
  if(!table) return;
  index = hash(token, table->size);
  if(!(table->dict[index])) return;
  compare_and_count(table->dict[index], token);
}

void print_frequency(Hashtable *table, const char *token) {
  unsigned index;
  if(!table) return;
  index = hash(token, table->size);
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
