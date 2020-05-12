#ifndef __FILE_H__
#define __FILE_H__
//structura in care tin matricea de adiacenta, vectorul de parinti si vectorul
//de aparitii
typedef struct graph {
    int **adjacency_matrix;
    int **cost_matrix;
    int nr_nodes;
} Graph;
typedef struct heap {
    int *values;
    int length;
} Heap;
#endif /* __FILE_H__ */