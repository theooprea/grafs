#ifndef _HOME_STUDENT_CERINTA4_H_
#define _HOME_STUDENT_CERINTA4_H_
typedef struct arbore {
    int row;
    int column;
    struct arbore *next;
    struct arbore *parent;
    struct arbore *child;
    int level;
} arbore_node;
#endif  // _HOME_STUDENT_CERINTA4_H_
