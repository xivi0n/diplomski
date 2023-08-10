#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../inc/prepare_input.h"

float distance_between(float xi, float yi, float xj, float yj) {
    return sqrt(pow(xi - xj, 2) + pow(yi - yj, 2));
}

void print_distance(int N, float **distance) {
    printf("--------------- Distance matrix ---------------\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%10.10f\t", distance[i][j]);
        }
        printf("\n");
    }
}

float** get_points(int MAX_N, const char *filename, int *N) {
    float *x = (float*) malloc(MAX_N * sizeof(float));
    float *y = (float*) malloc(MAX_N * sizeof(float));

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    const char *separator = ", ";
    char *line = (char*) malloc(100 * sizeof(char));
    int is_x = 1;
    while (fscanf(fp, "%[^\n]%*c", line) != EOF && *N < MAX_N) {
        char *token = strtok(line, separator);
        while (token != NULL) {
            if (is_x) {
                x[*N] = atof(token);
                is_x = 0;
            } else {
                y[*N] = atof(token);
                is_x = 1;
            }
            token = strtok(NULL, separator);
        }
        (*N)++;
    }
    fclose(fp);
    float **points = (float**) malloc(2 * sizeof(float*));
    points[0] = x;
    points[1] = y;
    return points;
}

float** generate_distances(int N, float** points) {
    float **distance = (float**) malloc(N * sizeof(float*));
    for (int i = 0; i < N; i++) {
        distance[i] = (float*) malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            distance[i][j] = distance_between(points[0][i], points[1][i], points[0][j], points[1][j]);
        }
    }
    return distance;
}