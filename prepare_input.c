#include "prepare_input.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float distance_between(float xi, float yi, float xj, float yj) {
    return sqrt(pow(xi - xj, 2) + pow(yi - yj, 2));
}

void print_distance(int n, float **distance) {
    printf("--------------- Distance matrix ---------------\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%10.10f\t", distance[i][j]);
        }
        printf("\n");
    }
}

float** prepare_input(int MAX_N, char *filename, int *n) {
    float *x = (float*) malloc(MAX_N * sizeof(float));
    float *y = (float*) malloc(MAX_N * sizeof(float));

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    char *separator = " ";
    char *line = (char*) malloc(100 * sizeof(char));
    int is_x = 1;
    while (fscanf(fp, "%[^\n]%*c", line) != EOF && *n < MAX_N) {
        char *token = strtok(line, separator);
        while (token != NULL) {
            if (is_x) {
                x[*n] = atof(token);
                is_x = 0;
            } else {
                y[*n] = atof(token);
                is_x = 1;
            }
            token = strtok(NULL, separator);
        }
        (*n)++;
    }
    fclose(fp);
    float **distance = (float**) malloc(*n * sizeof(float*));
    for (int i = 0; i < *n; i++) {
        distance[i] = (float*) malloc(*n * sizeof(float));
        for (int j = 0; j < *n; j++) {
            distance[i][j] = distance_between(x[i], y[i], x[j], y[j]);
        }
    }
    free(x);free(y);
    return distance;
}