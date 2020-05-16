#include <stdio.h>
#include <stdlib.h>
#include "./bonus.h"

#define INFINITY 99999999
Heap *make_heap() {
    Heap *new_heap = malloc(sizeof(Heap));
    new_heap->values = malloc(10005 * sizeof(int));
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
int maxim(int a, int b) {
    if (a >= b)
        return a;

    return b;
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
void add_edge_and_cost(Graph *graf, int source,
int destination, int cost) {
    graf->adjacency_matrix[source][destination] = 1;
    graf->adjacency_matrix[destination][source] = 1;
    graf->cost_matrix[source][destination] = cost;
    graf->cost_matrix[destination][source] = cost;
    graf->cost_matrix[source][source] = 0;
    graf->cost_matrix[destination][destination] = 0;
    graf->noduri[source].vecini[graf->noduri[source].nr_vecini] = destination;
    graf->noduri[destination].vecini[graf->noduri[destination].nr_vecini]
    = source;
    graf->noduri[source].nr_vecini++;
    graf->noduri[destination].nr_vecini++;
}
void remove_edge(Graph *graf, int source, int destination,
int *cost) {
    *cost = graf->cost_matrix[source][destination];
    graf->adjacency_matrix[source][destination] = 0;
    graf->adjacency_matrix[destination][source] = 0;
    graf->cost_matrix[source][destination] = INFINITY;
    graf->cost_matrix[destination][source] = INFINITY;
}
int Dijkstra(Graph *graf, int start, int destination, int *distance, int *selectat, Heap *min_heap) {
    int i;
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
        for (i = 0; i < graf->noduri[vecin].nr_vecini; i++) {
            if (selectat[graf->noduri[vecin].vecini[i]] == 0 &&
            distance[graf->noduri[vecin].vecini[i]] >= distance[vecin] &&
            distance[graf->noduri[vecin].vecini[i]] >=
            graf->cost_matrix[vecin][graf->noduri[vecin].vecini[i]]) {
                distance[graf->noduri[vecin].vecini[i]] =
                maxim(distance[vecin],
                graf->cost_matrix[vecin][graf->noduri[vecin].vecini[i]]);
                if (is_in_queue(min_heap,
                graf->noduri[vecin].vecini[i]) == 1) {
                    decrease_priority(min_heap,
                    graf->noduri[vecin].vecini[i], distance);
                } else {
                    heap_push(min_heap, graf->noduri[vecin].vecini[i],
                    distance);
                }
            }
        }
    }
    return distance[destination];
}
Graph *make_graph(int nr_nodes) {
    Graph *new_graf = malloc(sizeof(Graph));
    int i, j;
    new_graf->nr_nodes = nr_nodes;
    new_graf->adjacency_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->cost_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->noduri = malloc((nr_nodes + 1) * sizeof(Nod));
    for (i = 0; i < nr_nodes + 1; i++) {
        new_graf->adjacency_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        new_graf->cost_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        new_graf->noduri[i].nr_nod = i;
        new_graf->noduri[i].nr_vecini = 0;
        new_graf->noduri[i].vecini = malloc(nr_nodes * sizeof(int));
        for (j = 0; j < nr_nodes + 1; j++) {
            new_graf->adjacency_matrix[i][j] = 0;
            new_graf->cost_matrix[i][j] = INFINITY;
        }
    }
    return new_graf;
}
void free_graf(Graph *graf) {
    int i;
    for (i = 0; i < graf->nr_nodes + 1; i++) {
        free(graf->adjacency_matrix[i]);
        free(graf->cost_matrix[i]);
        free(graf->noduri[i].vecini);
    }
    free(graf->noduri);
    free(graf->adjacency_matrix);
    free(graf->cost_matrix);
    free(graf);
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
void print_neighbours(Graph *graf) {
    int i, j;
    for (i = 1; i < graf->nr_nodes + 1; i++) {
        printf("%d -> ", i);
        for (j = 0; j < graf->noduri[i].nr_vecini; j++) {
            printf("%d ", graf->noduri[i].vecini[j]);
        }
        printf("\n");
    }
}
int main() {
    FILE *fisier_in, *fisier_out;
    fisier_in = fopen("bonus6.in", "r");
    fisier_out = fopen("bonus.out", "w");
    int nr_nodes, nr_vertices, i, source, destination, cost, nr_query, *selected, **new_matrix, *did_Dijkstra;
    fscanf(fisier_in, "%d %d %d", &nr_nodes, &nr_vertices, &nr_query);
    Graph *graf = make_graph(nr_nodes);
    for (i = 0; i < nr_vertices; i++) {
        fscanf(fisier_in, "%d %d %d", &source, &destination, &cost);
        add_edge_and_cost(graf, source, destination, cost);
    }
    new_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    selected = malloc((nr_nodes + 1) * sizeof(int));
    Heap *min_heap = make_heap();
    did_Dijkstra = calloc(nr_nodes + 1, sizeof(int));
    for (i = 0; i < nr_query; i++) {
        fscanf(fisier_in, "%d %d", &source, &destination);
        if (did_Dijkstra[source] == 1 || did_Dijkstra[destination] == 1)
        {
            if(did_Dijkstra[source] == 1)
            {
                fprintf(fisier_out, "%d\n", new_matrix[source][destination]);
            }
            else
            {
                fprintf(fisier_out, "%d\n", new_matrix[destination][source]);
            }
        }
        else
        {
            new_matrix[source] = malloc((nr_nodes + 1) * sizeof(int));
            did_Dijkstra[source] = 1;
            fprintf(fisier_out, "%d\n", Dijkstra(graf, source, destination, new_matrix[source], selected, min_heap));            
        }
    }
    for(i = 0; i < nr_nodes + 1; i++)
    {
        free(new_matrix[i]);
    }
    free(did_Dijkstra);
    free(new_matrix);
    free_graf(graf);
    free(selected);
    free(min_heap->values);
    free(min_heap);
    fclose(fisier_in);
    fclose(fisier_out);
    return 0;
}
