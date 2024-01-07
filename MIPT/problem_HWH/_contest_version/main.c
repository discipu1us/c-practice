/* problem HWH contest version - everything in one file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MULT 100

typedef struct node_t {
  char *token;
  unsigned long freq;
  struct node_t *next;
} Node;

typedef struct hashtable_t {
  unsigned long size;
  Node **dict;
} Hashtable;

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

void compare_and_count(Node *top, const char *token) {
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

void compare_and_print(Node *top, const char *token) {
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
unsigned long sdbm(const char *str) {
  unsigned long hash = 0;
  int c;
  while ((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;
  return hash;
}

//djb2 algo
unsigned long djb2(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

unsigned long hash(const char *str, unsigned long m) {
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
  table->dict[index] = add(table->dict[index], token);
}

void count_words(Hashtable *table, const char *token) {
  unsigned long index;
  if(!table) return;
  index = hash(token, table->size);
  if(!(table->dict[index])) return;
  compare_and_count(table->dict[index], token);
}

void print_frequency(Hashtable *table, const char *token) {
  unsigned long index;
  if(!table) return;
  index = hash(token, table->size);
  if(!(table->dict[index])) return;
  compare_and_print(table->dict[index], token);
}

void free_table(Hashtable *table) {
  unsigned long i;
  if(!table) return;
  for(i = 0; i < table->size; i++)
    free_list(table->dict[i]);
  free(table->dict);
  free(table);
  table = NULL;
}

int main(void) {

    unsigned int size, len1, len2;
    char *str1, *str2, *str3, *token;
    unsigned long m;
    Hashtable *hashtable = NULL;

    /* Input handler */

    fscanf(stdin, "%u", &size);
    fscanf(stdin, "%u\n", &len1);
    str1 = malloc_wrap(++len1 * sizeof(char));
    fgets(str1, len1, stdin);
    fscanf(stdin, "%u\n", &len2);
    str2 = malloc_wrap(++len2 * sizeof(char));
    fgets(str2, len2, stdin);
    str3 = malloc_wrap(len2 * sizeof(char));
    strcpy(str3, str2);
    

    m = (unsigned long)size * MULT;
    hashtable = init(hashtable, m);

    /* Parse str2 with strtok and fill hashtable with results */

    token = strtok(str2, " ");
    while (token != NULL) {
        add2table(hashtable, token);
        token = strtok(NULL, " ");
    };
    free(str2);
    
    /* Parse str1 with strtok and count frequencies */

    token = strtok(str1, " ");
    while (token != NULL) {
        count_words(hashtable, token);
        token = strtok(NULL, " ");
    };
    free(str1);

    /* Print results and free hashtable */ 

    token = strtok(str3, " ");
    while (token != NULL) {
        print_frequency(hashtable, token);
        token = strtok(NULL, " ");
    };
    printf("\n");
    free(str3);
    free_table(hashtable);
    return 0;
}
