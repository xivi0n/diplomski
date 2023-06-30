#ifndef __bhk_h_
#define __bhk_h_

float bhk_tsp(float **distances, int N, int path[]);
void print_path(int path[], int N);
int find_k_one(unsigned int state, int N);

#endif