#include <stdint.h>
#include <stdio.h>
#include "k_combination.h"

#define left 1
#define riht -1
#define forw 1
#define back -1

typedef enum {
    TUP, TUQ, TDP, TDQ, PQ, QP, PP, QQ,
    ULP, ULQ, LUP, LUQ, INIT, FINI
} Moves;

typedef struct {
    int spec;
    enum { lower, upper} side;
    int p;
    Moves nm;
} StackElement;

#define MAX_N 256

#define sbit(bf, k)   ( bf[(k) / 64] |=  (1 << ((k) % 64)) )
#define cbit(bf, k)   ( bf[(k) / 64] &= ~(1 << ((k) % 64)) )
#define tbit(bf, k)   ( bf[(k) / 64] &   (1 << ((k) % 64)) )

int             k_comb_n, k_comb_k;
int             k_comb_count;
uint64_t        k_comb_x[MAX_N];
int             k_comb_p1[MAX_N];
StackElement    k_comb_stk[MAX_N];

void k_comb_write() {
    int i;
    k_comb_count++;
    for (i = k_comb_n; i >= 1; i--) {
        printf("%lld ", k_comb_x[i]);
    }
    printf("\n");
}


void k_comb_move(int n, int dir) {
    k_comb_count++;
    k_comb_x[k_comb_p1[n]] = 0;
    k_comb_p1[n] += dir;
    k_comb_x[k_comb_p1[n]] = 1;
}

void k_comb_init(int dir, int level) {
    if (dir == forw) {
        k_comb_stk[level].nm = INIT;
        k_comb_stk[level].spec = 0;
        k_comb_stk[level].side = upper;
    } else if (dir == back) {
        k_comb_stk[level].nm = FINI;
        k_comb_stk[level].spec = 1;
        k_comb_stk[level].side = lower;
    }
    k_comb_stk[level].p = 1;
}

void k_comb_next(int n, int k, int level, int dir) {
    if (k == 1) {
        k_comb_move(1, -dir);
    } else if (k == n - 1) {
        if (dir == forw && k_comb_p1[1] == 2) k_comb_stk[level].p = 1;
        if (dir == back && k_comb_p1[k] == k) k_comb_stk[level].p = k;
        k_comb_move(k_comb_stk[level].p, -dir);
        k_comb_stk[level].p += dir;
    } else {
        switch (k_comb_stk[level].nm)
        {
        case INIT:
            if (dir == forw) {
                if (k_comb_p1[k-2] == k - 2) {
                    k_comb_move(k - 1, riht);
                    k_comb_stk[level].spec = 1;
                    k_comb_stk[level].side = upper;
                    k_comb_stk[level].nm = PQ;
                } else {
                    if (k_comb_p1[1] == n - k + 1) k_comb_init(forw, level + 1);
                    k_comb_next(n - 2, k - 2, level + 1, forw);
                }
            } else if (dir == back) {
                k_comb_next(n - 2, k - 2, level + 1, back);
            }
            break;
        case FINI:
            if (dir == forw) {
                k_comb_next(n - 2, k - 2, level + 1, forw);
            } else if (dir == back) {
                if (k_comb_p1[1] == n - k - 1) {
                    k_comb_move(k, left);
                    k_comb_stk[level].spec = 0;
                    k_comb_stk[level].side = lower;
                    k_comb_stk[level].nm = LUQ;
                } else {
                    if (k_comb_p1[k] == k) k_comb_init(back, level + 1);
                    k_comb_next(n - 2, k, level + 1, back);
                }
            }
            break;
        case PP:
            k_comb_next(n - 4, k - 2, level + 1, -dir);
            if (dir == back && k_comb_p1[k - 2] == k - 2) {
                k_comb_stk[level].nm = PQ;
                k_comb_stk[level].spec = 1;
            } else {
                if (k_comb_x[n - 4] == 0)
                    k_comb_stk[level].nm = LUP;
                else {
                    k_comb_stk[level].nm = TDP;
                    k_comb_stk[level].p = k - 2;
                }
            } 
            break;
        case QQ:
            if (dir == forw) {
                if (k_comb_p1[1] == n - k - 1) {
                    k_comb_move(k, riht);
                    k_comb_stk[level].nm = FINI;
                    k_comb_stk[level + 1].p = 1;
                } else {
                    k_comb_next(n - 4, k - 2, level + 1, back);
                    if (k_comb_x[n - 4] == 1)
                        k_comb_stk[level].nm = LUQ;
                    else
                        k_comb_stk[level].nm = TDQ;
                }
            } else if (dir == back) {
                k_comb_next(n - 4, k - 2, level + 1, back);
                if (k_comb_x[n - 4] == 1)
                    k_comb_stk[level].nm = LUQ;
                else
                    k_comb_stk[level].nm = TDQ;
            }
            break;
        case PQ:
            k_comb_move(k - 1, riht);
            if (k_comb_x[n - 4] == 1)
                k_comb_stk[level].nm = ULQ;
            else
                k_comb_stk[level].nm = TDQ;
            break;
        case QP:
            k_comb_move(k - 1, left);
            if (dir == forw && k_comb_stk[level].spec) {
                k_comb_stk[level].nm = PP;
                k_comb_stk[level].spec = 0;
            } else {
                if (k_comb_x[n - 4] == 1) {
                    k_comb_stk[level].nm = TDP;
                    k_comb_stk[level].p = k - 2;
                } else {
                    k_comb_stk[level].nm = ULP;
                }
            }
            break;
        case ULQ:
            k_comb_move(k, riht);
            if (k_comb_x[n - 3] == 0)
                k_comb_stk[level].nm = TUQ;
            else if (k_comb_stk[level].spec)
                k_comb_stk[level].nm = QP;
            else
                k_comb_stk[level].nm = QQ;
            k_comb_stk[level].side = lower;
            break;
        case LUQ:
            k_comb_move(k, left);
            if (k_comb_x[n - 3] == 0)
                k_comb_stk[level].nm = TUP;
            else k_comb_stk[level].nm = QP;
            k_comb_stk[level].side = upper;
            break;
        case ULP:
            if (dir == back && k_comb_stk[level].spec) {
                k_comb_move(k - 1, left);
                k_comb_stk[level].nm = INIT;
            } else {
                k_comb_move(k, riht);
                if (k_comb_x[n - 3] == 1) 
                    k_comb_stk[level].nm = TUP;
                else
                    k_comb_stk[level].nm = PP;
                k_comb_stk[level].side = lower;
            }
            break;
        case LUP:
            k_comb_move(k, left);
            if (k_comb_x[n - 3] == 1)
                k_comb_stk[level].nm = TUP;
            else
                k_comb_stk[level].nm = PQ;
            k_comb_stk[level].side = upper;
            break;
        case TUQ:
            k_comb_move(k - 1, left);
            if (k_comb_x[n - 3] == 1) {
                if (k_comb_stk[level].spec)
                    k_comb_stk[level].nm = QP;
                else if (k_comb_stk[level].side == upper)
                    k_comb_stk[level].nm = QP;
                else
                    k_comb_stk[level].nm = QQ;
            }
            break;
        case TDQ:
            k_comb_move(k - 1, riht);
            if (k_comb_p1[k - 1] == k_comb_p1[k - 2] + 1) {
                if (k_comb_stk[level].side == upper)
                    k_comb_stk[level].nm = ULQ;
                else
                    k_comb_stk[level].nm = LUQ;
            }
            break;
        case TUP:
            k_comb_move(k_comb_stk[level].p, riht);
            if (k_comb_stk[level].p == k - 2) {
                if (k_comb_stk[level].side == upper)
                    k_comb_stk[level].nm = PQ;
                else k_comb_stk[level].nm = PP;
            } else k_comb_stk[level].p = k_comb_stk[level].p + 1;
            break;
        case TDP:
            k_comb_move(k_comb_stk[level].p, left);
            if (k_comb_x[k_comb_p1[k_comb_stk[level].p] - 2] == 0) {
                if (k_comb_stk[level].side == upper)
                    k_comb_stk[level].nm = ULP;
                else
                    k_comb_stk[level].nm = LUP;
            } else k_comb_stk[level].p = k_comb_stk[level].p + 1;
            break;
        default:
            break;
        }
    }
}

void k_comb_initialize() {
    int i;
    for(i = 1; i <= k_comb_n - k_comb_k; ++i) {
        k_comb_x[i] = 0;
    }

    for(i = 1; i <= k_comb_k; ++i) {
        k_comb_x[k_comb_n - k_comb_k + i] = 1;
        k_comb_p1[i] = k_comb_n - k_comb_k + i;
    }
    k_comb_count = 0;
}

void g(int n, int k) {
    int i;
    if (k == 1) {
        for (i = 1; i <= n - 1; ++i) {
            k_comb_move(1, riht);
        }
    } else if (k == n - 1) {
        for (i = 1; i <= n - 1; ++i) {
            k_comb_move(i, riht);
        }
    } else {
        g(n - 2, k - 2);
        k_comb_move(k - 1, riht);
        k_comb_stk[1].spec = 1;
        k_comb_stk[1].nm = PQ;
        k_comb_stk[1].side = upper;
        k_comb_init(back, 2);
        do {
            k_comb_next(n, k, 1, forw);
        } while (k_comb_p1[k] == n - 1 && k_comb_p1[1] == n - k - 1 && k_comb_p1[k - 1] == n - 3);
        k_comb_move(k, riht);
        g(n - 2, k);
    }
}

int main() {
    int k_comb_n = 5;
    int k_comb_k = 1;
    k_comb_initialize();
    printf("init done\n");
    g(k_comb_n, k_comb_k);
    k_comb_write();
    return 0;
}