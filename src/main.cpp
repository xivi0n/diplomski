#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include "../inc/prepare_input.h"
#include "../inc/bhk.h"
#include "../inc/btk.h"

#define MAX_N 20

float **distances = NULL, **points = NULL;
const char *filename = "inputs/_in263.txt";
int N;
int path[MAX_N];
float min_path;
clock_t start, end;

int main() {
    points = get_points(MAX_N, filename, &N);
    distances = generate_distances(N, points);

    for (N = 3; N <= MAX_N; ++N) {
        printf("N = %d\n", N);

        start = clock();
        min_path = bhk_tsp(distances, N, path);
        end = clock();

        print_path(path, N);
        printf("Cost: %f\n", min_path);
        printf("Time: %fs\n", 1.0 * (end - start) / CLOCKS_PER_SEC);
        printf("=============================\n");

        // memset(path, 0, MAX_N * sizeof(int));

        // start = clock();
        // min_path = btk_tsp(distances, N, path);
        // end = clock();

        // print_path(path, N);
        // printf("Cost: %f\n", min_path);
        // printf("Time: %fs\n", 1.0 * (end - start) / CLOCKS_PER_SEC);
        // printf("=============================\n");
    }
    return 0;
}
