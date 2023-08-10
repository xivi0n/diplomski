#ifndef __prepare_input_h_
#define __prepare_input_h_

float distance_between(float xi, float yi, float xj, float yj);
void print_distance(int N, float **distance);
float** get_points(int MAX_N, const char *filename, int *N);
float** generate_distances(int N, float** points);

#endif