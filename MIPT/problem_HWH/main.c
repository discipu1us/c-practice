#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#define MULT 1

int main(void) {

    unsigned int size, len1, len2;
    char *str1, *str2, *token;
    unsigned long m;
    //Node *list = NULL;
    Hashtable *hashtable = NULL;

    fscanf(stdin, "%u", &size);
    fscanf(stdin, "%u\n", &len1);
    str1 = malloc_wrap(len1++ * sizeof(char));
    fgets(str1, len1, stdin);
    fscanf(stdin, "%u\n", &len2);
    str2 = malloc_wrap(len2++ * sizeof(char));
    fgets(str2, len2, stdin);
    
    //printf("%s", str1);
    //printf("%s", str2);
    //printf("%u %u\n", len1, len2);
    //printf("%zu\n", sizeof(Node*));
    //printf("%d %d\n", hash(str1, size*2), hash(str2, size*2));

    m = (unsigned long)size * MULT;
    hashtable = init(hashtable, m);

    /* Parse str2 with strtok and fill hashtable with results */

    token = strtok(str2, " ");
    while (token != NULL) {
        //printf("%s %lu \n", token, hash(token, m));
        //list = add(list, token);
        //hashtable[hash(token, m)] = add(hashtable[hash(token, m)], token);
        //print_list(hashtable[hash(token, m)]);
        add2table(hashtable, token);
        token = strtok(NULL, " ");
        //list = NULL;
    };

    /* Parse str1 with strtok and count with hashtable*/


    //list = add(list, str1);
    //list = add(list, str2);
    //print_list(list);
    //free_list(list);
    return 0;

}
