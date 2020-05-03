#ifndef __FILE_H__
#define __FILE_H__
//structura in care tin matricea de adiacenta, vectorul de parinti si vectorul
//de aparitii
typedef struct graph {
    int **adjacency_matrix;
    int **cost_matrix;
    int *parent_vector;
    int *visited_vector;
    int *is_part_of_cycle;
    int nr_nodes;
} Graph;
#endif /* __FILE_H__ */