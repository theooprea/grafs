#ifndef _HOME_STUDENT_BONUS_H_
#define _HOME_STUDENT_BONUS_H_
// structura in care tin matricea de adiacenta, vectorul de parinti si vectorul
// de aparitii
typedef struct nod {
    int nr_nod;
    int nr_vecini;
    int *vecini;
} Nod;
typedef struct graph {
    int **adjacency_matrix;
    int **cost_matrix;
    Nod *noduri;
    int nr_nodes;
} Graph;
typedef struct heap {
    int *values;
    int length;
} Heap;
#endif  // _HOME_STUDENT_BONUS_H_
