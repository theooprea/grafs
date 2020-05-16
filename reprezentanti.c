#include <stdio.h>
#include <stdlib.h>
#include "./cerinta2.h"

#define INFINITY 99999999
Heap *make_heap() {
    Heap *new_heap = malloc(sizeof(Heap));
    new_heap->values = malloc(20000 * sizeof(int));
    new_heap->length = 0;
    return new_heap;
}
void precolate_up(Heap *heap, int index_current, int *distance) {
    int aux;
    while (index_current != 0 && distance[heap->values[index_current]]
    <= distance[heap->values[(index_current - 1) / 2]]) {
        aux = heap->values[index_current];
        heap->values[index_current] = heap->values[(index_current - 1) / 2];
        heap->values[(index_current - 1) / 2] = aux;
        index_current = (index_current - 1) / 2;
    }
}
void heap_push(Heap *heap, int value, int *distance) {
    heap->values[heap->length++] = value;
    precolate_up(heap, heap->length - 1, distance);
}
void percolate_down(Heap *heap, int index_current, int *distance) {
    int minimum_child_idx = index_current;
    int index_left_child = index_current * 2 + 1;
    int index_right_child = index_current * 2 + 2;

    if (index_left_child < heap->length &&
    distance[heap->values[minimum_child_idx]] >=
    distance[heap->values[index_left_child]]) {
        minimum_child_idx = index_left_child;
    }

    if (index_right_child < heap->length &&
    distance[heap->values[minimum_child_idx]] >=
    distance[heap->values[index_right_child]]) {
        minimum_child_idx = index_right_child;
    }

    if (minimum_child_idx == index_current)
        return;

    int aux = heap->values[index_current];
    heap->values[index_current] = heap->values[minimum_child_idx];
    heap->values[minimum_child_idx] = aux;
    percolate_down(heap, minimum_child_idx, distance);
}
void heap_pop(Heap *heap, int *distance) {
    heap->values[0] = heap->values[heap->length - 1];
    heap->length -= 1;
    percolate_down(heap, 0, distance);
}
void print_heap(Heap *heap) {
    int i;
    for (i = 0; i < heap->length; i++) {
        printf("%d ", heap->values[i]);
    }
}
int closest_reprezentant(int *distance, int *predecesori, int nod) {
    int aux = nod;
    while (distance[aux] != 0) {
        aux = predecesori[aux];
    }
    return aux;
}
void decrease_priority(Heap *heap, int nod, int *distance) {
    int i;
    for (i = 0; i < heap->length; i++) {
        if (heap->values[i] == nod) {
            precolate_up(heap, i, distance);
        }
    }
}
int is_in_queue(Heap *heap, int nod) {
    int i;
    for (i = 0; i < heap->length; i++) {
        if (heap->values[i] == nod)
            return 1;
    }
    return 0;
}
void Dijkstra(Graph *graf, int start, int *distance, int *predecesori) {
    Heap *min_heap = make_heap();
    int *selectat = malloc(10000 * sizeof(int));
    int i, j;
    for (i = 1; i < graf->nr_nodes + 1; i++) {
        distance[i] = graf->cost_matrix[start][i];
        selectat[i] = 0;
        predecesori[i] = -1;
    }
    selectat[start] = 1;
    distance[start] = 0;
    heap_push(min_heap, start, distance);
    while (min_heap->length > 0) {
        int vecin = min_heap->values[0];
        selectat[vecin] = 1;
        heap_pop(min_heap, distance);
        for (j = 1; j < graf->nr_nodes + 1; j++) {
            if (graf->adjacency_matrix[vecin][j] == 1 && selectat[j] == 0 &&
            distance[j] >= distance[vecin] + graf->cost_matrix[vecin][j]) {
                if (distance[j] == distance[vecin] +
                graf->cost_matrix[vecin][j]) {
                    if (distance[vecin] == 0) {
                        distance[j] = distance[vecin] +
                        graf->cost_matrix[vecin][j];
                        predecesori[j] = vecin;
                        if (is_in_queue(min_heap, j) == 1) {
                            decrease_priority(min_heap, j, distance);
                        } else {
                            heap_push(min_heap, j, distance);
                        }
                    }
                    int predj = closest_reprezentant(distance, predecesori, j);
                    int predvecin = closest_reprezentant(distance,
                    predecesori, vecin);
                    if (predvecin < predj) {
                        distance[j] = distance[vecin] +
                        graf->cost_matrix[vecin][j];
                        predecesori[j] = vecin;
                        if (is_in_queue(min_heap, j) == 1) {
                            decrease_priority(min_heap, j, distance);
                        } else {
                            heap_push(min_heap, j, distance);
                        }
                    }
                } else {
                    distance[j] = distance[vecin] +
                    graf->cost_matrix[vecin][j];
                    predecesori[j] = vecin;
                    if (is_in_queue(min_heap, j) == 1) {
                        decrease_priority(min_heap, j, distance);
                    } else {
                        heap_push(min_heap, j, distance);
                    }
                }
            }
        }
    }
    free(min_heap->values);
    free(selectat);
    free(min_heap);
}
Graph *make_graph(int nr_nodes) {
    Graph *new_graf = malloc(sizeof(Graph));
    int i, j;
    new_graf->nr_nodes = nr_nodes;
    new_graf->adjacency_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->cost_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    for (i = 0; i < nr_nodes + 1; i++) {
        new_graf->adjacency_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        new_graf->cost_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        for (j = 0; j < nr_nodes + 1; j++) {
            new_graf->adjacency_matrix[i][j] = 0;
            new_graf->cost_matrix[i][j] = INFINITY;
        }
    }
    return new_graf;
}
void print_graf_matrixes(Graph *graf) {
    int i, j;
    printf("matricea de adiacenta:\n");
    for (i = 1; i < graf->nr_nodes + 1; i++) {
        for (j = 1; j < graf->nr_nodes + 1; j++) {
            printf("%d ", graf->adjacency_matrix[i][j]);
        }
        printf("\n");
    }
    printf("matricea de costuri:\n");
    for (i = 1; i < graf->nr_nodes + 1; i++) {
        for (j = 1; j < graf->nr_nodes + 1; j++) {
            printf("%d ", graf->cost_matrix[i][j]);
        }
        printf("\n");
    }
}
void add_edge_and_cost(Graph *graf, int source, int destination, int cost) {
    graf->adjacency_matrix[source][destination] = 1;
    graf->adjacency_matrix[destination][source] = 1;
    graf->cost_matrix[source][destination] = cost;
    graf->cost_matrix[destination][source] = cost;
    graf->cost_matrix[source][source] = 0;
    graf->cost_matrix[destination][destination] = 0;
}
void free_graf(Graph *graf) {
    int i;
    for (i = 0; i < graf->nr_nodes + 1; i++) {
        free(graf->adjacency_matrix[i]);
        free(graf->cost_matrix[i]);
    }
    free(graf->adjacency_matrix);
    free(graf->cost_matrix);
    free(graf);
}
void print_matrix(int **matrix, int rows, int columns) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 1; j < columns + 1; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
void free_rest(int **matrix, int rows, int *vector) {
    int i;
    for (i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
}
int main() {
    FILE *fisier_in, *fisier_out;
    fisier_in = fopen("reprezentanti.in", "r");
    fisier_out = fopen("reprezentanti.out", "w");
    int nr_nodes, nr_vertices, nr_reprezentanti, i, source, destination,
    cost, *reprezentanti, *distance, *predecesori;
    fscanf(fisier_in, "%d %d %d", &nr_nodes, &nr_vertices, &nr_reprezentanti);
    Graph *graf = make_graph(nr_nodes);
    for (i = 0; i < nr_vertices; i++) {
        fscanf(fisier_in, "%d %d %d", &source, &destination, &cost);
        add_edge_and_cost(graf, source, destination, cost);
    }
    reprezentanti = malloc(nr_reprezentanti * sizeof(int));
    distance = malloc((nr_nodes + 1) * sizeof(int));
    predecesori = malloc((nr_nodes + 1) * sizeof(int));
    for (i = 0; i < nr_reprezentanti; i++) {
        if (i == 0) {
            fscanf(fisier_in, "%d ", &reprezentanti[i]);
        } else {
            fscanf(fisier_in, "%d ", &reprezentanti[i]);
            add_edge_and_cost(graf, reprezentanti[0], reprezentanti[i], 0);
        }
    }
    Dijkstra(graf, reprezentanti[0], distance, predecesori);
    for (i = 1; i < graf->nr_nodes + 1; i++) {
        if (distance[i] == 0) {
            fprintf(fisier_out, "%d %d\n", i, 0);
        } else {
            if (distance[i] == INFINITY) {
                fprintf(fisier_out, "%d\n", -1);
            } else {
                fprintf(fisier_out, "%d %d\n", closest_reprezentant(distance,
                predecesori, i), distance[i]);
            }
        }
    }
    free_graf(graf);
    fclose(fisier_in);
    fclose(fisier_out);
    return 0;
}
