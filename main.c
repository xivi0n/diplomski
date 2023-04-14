#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "prepare_input.h"
#include "k_combination.h"

#define MAX_N 256

float **distance = NULL;
char *filename = "input.txt";
int n = 0;

int main() {
    distance = prepare_input(MAX_N, filename, &n);
    printf("n = %d\n", n);
    return 0;
}
