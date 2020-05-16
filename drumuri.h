#ifndef __DRUMURI_H__
#define __DRUMURI_H__
typedef struct heap {
    int *values;
    int length;
} Heap;
typedef struct list {
    int vecin;
    struct list *next;
    struct list *prev;
} list_node;
typedef struct graf {
    int nr_noduri;
    list_node **lista_adiacenta;
} Graph;
#endif  // __DRUMURI_H__