/*
   Implemantation of hashtable with chaining.
   Chaining is implemented with a linked list data structure.
   This is not generic hashtable.
   It's purpose is searching and counting the occurences of specified 
        tokens(words) in a textual input.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hashtable.h"

typedef struct node_t {
  char *token;
  unsigned long freq;
  struct node_t *next;
} Node;

struct hashtable_t {
  unsigned size;  // size of hashtable
  unsigned elem;  // number of elements in hashtable
  double load;    // limiting load factor for automatic rehashing
  char rehash;    // boolean to control automatic rehashing
  unsigned long (*prehash)(const char *str); // custom hash function
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

static void print_list(Node *top) {
  if(!top) return;
  do {
    printf(" %s = %lu", top->token, top->freq);
    top = top->next;
  } while (top);
}

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

/* Functions for struct hashtable_t aka Hashtable */

Hashtable *init(Hashtable *table, unsigned size,
  unsigned long (*hash)(const char *str), double llf) {
  table = malloc_wrap(sizeof(Hashtable));
  table->size = size;
  table->elem = 0;
  // user can choose llf from 0.5 to 2 (default 0.75)
  table->load = (llf >= 0.5 && llf <= 3) ? llf : 0.75;
  table->rehash = 1;
  table->prehash = hash;
  table->dict = calloc_wrap(size, sizeof(Node*));
  return table;
}

static unsigned hash(Hashtable *table, const char *str) {
  return (unsigned)(table->prehash(str) % table->size);
}

double get_load(Hashtable *table) {
  if(!table) return 0;
  assert(table->size != 0);
  return ((double)(table->elem) / table->size);
}

// function rehashing the contents of hashtable
// no additional memory is required, only pointers manipulation
static void rehash_bucket(Hashtable *table, unsigned i, unsigned size) {
  unsigned index;
  Node *tmp, *top;
  // 1. Nothing to do
  if(!table->dict[i])
    return;
  // 2. If a top node should be moved to a new bucket
  index = hash(table, table->dict[i]->token);
  while(index >= size) { // have to move head to new bucket
    tmp = table->dict[i];
    table->dict[i] = table->dict[i]->next;
    tmp->next = table->dict[index];
    table->dict[index] = tmp;
    if((table->dict[i]))
      index = hash(table, table->dict[i]->token);
    else
      return;
  };
  // 3. func pass the top node 
  //    need to check if some deeper nodes should be moved
  top = table->dict[i];
  while(top->next) {
    index = hash(table, top->next->token);
    if(index >= size) {
      tmp = top->next;
      top->next = top->next->next;
      tmp->next = table->dict[index];
      table->dict[index] = tmp;
    } else
      top = top->next; // advance further 
  };
  return;
}

void rehash(Hashtable *table, double mult) {
  Node **new_dict;
  unsigned i, old_size, new_size;
  if(mult <= 1 || mult > 3 ) return; // allow to expand only up to a factor of 3  
  new_size = (unsigned)(round(table->size * mult));
  if(new_size > table->size) {
    new_dict = realloc(table->dict, new_size * sizeof(Node*));
    if(new_dict == NULL) {  // failed to allocate memory
      table->rehash = 0;    // turn off auto rehashing and leave original table       
      return;
    }
    table->dict = new_dict;
    old_size = table->size;
    table->size = new_size; 
    // Nullify uninitialized part of reallocated array
    for(i = old_size; i < new_size; i++)
      table->dict[i] = NULL;
    // update elements postions with rehash_bucket
    for(i = 0; i < old_size; i++) 
      rehash_bucket(table, i, old_size);
    return;
  }
  table->rehash = 0; // new_size overflow -> no rehashing
}

void add2table(Hashtable *table, const char *token) {
  unsigned index;
  Node *prev;
  if(!table) return;
  index = hash(table, token);
  prev = table->dict[index];
  table->dict[index] = add(table->dict[index], token);
  if(prev != table->dict[index]) {
    // above inequality means that new element was added
    table->elem++;
    // so it's time to check if rehashing is needed
    if(table->rehash && (get_load(table) > table->load))
      rehash(table, 2); // double the size
  };
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

// diagnostic function to print hashtable content.
void print_table(Hashtable *table) {
  unsigned i;
  for(i = 0; i < table->size; i++) {
    printf("%u:", i);
    print_list(table->dict[i]);
    printf("\n");
  };
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
