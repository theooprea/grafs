#include <stdio.h>
#include <stdlib.h>
#include "graf.h"
#define INFINITY 99999999
Heap *make_heap(int nr_noduri) {
    Heap *new_heap = malloc(sizeof(Heap));
    new_heap->values = malloc(nr_noduri * sizeof(int));
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
int get_cost_a_to_b(Graph *graf, int a, int b)
{
    list_node *aux = graf->lista_adiacenta[a];
    while(aux != NULL)
    {
        if(aux->vecin == b)
            return aux->cost;

        aux = aux->next;
    }
    return INFINITY;
}
int Dijkstra(Graph *graf, int start, int destination) {
    int i;
    list_node *aux;
    Heap *min_heap = make_heap(graf->nr_noduri + 1);
    int *selectat = malloc((graf->nr_noduri + 1) * sizeof(int));
    int *distance = malloc((graf->nr_noduri + 1) * sizeof(int));
    for (i = 1; i < graf->nr_noduri + 1; i++) {
        distance[i] = INFINITY;
        selectat[i] = 0;
    }
    selectat[start] = 1;
    distance[start] = 0;
    heap_push(min_heap, start, distance);
    while (min_heap->length > 0) {
        int vecin = min_heap->values[0];
        selectat[vecin] = 1;
        heap_pop(min_heap, distance);
        aux = graf->lista_adiacenta[vecin];
        while(aux != NULL)
        {
            if(selectat[aux->vecin] == 0 && distance[aux->vecin] >= distance[vecin] &&
            distance[aux->vecin] >= get_cost_a_to_b(graf, vecin, aux->vecin))
            {
                distance[aux->vecin] = maxim(distance[vecin], get_cost_a_to_b(graf, vecin, aux->vecin));
                if(is_in_queue(min_heap, aux->vecin) == 1)
                {
                    decrease_priority(min_heap, aux->vecin, distance);
                }
                else
                {
                    heap_push(min_heap, aux->vecin, distance);
                }
            }
            aux = aux->next;
        }
    }
    int distanta = distance[destination];
    free(min_heap->values);
    free(selectat);
    free(distance);
    free(min_heap);
    return distanta;
}
Graph *new_graf(int nr_nodes)
{
    int i;
    Graph *graf = malloc(sizeof(Graph));
    graf->nr_noduri = nr_nodes;
    graf->lista_adiacenta = malloc((nr_nodes + 1) * sizeof(list_node *));
    for(i = 0; i < nr_nodes + 1; i++)
    {
        graf->lista_adiacenta[i] = NULL;
    }
    return graf;
}
list_node *new_node(int nr_nod, int cost)
{
    list_node *nod = malloc(sizeof(list_node));
    nod->next = NULL;
    nod->prev = NULL;
    nod->cost = cost;
    nod->vecin = nr_nod;
    return nod;
}
void add_in_list(list_node **head, list_node *nod)
{
    if((*head) == NULL)
    {
        (*head) = nod;
    }
    else
    {
        nod->next = (*head);
        (*head)->prev = nod;
        (*head) = nod;  
    }
}
void add_edge(Graph *graf, int source, int destination, int cost)
{
    list_node *nod_source_to_destination = new_node(destination, cost);
    add_in_list(&graf->lista_adiacenta[source], nod_source_to_destination);
    list_node *nod_destination_to_source = new_node(source, cost);
    add_in_list(&graf->lista_adiacenta[destination], nod_destination_to_source);
}
void print_lists(Graph *graf)
{
    int i;
    list_node *aux;
    for(i = 1; i < graf->nr_noduri + 1; i++)
    {
        printf("%d -> ", i);
        if(graf->lista_adiacenta[i] != NULL)
        {
            aux = graf->lista_adiacenta[i];
            while(aux != NULL)
            {
                printf("(%d %d) ", aux->vecin, aux->cost);
                aux = aux->next;
            }
        }
        printf("\n");
    }
}
void free_list(list_node *head)
{
    list_node *aux;
    while(head != NULL)
    {
        aux = head;
        head = head->next;
        free(aux);
    }
}
void free_graf(Graph *graf)
{
    int i;
    for(i = 0; i < graf->nr_noduri + 1; i++)
    {
        free_list(graf->lista_adiacenta[i]);
    }
    free(graf->lista_adiacenta);
    free(graf);
}
int main()
{
    FILE *fisier_in, *fisier_out;
    fisier_in = fopen("bonus7.in", "r");
    fisier_out = fopen("bonus.out", "w");
    int nr_nodes, nr_edges, source, destination, cost, nr_query, i;
    fscanf(fisier_in, "%d %d %d", &nr_nodes, &nr_edges, &nr_query);
    Graph *graf = new_graf(nr_nodes);
    for (i = 0; i < nr_edges; i++)
    {
        fscanf(fisier_in, "%d %d %d", &source, &destination, &cost);
        add_edge(graf, source, destination, cost);
    }
    for (i = 0; i < nr_query; i++)
    {
        fscanf(fisier_in, "%d %d", &source, &destination);
        fprintf(fisier_out, "%d\n", Dijkstra(graf, source, destination));
        /*if (did_Dijkstra[source] == 1 || did_Dijkstra[destination] == 1)
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
            //new_matrix[source] = malloc((nr_nodes + 1) * sizeof(int));
            did_Dijkstra[source] = 1;
            fprintf(fisier_out, "%d\n", Dijkstra(graf, source, destination, new_matrix[source], selectat, min_heap));            
        }*/
    }
    free_graf(graf);
    fclose(fisier_in);
    fclose(fisier_out);
    return 0;
}