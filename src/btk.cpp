#include <queue>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inc/btk.h"
#include <unordered_map>


inline int count_ones(unsigned long long number) {
    int count = 0;

    while (number != 0) {
        if (number & 1) {
            count++;
        }
        number >>= 1;
    }
    return count;
}

static inline unsigned long long add_point(unsigned long long state, int k) {
    return state | (1 << k);
}

static inline unsigned long long check_point(unsigned long long state, int k) {
    return state & (1 << k);
}

static inline unsigned long long remove_point(unsigned long long state, int k) {
    return state & ~(1 << k);
}

void print_binary_1(unsigned long long combination, int n) {
    if (--n) {
        print_binary_1(combination / 2, n);
    }
    printf("%llu", combination % 2);
}

struct Element {
    float path_cost;
    unsigned long long state;
    short point;

    Element(float pc, unsigned long long s, char p) : 
        path_cost(pc), state(s), point(p) {}
};

struct ElementComparator {
    bool operator()(const Element* lhs, const Element* rhs) const {
        if (lhs->path_cost != rhs->path_cost) {
            return lhs->path_cost > rhs->path_cost;
        } else {
            return count_ones(lhs->state) < count_ones(rhs->state);
        }
    }
};

float btk_tsp(float **distances, int N, int path[]) {
    std::priority_queue<Element*, std::vector<Element*>, ElementComparator> pq;
    std::vector<std::unordered_map<unsigned long long, float> > hashed_paths(N);

    // hashed_paths = (std::unordered_map<unsigned long long, float>*)
    //     malloc(N * sizeof(std::unordered_map<unsigned long long, float>));

    int count;
    unsigned long long state;

    Element *item = new Element(0, 0, 0);
    pq.push(item);

    while(!pq.empty()) {

        // std::priority_queue<Element*, std::vector<Element*>, ElementComparator> tempPq = pq;
        // printf("PQ START:\n=========\n");
        // while (!tempPq.empty()) {
        //     printf("%d %10.5f ", tempPq.top()->point, tempPq.top()->path_cost);print_binary_1(tempPq.top()->state, N);printf("\n");
        //     tempPq.pop();
        // }
        // printf("=========\n");

        item = pq.top();
        pq.pop();

        count = count_ones(item->state);

        if (count == N) {
            break;
        }
        auto element = hashed_paths[item->point].find(item->state);
        if (element != hashed_paths[item->point].end() && element->second < item->path_cost) continue;

        hashed_paths[item->point][item->state] = item->path_cost;

        if (count == N - 1) {
            state = add_point(item->state, 0);
            pq.push(new Element(item->path_cost + distances[0][item->point], state, 0));
        } else {
            for (int i = 1; i < N; ++i) {
                if (!check_point(item->state, i)) {
                    state = add_point(item->state, i);
                    pq.push(new Element(item->path_cost + distances[item->point][i], state, i));
                }
            }
        } 
        free(item);
    }

    return item->path_cost;
}