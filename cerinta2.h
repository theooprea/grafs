#ifndef _HOME_STUDENT_CERINTA2_H_
#define _HOME_STUDENT_CERINTA2_H_
// structura in care tin matricea de adiacenta, vectorul de parinti si vectorul
// de aparitii
typedef struct graph {
    int **adjacency_matrix;
    int **cost_matrix;
    int nr_nodes;
} Graph;
typedef struct heap {
    int *values;
    int length;
} Heap;
#endif  // _HOME_STUDENT_CERINTA2_H_
