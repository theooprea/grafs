#include <stdio.h>
#include <stdlib.h>
#include "cerinta1.h"

#define INFINITY 99999999
Heap *make_heap()
{
    Heap *new_heap = malloc(sizeof(Heap));
    new_heap->values = malloc(1005 * sizeof(int));
    new_heap->length = 0;
}
void precolate_up(Heap *heap, int index_current, int *distance)
{
    int aux;
    while(index_current != 0 && distance[heap->values[index_current]] <= distance[heap->values[(index_current - 1) / 2]])
    {
        aux = heap->values[index_current];
        heap->values[index_current] = heap->values[(index_current - 1) / 2];
        heap->values[(index_current - 1) / 2] = aux;
        index_current = (index_current - 1) / 2;
    }
}
void heap_push(Heap *heap, int value, int *distance)
{
    heap->values[heap->length++] = value;
    precolate_up(heap, heap->length - 1, distance);
}
void percolate_down(Heap *heap, int index_current, int *distance)
{
    int minimum_child_idx = index_current;
    int index_left_child = index_current * 2 + 1;
    int index_right_child = index_current * 2 + 2;

    if(index_left_child < heap->length && distance[heap->values[minimum_child_idx]] >= distance[heap->values[index_left_child]])
    {
        minimum_child_idx = index_left_child;
    }

    if(index_right_child < heap->length && distance[heap->values[minimum_child_idx]] >= distance[heap->values[index_right_child]])
    {
        minimum_child_idx = index_right_child;
    }

    if(minimum_child_idx == index_current)
        return;

    int aux = heap->values[index_current];
    heap->values[index_current] = heap->values[minimum_child_idx];
    heap->values[minimum_child_idx] = aux;
    percolate_down(heap, minimum_child_idx, distance);
}
void heap_pop(Heap *heap, int *distance)
{
    heap->values[0] = heap->values[heap->length - 1];
    heap->length -= 1;
    percolate_down(heap, 0, distance);
}
void print_heap(Heap *heap)
{
    int i;
    for(i = 0; i < heap->length; i++)
    {
        printf("%d ", heap->values[i]);
    }
}
int Dijkstra(Graph *graf, int start, int destination)
{
    Heap *min_heap = make_heap();
    int *selectat = malloc(1005 * sizeof(int));
    int *distance = malloc(1005 * sizeof(int));
    int i, j;
    for(i = 1; i < graf->nr_nodes + 1; i++)
    {
        distance[i] = graf->cost_matrix[start][i];
        selectat[i] = 0;
    }
    selectat[start] = 1;
    distance[start] = 0;
    /*for(i = 1; i < graf->nr_nodes + 1; i++)
    {
        printf("%d ", distance[i]);
    }
    printf("\n");
    for(i = 1; i < graf->nr_nodes + 1; i++)
    {
        heap_push(min_heap, i, distance);
    }
    print_heap(min_heap);
    printf("\n");
    heap_pop(min_heap, distance);
    heap_pop(min_heap, distance);
    print_heap(min_heap);
    printf("\n");*/
    heap_push(min_heap, start, distance);
    while(min_heap->length > 0)
    {
        int vecin = min_heap->values[0];
        selectat[vecin] = 1;
        heap_pop(min_heap, distance);
        for(j = 1; j < graf->nr_nodes + 1; j++)
        {
            if(graf->adjacency_matrix[vecin][j] == 1 && selectat[j] == 0 && distance[j] >= distance[vecin] + graf->cost_matrix[vecin][j])
            {
                distance[j] = distance[vecin] + graf->cost_matrix[vecin][j];
                heap_push(min_heap, j, distance);
            }
        }
    }
    return distance[destination];
}
Graph *make_graph(int nr_nodes)
{
    Graph *new_graf = malloc(sizeof(Graph));
    int i, j;
    new_graf->nr_nodes = nr_nodes;
    new_graf->adjacency_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->cost_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->parent_vector = malloc((nr_nodes + 1) * sizeof(int));
    new_graf->visited_vector = malloc((nr_nodes + 1) * sizeof(int));
    for(i = 0; i < nr_nodes + 1; i++)
    {
        new_graf->adjacency_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        new_graf->cost_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        for(j = 0; j < nr_nodes + 1; j++)
        {
            new_graf->adjacency_matrix[i][j] = 0;
            new_graf->cost_matrix[i][j] = INFINITY;
        }
        new_graf->parent_vector[i] = -1;
        new_graf->visited_vector[i] = 0;
    }
    return new_graf;
}
void print_graf_matrixes(Graph *graf)
{
    int i, j;
    printf("matricea de adiacenta:\n");
    for(i = 1; i < graf->nr_nodes + 1; i++)
    {
        for(j = 1; j < graf->nr_nodes + 1; j++)
        {
            printf("%d ", graf->adjacency_matrix[i][j]);
        }
        printf("\n");
    }
    printf("matricea de costuri:\n");
    for(i = 1; i < graf->nr_nodes + 1; i++)
    {
        for(j = 1; j < graf->nr_nodes + 1; j++)
        {
            printf("%d ", graf->cost_matrix[i][j]);
        }
        printf("\n");
    }
}
void add_edge_and_cost(Graph *graf, int source, int destination, int cost_sd, int cost_ds)
{
    graf->adjacency_matrix[source][destination] = 1;
    graf->adjacency_matrix[destination][source] = 1;
    graf->cost_matrix[source][destination] = cost_sd;
    graf->cost_matrix[destination][source] = cost_ds;
}
void remove_edge(Graph *graf, int source, int destination, int *cost_source_destination, int *cost_destination_source)
{
    *cost_source_destination = graf->cost_matrix[source][destination];
    *cost_destination_source = graf->cost_matrix[destination][source];
    graf->adjacency_matrix[source][destination] = 0;
    graf->adjacency_matrix[destination][source] = 0;
    graf->cost_matrix[source][destination] = INFINITY;
    graf->cost_matrix[destination][source] = INFINITY;
}
void free_graf(Graph *graf)
{
    int i;
    free(graf->visited_vector);
    free(graf->parent_vector);
    for(i = 0; i < graf->nr_nodes + 1; i++)
    {
        free(graf->adjacency_matrix[i]);
        free(graf->cost_matrix[i]);
    }
    free(graf->adjacency_matrix);
    free(graf->cost_matrix);
    free(graf);
}
int minimum(int a, int b)
{
    if(a > b)
        return b;

    return a;
}
int FloydWarshall(Graph *graf, int start, int destination)
{
	int size = graf->nr_nodes;
	int i, j, k;
	int matrix[1005][1005];
	for(i = 1; i < size + 1; i++)
	{
		for(j = 1; j < size + 1; j++)
		{
			matrix[i][j] = graf->cost_matrix[i][j];
		}
	}
	for(k = 1; k < size + 1; k++)
	{
		for(i = 1; i < size + 1; i++)
		{
			for(j = 1; j < size + 1; j++)
			{
				if(matrix[i][k] + matrix[k][j] < matrix[i][j])
				{
					matrix[i][j] = matrix[i][k] + matrix[k][j];
				}
			}
		}
	}
	return matrix[start][destination];
}
void work_with_edge(Graph *graf, int start, int destination, int *cost_minim)
{
    int cost_sd, cost_ds;
    remove_edge(graf, start, destination, &cost_sd, &cost_ds);
    int cost = FloydWarshall(graf, start, destination);
    //int cost = Dijkstra(graf, start, destination);
    cost += cost_ds;
    if(cost < (*cost_minim))
    {
        (*cost_minim) = cost;
    }
    add_edge_and_cost(graf, start, destination, cost_sd, cost_ds);
}
void parcurge_edges(Graph *graf, int *cost_minim)
{
    int i, j;
    for(i = 1; i < graf->nr_nodes + 1; i++)
    {
        for(j = 1; j < graf->nr_nodes + 1; j++)
        {
            if(graf->adjacency_matrix[i][j] == 1)
            {
                work_with_edge(graf, i, j, cost_minim);
            }
        }
    }
}
void DFS(Graph *graf, int start_node,int *cost_minim)
{
    graf->visited_vector[start_node] = 1;
    int vecin;
    //printf("%d\n", start_node);
    for(vecin = 0; vecin < graf->nr_nodes + 1; vecin++)
    {
        if(graf->adjacency_matrix[start_node][vecin] == 1 && graf->visited_vector[vecin] == 1
        && graf->parent_vector[start_node] != vecin)
        {
            //printf("avem un ciclu\n%d %d\n", start_node, vecin);
            int cost_sd, cost_ds;
            remove_edge(graf, start_node, vecin, &cost_sd, &cost_ds);
            //int cost = FloydWarshall(graf, start_node, vecin);
            int cost = Dijkstra(graf, start_node, vecin);
            cost += cost_ds;
            if(cost < (*cost_minim))
            {
                (*cost_minim) = cost;
            }
            add_edge_and_cost(graf, start_node, vecin, cost_sd, cost_ds);
        }
        if(graf->adjacency_matrix[start_node][vecin] == 1 && graf->visited_vector[vecin] == 0)
        {
            graf->parent_vector[vecin] = start_node;
            DFS(graf, vecin, cost_minim);
        }
    }
}
int main()
{
    int nr_nodes, nr_vertices, i, source, destination, cost;
    scanf("%d %d", &nr_nodes, &nr_vertices);
    Graph *graf = make_graph(nr_nodes);
    for(i = 0; i < nr_vertices; i++)
    {
        scanf("%d %d %d", &source, &destination, &cost);
        add_edge_and_cost(graf, source, destination, 0, cost);
    }
    //print_graf_matrixes(graf);
    //int cost_minim = FloydWarshall(graf, 1, 2);
    //printf("\n%d\n", cost_minim);
    int cost_minim = INFINITY;
    for(i = nr_nodes; i > 0; i--)
    {
        if(graf->visited_vector[i] == 0)
        {
            DFS(graf, i, &cost_minim);
        }
    }
    printf("\n%d\n", cost_minim);
    //cost_minim = Dijkstra(graf, 5, 1);
    //printf("%d\n", cost_minim);
    free_graf(graf);
    return 0;
}
