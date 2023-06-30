#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <algorithm>
#include "../inc/bhk.h"
// #include "../inc/k_combination.h"

void print_binary(unsigned int combination, int n) {
    if (--n) {
        print_binary(combination / 2, n);
    }
    printf("%d", combination % 2);
}

static inline unsigned int k_combination_init(int k) {
    return (1 << k) - 1;
}

static inline unsigned int k_combination_limit(int n) {
    return (1 << n);
}

static inline unsigned int k_combination_next(int combination) {
    int x, y;
    x = combination & -combination;
    y = combination + x;
    return (((combination & ~y) / x) >> 1) | y;
}

static inline unsigned int add_point(unsigned int state, int k) {
    return state | (1 << k);
}

static inline int check_point(unsigned int state, int k) {
    return state & (1 << k);
}

static inline unsigned int remove_point(unsigned int state, int k) {
    return state & ~(1 << k);
}

void print_dp(float **dp, int state_count, int N) {
    unsigned int state_limit = k_combination_limit(N);
    unsigned int current_state;
    printf("===============================\n");
    for (int i = 1; i < N + 1; ++i) {
        current_state = k_combination_init(i);
        while (current_state < state_limit) {
            if (dp[current_state]) {
                print_binary(current_state, N);printf("\t");
                for (int j = 0; j < i; ++j) {
                    printf("%10.10f\t", dp[current_state][j]);
                }
                printf("\n");
            }
            current_state = k_combination_next(current_state);
        }
    }
}

void print_nh(char **next_hop, int state_count, int N) {
    unsigned int state_limit = k_combination_limit(N);
    unsigned int current_state;
    printf("===============================\n");
    for (int i = 1; i < N + 1; ++i) {
        current_state = k_combination_init(i);
        while (current_state < state_limit) {
            if (next_hop[current_state]) {
                print_binary(current_state, N);printf("\t");
                for (int j = 0; j < i; ++j) {
                    printf("%d\t", next_hop[current_state][j]);
                }
                printf("\n");
            }
            current_state = k_combination_next(current_state);
        }
    }
}

int find_k_one(unsigned int state, int N) {
    int count = 0;
    int position = 0;

    while (state > 0) {
        if (state & 1) {
            count++;
            if (count == N) {
                return position;
            }
        }
        state >>= 1;
        position++;
    }

    return -1;
}

void print_path(int path[], int N) {
    printf("Path: ");
    for (int i = 0; i < N; ++i) {
        printf("%d ", path[i]);
    }
    printf("\n");
}

static inline float* make_array(int N, float value) {
    float* arr = (float*) malloc(N * sizeof(float));
    std::fill_n(arr, N, value);
    return arr;
}

float bhk_tsp(float **distances, int N, int path[]) {
    float min_path = FLT_MAX;
    int state_count = (1 << (N - 1));
    float **dp = (float**) malloc(state_count * sizeof(float*));
    char **next_hop = (char**) malloc(state_count * sizeof(char *));

    unsigned int current_state;
    unsigned int calculation_state;
    unsigned int state_limit = k_combination_limit(N - 1);
    unsigned int combination;
    float cost, tmp;
    char save_m;
    int cnt_k, cnt_m;

    for (int i = 0; i < N - 1; ++i) {
        current_state = add_point(0, i);
        if (dp[current_state] == NULL) {
            dp[current_state] = make_array(1, FLT_MAX);
            next_hop[current_state] = (char*) malloc(1  * sizeof(char));
        }
        dp[current_state][0] = distances[i + 1][0];
        next_hop[current_state][0] = 0;
    }

    // printf("\nINIT\n");
    // print_dp(dp, state_count, N - 1);

    for (int i = 2; i < N; ++i) {
        current_state = k_combination_init(i);
        while (current_state < state_limit) {
            // printf("\n\ngenerating: ");print_binary(current_state, N - 1);printf("\n");
            dp[current_state] = make_array(i, FLT_MAX);
            next_hop[current_state] = (char*) malloc(i * sizeof(char));
            cnt_k = 0;
            for (int k = 1; k < N; ++k) {
                if (!check_point(current_state, k - 1)) continue;
                combination = remove_point(current_state, k - 1);
                cost = FLT_MAX;
                cnt_m = 0;
                for (int m = 1; m < N; ++m) {
                    if (!check_point(combination, m - 1) || k == m) continue;
                    tmp = dp[combination][cnt_m] + distances[m][k];
                    if (tmp < cost) {
                        cost = tmp;
                        save_m = cnt_m;
                    }
                    cnt_m++;
                }

                // printf("\nlast step: ");print_binary(combination, N - 1);printf("\n");
                if (cost < dp[current_state][cnt_k]) {
                    dp[current_state][cnt_k] = cost;
                    next_hop[current_state][cnt_k] = save_m;
                    // printf("closest point: from %d to %d for %f\n", save_m, k, distances[save_m][cnt_k]);
                }
                cnt_k++;
                // print_dp(dp, state_count, N - 1);
                // print_nh(next_hop, state_count, N - 1);
            }
            current_state = k_combination_next(current_state);
        }
    }

    current_state = state_count - 1;
    for (int i = 0; i < N - 1; ++i) {
        cost = dp[current_state][i] + distances[0][i + 1];
        if (min_path > cost) {
            min_path = cost;
            save_m = i;
        }
    }

    for (int i = N - 1; i > 0; --i) {
        path[i] = find_k_one(current_state, save_m + 1) + 2;
        save_m = next_hop[current_state][save_m];
        current_state = remove_point(current_state, path[i] - 2);
    }
    path[0] = 1;

    return min_path;
}