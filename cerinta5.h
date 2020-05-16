#ifndef _HOME_STUDENT_CERINTA5_H_
#define _HOME_STUDENT_CERINTA5_H_
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
#endif  // _HOME_STUDENT_CERINTA5_H_
