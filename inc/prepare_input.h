#ifndef __prepare_input_h_
#define __prepare_input_h_

float distance_between(float xi, float yi, float xj, float yj);
void print_distance(int n, float **distance);
float** prepare_input(int MAX_N, char *filename, int *n);

#endif