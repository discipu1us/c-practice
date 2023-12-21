/*
   
  Custom function to convert string to a POSITIVE unsigned int.
  Correct string representation is a positive number in base 10 and upper bound of INT32_MAX.
  Whitespace and any other symbols except digits in the string will produce an error.
 
*/
#include "str2i.h"

uint32_t str2i(char *str) {
    int len, i; 
    int overflow; // boolean to check overflow in main loop
    size_t slen;
    char u[32];
    uint32_t num, d1, d2;
    uint32_t u_max = INT32_MAX;
    len = sprintf(u, "%u", u_max);
    slen = strlen(str);
    // certain overflow
    if(slen > len) {
        fprintf(stderr, "error: incorrect input: %s\n", str);
        exit(EXIT_FAILURE);
    }
    // zero at 0th index
    if(str[0] == '0') {
        fprintf(stderr, "error: incorrect input: %s\n", str);
        exit(EXIT_FAILURE);
    };
    // conversion 
    overflow = (slen == len) ? 1: 0;
    for(i = 0; i < slen; i++) {
        if(!isdigit(str[i])) {
            fprintf(stderr, "error: incorrect input: %s\n", str);
            exit(EXIT_FAILURE);
        };
        d1 = str[i] - '0';
        if(!i) num = d1;
        if(overflow) {
            d2 = u[i] - '0';
            if(d1 > d2) {
                fprintf(stderr, "error: incorrect input: %s\n", str);
                exit(EXIT_FAILURE);
            };
            if(d1 < d2) overflow = 0;
        }
        if(i) {
            num *= BASE;
            num += d1;
        }
    }
    return num;
}
