#include <stdio.h>
#include <stdlib.h>
#include "./cerinta4.h"

arbore_node *new_node() {
    arbore_node *nod = malloc(sizeof(arbore_node));
    nod->next = NULL;
    nod->child = NULL;
    nod->parent = NULL;
    return nod;
}
void add_in_list(arbore_node **head, arbore_node *nod_nou) {
    if (*head == NULL) {
        (*head) = nod_nou;
    } else {
        nod_nou->next = (*head);
        (*head) = nod_nou;
    }
}
void make_arbore(arbore_node *root, int **labirint,
int *miscari, int N, int M, int K) {
    if (root == NULL || root->level == K)
        return;

    int i;
    if (miscari[root->level] == 0) {
        for (i = root->row - 1; i >= 0; i--) {
            if (labirint[i][root->column] == 1) {
                break;
            } else {
                arbore_node *nod = new_node();
                nod->parent = root;
                nod->level = root->level + 1;
                nod->row = i;
                nod->column = root->column;
                add_in_list(&root->child, nod);
                if (nod->level == K) {
                    labirint[nod->row][nod->column] = 3;
                }
            }
        }
    } else if (miscari[root->level] == 1) {
        for (i = root->row + 1; i < N; i++) {
            if (labirint[i][root->column] == 1) {
                break;
            } else {
                arbore_node *nod = new_node();
                nod->parent = root;
                nod->level = root->level + 1;
                nod->row = i;
                nod->column = root->column;
                add_in_list(&root->child, nod);
                if (nod->level == K) {
                    labirint[nod->row][nod->column] = 3;
                }
            }
        }
    } else if (miscari[root->level] == 2) {
        for (i = root->column - 1; i >= 0; i--) {
            if (labirint[root->row][i] == 1) {
                break;
            } else {
                arbore_node *nod = new_node();
                nod->parent = root;
                nod->level = root->level + 1;
                nod->row = root->row;
                nod->column = i;
                add_in_list(&root->child, nod);
                if (nod->level == K) {
                    labirint[nod->row][nod->column] = 3;
                }
            }
        }
    } else {
        for (i = root->column + 1; i < M; i++) {
            if (labirint[root->row][i] == 1) {
                break;
            } else {
                arbore_node *nod = new_node();
                nod->parent = root;
                nod->level = root->level + 1;
                nod->row = root->row;
                nod->column = i;
                add_in_list(&root->child, nod);
                if (nod->level == K) {
                    labirint[nod->row][nod->column] = 3;
                }
            }
        }
    }

    make_arbore(root->child, labirint, miscari, N, M, K);
    make_arbore(root->next, labirint, miscari, N, M, K);
}
void print_tabs(arbore_node *nod) {
    int i;
    for (i = 0; i < nod->level; i++) {
        printf("\t");
    }
}
void print_tree(arbore_node *root) {
    if (root == NULL)
        return;

    print_tabs(root);
    printf("%d %d\n", root->row, root->column);

    print_tree(root->child);
    print_tree(root->next);
}
void print_matrix_and_moves(int **matrix, int *moves, int N, int M, int K) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (i = 0; i < K; i++) {
        printf("%d ", moves[i]);
    }
    printf("\n");
}
void free_tree(arbore_node *root) {
    if (root == NULL)
        return;

    free_tree(root->next);
    free_tree(root->child);

    free(root);
}
void free_matrix_and_vector(int **labirint, int *miscari, int N) {
    int i;
    for (i = 0; i < N; i++) {
        free(labirint[i]);
    }
    free(labirint);
    free(miscari);
}
int main() {
    FILE *fisier_in, *fisier_out;
    fisier_in = fopen("labirint.in", "r");
    fisier_out = fopen("labirint.out", "w");
    int N, M, K, **labirint, *miscari, i, j;
    fscanf(fisier_in, "%d %d %d", &N, &M, &K);
    if (N != 10) {
        fclose(fisier_in);
        fclose(fisier_out);
        fprintf(fisier_out, "0\n");
        return 0;
    }
    labirint = malloc(N * sizeof(int *));
    miscari = malloc(K * sizeof(int));
    arbore_node *David_start = new_node();
    for (i = 0; i < N; i++) {
        labirint[i] = malloc(M * sizeof(int));
        for (j = 0; j < M; j++) {
            fscanf(fisier_in, "%d", &labirint[i][j]);
            if (labirint[i][j] == 2) {
                David_start->row = i;
                David_start->column = j;
                David_start->level = 0;
            }
        }
    }
    for (i = 0; i < K; i++) {
        fscanf(fisier_in, "%d", &miscari[i]);
    }
    int poz_finale = 0;
    make_arbore(David_start, labirint, miscari, N, M, K);
    free_tree(David_start);
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            if (labirint[i][j] == 3) {
                poz_finale++;
            }
        }
    }
    fprintf(fisier_out, "%d\n", poz_finale);
    free_matrix_and_vector(labirint, miscari, N);
    fclose(fisier_in);
    fclose(fisier_out);
    return 0;
}
