#include <stdio.h>
#include <stdlib.h>
#include "./cerinta1.h"

#define INFINITY 99999999
Heap *make_heap() {
    Heap *new_heap = malloc(sizeof(Heap));
    new_heap->values = malloc(1005 * sizeof(int));
    new_heap->length = 0;
    return new_heap;
}
void precolate_up(Heap *heap, int index_current, int *distance) {
    int aux;
    while (index_current != 0 && distance[heap->values[index_current]] <=
    distance[heap->values[(index_current - 1) / 2]]) {
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
int Dijkstra(Graph *graf, int start, int destination) {
    Heap *min_heap = make_heap();
    int *selectat = malloc(1005 * sizeof(int));
    int *distance = malloc(1005 * sizeof(int));
    int i, j;
    for (i = 1; i < graf->nr_nodes + 1; i++) {
        distance[i] = graf->cost_matrix[start][i];
        selectat[i] = 0;
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
                distance[j] = distance[vecin] + graf->cost_matrix[vecin][j];
                heap_push(min_heap, j, distance);
            }
        }
    }
    int distanta = distance[destination];
    free(min_heap->values);
    free(selectat);
    free(distance);
    free(min_heap);
    return distanta;
}
Graph *make_graph(int nr_nodes) {
    Graph *new_graf = malloc(sizeof(Graph));
    int i, j;
    new_graf->nr_nodes = nr_nodes;
    new_graf->adjacency_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->cost_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->parent_vector = malloc((nr_nodes + 1) * sizeof(int));
    new_graf->visited_vector = malloc((nr_nodes + 1) * sizeof(int));
    for (i = 0; i < nr_nodes + 1; i++) {
        new_graf->adjacency_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        new_graf->cost_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        for (j = 0; j < nr_nodes + 1; j++) {
            new_graf->adjacency_matrix[i][j] = 0;
            new_graf->cost_matrix[i][j] = INFINITY;
        }
        new_graf->parent_vector[i] = -1;
        new_graf->visited_vector[i] = 0;
    }
    return new_graf;
}
void add_edge_and_cost(Graph *graf, int source,
int destination, int cost_sd, int cost_ds) {
    if (graf->cost_matrix[destination][source] == 0) {
        graf->adjacency_matrix[source][destination] = 1;
        graf->adjacency_matrix[destination][source] = 1;
        graf->cost_matrix[source][destination] = 0;
        graf->cost_matrix[destination][source] = 0;
        return; 
    }
    graf->adjacency_matrix[source][destination] = 1;
    graf->adjacency_matrix[destination][source] = 1;
    graf->cost_matrix[source][destination] = cost_sd;
    graf->cost_matrix[destination][source] = cost_ds;
}
void remove_edge(Graph *graf, int source, int destination,
int *cost_source_destination, int *cost_destination_source) {
    *cost_source_destination = graf->cost_matrix[source][destination];
    *cost_destination_source = graf->cost_matrix[destination][source];
    graf->adjacency_matrix[source][destination] = 0;
    graf->adjacency_matrix[destination][source] = 0;
    graf->cost_matrix[source][destination] = INFINITY;
    graf->cost_matrix[destination][source] = INFINITY;
}
void free_graf(Graph *graf) {
    int i;
    free(graf->visited_vector);
    free(graf->parent_vector);
    for (i = 0; i < graf->nr_nodes + 1; i++) {
        free(graf->adjacency_matrix[i]);
        free(graf->cost_matrix[i]);
    }
    free(graf->adjacency_matrix);
    free(graf->cost_matrix);
    free(graf);
}
void DFS(Graph *graf, int start_node,int *cost_minim)
{
    graf->visited_vector[start_node] = 1;
    int vecin;
    for (vecin = 0; vecin < graf->nr_nodes + 1; vecin++) {
        if (graf->adjacency_matrix[start_node][vecin] == 1 &&
        graf->cost_matrix[start_node][vecin] == 0 &&
        graf->cost_matrix[vecin][start_node] == 0) {
            *cost_minim = 0;
            return;
        }
        if (graf->adjacency_matrix[start_node][vecin] == 1 &&
        graf->visited_vector[vecin] == 1
        && graf->parent_vector[start_node] != vecin) {
            int cost_sd, cost_ds;
            remove_edge(graf, start_node, vecin, &cost_sd, &cost_ds);
            int cost = Dijkstra(graf, start_node, vecin);
            cost += cost_ds;
            if (cost < (*cost_minim)) {
                (*cost_minim) = cost;
            }
            add_edge_and_cost(graf, start_node, vecin, cost_sd, cost_ds);
        }
        if (graf->adjacency_matrix[start_node][vecin] == 1 &&
        graf->visited_vector[vecin] == 0) {
            graf->parent_vector[vecin] = start_node;
            DFS(graf, vecin, cost_minim);
        }
    }
}
int main()
{
    FILE *fisier_in, *fisier_out;
    fisier_in = fopen("schimbare.in", "r");
    fisier_out = fopen("schimbare.out", "w");
    int nr_nodes, nr_vertices, i, source, destination, cost;
    fscanf(fisier_in, "%d %d", &nr_nodes, &nr_vertices);
    Graph *graf = make_graph(nr_nodes);
    for (i = 0; i < nr_vertices; i++) {
        fscanf(fisier_in, "%d %d %d", &source, &destination, &cost);
        add_edge_and_cost(graf, source, destination, 0, cost);
    }
    int cost_minim = INFINITY;
    for (i = nr_nodes; i > 0; i--) {
        if (graf->visited_vector[i] == 0) {
            DFS(graf, i, &cost_minim);
        }
    }
    if (cost_minim == INFINITY) {
        cost_minim = -1;
    }
    fprintf(fisier_out, "%d\n", cost_minim);
    free_graf(graf);
    fclose(fisier_in);
    fclose(fisier_out);
    return 0;
}
