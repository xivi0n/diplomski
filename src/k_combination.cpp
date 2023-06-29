#include <stdio.h>
#include "../inc/k_combination.h"

inline unsigned int k_combination_init(int k) {
    return (1 << k) - 1;
}

inline unsigned int k_combination_limit(int n) {
    return (1 << n);
}

inline unsigned int k_combination_next(int combination) {
    int x, y;
    x = combination & -combination;
    y = combination + x;
    return (((combination & ~y) / x) >> 1) | y;
}

void print_binary(unsigned int combination, int n) {
    if (--n) {
        print_binary(combination / 2, n);
    }
    printf("%d", combination % 2);
}

void print_combination(unsigned int combination, int n) {
    for (int i = 0; i < n; ++i) {
        if (combination & (1 << i)) {
            printf("%d ", i);
        }
    }
}

void generate_combinations(int n, int k) {
    int combination = k_combination_init(k);
    int limit = k_combination_limit(n);
    
    while (combination < limit) {
        print_binary(combination, n); printf(" ");
        print_combination(combination, n); printf("\n");
        
        combination = k_combination_next(combination);
    }
}