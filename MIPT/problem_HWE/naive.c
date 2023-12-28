#include "naive.h"

#if 0
int prime_test(uint64_t n) {
    uint64_t i;
    if(1 == n) return 0;
    if(2 == n || 3 == n) return 1;
    if(n % 6 == 1 || n % 6 == 5) { // redundant condition in program context
        for(i = 5; i * i <= n; i += 2)
            if(!(n % i)) return 0;
        return 1;
    }
    return 0;
}
# endif

int is_prime2(uint64_t n) {
    uint64_t i;
    if(1 == n) return 0;
    for(i = 5; i * i <= n; i += 2)
        if(!(n % i)) return 0;
    return 1;
}

uint64_t find_prime2(uint32_t n) {
    uint64_t i, num;
    uint32_t counter = 2;
    if(1 == n) return 2;
    if(2 == n) return 3;
    for(i = 0;; i++) {
        num = 6 * i + 1;
        if(is_prime2(num))
            counter++;
        if(counter == n)
            return num;
        num = 6 * i + 5;
        if(is_prime2(num))
            counter++;
        if(counter == n)
            return num;
    }
}
