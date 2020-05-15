#ifndef __GRAF_H__
#define __GRAF_H__
typedef struct heap {
    int *values;
    int length;
} Heap;
typedef struct list {
    int vecin;
    int cost;
    struct list *next;
    struct list *prev;
} list_node;
typedef struct graf {
    int nr_noduri;
    list_node **lista_adiacenta;
} Graph;
#endif  // __GRAF_H__