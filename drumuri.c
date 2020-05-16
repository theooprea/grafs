#include <stdio.h>
#include <stdlib.h>
#include "drumuri.h"
#define INFINITY 99999999
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
list_node *new_node(int nr_nod)
{
    list_node *nod = malloc(sizeof(list_node));
    nod->next = NULL;
    nod->prev = NULL;
    nod->vecin = nr_nod;
    return nod;
}
void enqueue(list_node **queue_head, int node)
{
    list_node *nod = new_node(node);
    if((*queue_head) == NULL)
    {
        (*queue_head) = nod;
    }
    else
    {
        list_node *iterator = *queue_head;
        while(iterator->next != NULL)
        {
            iterator = iterator->next;
        }
        iterator->next = nod;
        nod->prev = iterator;
    }
}
int queue_head(list_node *queue)
{
    return queue->vecin;
}
void dequeue(list_node **queue_head)
{
    if((*queue_head) == NULL)
        return;

    list_node *aux = *queue_head;
    if((*queue_head)->next == NULL)
    {
        aux = *queue_head;
        *queue_head = NULL;
        free(aux);
        return;
    }

    (*queue_head) = (*queue_head)->next;
    (*queue_head)->prev = NULL;
    free(aux);
}
void add_in_list(list_node **head, int node)
{
    list_node *nod = new_node(node);
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
void add_edge(Graph *graf, int source, int destination)
{
    add_in_list(&graf->lista_adiacenta[source], destination);
    add_in_list(&graf->lista_adiacenta[destination], source);
}
void printlist(list_node *head)
{
    list_node *aux = head;
    while(aux != NULL)
    {
        printf("%d ", aux->vecin);
        aux = aux->next;
    }
    printf("\n");
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
                printf("%d ", aux->vecin);
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
int Dijkstra(Graph *graf, int start, int destination, int *predecesori, int *distance, int *selectat) {
    int i;
    list_node *aux;
    list_node *queue = NULL;
    for (i = 1; i < graf->nr_noduri + 1; i++) {
        distance[i] = INFINITY;
        selectat[i] = 0;
        predecesori[i] = 0;
    }
    selectat[start] = 1;
    distance[start] = 0;
    enqueue(&queue, start);
    while (queue != NULL) {
        int vecin = queue_head(queue);
        selectat[vecin] = 1;
        dequeue(&queue);
        aux = graf->lista_adiacenta[vecin];
        while(aux != NULL)
        {
            if(selectat[aux->vecin] == 0)
            {
                distance[aux->vecin] = distance[vecin] + 1;
                selectat[aux->vecin] = 1;
                predecesori[aux->vecin] = vecin;
                enqueue(&queue, aux->vecin);
                if(aux->vecin == destination)
                {
                    while(queue != NULL)
                    {
                        dequeue(&queue);
                    }
                    int distanta = distance[destination];
                    return distanta;
                }
            }
            aux = aux->next;
        }
    }
    int distanta = distance[destination];
    return distanta;
}
int compare_function(const void *a, const void *b)
{
    int *a2 = (int *)a;
    int *b2 = (int *)b;
    return *a2 - *b2;
}
void work_with_query(Graph *graf, int source, int destination, FILE *fisier_out)
{
    int aux = destination;
    int *predecesori = malloc((graf->nr_noduri + 1) * sizeof(int));
    int *distance = malloc((graf->nr_noduri + 1) * sizeof(int));
    int *selectat = malloc((graf->nr_noduri + 1) * sizeof(int));
    int cost_minim = Dijkstra(graf, source, destination, predecesori, distance, selectat);
    int *drum_minim = malloc((cost_minim + 1) * sizeof(int));
    int *common_nodes = malloc((cost_minim + 1) * sizeof(int));
    int index = 0, i, nr_common_nodes = 0;
    while(aux != 0)
    {
        drum_minim[index++] = aux;
        aux = predecesori[aux];
    }
    if(index == 1)
    {
        fprintf(fisier_out, "0\n");
    }
    else
    {
        for(i = 0; i < index; i++)
        {
            list_node *lista_originala = graf->lista_adiacenta[drum_minim[i]];
            graf->lista_adiacenta[drum_minim[i]] = NULL;
            int cost_nou = Dijkstra(graf, source, destination, predecesori, distance, selectat);
            graf->lista_adiacenta[drum_minim[i]] = lista_originala;
            if(cost_nou != cost_minim)
            {
                common_nodes[nr_common_nodes++] = drum_minim[i];
            }
        }
        common_nodes[nr_common_nodes++] = destination;
        qsort(common_nodes, nr_common_nodes, sizeof(int), compare_function);
        fprintf(fisier_out, "%d ", nr_common_nodes);
        for(i = 0; i < nr_common_nodes; i++)
        {
            fprintf(fisier_out, "%d ", common_nodes[i]);
        }
        fprintf(fisier_out, "\n");        
    }
    free(distance);
    free(selectat);
    free(common_nodes);
    free(predecesori);
    free(drum_minim);
}
int main()
{
    FILE *fisier_in, *fisier_out;
    fisier_in = fopen("drumuri.in", "r");
    fisier_out = fopen("drumuri.out", "w");
    int nr_nodes, nr_edges, nr_query, i, source, destination;
    fscanf(fisier_in, "%d %d %d", &nr_nodes, &nr_edges, &nr_query);
    Graph *graf = new_graf(nr_nodes);
    for(i = 0; i < nr_edges; i++)
    {
        fscanf(fisier_in, "%d %d", &source, &destination);
        add_edge(graf, source, destination);
    }
    for(i = 0; i < nr_query; i++)
    {
        fscanf(fisier_in, "%d %d", &source, &destination);
        work_with_query(graf, source, destination, fisier_out);
    }
    free_graf(graf);
    fclose(fisier_in);
    fclose(fisier_out);
    return 0;
}