#ifndef __k_combination_h_
#define __k_combination_h_

inline unsigned int k_combination_init(int k);
inline unsigned int k_combination_limit(int n);
inline unsigned int k_combination_next(int combination);
void generate_combinations(int n, int k);
void print_binary(unsigned int combination, int n);
void print_combination(unsigned int combination, int n);

#endif