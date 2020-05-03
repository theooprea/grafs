#include <stdio.h>
#include <stdlib.h>
#include "cerinta1.h"

#define INFINITY 99999999

Graph *make_graph(int nr_nodes)
{
    Graph *new_graf = malloc(sizeof(Graph));
    int i, j;
    new_graf->nr_nodes = nr_nodes;
    new_graf->adjacency_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->cost_matrix = malloc((nr_nodes + 1) * sizeof(int *));
    new_graf->parent_vector = malloc((nr_nodes + 1) * sizeof(int));
    new_graf->visited_vector = malloc((nr_nodes + 1) * sizeof(int));
    new_graf->is_part_of_cycle = malloc((nr_nodes + 1) * sizeof(int));
    for(i = 0; i < nr_nodes + 1; i++)
    {
        new_graf->adjacency_matrix[i] = calloc((nr_nodes + 1), sizeof(int));
        new_graf->cost_matrix[i] = malloc((nr_nodes + 1) * sizeof(int));
        for(j = 0; j < nr_nodes + 1; j++)
        {
            new_graf->adjacency_matrix[i][j] = 0;
            new_graf->cost_matrix[i][j] = INFINITY;
        }
        new_graf->parent_vector[i] = -1;
        new_graf->visited_vector[i] = 0;
        new_graf->is_part_of_cycle[i] = 0;
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
void add_vertex_and_cost(Graph *graf, int source, int destination, int cost)
{
    graf->adjacency_matrix[source][destination] = 1;
    graf->adjacency_matrix[destination][source] = 1;
    graf->cost_matrix[source][destination] = 0;
    graf->cost_matrix[destination][source] = cost;
}
void free_graf(Graph *graf)
{
    int i;
    free(graf->is_part_of_cycle);
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
int one_way(Graph *graf, int *vector, int nr_elem)
{
    int cost = 0;
    int i;
    for(i = 0; i < nr_elem - 1; i++)
    {
        cost = cost + graf->cost_matrix[vector[i]][vector[i + 1]];
    }
    cost = cost + graf->cost_matrix[vector[nr_elem - 1]][vector[0]];
    return cost;
}
int or_another(Graph *graf, int *vector, int nr_elem)
{
    int cost = 0;
    int i;
    for(i = nr_elem - 1; i > 0; i--)
    {
        cost = cost + graf->cost_matrix[vector[i]][vector[i - 1]];
    }
    cost = cost + graf->cost_matrix[vector[0]][vector[nr_elem - 1]];
    return cost;
}
int minimum(int a, int b)
{
    if(a > b)
        return b;

    return a;
}
int prelucreaza(Graph *graf, int *vector, int nr_elem)
{
    int i;
    for(i = 0; i < nr_elem; i++)
    {
        printf("%d ", vector[i]);
    }
    printf("\n");
    int cost_minim = minimum(one_way(graf, vector, nr_elem), or_another(graf, vector, nr_elem));
    return cost_minim;
}
int work_with_cycle(Graph *graf, int start, int stop)
{
    int aux = graf->parent_vector[stop];
    int nr_nodes_in_cycle, index = 1;
    int vector[1000];
    vector[0] = stop;
    while(aux != start)
    {
        vector[index++] = aux;
        aux = graf->parent_vector[aux];
    }
    vector[index++] = aux;
    nr_nodes_in_cycle = index;
    int cost_minim = prelucreaza(graf, vector, nr_nodes_in_cycle);
    return cost_minim;
}
void DFS(Graph *graf, int start_node,int *cost_minim)
{
    graf->visited_vector[start_node] = 1;
    int vecin;
    //printf("%d\n", start_node);
    for(vecin = 0; vecin < graf->nr_nodes + 1; vecin++)
    {
        if(graf->adjacency_matrix[start_node][vecin] == 1 && graf->visited_vector[vecin] == 1
        && graf->parent_vector[start_node] != vecin && (graf->is_part_of_cycle[start_node] == 0 || graf->is_part_of_cycle[vecin] == 0 ))
        {
            graf->is_part_of_cycle[start_node] = 1;
            graf->is_part_of_cycle[vecin] = 1;
            printf("avem un ciclu\n");
            int cost_of_cycle = work_with_cycle(graf, vecin, start_node);
            printf("%d\n", cost_of_cycle);
            if(cost_of_cycle < (*cost_minim))
            {
                (*cost_minim) = cost_of_cycle;
            }
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
        add_vertex_and_cost(graf, source, destination, cost);
    }
    //print_graf_matrixes(graf);
    int cost_minim = INFINITY;
    for(i = 0; i < nr_nodes + 1; i++)
    {
        if(graf->visited_vector[i] == 0)
        {
            DFS(graf, i, &cost_minim);
        }
    }
    /*printf("\n");
    for(i = 0; i < nr_nodes + 1; i++)
    {
        printf("%d ", graf->parent_vector[i]);
    }
    printf("\n");*/
    if(cost_minim == INFINITY)
    {
        cost_minim = -1;
    }
    printf("%d\n", cost_minim);
    free_graf(graf);
    return 0;
}