#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#define MULT 100

//djb2 algo
unsigned long djb2(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

int main(void) {

    unsigned int size, len1, len2;
    char *str1, *str2, *str3, *token;
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
    
    hashtable = init(hashtable, size, djb2, 3);

    /* Parse str2 with strtok and fill hashtable with results */

    printf("\nDemonstration of insertion and rehashing with load factor = 3:\n\n");
    token = strtok(str2, " ");
    while (token != NULL) {
        printf("Adding %s to hashtable\n\n", token);
        add2table(hashtable, token);
        print_table(hashtable);
        printf("\n");
        token = strtok(NULL, " ");
    };
    free(str2);
    
    /* Parse str1 with strtok and count frequencies */

    //printf("COUNTING PART:\n");
    token = strtok(str1, " ");
    while (token != NULL) {
        count_words(hashtable, token);
        //print_table(hashtable);
        //printf("\n");
        token = strtok(NULL, " ");
    };
    free(str1);

    /* Print results and free hashtable */ 
    
    token = strtok(str3, " ");
    while (token != NULL) {
        printf("%s = ", token);
        print_frequency(hashtable, token);
        token = strtok(NULL, " ");
    };
    printf("\n");
    free(str3);
    free_table(hashtable);
    return 0;
}
